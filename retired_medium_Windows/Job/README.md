## Job
```
[★]$ nmap -sV -sC 10.129.234.73
Starting Nmap 7.94SVN ( https://nmap.org ) at 2026-03-04 00:26 CST
Nmap scan report for 10.129.234.73
Host is up (0.0091s latency).
Not shown: 996 filtered tcp ports (no-response)
PORT     STATE SERVICE       VERSION
25/tcp   open  smtp          hMailServer smtpd
| smtp-commands: JOB, SIZE 20480000, AUTH LOGIN, HELP
|_ 211 DATA HELO EHLO MAIL NOOP QUIT RCPT RSET SAML TURN VRFY
80/tcp   open  http          Microsoft IIS httpd 10.0
|_http-server-header: Microsoft-IIS/10.0
|_http-title: Job.local
| http-methods: 
|_  Potentially risky methods: TRACE
445/tcp  open  microsoft-ds?
3389/tcp open  ms-wbt-server Microsoft Terminal Services
| rdp-ntlm-info: 
|   Target_Name: JOB
|   NetBIOS_Domain_Name: JOB
|   NetBIOS_Computer_Name: JOB
|   DNS_Domain_Name: job
|   DNS_Computer_Name: job
|   Product_Version: 10.0.20348
|_  System_Time: 2026-03-04T06:26:39+00:00
|_ssl-date: 2026-03-04T06:27:19+00:00; -1s from scanner time.
| ssl-cert: Subject: commonName=job
| Not valid before: 2026-03-03T06:24:05
|_Not valid after:  2026-09-02T06:24:05
Service Info: Host: JOB; OS: Windows; CPE: cpe:/o:microsoft:windows

Host script results:
|_clock-skew: mean: -1s, deviation: 0s, median: -1s
| smb2-time: 
|   date: 2026-03-04T06:26:41
|_  start_date: N/A
| smb2-security-mode: 
|   3:1:1: 
|_    Message signing enabled but not required

```
