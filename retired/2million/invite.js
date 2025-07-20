function verifyInviteCode(code) { //接受一个参数 code（用户输入的邀请码）
	var formDate = { //创建一个对象 formData，用于将用户输入的 code 发送到服务器
		"code": code  //"属性名": 变量
	};
	$.ajax({ //使用 jQuery 的 ajax() 方法，发起一个 AJAX 异步请求
		type: "POST", //请求类型是 POST，表示要向服务器提交数据
		dataType: "json",
		url: '/api/v1/invite/verify',
		success: function (response) {
			console.log(response)
		},
		error: function (response) {
			console.log(response)
		}
	})

	function makeInviteCode() {
		$.ajax({
			type: "POST",
			dataType: "json",
			url: '/api/v1/invite/how/to/generate',
			success: function (response) {
				console.log(response)
			},
			error: function (response) {
				console.log(response)
			}
		})
	}
