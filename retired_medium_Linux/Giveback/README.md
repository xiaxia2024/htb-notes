## Giveback [oxdf]
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
