## 遇到的问题
-------------------------------------------------------
### Challenge 0 - Secura
<details>
<summary>$ nxc smb 192.168.238.95 -u 'Eric.Wallows' -p 'EricLikesRunning800' -M lsassy</summary>
  
```
┌──(syareya55㉿kali)-[~]
└─$ nxc smb 192.168.238.95 -u 'Eric.Wallows' -p 'EricLikesRunning800' -M lsassy --no-smb
SMB         192.168.238.95  445    SECURE           [*] Windows 10 / Server 2019 Build 19041 x64 (name:SECURE) (domain:secura.yzx) (signing:False) (SMBv1:None)                                                                        
SMB         192.168.238.95  445    SECURE           [+] secura.yzx\Eric.Wallows:EricLikesRunning800 (Pwn3d!)
[03:17:49] ERROR    Exception while calling proto_flow() on connection.py:187
                    target 192.168.238.95: [Errno 32]                        
                    Broken pipe
```

</details>

#### 解决的方式是：在OSCP官方的MyKali上运作
-------------------------------------------------------
### Challenge 1 - Medtech

<details>
<summary>C:\Users\Administrator\Desktop>net user /domain</summary>
  
```
C:\Users\Administrator\Desktop>net user /domain
net user /domain
The request will be processed at a domain controller for domain medtech.com.

System error 1355 has occurred.

The specified domain either does not exist or could not be contacted.
```

</details>

#### 解决的方式是：换个靶机打
-------------------------------------------------------
### Challenge 4 OSCP A

<details>
<summary>pspy64s</summary>
  
```
aero@oscp:/etc$ scp kali@192.168.49.54:~/Downloads/pspy64s /tmp/                                                             
kali@192.168.49.54's password:                                                                                               
pspy64s                                       100% 1205KB 155.0MB/s   00:00                                                  
aero@oscp:/etc$ ls /tmp/pspy64s                                                                                              
/tmp/pspy64s

aero@oscp:/etc$ chmod +x /tmp/pspy64s
aero@oscp:/etc$ /tmp/pspy64s
/tmp/pspy64s: /lib/x86_64-linux-gnu/libc.so.6: version `GLIBC_2.32' not found (required by /tmp/pspy64s)
/tmp/pspy64s: /lib/x86_64-linux-gnu/libc.so.6: version `GLIBC_2.34' not found (required by /tmp/pspy64s)
aero@oscp:/etc$ 
```

</details>

<details>
<summary>手动搜索代替pspy64s</summary>

```
-------------------------------------------------------
aero@oscp:/etc$ systemctl list-timers --all
NEXT         LEFT         LAST          PASSED       UNIT          ACTIVATES    
Mon 2026-07? 2s left      Mon 2026-07-? 15s ago      aerocheck.ti? aerocheck.se?
Mon 2026-07? 16min left   Mon 2026-07-? 13min ago    phpsessioncl? phpsessioncl?
Mon 2026-07? 2h 23min le? Mon 2026-07-? 1h 20min ago apt-daily-up? apt-daily-up?
Mon 2026-07? 5h 49min le? Mon 2026-07-? 43min ago    ua-timer.tim? ua-timer.ser?
Mon 2026-07? 13h left     Mon 2026-07-? 1h 20min ago apt-daily.ti? apt-daily.se?
Mon 2026-07? 14h left     Mon 2026-07-? 1h 20min ago fwupd-refres? fwupd-refres?
Mon 2026-07? 15h left     Mon 2026-07-? 1h 20min ago motd-news.ti? motd-news.se?
Tue 2026-07? 20h left     Mon 2026-07-? 1h 20min ago logrotate.ti? logrotate.se?
Tue 2026-07? 20h left     Mon 2026-07-? 1h 20min ago man-db.timer  man-db.servi?
Tue 2026-07? 22h left     Mon 2026-07-? 1h 8min ago  systemd-tmpf? systemd-tmpf?
Sun 2026-07? 5 days left  Mon 2026-07-? 1h 20min ago e2scrub_all.? e2scrub_all.?
Mon 2026-07? 6 days left  Mon 2026-07-? 1h 20min ago fstrim.timer  fstrim.servi?
n/a          n/a          n/a           n/a          aero.timer                 
n/a          n/a          n/a           n/a          snapd.snap-r? snapd.snap-r?
n/a          n/a          n/a           n/a          ua-license-c? ua-license-c?
-------------------------------------------------------
# /etc/systemd/system/aerocheck.timer
[Unit]
Description=Aerospike check timer
Requires=aerocheck.service

[Timer]
Unit=aerocheck.service
OnCalendar=*:*:0/20

[Install]
WantedBy=timers.target
aero@oscp:/etc$ systemctl cat aerocheck.service
# /etc/systemd/system/aerocheck.service
[Unit]
Description=Aerospike live check
After=network.target
Wants=aero.timer

[Service]
Type=oneshot
User=root
ExecStart=/usr/bin/bash /root/aerospike.sh

[Install]
WantedBy=multi-user.target
aero@oscp:/etc$ 
-------------------------------------------------------
aero@oscp:/etc$ ls -la /root/aerospike.sh
ls: cannot access '/root/aerospike.sh': Permission denied
aero@oscp:/etc$ ls -ld /root
drwx------ 9 root root 4096 Jul 20 02:31 /root
aero@oscp:/etc$ cat /root/aerospike.sh
cat: /root/aerospike.sh: Permission denied
-------------------------------------------------------
```

