## Bruno
```
[★]$ ports=$(nmap -p- --min-rate=1000 -T4 10.129.6.204 | grep ^[0-9] | cut -d '/' -f 1 | tr '\n' ',' | sed s/,$//)
[★]$ nmap -p$ports -sC -sV 10.129.6.204
Starting Nmap 7.94SVN ( https://nmap.org ) at 2026-03-13 03:07 CDT
Nmap scan report for brunodc.bruno.vl (10.129.6.204)
Host is up (0.30s latency).

PORT      STATE SERVICE       VERSION
21/tcp    open  ftp           Microsoft ftpd
| ftp-syst: 
|_  SYST: Windows_NT
| ftp-anon: Anonymous FTP login allowed (FTP code 230)
| 06-29-22  04:55PM       <DIR>          app
| 06-29-22  04:33PM       <DIR>          benign
| 06-29-22  01:41PM       <DIR>          malicious
|_06-29-22  04:33PM       <DIR>          queue
53/tcp    open  domain        Simple DNS Plus
80/tcp    open  http          Microsoft IIS httpd 10.0
|_http-title: IIS Windows Server
| http-methods: 
|_  Potentially risky methods: TRACE
|_http-server-header: Microsoft-IIS/10.0
88/tcp    open  kerberos-sec  Microsoft Windows Kerberos (server time: 2026-03-13 08:07:29Z)
135/tcp   open  msrpc         Microsoft Windows RPC
139/tcp   open  netbios-ssn   Microsoft Windows netbios-ssn
389/tcp   open  ldap          Microsoft Windows Active Directory LDAP (Domain: bruno.vl0., Site: Default-First-Site-Name)
| ssl-cert: Subject: 
| Subject Alternative Name: DNS:brunodc.bruno.vl, DNS:bruno.vl, DNS:BRUNO
| Not valid before: 2025-10-09T09:54:08
|_Not valid after:  2105-10-09T09:54:08
|_ssl-date: 2026-03-13T08:09:02+00:00; 0s from scanner time.
443/tcp   open  ssl/http      Microsoft IIS httpd 10.0
|_http-server-header: Microsoft-IIS/10.0
| http-methods: 
|_  Potentially risky methods: TRACE
|_ssl-date: TLS randomness does not represent time
| ssl-cert: Subject: commonName=bruno-BRUNODC-CA
| Not valid before: 2022-06-29T13:23:01
|_Not valid after:  2121-06-29T13:33:00
|_http-title: IIS Windows Server
| tls-alpn: 
|_  http/1.1
445/tcp   open  microsoft-ds?
464/tcp   open  kpasswd5?
593/tcp   open  ncacn_http    Microsoft Windows RPC over HTTP 1.0
636/tcp   open  ssl/ldap
| ssl-cert: Subject: 
| Subject Alternative Name: DNS:brunodc.bruno.vl, DNS:bruno.vl, DNS:BRUNO
| Not valid before: 2025-10-09T09:54:08
|_Not valid after:  2105-10-09T09:54:08
|_ssl-date: 2026-03-13T08:09:02+00:00; +1s from scanner time.
3268/tcp  open  ldap          Microsoft Windows Active Directory LDAP (Domain: bruno.vl0., Site: Default-First-Site-Name)
|_ssl-date: 2026-03-13T08:09:02+00:00; 0s from scanner time.
| ssl-cert: Subject: 
| Subject Alternative Name: DNS:brunodc.bruno.vl, DNS:bruno.vl, DNS:BRUNO
| Not valid before: 2025-10-09T09:54:08
|_Not valid after:  2105-10-09T09:54:08
3269/tcp  open  ssl/ldap      Microsoft Windows Active Directory LDAP (Domain: bruno.vl0., Site: Default-First-Site-Name)
|_ssl-date: 2026-03-13T08:09:02+00:00; +1s from scanner time.
| ssl-cert: Subject: 
| Subject Alternative Name: DNS:brunodc.bruno.vl, DNS:bruno.vl, DNS:BRUNO
| Not valid before: 2025-10-09T09:54:08
|_Not valid after:  2105-10-09T09:54:08
3389/tcp  open  ms-wbt-server Microsoft Terminal Services
| rdp-ntlm-info: 
|   Target_Name: BRUNO
|   NetBIOS_Domain_Name: BRUNO
|   NetBIOS_Computer_Name: BRUNODC
|   DNS_Domain_Name: bruno.vl
|   DNS_Computer_Name: brunodc.bruno.vl
|   DNS_Tree_Name: bruno.vl
|   Product_Version: 10.0.20348
|_  System_Time: 2026-03-13T08:08:22+00:00
| ssl-cert: Subject: commonName=brunodc.bruno.vl
| Not valid before: 2026-03-12T07:54:56
|_Not valid after:  2026-09-11T07:54:56
|_ssl-date: 2026-03-13T08:09:02+00:00; +1s from scanner time.
49664/tcp open  msrpc         Microsoft Windows RPC
49669/tcp open  msrpc         Microsoft Windows RPC
54566/tcp open  msrpc         Microsoft Windows RPC
54571/tcp open  msrpc         Microsoft Windows RPC
54707/tcp open  msrpc         Microsoft Windows RPC
57617/tcp open  ncacn_http    Microsoft Windows RPC over HTTP 1.0
57618/tcp open  msrpc         Microsoft Windows RPC
Service Info: Host: BRUNODC; OS: Windows; CPE: cpe:/o:microsoft:windows

Host script results:
| smb2-security-mode: 
|   3:1:1: 
|_    Message signing enabled and required
| smb2-time: 
|   date: 2026-03-13T08:08:26
|_  start_date: N/A
```
```
[★]$ echo '10.129.6.204 brunodc.bruno.vl bruno.vl' | sudo tee -a /etc/hosts
10.129.6.204 brunodc.bruno.vl bruno.vl
```
#### FTP 
```
[★]$ ftp bruno.vl
Connected to brunodc.bruno.vl.
220 Microsoft FTP Service
Name (bruno.vl:root): anonymous
331 Anonymous access allowed, send identity (e-mail name) as password.
Password: 
230 User logged in.
Remote system type is Windows_NT.
ftp> ls
229 Entering Extended Passive Mode (|||55040|)
125 Data connection already open; Transfer starting.
06-29-22  04:55PM       <DIR>          app
06-29-22  04:33PM       <DIR>          benign
06-29-22  01:41PM       <DIR>          malicious
06-29-22  04:33PM       <DIR>          queue
226 Transfer complete.
ftp> cd app
250 CWD command successful.
ftp> ls
229 Entering Extended Passive Mode (|||55050|)
150 Opening ASCII mode data connection.
06-29-22  05:42PM                  165 changelog
06-28-22  07:15PM                  431 SampleScanner.deps.json
06-29-22  03:58PM                 7168 SampleScanner.dll
06-29-22  03:58PM               174592 SampleScanner.exe
06-28-22  07:15PM                  170 SampleScanner.runtimeconfig.dev.json
06-28-22  07:15PM                  154 SampleScanner.runtimeconfig.json
226 Transfer complete.

ftp> cd benign
250 CWD command successful.
ftp> ls
229 Entering Extended Passive Mode (|||55056|)
150 Opening ASCII mode data connection.
06-29-22  04:32PM                    4 test.exe
226 Transfer complete.

ftp> cd malicious
250 CWD command successful.
ftp> ls
229 Entering Extended Passive Mode (|||55101|)
125 Data connection already open; Transfer starting.
226 Transfer complete.

ftp> cd queue
250 CWD command successful.
ftp> ls
229 Entering Extended Passive Mode (|||55123|)
150 Opening ASCII mode data connection.
226 Transfer complete.
```
#### malicious恶意文件夹和queue队列似乎为空。良性文件夹中仅有一个名为 test.exe 的文件。之后
#### 下载下来后，我们可以看到它并非可执行文件，而是一个包含字符串“123\n”的 ASCII 文本文件。
```
[★]$ file test.exe
test.exe: ASCII text
[★]$ cat test.exe
123
```
#### 这让我们把目光转向“应用”文件夹，它也是包含文件最多的文件夹。
### 分析 SampleScanner .NET 应用程序
```
[★]$ file changelog
changelog: ASCII text
[★]$ cat changelog
Version 0.3
- integrated with dev site
- automation using svc_scan

Version 0.2
- additional functionality 

Version 0.1
- initial support for EICAR string
```
#### 它并未提供有关该应用程序的性质和功能的太多信息。然而，“使用 svc_scan 的自动化”这一条提示我们存在一个可能用于通过此应用程序进行自动化操作的账户。
#### 从 SampleScanner.deps.json 文件中，我们可以获取到该可执行文件所使用的 .NET 版本，即 3.1 。我们还能看到它加载了 SampleScanner.dll 组件。
```
[★]$ file SampleScanner.deps.json
SampleScanner.deps.json: JSON text data
[★]$ cat  SampleScanner.deps.json
{
  "runtimeTarget": {
    "name": ".NETCoreApp,Version=v3.1",
    "signature": ""
  },
  "compilationOptions": {},
  "targets": {
    ".NETCoreApp,Version=v3.1": {
      "SampleScanner/1.0.0": {
        "runtime": {
          "SampleScanner.dll": {}
        }
      }
    }
  },
  "libraries": {
    "SampleScanner/1.0.0": {
      "type": "project",
      "serviceable": false,
      "sha512": ""
    }
  }
}
```
#### 从 SampleScanner.runtimeconfig.dev.json 文件中，我们可以获取到一个用户名--xct 。
```
[★]$ file SampleScanner.runtimeconfig.dev.json
SampleScanner.runtimeconfig.dev.json: JSON text data
[★]$ cat  SampleScanner.runtimeconfig.dev.json
{
  "runtimeOptions": {
    "additionalProbingPaths": [
      "C:\\Users\\xct\\.dotnet\\store\\|arch|\\|tfm|",
      "C:\\Users\\xct\\.nuget\\packages"
    ]
  }
}
```
#### 由于我们知道.NET 程序集会被编译成中间语言，所以我们将使用“文件”命令来确认我们获取的可执行文件和.dll 文件中哪些是.NET 程序集。
```
[★]$ file SampleScanner.dll
SampleScanner.dll: MS-DOS executable
[★]$ file SampleScanner.exe
SampleScanner.exe: MS-DOS executable
```
#### 该 dll 实际上是一个 .Net 程序集，因此我们可以使用像 dnspy 这样的工具来解编这个程序集并获取其库的源代码。而另一个可执行文件则是以原生 x64 机器代码编译的，这意味着我们不得不采用静态和动态分析的方式来理解其工作流程。不过，这种情况可能并非总是需要的——该可执行文件很可能只是 dll 的加载器，其中将包含工作流程的主要功能。
#### 如果我们在自己控制的 Windows 电脑上尝试运行 SampleScanner.exe，就会出现以下错误（除非我们已经安装了.NET 3.1.0 版本）：
#### 在访问了所提供的链接并下载了目标框架后，我们运行安装程序。然后，我们可以再次运行该可执行文件，这次就不会出现错误了。如果我们使用工具“进程监视器”，并仅筛选出显示与“进程名称为 SampleScanner.exe”相匹配的结果，例如这样：
https://learn.microsoft.com/en-us/sysinternals/downloads/procmon
#### 我们可以看到，该可执行文件确实加载了 SampleScanner.dll 文件。
#### 现在我们可以对 SampleScanner.dll 进行反编译，并查看源代码以找出任何我们能够利用的潜在漏洞。
