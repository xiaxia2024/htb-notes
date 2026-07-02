## 0-Secura
```
?$ nmap -p- -sV -sC  192.168.62.97
Starting Nmap 7.98 ( https://nmap.org ) at 2026-07-02 05:42 +0000
Stats: 0:02:42 elapsed; 0 hosts completed (1 up), 1 undergoing Script Scan
NSE Timing: About 98.30% done; ETC: 05:45 (0:00:00 remaining)
Nmap scan report for 192.168.62.97
Host is up (0.00031s latency).
Not shown: 65514 filtered tcp ports (no-response)
PORT      STATE SERVICE      VERSION
53/tcp    open  domain       Simple DNS Plus
88/tcp    open  kerberos-sec Microsoft Windows Kerberos (server time: 2026-07-02 05:44:37Z)
135/tcp   open  msrpc        Microsoft Windows RPC
139/tcp   open  netbios-ssn  Microsoft Windows netbios-ssn
389/tcp   open  ldap         Microsoft Windows Active Directory LDAP (Domain: secura.yzx, Site: Default-First-Site-Name)
445/tcp   open  microsoft-ds Windows Server 2016 Standard 14393 microsoft-ds (workgroup: SECURA)
464/tcp   open  kpasswd5?
593/tcp   open  ncacn_http   Microsoft Windows RPC over HTTP 1.0
636/tcp   open  tcpwrapped
3268/tcp  open  ldap         Microsoft Windows Active Directory LDAP (Domain: secura.yzx, Site: Default-First-Site-Name)
3269/tcp  open  tcpwrapped
5985/tcp  open  http         Microsoft HTTPAPI httpd 2.0 (SSDP/UPnP)
|_http-server-header: Microsoft-HTTPAPI/2.0
|_http-title: Not Found
9389/tcp  open  mc-nmf       .NET Message Framing
49665/tcp open  msrpc        Microsoft Windows RPC
49666/tcp open  msrpc        Microsoft Windows RPC
49668/tcp open  msrpc        Microsoft Windows RPC
49677/tcp open  ncacn_http   Microsoft Windows RPC over HTTP 1.0
49678/tcp open  msrpc        Microsoft Windows RPC
49681/tcp open  msrpc        Microsoft Windows RPC
49708/tcp open  msrpc        Microsoft Windows RPC
49798/tcp open  msrpc        Microsoft Windows RPC
Service Info: Host: DC01; OS: Windows; CPE: cpe:/o:microsoft:windows

Host script results:
| smb-security-mode: 
|   account_used: guest
|   authentication_level: user
|   challenge_response: supported
|_  message_signing: required
| smb2-time: 
|   date: 2026-07-02T05:45:27
|_  start_date: 2026-06-01T23:46:40
| smb2-security-mode: 
|   3.1.1: 
|_    Message signing enabled and required
|_clock-skew: mean: 1s, deviation: 2s, median: 0s
| smb-os-discovery: 
|   OS: Windows Server 2016 Standard 14393 (Windows Server 2016 Standard 6.3)
|   Computer name: dc01
|   NetBIOS computer name: DC01\x00
|   Domain name: secura.yzx
|   Forest name: secura.yzx
|   FQDN: dc01.secura.yzx
|_  System time: 2026-07-02T05:45:29+00:00
```
```
$ nxc smb 192.168.62.97 -u Eric.Wallows -p EricLikesRunning800
SMB         192.168.62.97   445    DC01             [*] Windows Server 2016 Standard 14393 x64 (name:DC01) (domain:secura.yzx) (signing:True) (SMBv1:True) (Null Auth:True)
SMB         192.168.62.97   445    DC01             [+] secura.yzx\Eric.Wallows:EricLikesRunning800
```
```
$ smbclient //192.168.62.97/SYSVOL -U Eric.Wallows

smb: \secura.yzx\Policies\> dir
  .                                   D        0  Tue Oct 25 17:34:04 2022
  ..                                  D        0  Tue Oct 25 17:34:04 2022
  {31B2F340-016D-11D2-945F-00C04FB984F9}      D        0  Tue Oct 25 17:34:04 2022
  {6AC1786C-016F-11D2-945F-00C04FB984F9}      D        0  Tue Oct 25 17:34:04 2022

                10340607 blocks of size 4096. 6693356 blocks available
```
#### 这个 GPO 只是“强制开启 WinRM 服务”
```
smb: \secura.yzx\Policies\{31B2F340-016D-11D2-945F-00C04FB984F9}\MACHINE\> dir
  .                                   D        0  Tue Oct 25 17:37:25 2022
  ..                                  D        0  Tue Oct 25 17:37:25 2022
  comment.cmtx                        A      807  Tue Oct 25 17:36:34 2022
  Microsoft                           D        0  Tue Oct 25 17:34:04 2022
  Preferences                         D        0  Tue Oct 25 17:35:15 2022
  Registry.pol                        A     3842  Tue Oct 25 17:37:25 2022
  Scripts                             D        0  Tue Oct 25 17:36:39 2022

                10340607 blocks of size 4096. 6693340 blocks available
smb: \secura.yzx\Policies\{31B2F340-016D-11D2-945F-00C04FB984F9}\MACHINE\> cd Preferences
smb: \secura.yzx\Policies\{31B2F340-016D-11D2-945F-00C04FB984F9}\MACHINE\Preferences\> dir
  .                                   D        0  Tue Oct 25 17:35:15 2022
  ..                                  D        0  Tue Oct 25 17:35:15 2022
  Services                            D        0  Tue Oct 25 17:35:15 2022
```
```
$ cat Services.xml                            
<?xml version="1.0" encoding="utf-8"?>
<NTServices clsid="{2CFB484A-4E96-4b5d-A0B6-093D2F91E6AE}"><NTService clsid="{AB6F0B67-341F-4e51-92F9-005FBFBA1A43}" name="WinRM" image="2" changed="2022-10-25 17:35:16" uid="{401AC3E6-C47E-44A5-89E4-FC427698E07D}"><Properties startupType="AUTOMATIC" serviceName="WinRM" serviceAction="START" timeout="30"/></NTService>
</NTServices>
```
#### 
```
smb: \secura.yzx\Policies\{6AC1786C-016F-11D2-945F-00C04FB984F9}\MACHINE\Microsoft\Windows NT\SecEdit\> dir
  .                                   D        0  Tue Oct 25 17:34:04 2022
  ..                                  D        0  Tue Oct 25 17:34:04 2022
  GptTmpl.inf                         A     3740  Tue Oct 25 17:34:04 2022

                10340607 blocks of size 4096. 6693340 blocks available
```
| 文件           | 意义       |
| ------------ | -------- |
| Groups.xml   | 💣 直接密码  |
| Services.xml | ⚠️ 低概率密码 |
| GptTmpl.inf  | 🧠 权限策略  |
| Scripts      | 💣 可能密码  |
| Drives.xml   | 💣 可能密码  |

#### 关于 GptTmpl.inf
#### 1.本地管理员组[Group Membership]：*S-1-5-32-544 = Eric.Wallows 直接 = admin
#### 2.登录权限SeRemoteInteractiveLogonRight：Domain Users，Specific users，Admin groups
#### 3.明确风险“Everyone”，“Authenticated Users”，“Domain Users” -->说明权限可能很宽松
