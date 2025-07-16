### Dog 过期机器，我挑战的第4天，记住一下卡住我的点
##### 因为我是新手，我的能力只能够打过期有文档的靶机，所以整个upwrite的逻辑的无法完全串通的，但是对详细操作的小细节是OK的，觉得超级新鲜的。

### 首先先简单描述一下过程
#### 1.加入域名 ‘$ echo "ip dog.htb" | sudo tee -a /etc/hosts'

#### 2.扫描 
`$ ports=$(namp -Pn -p- --min-rate=1000 -T4 目标IP | grep ^[0-9] | cut -d '/' -f 1 | tr '\n' ',' | sed s/,$//) `

` $ nmap -Pn -p$ports -sC -sV 目标IP `

Nmap扫描显示了两个端口：SSH和HTTP服务。HTTP端口的版本为Apache 2.4.41运行。

Nmap还表明它是一个背景CMS和git存储库，需要用到gitdump去扫描收集这个目标的内容。

#### 3.导入gitdump，是python文件，python需要虚拟环境
```git-dumper 是一个用来从目标网站还原泄露的 Git 仓库的工具。
$ virtualenv env
$ source env/bin/activate
$ mkdir dump
$ cd dump
$ pip install git-dumper
$ git-dumper http://dog.htb/ dump
$ git restore .
$ cat settings.php
```
git restore . 会撤销当前目录下所有文件在工作区的未提交修改，让它们回到最近一次 commit 的状态。
#### 4.收集到了settings.php文件,找到了mysql的密码
里面有段内容：`$database = 'mysql://root:BackDropJ2024DS2024@127.0.0.1/backdrop';`
#### 5.使用ffut 快速模糊测试工具，用来对URL、参数或路径做暴力测试
```
$ ls /usr/share/seclists/Usernames/xato-net-10-million-usernames.txt
$ ffuf -w /usr/share/seclists/Username/xato-net-10-millon-username.txt -u http://dog.htb/\?q=accounts/FUZZ -c -v -mc 403
```

解析一下命令行：

【1】FUFF是ffuf的占位符，会被字典里的每一行替换。

【2】\? 访问的是http://dog.htb/?q=accounts/FUZZ

【3】 使用的字典（wordlist）。这里是一个常用的用户名列表：xato 的 1000 万用户名合集

【4】 -c 彩色输出（colorize），更易读

【5】 -v verbose（详细模式），会显示更多调试/请求信息

【6】 -mc 403 只显示响应状态码是 403 Forbidden 的结果（Match Codes）

原因：目标站点 dog.htb 是 Backdrop CMS，有一个特殊的 URL：` ?q=accounts/用户名`

如果 fuzz 到一个存在的用户名：可能返回 403 Forbidden（存在但不允许匿名访问）或返回 200/301（正常页面）
通过这些差异可以枚举出有效用户名

## 第一个卡住我点,too long time
其一 FUFF命令跑了5个小时，别看它现在6位数，要跑到7位数还需要4天半。。。
![你还好吗](images/001.png)
其二 按了Ctrl+C不会停下来，直接按回车就会跳出提示选项进入交互模式，退出只能手动关闭窗口。
![嘿我是小黑](images/002.png)
结果就是有两用户名john && tiffany

#### 6.获知exploit版本
https://github.com/FisMatHack/BackDropScan/blob/main/BackDropScan.py#L35

`$ curl http://dog.htb/core/profiles/testing.info`

## 第二个卡住我的点 
其一 因为跑太长时间了，把虚拟机跑崩了，登不了目标网站了，还以为是那个网站本来就不能登 哼

目标网站为： http://dog.htb
登陆 tiffany用户 输入mysql密码

其二 HackTheBox 自带的VPN虚拟机，不能粘贴，我得手动编辑 BackDrop_CMS_1.27.1_exploit.py

https://www.exploit-db.com/exploits/52021  //BackDrop_CMS_1.27.1_exploit.py的原创

//我要添加自己的解析

其三 老辛苦找到Manual installion上传文件的地方，但是在下载文件的地方 没有找到 我上传的文件 

天塌了。。这就是exploit如何利用的神奇好玩的地方，哼因为我找到了，所以我觉得好玩
![小黑哒](images/0003.png)



