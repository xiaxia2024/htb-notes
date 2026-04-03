## Gavel
```
[★]$ nmap -sCV 10.129.242.203
Starting Nmap 7.94SVN ( https://nmap.org ) at 2026-04-03 04:12 CDT
Nmap scan report for 10.129.242.203
Host is up (0.011s latency).
Not shown: 998 closed tcp ports (reset)
PORT   STATE SERVICE VERSION
22/tcp open  ssh     OpenSSH 8.9p1 Ubuntu 3ubuntu0.13 (Ubuntu Linux; protocol 2.0)
| ssh-hostkey: 
|   256 1f:de:9d:84:bf:a1:64:be:1f:36:4f:ac:3c:52:15:92 (ECDSA)
|_  256 70:a5:1a:53:df:d1:d0:73:3e:9d:90:ad:c1:aa:b4:19 (ED25519)
80/tcp open  http    Apache httpd 2.4.52
|_http-server-header: Apache/2.4.52 (Ubuntu)
|_http-title: Did not follow redirect to http://gavel.htb/
Service Info: Host: gavel.htb; OS: Linux; CPE: cpe:/o:linux:linux_kernel

Service detection performed. Please report any incorrect results at https://nmap.org/submit/ .
Nmap done: 1 IP address (1 host up) scanned in 7.23 seconds

[★]$ echo '10.129.242.203 gavel.htb' | sudo tee -a /etc/hosts
10.129.242.203 gavel.htb
```

```
[★]$ ffuf -w /usr/share/seclists/Discovery/Web-Content/common.txt -u http://gavel.htb/FUZZ

        /'___\  /'___\           /'___\       
       /\ \__/ /\ \__/  __  __  /\ \__/       
       \ \ ,__\\ \ ,__\/\ \/\ \ \ \ ,__\      
        \ \ \_/ \ \ \_/\ \ \_\ \ \ \ \_/      
         \ \_\   \ \_\  \ \____/  \ \_\       
          \/_/    \/_/   \/___/    \/_/       

       v2.1.0-dev
________________________________________________

 :: Method           : GET
 :: URL              : http://gavel.htb/FUZZ
 :: Wordlist         : FUZZ: /usr/share/seclists/Discovery/Web-Content/common.txt
 :: Follow redirects : false
 :: Calibration      : false
 :: Timeout          : 10
 :: Threads          : 40
 :: Matcher          : Response status: 200-299,301,302,307,401,403,405,500
________________________________________________
:: Progress: [1/4723] :: Job [1/1] :: 0 req/sec :: Duration: [0:00:00] :: Errors
.htaccess               [Status: 403, Size: 274, Words: 20, Lines: 10, Duration: 11ms]
.git/logs/              [Status: 200, Size: 1128, Words: 77, Lines: 18, Duration: 54ms]
.git/index              [Status: 200, Size: 224718, Words: 313, Lines: 355, Duration: 45ms]
admin.php               [Status: 302, Size: 0, Words: 1, Lines: 1, Duration: 21ms]
assets                  [Status: 301, Size: 307, Words: 20, Lines: 10, Duration: 8ms]
.git/HEAD               [Status: 200, Size: 23, Words: 2, Lines: 2, Duration: 900ms]
.hta                    [Status: 403, Size: 274, Words: 20, Lines: 10, Duration: 906ms]
.htpasswd               [Status: 403, Size: 274, Words: 20, Lines: 10, Duration: 1447ms]
index.php               [Status: 200, Size: 13951, Words: 4616, Lines: 223, Duration: 12ms]
.git/config             [Status: 200, Size: 136, Words: 13, Lines: 9, Duration: 1788ms]
.git                    [Status: 301, Size: 305, Words: 20, Lines: 10, Duration: 1791ms]
rules                   [Status: 301, Size: 306, Words: 20, Lines: 10, Duration: 16ms]
server-status           [Status: 403, Size: 274, Words: 20, Lines: 10, Duration: 8ms]
```
