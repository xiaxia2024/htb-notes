## ServMon
```
[★]$ ports=$(nmap -p- --min-rate=1000 -T4 10.129.227.77 | grep ^[0-9] | cut -d '/' -f 1 | tr '\n' ',' | sed s/,$//)
[★]$ nmap -p$ports -sC -sV 10.129.227.77
Starting Nmap 7.94SVN ( https://nmap.org ) at 2026-02-08 01:01 CST
Nmap scan report for 10.129.227.77
Host is up (0.066s latency).

PORT      STATE SERVICE       VERSION
21/tcp    open  ftp           Microsoft ftpd
| ftp-syst: 
|_  SYST: Windows_NT
| ftp-anon: Anonymous FTP login allowed (FTP code 230)
|_02-28-22  06:35PM       <DIR>          Users
22/tcp    open  ssh           OpenSSH for_Windows_8.0 (protocol 2.0)
| ssh-hostkey: 
|   3072 c7:1a:f6:81:ca:17:78:d0:27:db:cd:46:2a:09:2b:54 (RSA)
|   256 3e:63:ef:3b:6e:3e:4a:90:f3:4c:02:e9:40:67:2e:42 (ECDSA)
|_  256 5a:48:c8:cd:39:78:21:29:ef:fb:ae:82:1d:03:ad:af (ED25519)
80/tcp    open  http
| fingerprint-strings: 
|   GetRequest, HTTPOptions, RTSPRequest: 
|     HTTP/1.1 200 OK
|     Content-type: text/html
|     Content-Length: 340
|     Connection: close
|     AuthInfo: 
|     <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
|     <html xmlns="http://www.w3.org/1999/xhtml">
|     <head>
|     <title></title>
|     <script type="text/javascript">
|     window.location.href = "Pages/login.htm";
|     </script>
|     </head>
|     <body>
|     </body>
|     </html>
|   NULL: 
|     HTTP/1.1 408 Request Timeout
|     Content-type: text/html
|     Content-Length: 0
|     Connection: close
|_    AuthInfo:
|_http-title: Site doesn't have a title (text/html).
135/tcp   open  msrpc         Microsoft Windows RPC
139/tcp   open  netbios-ssn   Microsoft Windows netbios-ssn
445/tcp   open  microsoft-ds?
5666/tcp  open  tcpwrapped
6063/tcp  open  tcpwrapped
6699/tcp  open  tcpwrapped
8443/tcp  open  ssl/https-alt
|_ssl-date: TLS randomness does not represent time
| ssl-cert: Subject: commonName=localhost
| Not valid before: 2020-01-14T13:24:20
|_Not valid after:  2021-01-13T13:24:20
| http-title: NSClient++
|_Requested resource was /index.html
| fingerprint-strings: 
|   FourOhFourRequest, HTTPOptions, RTSPRequest, SIPOptions: 
|     HTTP/1.1 404
|     Content-Length: 18
|     Document not found
|   GetRequest: 
|     HTTP/1.1 302
|     Content-Length: 0
|_    Location: /index.html
49664/tcp open  msrpc         Microsoft Windows RPC
49665/tcp open  msrpc         Microsoft Windows RPC
49666/tcp open  msrpc         Microsoft Windows RPC
49667/tcp open  msrpc         Microsoft Windows RPC
49668/tcp open  msrpc         Microsoft Windows RPC
49669/tcp open  msrpc         Microsoft Windows RPC
49670/tcp open  msrpc         Microsoft Windows RPC
2 services unrecognized despite returning data.
```
```
[★]$ ftp 10.129.227.77
Connected to 10.129.227.77.
220 Microsoft FTP Service
Name (10.129.227.77:root): anonymous
331 Anonymous access allowed, send identity (e-mail name) as password.
Password: 
230 User logged in.
Remote system type is Windows_NT.

ftp> dir
229 Entering Extended Passive Mode (|||49680|)
125 Data connection already open; Transfer starting.
02-28-22  06:35PM       <DIR>          Users
226 Transfer complete.
ftp> cd Users
250 CWD command successful.
ftp> ls
229 Entering Extended Passive Mode (|||49681|)
150 Opening ASCII mode data connection.
02-28-22  06:36PM       <DIR>          Nadine
02-28-22  06:37PM       <DIR>          Nathan
226 Transfer complete.

ftp> ls Nathan
229 Entering Extended Passive Mode (|||49683|)
125 Data connection already open; Transfer starting.
02-28-22  06:36PM                  182 Notes to do.txt
226 Transfer complete.
ftp> get "Nathan\\Notes to do.txt"
local: Nathan\\Notes to do.txt remote: Nathan\\Notes to do.txt
229 Entering Extended Passive Mode (|||49684|)
125 Data connection already open; Transfer starting.
100% |***********************************|   182        2.67 KiB/s    00:00 ETA
226 Transfer complete.
WARNING! 4 bare linefeeds received in ASCII mode.
File may not have transferred correctly.
182 bytes received in 00:00 (2.66 KiB/s)

ftp> cd Nadine
250 CWD command successful.
ftp> dir
229 Entering Extended Passive Mode (|||49685|)
125 Data connection already open; Transfer starting.
02-28-22  06:36PM                  168 Confidential.txt
226 Transfer complete.
ftp> get "Nadine\\Confidential.exe
local: Nadine\\Confidential.exe remote: Nadine\\Confidential.exe
229 Entering Extended Passive Mode (|||49686|)
550 The system cannot find the path specified. 

ftp> exit
221 Goodbye.
[★]$ ls
'Nathan\\Notes to do.txt'

[★]$ cd desktop
bash: cd: desktop: No such file or directory
[★]$ ls Desktop
htb_vpn_logs.log  my_credentials.txt  my_data  README.license

//Notes to do.txt包含关于已安装的任务的已完成和未完成任务的信息监控应用程序
[★]$ cat 'Nathan\\Notes to do.txt'
1) Change the password for NVMS - Complete
2) Lock down the NSClient Access - Complete
3) Upload the passwords
4) Remove public access to NVMS
5) Place the secret files in SharePoint

[★]$ ls Desktop
htb_vpn_logs.log  my_credentials.txt  my_data  README.license
[★]$ cat Desktop/my_credentials.txt
Username: syareya55
Password: cqroAqyi
```
#### 在浏览器中检查80端口，可以看到NVMS-1000网络监控的登录页面软件。默认凭证admin / 123456或其他常见凭证不给我们访问。
```
[★]$ gobuster dir -u http://10.129.227.77 -w /usr/share/wordlists/dirbuster/directory-list-lowercase-2.3-medium.txt -t 40 -o gobuster-80-root-medium
===============================================================
Gobuster v3.6
by OJ Reeves (@TheColonial) & Christian Mehlmauer (@firefart)
===============================================================
[+] Url:                     http://10.129.227.77
[+] Method:                  GET
[+] Threads:                 40
[+] Wordlist:                /usr/share/wordlists/dirbuster/directory-list-lowercase-2.3-medium.txt
[+] Negative Status codes:   404
[+] User Agent:              gobuster/3.6
[+] Timeout:                 10s
===============================================================
Starting gobuster in directory enumeration mode
===============================================================

Error: the server returns a status code that matches the provided options for non existing urls. http://10.129.227.77/41b172ec-d70c-40c2-a83c-d7619ddf5e13 => 200 (Length: 118). To continue please exclude the status code or the length
```
![images/2026020801.png)
```
[★]$ curl http://10.129.227.77/41b172ec-d70c-40c2-a83c-d7619ddf5e13
<?xml version="1.0" encoding="UTF-8"?>
<response>	<status>fail</status>
	<errorCode>536870934</errorCode>
</response>
```
#### 我尝试了wfuzz，在那里我可以根据响应长度进行过滤。它可以处理几千个请求，但每次都失败了：
```
[★]$ wfuzz -c -u http://10.129.227.77/FUZZ -w /usr/share/wordlists/dirbuster/directory-list-lowercase-2.3-medium.txt --hh 118
 /usr/lib/python3/dist-packages/wfuzz/__init__.py:34: UserWarning:Pycurl is not compiled against Openssl. Wfuzz might not work correctly when fuzzing SSL sites. Check Wfuzz's documentation for more information.
********************************************************
* Wfuzz 3.1.0 - The Web Fuzzer                         *
********************************************************

Target: http://10.129.227.77/FUZZ
Total requests: 207643

=====================================================================
ID           Response   Lines    Word       Chars       Payload        
=====================================================================

000000001:   200        12 L     22 W       338 Ch      "# directory-li
                                                        st-lowercase-2.
                                                        3-medium.txt"  
000000007:   200        12 L     22 W       338 Ch      "# license, vis
                                                        it http://creat
                                                        ivecommons.org/
                                                        licenses/by-sa/
                                                        3.0/"          
000000003:   200        12 L     22 W       338 Ch      "# Copyright 20
                                                        07 James Fisher
                                                        "              
000000014:   200        12 L     22 W       338 Ch      "http://10.129.
                                                        227.77/"       
000000013:   200        12 L     22 W       338 Ch      "#"            
```
### Vulnerabilities  缺陷
#### searchsplit显示了此应用程序中的目录遍历漏洞：
```
[★]$ searchsploit "nvms 1000"
---------------------------------------------- ---------------------------------
 Exploit Title                                |  Path
---------------------------------------------- ---------------------------------
NVMS 1000 - Directory Traversal               | hardware/webapps/47774.txt
TVT NVMS 1000 - Directory Traversal           | hardware/webapps/48311.py
---------------------------------------------- ---------------------------------
Shellcodes: No Results
```
#### 在阅读文本文件,它基本上是说我可以请求  /../../../../../../../../../../../../windows/win.ini和得到它。我会将请求发送到打嗝中继器，它会工作：
