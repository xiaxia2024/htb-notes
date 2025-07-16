### Dog过期机器，我挑战的第4天，记住一下卡住我的点

#### ‘$ echo "ip dog.htb" | sudo tee -a /etc/hosts'

#### 扫描 
`$ ports=$(namp -Pn -p- --min-rate=1000 -T4 目标IP | grep ^[0-9] | cut -d '/' -f 1 | tr '\n' ',' | sed s/,$//) `

` $ nmap -Pn -p$ports -sC -sV 目标IP `

Nmap扫描显示了两个端口：SSH和HTTP服务。HTTP端口的版本为Apache 2.4.41运行。
Nmap还表明它是一个背景CMS和git存储库。
