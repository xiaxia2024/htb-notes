## EscapeTwo
```
[★]$ nmap -sC -sV 10.129.63.253 
Starting Nmap 7.94SVN ( https://nmap.org ) at 2026-01-16 02:18 CST
Nmap scan report for 10.129.63.253
Host is up (0.0096s latency).
Not shown: 988 filtered tcp ports (no-response)
PORT     STATE SERVICE       VERSION
53/tcp   open  domain        Simple DNS Plus
88/tcp   open  kerberos-sec  Microsoft Windows Kerberos (server time: 2026-01-16 08:19:05Z)
135/tcp  open  msrpc         Microsoft Windows RPC
139/tcp  open  netbios-ssn   Microsoft Windows netbios-ssn
389/tcp  open  ldap          Microsoft Windows Active Directory LDAP (Domain: sequel.htb0., Site: Default-First-Site-Name)
|_ssl-date: 2026-01-16T08:20:24+00:00; -3s from scanner time.
| ssl-cert: Subject: 
| Subject Alternative Name: DNS:DC01.sequel.htb, DNS:sequel.htb, DNS:SEQUEL
| Not valid before: 2025-06-26T11:46:45
|_Not valid after:  2124-06-08T17:00:40
445/tcp  open  microsoft-ds?
464/tcp  open  kpasswd5?
593/tcp  open  ncacn_http    Microsoft Windows RPC over HTTP 1.0
636/tcp  open  ssl/ldap      Microsoft Windows Active Directory LDAP (Domain: sequel.htb0., Site: Default-First-Site-Name)
|_ssl-date: 2026-01-16T08:20:24+00:00; -3s from scanner time.
| ssl-cert: Subject: 
| Subject Alternative Name: DNS:DC01.sequel.htb, DNS:sequel.htb, DNS:SEQUEL
| Not valid before: 2025-06-26T11:46:45
|_Not valid after:  2124-06-08T17:00:40
1433/tcp open  ms-sql-s      Microsoft SQL Server 2019 15.00.2000.00; RTM
| ms-sql-info: 
|   10.129.63.253:1433: 
|     Version: 
|       name: Microsoft SQL Server 2019 RTM
|       number: 15.00.2000.00
|       Product: Microsoft SQL Server 2019
|       Service pack level: RTM
|       Post-SP patches applied: false
|_    TCP port: 1433
| ssl-cert: Subject: commonName=SSL_Self_Signed_Fallback
| Not valid before: 2026-01-16T08:15:29
|_Not valid after:  2056-01-16T08:15:29
|_ssl-date: 2026-01-16T08:20:24+00:00; -3s from scanner time.
| ms-sql-ntlm-info: 
|   10.129.63.253:1433: 
|     Target_Name: SEQUEL
|     NetBIOS_Domain_Name: SEQUEL
|     NetBIOS_Computer_Name: DC01
|     DNS_Domain_Name: sequel.htb
|     DNS_Computer_Name: DC01.sequel.htb
|     DNS_Tree_Name: sequel.htb
|_    Product_Version: 10.0.17763
3268/tcp open  ldap          Microsoft Windows Active Directory LDAP (Domain: sequel.htb0., Site: Default-First-Site-Name)
|_ssl-date: 2026-01-16T08:20:24+00:00; -3s from scanner time.
| ssl-cert: Subject: 
| Subject Alternative Name: DNS:DC01.sequel.htb, DNS:sequel.htb, DNS:SEQUEL
| Not valid before: 2025-06-26T11:46:45
|_Not valid after:  2124-06-08T17:00:40
3269/tcp open  ssl/ldap      Microsoft Windows Active Directory LDAP (Domain: sequel.htb0., Site: Default-First-Site-Name)
| ssl-cert: Subject: 
| Subject Alternative Name: DNS:DC01.sequel.htb, DNS:sequel.htb, DNS:SEQUEL
| Not valid before: 2025-06-26T11:46:45
|_Not valid after:  2124-06-08T17:00:40
|_ssl-date: 2026-01-16T08:20:24+00:00; -3s from scanner time.
Service Info: Host: DC01; OS: Windows; CPE: cpe:/o:microsoft:windows

Host script results:
| smb2-security-mode: 
|   3:1:1: 
|_    Message signing enabled and required
| smb2-time: 
|   date: 2026-01-16T08:19:48
|_  start_date: N/A
|_clock-skew: mean: -3s, deviation: 0s, median: -3s
```
#### 加入域名
```
[★]$ echo '10.129.63.253 sequel.htb dc01.sequel.htb' | sudo tee -a /etc/hosts
10.129.63.253 sequel.htb dc01.sequel.htb
```
#### As is common in real life Windows pentests, you will start this box with credentials for the following account:
#### rose / KxEPkKe6R8su
```
[★]$ netexec smb 10.129.63.253 -u rose -p 'KxEPkKe6R8su' --shares
<SNIP>
SMB         10.129.63.253   445    DC01             [*] Windows 10 / Server 2019 Build 17763 x64 (name:DC01) (domain:sequel.htb) (signing:True) (SMBv1:False)
SMB         10.129.63.253   445    DC01             [+] sequel.htb\rose:KxEPkKe6R8su
SMB         10.129.63.253   445    DC01             [*] Enumerated shares
SMB         10.129.63.253   445    DC01             Share           Permissions     Remark
SMB         10.129.63.253   445    DC01             -----           -----------     ------
SMB         10.129.63.253   445    DC01             Accounting Department READ  
SMB         10.129.63.253   445    DC01             ADMIN$                          Remote Admin
SMB         10.129.63.253   445    DC01             C$                              Default share
SMB         10.129.63.253   445    DC01             IPC$            READ            Remote IPC
SMB         10.129.63.253   445    DC01             NETLOGON        READ            Logon server share
SMB         10.129.63.253   445    DC01             SYSVOL          READ            Logon server share
SMB         10.129.63.253   445    DC01             Users           READ
```
#### 我们看到我们已经读取了对会计部门共享的访问权限，我们继续进行枚举smbclient。
```
 [★]$ impacket-smbclient sequel.htb/rose:'KxEPkKe6R8su'@10.129.63.253
Impacket v0.13.0.dev0+20250130.104306.0f4b866 - Copyright Fortra, LLC and its affiliated companies 

Type help for list of commands
# whoami
*** Unknown syntax: whoami
# shares
Accounting Department
ADMIN$
C$
IPC$
NETLOGON
SYSVOL
Users
# use Accounting Department
# ls
drw-rw-rw-          0  Sun Jun  9 06:11:31 2024 .
drw-rw-rw-          0  Sun Jun  9 06:11:31 2024 ..
-rw-rw-rw-      10217  Sun Jun  9 06:11:31 2024 accounting_2024.xlsx
-rw-rw-rw-       6780  Sun Jun  9 06:11:31 2024 accounts.xlsx
# get accounting_2024.xlsx
# get accounts.xlsx
# exit
```
#### 查看内容，我们看到两个Excel表格。如果我们试图打开accounts.xlsx，我们就知道了它被破坏了
#### 使用file检查它的文件类型，我们看到它是一个zip文件。文件命令有帮助根据文件的内容而不是扩展名来确定文件的实际类型文件系统、魔法和语言测试
```
[★]$ file accounts.xlsx
accounts.xlsx: Zip archive data, made by v2.0, extract using at least v2.0, last modified, last modified Sun, Jun 09 2024 10:47:44, uncompressed size 681, method=deflate
[★]$ file accounting_2024.xlsx
accounting_2024.xlsx: Zip archive data, made by v4.5, extract using at least v2.0, last modified, last modified Sun, Jan 01 1980 00:00:00, uncompressed size 1284, method=deflate
```
#### 如果我们尝试使用7z（一个命令行归档实用程序）打开该文件，我们会遇到另一个错误以处理多种压缩格式而闻名，包括7z和ZIP。
```
★]$ 7z x accounts.xlsx

7-Zip [64] 16.02 : Copyright (c) 1999-2016 Igor Pavlov : 2016-05-21
p7zip Version 16.02 (locale=en_US.UTF-8,Utf16=on,HugeFiles=on,64 bits,128 CPUs AMD EPYC 7543 32-Core Processor                 (A00F11),ASM,AES-NI)

Scanning the drive for archives:
1 file, 6780 bytes (7 KiB)

Extracting archive: accounts.xlsx
WARNING:
accounts.xlsx
The archive is open with offset

--
Path = accounts.xlsx
Warning: The archive is open with offset
Type = zip
Physical Size = 6780

ERROR: Headers Error : xl/_rels/workbook.xml.rels
                                 
Sub items Errors: 1

Archives with Errors: 1

Sub items Errors: 1
```
#### 我们继续使用十六进制编辑器检查Excel文件的幻数，这是一个工具允许我们查看和编辑文件的原始二进制内容，我们看到它是50 48 04 03
```
[★]$ open accounts.xlsx  //乱码
```
#### 创建文件解压它
```
[★]$ mkdir accounts
[★]$ cd accounts/
[~/accounts][★]$ unzip ../accounts.xlsx   
[~/accounts][★]$ ls
'[Content_Types].xml'   docProps   _rels   xl
[~/accounts][★]$ cd xl
[~/accounts/xl][★]$ ls
sharedStrings.xml  styles.xml  theme  workbook.xml  worksheets

[~/accounts/xl][★]$ ls worksheets/
_rels  sheet1.xml
[~/accounts/xl][★]$ less worksheets/sheet1.xml
<SNIP>...
</sheetData><hyperlinks><hyperlink ref="C2" r:id="rId1" display="angela@sequel.htb"/><hyperlink ref="C3" r:id="rId2" display="oscar@sequel.htb"/><hyperlink ref="C4" r:id="rId3" display="kevin@sequel.htb"/><hyperlink ref="C5" r:id="rId4" display="sa@sequel.htb"/>
...</SNIP>

[~/accounts/xl][★]$ ls
sharedStrings.xml  styles.xml  theme  workbook.xml  worksheets
[~/accounts/xl][★]$ less sharedStrings.xml
<SNIP>...
<si><t xml:space="preserve">kevin@sequel.htb</t></si><si><t xml:space="preserve">kevin</t></si><si><t xml:space="preserve">Md9Wlq1E5bZnVDVo</t></si><si><t xml:space="preserve">NULL</t></si><si><t xml:space="preserve">sa@sequel.htb</t></si><si><t xml:space="preserve">sa</t></si><si><t xml:space="preserve">MSSQLP@ssw0rd!</t></si>
..</SNIP>

[~/accounts/xl][★]$ cat sharedStrings.xml | sed 's/space="preserve">/\n/g'
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<sst xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main" count="25" uniqueCount="24"><si><t xml:
First Name</t></si><si><t xml:
Last Name</t></si><si><t xml:
</SNIP>

[~/accounts/xl][★]$ cat sharedStrings.xml | sed 's/space="preserve">/\n/g' | sed 's/<.*//g' > ../../output.txt
[★]$ grep -E '^[a-zA-Z0-9._%+-]+@sequel\.htb$|^[A-Za-z0-9@!#\$%\^&\*]+' ../../output.txt | paste - - - | awk '{print $1":"$3}'
First:Last
Username:Angela
Martin:angela
0fwz7Q4mSpurIt99:Martinez
oscar@sequel.htb:86LxLBMgEWaKUnBG
Kevin:kevin@sequel.htb
kevin:NULL
sa@sequel.htb:MSSQLP@ssw0rd!
```
```
[★]$ grep -E '^[a-zA-Z0-9._%+-]+@sequel\.htb$|^[A-Za-z0-9@!#\$%\^&\*]+' ../../output.txt \
| paste - - - \
| sed '1,2d; s/ .*/:&/; s/:.* //'
Martin	angela@sequel.htb	angela
0fwz7Q4mSpurIt99	Oscar	Martinez
oscar@sequel.htb	oscar	86LxLBMgEWaKUnBG
Kevin	Malone	kevin@sequel.htb
kevin	Md9Wlq1E5bZnVDVo	NULL
sa@sequel.htb	sa	MSSQLP@ssw0rd!
```
#### 有些用户是没有密码的，能不能从 ‘邮箱用户名密码’ 这样的结构里提取{print $1":"$3}
```
[★]$ awk '
/@sequel\.htb$/ {
    email = $0
    pass = ""
    next
} 

/^[A-Za-z0-9@!#\$%\^&\*]{6,}$/ && email {
    pass = $0
    print email ":" pass
    email = ""
}
' output.txt
angela@sequel.htb:angela
oscar@sequel.htb:86LxLBMgEWaKUnBG
kevin@sequel.htb:Md9Wlq1E5bZnVDVo
sa@sequel.htb:MSSQLP@ssw0rd!

//看到邮箱 → 保存到变量 ； next：继续读下一行 ； 此时 awk 进入“等待 password”状态
//至少 6 位； 包含特殊字符（弱排除名字）； 并且必须已经捕获 email； 防止串到下一个用户
```
