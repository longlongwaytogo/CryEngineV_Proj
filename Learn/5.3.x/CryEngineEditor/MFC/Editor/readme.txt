本项目主要用于测试CryEngine 与GUI的结合
测试结果：
GameLaucher 的模式下，不能正常创建窗口(包括普通window，模态/非模态对话框），即便创建，也无法实现窗口界面刷新（或者需要用户自己重写OnDraw或OnPaint函数，但窗口注册的回调事件可以正常执行）。 同时，Win32模式创建多窗口，不能处理消息循环，因为CryEngine底层封装了消息循环。

CE的Editor，其实开发流程和GameLaucher有些差别，GameLauncher使用内部消息处理（底层自己实现消息泵循环），Editor则是在UI界面框架中，如MFC内部，将渲染放在View视图中，该视图绑定了Context，然后，自己重载了paint函数，在paint函数内部，调用引擎，完成整个绘制阶段的处理。