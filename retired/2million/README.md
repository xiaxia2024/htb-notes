linux_JavaScript 

前后端通信（Ajax + API）:
技术                           作用	                    举例
Ajax（用 jQuery 或 原生 JS 写）	向服务器请求数据而不刷新页面	点按钮生成邀请码时从 /api/v1/... 获取数据
API接口	                      后台返回 JSON 数据	        { "invite_code": "abc123" }

jQuery 是个老牌工具库，很多老网站和项目用它，现在新项目用得较少（转向 Vue/React），但它对理解 Ajax 很重要
