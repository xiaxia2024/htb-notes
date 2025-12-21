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

【2】打开Parallels Desktop的win11, Win + R：输入mstsc,连接远程计算机
在连接时，我们看到一条消息，说明我们可以使用用户名KioskUser0和没有密码。
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
