<h1>蜡笔小新</h1>

<h3>版本</h3>
<p></p>

<h3>简介</h3>
<p></p>

<h3>功能</h3>
<p>远程 cmd</p>
<p>文件上传下载</p>

<h3>使用方法</h3>
<p>1、使用 shellcode\GetShellcode.py 生成 shellcode</p>
<p>2、使用 hta\DownloadURLFile.py 生成 hta 脚本</p>
<p>3、在 loader 项目中生成加载器</p>
<p>4、在 trojan 项目中生成客户端和服务端程序</p>

<h3>加载流程</h3>
<p>通过内存 shellcode 远程加载 hta 脚本并运行, hta 脚本会将客户端和加载器下载到当前目录, 然后运行加载器并等待加载器完成工作, 最后运行客户端以连接服务端</p>
  
