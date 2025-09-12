## PermX

### ffuf枚举
#### 使用暴力通过将Host标头中的FUZZ占位符替换为来自wordlist的条目来替换子域
```
[★]$ ls /usr/share/wordlists/seclists/Discovery/DNS/bitquark-subdomains-top100000.txt
/usr/share/wordlists/seclists/Discovery/DNS/bitquark-subdomains-top100000.txt
[★]$ ffuf -w /usr/share/wordlists/seclists/Discovery/DNS/bitquark-subdomains-top100000.txt -H "Host: FUZZ.permx.htb" -u http://permx.htb -t 200 -ic
```
```
[Status: 302, Size: 299, Words: 18, Lines: 10, Duration: 9ms]
[Status: 302, Size: 291, Words: 18, Lines: 10, Duration: 9ms]
[Status: 302, Size: 306, Words: 18, Lines: 10, Duration: 9ms]
```
#### 从输出中，我们看到多个子域返回类似的结果。我们现在可以过滤响应基于字数统计，专注于独特的子域
```
[★]$ ffuf -w /usr/share/wordlists/seclists/Discovery/DNS/bitquark-subdomains-top100000.txt -H "Host: FUZZ.permx.htb" -u http://permx.htb -t 200 -ic -fw 18

        /'___\  /'___\           /'___\       
       /\ \__/ /\ \__/  __  __  /\ \__/       
       \ \ ,__\\ \ ,__\/\ \/\ \ \ \ ,__\      
        \ \ \_/ \ \ \_/\ \ \_\ \ \ \ \_/      
         \ \_\   \ \_\  \ \____/  \ \_\       
          \/_/    \/_/   \/___/    \/_/       

       v2.1.0-dev
________________________________________________

 :: Method           : GET
 :: URL              : http://permx.htb
 :: Wordlist         : FUZZ: /usr/share/wordlists/seclists/Discovery/DNS/bitquark-subdomains-top100000.txt
 :: Header           : Host: FUZZ.permx.htb
 :: Follow redirects : false
 :: Calibration      : false
 :: Timeout          : 10
 :: Threads          : 200
 :: Matcher          : Response status: 200-299,301,302,307,401,403,405,500
 :: Filter           : Response words: 18
________________________________________________

www                     [Status: 200, Size: 36182, Words: 12829, Lines: 587, Duration: 12ms]
lms                     [Status: 200, Size: 19347, Words: 4910, Lines: 353, Duration: 67ms]
:: Progress: [100000/100000] :: Job [1/1] :: 10638 req/sec :: Duration: [0:00:16] :: Errors: 0 ::: Progress: [100000/100000] :: Job [1/1] :: 212 req/sec :: Duration: [0:00:21] :: Errors: 0 ::
```
#### 这揭示了两个不同的子域：www和lms
```
