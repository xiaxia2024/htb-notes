1.nc64.exe https://github.com/int0x33/nc.exe/tree/master

2.[★]$ wget https://github.com/vinsworldcom/NetCat64/releases/download/1.11.6.4/nc64.exe

```
-M adcs :objectClass=pKIEnrollmentService

Retro:ESC1 //创建用户，任意用户可以拿到全句的票据

Fluffy:ESC16
```

#### 关于 文件签名（file signatures）的列表和解释：
https://en.wikipedia.org/wiki/List_of_file_signatures
| Hex Signature | Meaning      | Extension |             |
| ------------- | ------------ | --------- | ----------- |
| `FF D8 FF E0` | JPEG image   | `.jpg`    |             |
| `25 50 44 46` | PDF document | `.pdf`    |             |

#### Minecraft关于Log4j的开发[Java库的名称log4j]from Crafty
```
[1]我的世界https://github.com/MCCTeam/Minecraft-Console-Client
[2]Java SE 8 Archive Downloads (JDK 8u202 and earlier)：
https://www.oracle.com/java/technologies/javase/javase8-archive-downloads.html
[3][★]$ git  clone https://github.com/kozmer/log4j-shell-poc.git
[4]在线反编译器来反编译插件返回到源代码https://www.javadecompilers.com/
[5]为了在没有适当shell的情况下使用RunAs特性，我们利用runasc
https://github.com/antonioCoco/RunasCs
[★]$ wget https://github.com/antonioCoco/RunasCs/releases/download/v1.5/RunasCs.zip
```

#### Apache Directory Studio = 图形化 LDAP 管理工具
https://directory.apache.org/studio/

#### 漏洞为BloodHound里面的点击GenericAll的右键Help，基于资源的约束授权（RBCD），S4U攻击得到票据，使用Impacket包 | form Support

### LAPS_Readers：
#### 有权限读取 LAPS 管理的本地管理员密码；LAPS 的 PowerShell 模块：AdmPwd.PS | form Timelapse
```
[★]$ zip2john winrm_backup.zip > zip.john
[★]$ john zip.john -wordlist:rockyou.txt
[★]$ unzip winrm_backup.zip

[★]$ python3 /usr/share/john/pfx2john.py legacyy_dev_auth.pfx > pfx.john
[★]$ john pfx.john -wordlist:rockyou.txt

[★]$ openssl pkcs12 -in legacyy_dev_auth.pfx -nocerts -out key.pem -nodes
[★]$ openssl pkcs12 -in legacyy_dev_auth.pfx -nokeys -out cert.pem

//PowerShell 历史记录文件的完整路径是什么？（从哪个路径开始$env:？）
$env:APPDATA\Microsoft\Windows\PowerShell\PSReadLine\ConsoleHost_history.txt
```
### Server Operators:
#### Metasploit Framework（MSF） 是一个开源的 渗透测试与漏洞利用框架（penetration testing framework）| from Return、Deiver、Love
#### 相同的特点是IIS是打印机，一个是Updata,一个是Submit
#### PowerShell的历史文件路径
```
cat C:\Users\tony\AppData\Roaming\Microsoft\Windows\PowerShell\PSReadline\ConsoleHost_history.txt
```
_____________________________
```
PowerShell 里：
Get-ChildItem -Path C:\ -Filter root.txt -Recurse -ErrorAction SilentlyContinue
CMD 里：
dir C:\root.txt /s
```
_____________________________
### //Windows AppLocker（应用程序白名单）策略 :
#### PS C:\xampp\htdocs\omrs\images> get-applockerpolicy -effective | select -expandproperty rulecollections  
_____________________________
### Docker-Toolbox 运行的是 boot2docker Linux 发行版。
#### 该发行版中 docker 用户的默认密码是什么？docker/tcuser
#### docker-toolbox有默认情况下访问c:\Users文件夹，该文件夹挂载在/c/Users 
#### 关于sqlmap | from Toolbox
