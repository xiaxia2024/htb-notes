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
#### [1]
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
#### [2]
```