</details>

<details>
<summary>kali自带的pspy</summary>

```
$ sudo apt install pspy  

$ pspy-binaries

> pspy ~ Monitor Linux processes without root permissions

/usr/share/pspy
??? pspy32
??? pspy32s
??? pspy64
??? pspy64s
```
```
aero@oscp:/etc$ scp http://192.168.49.56:/usr/share/pspy/pspy64s /tmp/

aero@oscp:/tmp$ ./pspy64s
./pspy64s: /lib/x86_64-linux-gnu/libc.so.6: version `GLIBC_2.34' not found (required by ./pspy64s)
./pspy64s: /lib/x86_64-linux-gnu/libc.so.6: version `GLIBC_2.32' not found (required by ./pspy64s)

aero@oscp:/etc$ scp kali@192.168.49.56:/usr/share/pspy/pspy64 /tmp/
  
aero@oscp:/tmp$ ./pspy64
./pspy64: /lib/x86_64-linux-gnu/libc.so.6: version `GLIBC_2.34' not found (required by ./pspy64)
./pspy64: /lib/x86_64-linux-gnu/libc.so.6: version `GLIBC_2.32' not found (required by ./pspy64)
```

</details>

<details>
<summary>自己编译pspy,but OSCP的MyKali不能编译go</summary>

```
$ git clone https://github.com/DominicBreuker/pspy.git

$ cd pspy

$ ls
cmd  docker  go.mod  go.sum  images  internal  LICENSE  main.go  Makefile  README.md

$ sudo apt install golang-go

$ go build -o pspy64 .
             
go: downloading github.com/spf13/cobra v1.4.0
go: downloading golang.org/x/sys v0.0.0-20220520151302-bc2c85ada10a
internal/fswatcher/inotify/event.go:3:8: golang.org/x/sys@v0.0.0-20220520151302-bc2c85ada10a: Get "https://proxy.golang.org/golang.org/x/sys/@v/v0.0.0-20220520151302-bc2c85ada10a.zip": dial tcp 64.233.178.141:443: i/o timeout
cmd/root.go:16:2: github.com/spf13/cobra@v1.4.0: Get "https://proxy.golang.org/github.com/spf13/cobra/@v/v1.4.0.zip": dial tcp 64.233.178.141:443: i/o timeout

$ go env -w GOPROXY=https://goproxy.cn,direct

$ go env GOPROXY                             
https://goproxy.cn,direct
[如果你是在中国大陆，切换到 https://goproxy.cn,direct 往往可以解决问题；如果你是在使用 OSCP VPN]
```

</details>

<details>
<summary>/usr/share/pspy/pspy32s </summary>

```
aero@oscp:/$ scp kali@192.168.49.56:/usr/share/pspy/pspy32s /tmp/
kali@192.168.49.56's password: 
pspy32s                                       100% 1272KB 134.1MB/s   00:00    
aero@oscp:/$ cd /tmp
aero@oscp:/tmp$ ./pspy32s


2026/07/21 03:24:08 CMD: UID=0     PID=10560  | python2.7 /opt/aerospike/bin/asadm --asinfo-mode -e 'STATUS'                                                                                                        
2026/07/21 03:24:08 CMD: UID=0     PID=10561  | python2.7 /opt/aerospike/bin/asadm --asinfo-mode -e 'STATU
```

</details>

-------------------------------------------------------
