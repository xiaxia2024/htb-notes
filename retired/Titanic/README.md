# Titanic

## BurpSuite
拦截下载页面，下载内容是提交信息，生成.json文件

### [1]直接Send,Forward,Forward立即跳出内容
![午后才起床打靶机](images/080201.png)
### [2]在获得的GET请求 /download?ticket=/etc/password Send，内容会在Respinse中出现
![午后才起床打靶机](images/080202.png)
