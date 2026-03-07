## Bamboo

#### 既然 3128 端口已开放，该端口通常用于代理服务器，尤其是 Squid 代理，那么配置错误的实例就可以用于转向内部服务。我们可以按照 HackTricks 上的指南来枚举通过 Squid 代理可访问的服务。还有一个工具我们可以使用。首先，我们下载 squidscan 仓库文件，包括 go.mod 和 squidscan.go。然后，我们编辑 squidscan.go，将 proxyURL 设置为代理服务器的 IP 和端口。
