## Lock

```
[★]$ nmap -sC -sV 10.129.234.64
PORT     STATE SERVICE       VERSION
80/tcp   open  http          Microsoft IIS httpd 10.0
|_http-server-header: Microsoft-IIS/10.0
|_http-title: Lock - Index
| http-methods: 
|_  Potentially risky methods: TRACE
445/tcp  open  microsoft-ds?
3000/tcp open  ppp?
| fingerprint-strings: 
|   GenericLines, Help, RTSPRequest: 
|     HTTP/1.1 400 Bad Request
|     Content-Type: text/plain; charset=utf-8
|     Connection: close
|     Request
|   GetRequest: 
|     HTTP/1.0 200 OK
|     Cache-Control: max-age=0, private, must-revalidate, no-transform
|     Content-Type: text/html; charset=utf-8
|     Set-Cookie: i_like_gitea=04cd3721ed688f91; Path=/; HttpOnly; SameSite=Lax
|     Set-Cookie: _csrf=VDY9p6n1pQGQS70C-BmJ7gZt3GE6MTc2NDU3OTU4ODI1NjIzNTgwMA; Path=/; Max-Age=86400; HttpOnly; SameSite=Lax
|     X-Frame-Options: SAMEORIGIN
|     Date: Mon, 01 Dec 2025 08:59:48 GMT
|     <!DOCTYPE html>
|     <html lang="en-US" class="theme-auto">
|     <head>
|     <meta name="viewport" content="width=device-width, initial-scale=1">
|     <title>Gitea: Git with a cup of tea</title>
|     <link rel="manifest" href="data:application/json;base64,eyJuYW1lIjoiR2l0ZWE6IEdpdCB3aXRoIGEgY3VwIG9mIHRlYSIsInNob3J0X25hbWUiOiJHaXRlYTogR2l0IHdpdGggYSBjdXAgb2YgdGVhIiwic3RhcnRfdXJsIjoiaHR0cDovL2xvY2FsaG9zdDozMDAwLyIsImljb25zIjpbeyJzcmMiOiJodHRwOi8vbG9jYWxob3N0OjMwMDAvYXNzZXRzL2ltZy9sb2dvLnBuZyIsInR5cGUiOiJpbWFnZS9wbmciLCJzaXplcyI6IjU
|   HTTPOptions: 
|     HTTP/1.0 405 Method Not Allowed
|     Allow: HEAD
|     Allow: GET
|     Cache-Control: max-age=0, private, must-revalidate, no-transform
|     Set-Cookie: i_like_gitea=9093c093174715d3; Path=/; HttpOnly; SameSite=Lax
|     Set-Cookie: _csrf=sthmsRVkwzb4U-GzAxQf6zS9zQU6MTc2NDU3OTU5MzYzMDQ2ODYwMA; Path=/; Max-Age=86400; HttpOnly; SameSite=Lax
|     X-Frame-Options: SAMEORIGIN
|     Date: Mon, 01 Dec 2025 08:59:53 GMT
|_    Content-Length: 0
3389/tcp open  ms-wbt-server Microsoft Terminal Services
| rdp-ntlm-info: 
|   Target_Name: LOCK
|   NetBIOS_Domain_Name: LOCK
|   NetBIOS_Computer_Name: LOCK
|   DNS_Domain_Name: Lock
|   DNS_Computer_Name: Lock
|   Product_Version: 10.0.20348
|_  System_Time: 2025-12-01T09:01:09+00:00
|_ssl-date: 2025-12-01T09:01:49+00:00; 0s from scanner time.
| ssl-cert: Subject: commonName=Lock
| Not valid before: 2025-11-30T08:56:44
|_Not valid after:  2026-06-01T08:56:44
1 service unrecognized despite returning data. If you know the service/version, please submit the following fingerprint at 
Service Info: OS: Windows; CPE: cpe:/o:microsoft:windows

Host script results:
| smb2-time: 
|   date: 2025-12-01T09:01:13
|_  start_date: N/A
| smb2-security-mode: 
|   3:1:1: 
|_    Message signing enabled but not required
```
#### 访问80没有什么，访问端口3000,点击Expore
![图片](image/121301.png)
#### 点击Explore，我们会看到一个名为dev-scripts的存储库，它属于ellen.freeman。的存储库是用Python编写的。
#### 在dev-scripts存储库中，我们看到一个名为repos.py的文件
#### 回顾rerepository .py文件的内容，我们注意到个人访问令牌是硬编码的直接放入脚本中。
```
//这个脚本的作用是通过 Gitea API 获取用户的仓库列表。
import requests //用于发送 HTTP 请求（GET/POST 等）
import sys //用于访问命令行参数和退出程序等功能
import os //用于访问环境变量等操作

def format_domain(domain): //用于标准化用户输入的域名
    if not domain.startswith(('http://', 'https://')): //检查 domain 是否以 http:// 或 https:// 开头
        domain = 'https://' + domain //如果没有，就自动在前面加上 https://（默认使用 HTTPS）
    return domain //返回处理后的完整域名

def get_repositories(token, domain): //来获取用户仓库列表
    headers = { ////使用个人访问令牌进行授权，HTTP 请求头里添加 Authorization: token <token>
        'Authorization': f'token {token}' 
    }   
    url = f'{domain}/api/v1/user/repos' //拼接 API URL，这是 Gitea 官方提供的获取当前用户仓库的接口
    response = requests.get(url, headers=headers) //发送 GET 请求到该 URL，并附带认证头

    if response.status_code == 200: //服务器返回 HTTP 200
        return response.json() //返回服务器响应的 JSON 数据（通常是仓库列表
    else: //如果状态码不是 200，抛出异常并显示状态码
        raise Exception(f'Failed to retrieve repositories: {response.status_code}')

def main():
    if len(sys.argv) < 2: //检查命令行参数是否足够（至少要输入一个域名）>1
        print("Usage: python script.py <gitea_domain>")
        sys.exit(1) //如果参数不足，打印用法提示并退出程序，返回码为 1（表示异常退出）

    gitea_domain = format_domain(sys.argv[1]) //获取命令行第一个参数（域名）并通过 format_domain() 函数处理，保证 URL 正确

    personal_access_token = os.getenv('GITEA_ACCESS_TOKEN') //从系统环境变量中读取 Gitea 的个人访问令牌
    if not personal_access_token: //如果没有设置这个环境变量，打印错误信息并退出程序
        print("Error: GITEA_ACCESS_TOKEN environment variable not set.")
        sys.exit(1)

    try: //尝试执行下面的代码，如果出错会跳到 except 块
        repos = get_repositories(personal_access_token, gitea_domain) //调用 get_repositories() 获取仓库列表
        print("Repositories:") //打印 “Repositories:” 表头
        for repo in repos: //遍历每个仓库对象
            print(f"- {repo['full_name']}") //打印每个仓库的全名（例如 username/reponame）
    except Exception as e: //捕获异常并打印错误信息
        print(f"Error: {e}")

if __name__ == "__main__": //判断保证 仅在直接运行脚本时 执行 main() 函数，而不是被导入为模块时执行
    main()
```
#### 更新后的脚本引入了使用环境变量GITEA ACCESS TOKEN来替换以前硬编码的个人访问令牌，提高了安全性，我们也继续在本地设置。
#### Update repos.py
```
import requests
import sys

# store this in env instead at some point //在某个时候将其存储在env中；以后建议改回用环境变量存储更安全
PERSONAL_ACCESS_TOKEN = '43ce39bb0bd6bc489284f2905f033ca467a6362f'
//直接在代码里写了令牌，不依赖环境变量
import os

def format_domain(domain):
    if not domain.startswith(('http://', 'https://')):
@ -28,8 +26,13 @@ def main():

    gitea_domain = format_domain(sys.argv[1])

    personal_access_token = os.getenv('GITEA_ACCESS_TOKEN')
    if not personal_access_token:
        print("Error: GITEA_ACCESS_TOKEN environment variable not set.")
        sys.exit(1)

    try:
        //这里出现了 重复调用
        repos = get_repositories(PERSONAL_ACCESS_TOKEN, gitea_domain)//第一次用硬编码的 PERSONAL_ACCESS_TOKEN
        repos = get_repositories(personal_access_token, gitea_domain)//第二次用环境变量的 personal_access_token
//最终 repos 会被第二行覆盖，所以如果环境变量存在，硬编码的令牌其实没生效
//令牌直接硬编码在脚本里，不安全，尤其如果把脚本上传到 GitHub 之类的平台
        print("Repositories:")
        for repo in repos:
            print(f"- {repo['full_name']}")

//@ -28,8 +26,13 @@ 是 hunk header（块头），用来描述变化位置：
//-28,8：表示旧文件（修改前）从第 28 行开始，有 8 行内容。
//+26,13：表示新文件（修改后）从第 26 行开始，有 13 行内容。
```
### 操作_Update repos.py
![图片](image/121302.png)

