### Dog过期机器，我挑战的第4天，记住一下卡住我的点

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

## 第一个卡住我点
其一 FUUF命令跑了5个小时，别看它现在6位数，要跑到7位数还需要4天半。。。
其二 按了Ctrl+C不会停下来，直接按回车就会跳出提示选项进入交互模式，退出只能手动关闭窗口。

![你还好吗](images/截屏2025-07-13 22.42.03.png)
