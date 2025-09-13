## BoardLight

```
[★]$ nmap -sC -sV 10.129.231.37
Starting Nmap 7.94SVN ( https://nmap.org ) at 2025-09-13 04:33 CDT
Nmap scan report for 10.129.231.37
Host is up (0.0099s latency).
Not shown: 998 closed tcp ports (reset)
PORT   STATE SERVICE VERSION
22/tcp open  ssh     OpenSSH 8.2p1 Ubuntu 4ubuntu0.11 (Ubuntu Linux; protocol 2.0)
| ssh-hostkey: 
|   3072 06:2d:3b:85:10:59:ff:73:66:27:7f:0e:ae:03:ea:f4 (RSA)
|   256 59:03:dc:52:87:3a:35:99:34:44:74:33:78:31:35:fb (ECDSA)
|_  256 ab:13:38:e4:3e:e0:24:b4:69:38:a9:63:82:38:dd:f4 (ED25519)
80/tcp open  http    Apache httpd 2.4.41 ((Ubuntu))
|_http-title: Site doesn't have a title (text/html; charset=UTF-8).
|_http-server-header: Apache/2.4.41 (Ubuntu)
Service Info: OS: Linux; CPE: cpe:/o:linux:linux_kernel

Service detection performed. Please report any incorrect results at https://nmap.org/submit/ .
Nmap done: 1 IP address (1 host up) scanned in 7.15 seconds
```
#### 不用域名就访问网站了
![距离那天第三天](images/091301.png)
```
[★]$ echo '10.129.231.37 board.htb' | sudo tee -a /etc/hosts
10.129.231.37 board.htb
```
#### 使用ffuf对单板上潜在的虚拟主机进行模糊处理。HTB域，我们测试了一系列可能的子域名。命令格式为bitquark-subdomains-top10000 .txtwordlist替换主机头中的FUZZ占位符。这允许我们将HTTP请求发送到各个子域并检查响应。我们过滤掉大小为15949字节的响应使用-fs标志，这有助于我们忽略不相关的结果并关注可能显示有效子域的唯一响应。
```
[★]$ ls /usr/share/wordlists/seclists/Discovery/DNS/bitquark-subdomains-top100000.txt
/usr/share/wordlists/seclists/Discovery/DNS/bitquark-subdomains-top100000.txt
[★]$ ffuf -w /usr/share/wordlists/seclists/Discovery/DNS/bitquark-subdomains-top100000.txt:FUZZ -u http://board.htb/ -H 'Host: FUZZ.board.htb' -fs 15949

 crm                     [Status: 200, Size: 6360, Words: 397, Lines: 150, Duraion: 826ms]
```
