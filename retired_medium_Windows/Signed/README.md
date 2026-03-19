## Signed
```
[★]$ nmap -sCV 10.129.242.173
Starting Nmap 7.94SVN ( https://nmap.org ) at 2026-03-19 03:47 CDT
Nmap scan report for 10.129.242.173
Host is up (0.0083s latency).
Not shown: 999 filtered tcp ports (no-response)
PORT     STATE SERVICE  VERSION
1433/tcp open  ms-sql-s Microsoft SQL Server 2022 16.00.1000.00; RC0+
| ms-sql-info: 
|   10.129.242.173:1433: 
|     Version: 
|       name: Microsoft SQL Server 2022 RC0+
|       number: 16.00.1000.00
|       Product: Microsoft SQL Server 2022
|       Service pack level: RC0
|       Post-SP patches applied: true
|_    TCP port: 1433
| ms-sql-ntlm-info: 
|   10.129.242.173:1433: 
|     Target_Name: SIGNED
|     NetBIOS_Domain_Name: SIGNED
|     NetBIOS_Computer_Name: DC01
|     DNS_Domain_Name: SIGNED.HTB
|     DNS_Computer_Name: DC01.SIGNED.HTB
|     DNS_Tree_Name: SIGNED.HTB
|_    Product_Version: 10.0.17763
| ssl-cert: Subject: commonName=SSL_Self_Signed_Fallback
| Not valid before: 2026-03-19T08:45:44
|_Not valid after:  2056-03-19T08:45:44
|_ssl-date: 2026-03-19T08:47:55+00:00; 0s from scanner time.
Service Info: OS: Windows; CPE: cpe:/o:microsoft:windows

Service detection performed. Please report any incorrect results at https://nmap.org/submit/ .
Nmap done: 1 IP address (1 host up) scanned in 23.20 seconds
[★]$ echo '10.129.242.173 dc01.signed.htb signed.htb' | sudo tee -a /etc/hosts
10.129.242.173 dc01.signed.htb signed.htb
```
#### Machine Information
```
As is common in real life Windows penetration tests, you will start the Signed box with credentials for the following account which can be used to access the MSSQL service: scott / Sm230#C5NatH
```
