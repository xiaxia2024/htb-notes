## Jerry

```
[★]$ nmap -sC -sV 10.129.136.9
Starting Nmap 7.94SVN ( https://nmap.org ) at 2025-12-17 20:10 CST
Nmap scan report for 10.129.136.9
Host is up (0.0086s latency).
Not shown: 999 filtered tcp ports (no-response)
PORT     STATE SERVICE VERSION
8080/tcp open  http    Apache Tomcat/Coyote JSP engine 1.1
|_http-server-header: Apache-Coyote/1.1
|_http-favicon: Apache Tomcat
|_http-title: Apache Tomcat/7.0.88
```
#### 浏览器http://IP:8080, 点击Manager App
