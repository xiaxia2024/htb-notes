## Watcher
```
[★]$ nmap -sC -sV 10.129.234.163
Starting Nmap 7.94SVN ( https://nmap.org ) at 2026-03-04 07:47 CST
Nmap scan report for 10.129.234.163
Host is up (0.011s latency).
Not shown: 998 closed tcp ports (reset)
PORT   STATE SERVICE VERSION
22/tcp open  ssh     OpenSSH 8.9p1 Ubuntu 3ubuntu0.13 (Ubuntu Linux; protocol 2.0)
| ssh-hostkey: 
|   256 f0:e4:e7:ae:27:22:14:09:0c:fe:1a:aa:85:a8:c3:a5 (ECDSA)
|_  256 fd:a3:b9:36:17:39:25:1d:40:6d:5a:07:97:b3:42:13 (ED25519)
80/tcp open  http    Apache httpd 2.4.52 ((Ubuntu))
|_http-server-header: Apache/2.4.52 (Ubuntu)
|_http-title: Did not follow redirect to http://watcher.vl/
Service Info: OS: Linux; CPE: cpe:/o:linux:linux_kernel

Service detection performed. Please report any incorrect results at https://nmap.org/submit/ .
Nmap done: 1 IP address (1 host up) scanned in 8.23 seconds
[★]$ echo '10.129.234.163 watcher.vl' | sudo tee -a /etc/hosts
10.129.234.163 watcher.vl
```
```
[★]$ ffuf -w /usr/share/seclists/Discovery/DNS/subdomains-top1million-110000.txt -u http://watcher.vl/ -H 'Host: FUZZ.watcher.vl' -fs 4991

        /'___\  /'___\           /'___\       
       /\ \__/ /\ \__/  __  __  /\ \__/       
       \ \ ,__\\ \ ,__\/\ \/\ \ \ \ ,__\      
        \ \ \_/ \ \ \_/\ \ \_\ \ \ \ \_/      
         \ \_\   \ \_\  \ \____/  \ \_\       
          \/_/    \/_/   \/___/    \/_/       

       v2.1.0-dev
________________________________________________

 :: Method           : GET
 :: URL              : http://watcher.vl/
 :: Wordlist         : FUZZ: /usr/share/seclists/Discovery/DNS/subdomains-top1million-110000.txt
 :: Header           : Host: FUZZ.watcher.vl
 :: Follow redirects : false
 :: Calibration      : false
 :: Timeout          : 10
 :: Threads          : 40
 :: Matcher          : Response status: 200-299,301,302,307,401,403,405,500
 :: Filter           : Response size: 4991
________________________________________________

:: Progress: [1/114441] :: Job [1/1] :: 0 req/sec :: Duration: [0:00:00] :: Erro
:: Progress: [117/114441] :: Job [1/1] :: 0 req/sec :: Duration: [0:00:00] :: Er
:: Progress: [201/114441] :: Job [1/1] :: 0 req/sec :: Duration: [0:00:00] :: Er
:: Progress: [291/114441] :: Job [1/1] :: 0 req/sec :: Duration: [0:00:00] :: Er
:: Progress: [381/114441] :: Job [1/1] :: 0 req/sec :: Duration: [0:00:00] :: Er
:: Progress: [465/114441] :: Job [1/1] :: 0 req/sec :: Duration: [0:00:00] :: Er
zabbix                  [Status: 200, Size: 3946, Words: 199, Lines: 33, Duration: 174ms]

```
```
[★]$ sudo sed -i '/10.129.234.163 watcher.vl/s/$/ zabbix.watcher.vl/' /etc/hosts
```
#### 访问浏览器http://zabbix.watcher.vl/
### Foothold
#### Zabbix 具备访客登录功能；如果机器开启了这一功能，我们就能以最低权限访问仪表板。点击 'or sign in as guest'
#### 在底下有Zabbix 7.0.0alpha1. © 2001–2023, Zabbix SIA
#### 在搜索针对此 Zabbix 版本的漏洞时，我们发现了 CVE-2024-22120 这个漏洞。该漏洞需要一个会话 ID 和主机 ID。
#### 由于我们启用了访客登录功能，我们可以对我们的 cookie 进行 base64 解码以获取会话 ID。要获取主机 ID，我们可以查看“库存”并检查其中的主机
https://github.com/W01fh4cker/CVE-2024-22120-RCE/blob/main/CVE-2024-22120-RCE.py
```
[★]$ wget https://raw.githubusercontent.com/W01fh4cker/CVE-2024-22120-RCE/refs/heads/main/CVE-2024-22120-RCE.py
```
#### 在Inventory -> Hosts 鼠标移到 'Zabbix server' 
#### 左下角就会显示http://zabbix.watcher.vl/hostinventories.php?hostid=10084
#### 主机 ID 为 10084 ，这是“库存”中的 Zabbix 服务器。我们将对 cookie 进行 base64 解码，并使用 sessionid 键。
#### 
