|类型	| 行为 |
| ---- | ----- |
|目录 |	mkdir |
|符号链接	| readlink + symlink |
|普通文件 |	copy_file |

#### pid_t pid = fork(); //fork() 的返回值。
|返回值|	身份|
| ---- | ----- |
|< 0	|失败|
|== 0|	👶 子进程|
|> 0	|👨 父进程（返回的是子进程 PID）|
```
父进程 = 制造“阻塞条件 + race窗口”
子进程 = 被卡住的“受害执行流”

pid == 0 是子进程（执行 snap-confine + 触发阻塞）
pid > 0 是父进程（制造 socket + 控制 race 时机）
```
#### 在 Linux 里，每个进程启动时默认有 3 个“固定编号”的输入输出：
|名称	|宏	|数字|
| ---- | ----- |-------- |
|标准输入|	STDIN_FILENO|	0|
|标准输出|	STDOUT_FILENO|	1|
|标准错误	|STDERR_FILENO|	2|
#### syscall(SYS_renameat2,AT_FDCWD, EXCHANGE_DST,AT_FDCWD, EXCHANGE_SRC,RENAME_EXCHANGE);
#### 对应原型：
#### renameat2(olddirfd, oldpath,newdirfd, newpath,flags)
|  参数 | 	作用 | 
| ---- | ----- |
|AT_FDCWD	| 表示“用当前工作目录” |
|EXCHANGE_DST	|旧路径（源）|
|AT_FDCWD	|新路径的目录|
|EXCHANGE_SRC	|新路径|
|RENAME_EXCHANGE	|控制行为的开关（flag）|
#### C预处理宏(macro)在 Linux 里（简化理解）：
```
#define RENAME_NOREPLACE (1 << 0)  // 第0位
#define RENAME_EXCHANGE  (1 << 1)  // 第1位
#define RENAME_WHITEOUT  (1 << 2)  // 第2位
```
| bit位 | 二进制   | 含义       |
| ---- | ----- | -------- |
| bit0 | 0b001 | 不允许覆盖    |
| bit1 | 0b010 | 交换       |
| bit2 | 0b100 | whiteout |
#### 这是 Linux / Unix 约定：对外的返回值
| return 值	| 含义| 
| ---- | ----- |
| 0	| 成功| 
| 非0（-1）| 	失败| 
#### status 是“原始编码”，要用宏解析： WIFEXITED(status) WEXITSTATUS(status)
```
int status;
waitpid(pid, &status, 0);
```
#### pid_t waitpid(pid_t pid, int *status, int options);
```
pid :要等待的子进程 ID
&status 这是一个“输出参数”:内核把子进程的退出信息写进这里
```
| 值| 	含义| 
| ---- | ----- |
| >0	| 等指定 pid 的子进程| 
| -1	| 等任意子进程| 
| 0	| 等同一进程组| 
| < -1	| 等某个进程组| 

| 值	| 含义| 
| ---- | ----- |
| 0	| 阻塞等待| 
| WNOHANG	| 非阻塞（立刻返回）| 
| WUNTRACED| 	追踪暂停进程| 
