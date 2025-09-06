## Strutted

```
[★]$ nmap -sC -sV 10.129.231.200
Starting Nmap 7.94SVN ( https://nmap.org ) at 2025-09-06 03:24 CDT
Nmap scan report for 10.129.231.200
Host is up (0.011s latency).
Not shown: 998 closed tcp ports (reset)
PORT   STATE SERVICE VERSION
22/tcp open  ssh     OpenSSH 8.9p1 Ubuntu 3ubuntu0.10 (Ubuntu Linux; protocol 2.0)
| ssh-hostkey: 
|   256 3e:ea:45:4b:c5:d1:6d:6f:e2:d4:d1:3b:0a:3d:a9:4f (ECDSA)
|_  256 64:cc:75:de:4a:e6:a5:b4:73:eb:3f:1b:cf:b4:e3:94 (ED25519)
80/tcp open  http    nginx 1.18.0 (Ubuntu)
|_http-server-header: nginx/1.18.0 (Ubuntu)
|_http-title: Did not follow redirect to http://strutted.htb/
Service Info: OS: Linux; CPE: cpe:/o:linux:linux_kernel
```
#### 加入域名
```
[★]$ echo '10.129.231.200 strutted.htb' | sudo tee -a /etc/hosts
```
#### 在浏览器的Download下载了strutted.zip
```
[★]$ unzip strutted.zip
```
```
[~/Downloads/strutted][★]$ ls
mvnw  mvnw.cmd  pom.xml  src  target

[~/Downloads/strutted][★]$ cat pom.xml
<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>

    <packaging>war</packaging>

    <artifactId>strutted</artifactId>
    <groupId>org.strutted.htb</groupId>
    <version>1.0.0</version>

    <name>Strutted™</name>
    <description>Instantly upload an image and receive a unique, shareable link. Keep your images secure, accessible, and easy to share—anywhere, anytime.</description>

    <properties>
        <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
        <maven.compiler.source>17</maven.compiler.source>
        <maven.compiler.target>17</maven.compiler.target>
        <struts2.version>6.3.0.1</struts2.version>
        <jetty-plugin.version>9.4.46.v20220331</jetty-plugin.version>
        <maven.javadoc.skip>true</maven.javadoc.skip>
        <jackson.version>2.14.1</jackson.version>
        <jackson-data-bind.version>2.14.1</jackson-data-bind.version>
    </properties>
```
#### <struts2.version>6.3.0.1</struts2.version>
#### Apache Struts2 6.3.0.1
https://security.netapp.com/advisory/ntap-20250103-0005/

https://y4tacker.github.io/2024/12/16/year/2024/12/Apache-Struts2-%E6%96%87%E4%BB%B6%E4%B8%8A%E4%BC%A0%E9%80%BB%E8%BE%91%E7%BB%95%E8%BF%87-CVE-2024-53677-S2-067/
#### Apache Struts2 文件上传逻辑绕过(CVE-2024-53677)(S2-067)
```
[~/Downloads/strutted/src/main/java/org/strutted/htb][★]$ ls
AboutAction.java  DatabaseUtil.java  HowAction.java  Upload.java  URLMapping.java  URLUtil.java
[~/Downloads/strutted/src/main/java/org/strutted/htb][★]$ cat Upload.java
```
### 首先我们要上传什么样的文件 

#### 下载一个随机图片生成网站，然后上传
```
[★]$ wget https://picsum.photos/200/300 -O test.jpg
```
