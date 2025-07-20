function verifyInviteCode(code) { //接受一个参数 code（用户输入的邀请码）
	var formDate = { //创建一个对象 formData，用于将用户输入的 code 发送到服务器
		"code": code  //"属性名": 变量
	};
	$.ajax({ //使用 jQuery 的 ajax() 方法，发起一个 AJAX 异步请求
		type: "POST", //请求类型是 POST，表示要向服务器提交数据
		dataType: "json", //预期服务器返回的是 JSON 格式的数据
		data: formData, //请求的正文（body）数据是 formData，也就是包含邀请码的对象
		url: '/api/v1/invite/verify', //请求的服务器地址是这个接口，作用是验证邀请码是否有效
		success: function (response) { //success:是在 $.ajax({...}) 请求中的一个配置项
					       //function(response) { ... }是一个匿名函数，作为回调函数传入；一旦成功，jQuery 就会把服务器返回的数据作为参数 response 传给这个函数
			// console.log(response)  //函数的主体语句，把收到的服务器响应数据打印到浏览器的控制台中（你可以按 F12 → Console 查看）
		},
		error: function (response) { //当 AJAX 请求失败（比如服务器报错、超时、URL 错了、断网等），就执行这个函数
					     //这个函数也会收到一个 response，但这次不是成功的数据，而是包含错误信息的对象
			console.log(response) 
		}
	})

	function makeInviteCode() { //这是一个函数声明。函数名是 makeInviteCode，意思是“生成邀请码”
		$.ajax({ //这里使用的是 jQuery 的 $.ajax() 方法，用来发起 AJAX 请求。$ 是 jQuery 的简写
			type: "POST",
			dataType: "json",
			url: '/api/v1/invite/how/to/generate', //请求的目标地址，也就是你要访问的接口是这个 API 路径，它的意思是“请求生成邀请码的方法”
			success: function (response) { //如果服务器返回成功，会执行 success 函数，把服务器返回的 response 打印出来
				console.log(response)
			},
			error: function (response) { //如果请求失败（比如网络错误或接口不存在），就执行 error 函数，同样打印错误信息
				console.log(response)
			}
		}); 
	} //AJAX 请求结束，函数闭合。

// jQuery 是一个轻量级、跨浏览器的 JavaScript 库;简化 JavaScript 的 DOM 操作（操作 HTML 元素）;简化事件绑定（比如点击事件）;简化 AJAX 请求（异步加载数据）;提供动画、特效支持
// AJAX（Asynchronous JavaScript and XML） 是一种技术，让网页可以在不刷新页面的情况下从服务器获取数据或发送数据

// 为什么代码要“混淆”或压缩？
// 保护逻辑：防止别人轻易看懂源码，尤其是含有商业逻辑的部分
// 减小文件体积：加载速度更快（少了空格、换行、缩写变量名等）
// 混淆后，别人难以修改功能
