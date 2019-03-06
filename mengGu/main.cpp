#include "ggl.h"
#include "scene.h"
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")

unsigned char * LoadFileContent(const char * path, int & filesize) {
	unsigned char * fileContent = nullptr;//把文件内容读到这里
	filesize = 0;
	FILE *pFile = fopen(path, "rb");
	//如果打开文件成功
	if (pFile) {
		fseek(pFile, 0, SEEK_END);//把文件的指针移动到某一个位置上去(移动到相距 SEEK_END 为 0 的地方上去)
		int nLen = ftell(pFile);//返回当前文件的位置，返回的单位为字节
		if (nLen > 0) {
			rewind(pFile);//把文件指针搬到文件头部
			fileContent = new unsigned char[nLen + 1];//开辟一个内存空间，比文件的实际大小大一个字节（因为要增加一个\0的标记来提高程序的健壮性）
			fread(fileContent, sizeof(unsigned char), nLen, pFile);//参数2：每组读多大的数据；参数3：读nLen组数据
			fileContent[nLen] = '\0';
			filesize = nLen;
		}
		fclose(pFile);
	}
	return fileContent;
}

LRESULT CALLBACK GLWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
	switch (msg) {
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

//参数：当前应用程序的实例，上一次应用程序的实例（如果启动两次或以上），命令行参数（如果打开cmd启动，同时传一个参数）,显示的命令（表示如何显示窗口）
INT WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevinstance, LPSTR IpCmdLine, int nShowCmd) 
{
	WNDCLASSEX wndclass;
	wndclass.cbClsExtra = 0;//窗口类型的额外空间
	wndclass.cbSize = sizeof(WNDCLASSEX);//WNDCLASSEX这个对象实际占用了多大内存
	wndclass.cbWndExtra = 0;//窗口的额外内存
	wndclass.hbrBackground = NULL;//背景
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);//设置光标：箭头光标
	wndclass.hIcon = NULL;//应用程序的图标
	wndclass.hIconSm = NULL;//应用程序运行后显示在左上角的图标
	wndclass.hInstance = hinstance;
	wndclass.lpfnWndProc = GLWindowProc;//如果用户操作了窗口,告诉我们是谁调用了
	wndclass.lpszClassName = L"GLWindow";//注册窗口使用的窗口名称
	wndclass.lpszMenuName = NULL;//菜单的名字
	wndclass.style = CS_VREDRAW | CS_HREDRAW;//窗口更新时采用怎样的重绘方式，采用水平和垂直重绘的方式
	ATOM atom = RegisterClassEx(&wndclass);//注册窗口
	if (!atom) {
		MessageBox(NULL, L"Notice", L"Error", MB_OK);
		return 0;
	}

	//视口大小
	RECT rect;
	rect.left = 0;
	rect.right = 800;
	rect.top = 0;
	rect.bottom = 600;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, NULL);//对视口的需求，窗口风格，菜单句柄（这里无菜单传空）//这个函数在满足对视口大小要求的情况下，返回一个对窗口的描述
	int windowWidth = rect.right - rect.left;
	int windowHeight = rect.bottom - rect.top;

	//创建窗口
	HWND hwnd = CreateWindowEx(NULL, L"GLWindow", L"OpenGL Window", WS_OVERLAPPEDWINDOW,//参数：窗口的额外风格，窗口的名称，一个字符串会显示到窗口的标记栏上，窗口的风格，
		100, 100, windowWidth, windowHeight,//窗口创建的起始位置，窗口的宽和高（包含窗口的边框），
		NULL, NULL, hinstance, NULL);//副窗口的句柄，菜单的句柄，应用程序的实例（WinMain参数列表例的那个），额外的参数（给WMCreate传消息）
	
	HDC dc = GetDC(hwnd);//获取设备上下文的句柄
	PIXELFORMATDESCRIPTOR pfd;//我们对像素格式的需求
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nVersion = 1;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.cColorBits = 32;//颜色缓冲区：一个像素占用32比特，RGBA分别为8比特
	pfd.cDepthBits = 24;//深度缓冲区：每个像素占用24位
	pfd.cStencilBits = 8;//蒙版缓冲区：每个像素占用8比特
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;//画面最终显示到Windows窗口；支持OpenGL；双缓冲区
	int pixelFormat = ChoosePixelFormat(dc, &pfd);//windows会根据pfd为我们选一个像素格式
	SetPixelFormat(dc, pixelFormat, &pfd);

	//搭建OpenGL渲染环境
	HGLRC rc = wglCreateContext(dc);
	//使渲染环境生效
	wglMakeCurrent(dc, rc);

	glewInit();//初始化Opengl的高级API
	Init();//初始化场景
	SetViewPortSize(800.0f, 600.0f);

	//显示窗口
	ShowWindow(hwnd, SW_SHOW);//窗口的句柄，如何显示
	UpdateWindow(hwnd);//刷新，确保窗口的画面是正确的

	//死循环保持窗口运行
	MSG msg;
	while (true) {
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) {//获取消息队列的消息：消息；获取哪个窗口的消息（传空是所有窗口）；消息ID的最小值；消息ID的最大值；获取消息后怎么处理(PM_REMOVE拿到消息后从消息队列里移除)		
			if (msg.message == WM_QUIT) {//处理WM_CLOSE
				break;
			}
			TranslateMessage(&msg);//转化成应用程序可以识别的消息
			DispatchMessage(&msg);//把消息派发出去
		}
		Draw();
		SwapBuffers(dc);//将前后两个缓冲区进行交换
	}
	return 0;
}