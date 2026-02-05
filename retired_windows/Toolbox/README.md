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
