## Editorial

```
[★]$ nmap -sC -sV 10.129.217.28
PORT   STATE SERVICE VERSION
22/tcp open  ssh     OpenSSH 8.9p1 Ubuntu 3ubuntu0.7 (Ubuntu Linux; protocol 2.0)
| ssh-hostkey: 
|   256 0d:ed:b2:9c:e2:53:fb:d4:c8:c1:19:6e:75:80:d8:64 (ECDSA)
|_  256 0f:b9:a7:51:0e:00:d5:7b:5b:7c:5f:bf:2b:ed:53:a0 (ED25519)
80/tcp open  http    nginx 1.18.0 (Ubuntu)
|_http-server-header: nginx/1.18.0 (Ubuntu)
|_http-title: Did not follow redirect to http://editorial.htb
Service Info: OS: Linux; CPE: cpe:/o:linux:linux_kernel

[★]$ echo '10.129.217.28 editorial.htb' | sudo tee -a /etc/hosts
10.129.217.28 editorial.htb
```

### 开始burpsuite枚举
```
[*]$ nc -lvnp 5555
listening on [any] 5555 ...
```
#### 浏览器Publish with us
![我爱死HTB](iamges/09120201.png)
![我爱死HTB](iamges/09120201.png)
#### 回头看看我们的Netcat侦听器，我们看到我们确实收到了一个回调。这证实了服务器试图连接回我们的本地机器，这表明应用程序是错误的易受服务器端请求伪造（SSRF）的攻击。

