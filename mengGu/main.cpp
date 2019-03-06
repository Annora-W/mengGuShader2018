#include "ggl.h"
#include "scene.h"
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")

unsigned char * LoadFileContent(const char * path, int & filesize) {
	unsigned char * fileContent = nullptr;//���ļ����ݶ�������
	filesize = 0;
	FILE *pFile = fopen(path, "rb");
	//������ļ��ɹ�
	if (pFile) {
		fseek(pFile, 0, SEEK_END);//���ļ���ָ���ƶ���ĳһ��λ����ȥ(�ƶ������ SEEK_END Ϊ 0 �ĵط���ȥ)
		int nLen = ftell(pFile);//���ص�ǰ�ļ���λ�ã����صĵ�λΪ�ֽ�
		if (nLen > 0) {
			rewind(pFile);//���ļ�ָ��ᵽ�ļ�ͷ��
			fileContent = new unsigned char[nLen + 1];//����һ���ڴ�ռ䣬���ļ���ʵ�ʴ�С��һ���ֽڣ���ΪҪ����һ��\0�ı������߳���Ľ�׳�ԣ�
			fread(fileContent, sizeof(unsigned char), nLen, pFile);//����2��ÿ����������ݣ�����3����nLen������
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

//��������ǰӦ�ó����ʵ������һ��Ӧ�ó����ʵ��������������λ����ϣ��������в����������cmd������ͬʱ��һ��������,��ʾ�������ʾ�����ʾ���ڣ�
INT WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevinstance, LPSTR IpCmdLine, int nShowCmd) 
{
	WNDCLASSEX wndclass;
	wndclass.cbClsExtra = 0;//�������͵Ķ���ռ�
	wndclass.cbSize = sizeof(WNDCLASSEX);//WNDCLASSEX�������ʵ��ռ���˶���ڴ�
	wndclass.cbWndExtra = 0;//���ڵĶ����ڴ�
	wndclass.hbrBackground = NULL;//����
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);//���ù�꣺��ͷ���
	wndclass.hIcon = NULL;//Ӧ�ó����ͼ��
	wndclass.hIconSm = NULL;//Ӧ�ó������к���ʾ�����Ͻǵ�ͼ��
	wndclass.hInstance = hinstance;
	wndclass.lpfnWndProc = GLWindowProc;//����û������˴���,����������˭������
	wndclass.lpszClassName = L"GLWindow";//ע�ᴰ��ʹ�õĴ�������
	wndclass.lpszMenuName = NULL;//�˵�������
	wndclass.style = CS_VREDRAW | CS_HREDRAW;//���ڸ���ʱ�����������ػ淽ʽ������ˮƽ�ʹ�ֱ�ػ�ķ�ʽ
	ATOM atom = RegisterClassEx(&wndclass);//ע�ᴰ��
	if (!atom) {
		MessageBox(NULL, L"Notice", L"Error", MB_OK);
		return 0;
	}

	//�ӿڴ�С
	RECT rect;
	rect.left = 0;
	rect.right = 800;
	rect.top = 0;
	rect.bottom = 600;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, NULL);//���ӿڵ����󣬴��ڷ�񣬲˵�����������޲˵����գ�//���������������ӿڴ�СҪ�������£�����һ���Դ��ڵ�����
	int windowWidth = rect.right - rect.left;
	int windowHeight = rect.bottom - rect.top;

	//��������
	HWND hwnd = CreateWindowEx(NULL, L"GLWindow", L"OpenGL Window", WS_OVERLAPPEDWINDOW,//���������ڵĶ����񣬴��ڵ����ƣ�һ���ַ�������ʾ�����ڵı�����ϣ����ڵķ��
		100, 100, windowWidth, windowHeight,//���ڴ�������ʼλ�ã����ڵĿ�͸ߣ��������ڵı߿򣩣�
		NULL, NULL, hinstance, NULL);//�����ڵľ�����˵��ľ����Ӧ�ó����ʵ����WinMain�����б������Ǹ���������Ĳ�������WMCreate����Ϣ��
	
	HDC dc = GetDC(hwnd);//��ȡ�豸�����ĵľ��
	PIXELFORMATDESCRIPTOR pfd;//���Ƕ����ظ�ʽ������
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nVersion = 1;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.cColorBits = 32;//��ɫ��������һ������ռ��32���أ�RGBA�ֱ�Ϊ8����
	pfd.cDepthBits = 24;//��Ȼ�������ÿ������ռ��24λ
	pfd.cStencilBits = 8;//�ɰ滺������ÿ������ռ��8����
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;//����������ʾ��Windows���ڣ�֧��OpenGL��˫������
	int pixelFormat = ChoosePixelFormat(dc, &pfd);//windows�����pfdΪ����ѡһ�����ظ�ʽ
	SetPixelFormat(dc, pixelFormat, &pfd);

	//�OpenGL��Ⱦ����
	HGLRC rc = wglCreateContext(dc);
	//ʹ��Ⱦ������Ч
	wglMakeCurrent(dc, rc);

	glewInit();//��ʼ��Opengl�ĸ߼�API
	Init();//��ʼ������
	SetViewPortSize(800.0f, 600.0f);

	//��ʾ����
	ShowWindow(hwnd, SW_SHOW);//���ڵľ���������ʾ
	UpdateWindow(hwnd);//ˢ�£�ȷ�����ڵĻ�������ȷ��

	//��ѭ�����ִ�������
	MSG msg;
	while (true) {
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) {//��ȡ��Ϣ���е���Ϣ����Ϣ����ȡ�ĸ����ڵ���Ϣ�����������д��ڣ�����ϢID����Сֵ����ϢID�����ֵ����ȡ��Ϣ����ô����(PM_REMOVE�õ���Ϣ�����Ϣ�������Ƴ�)		
			if (msg.message == WM_QUIT) {//����WM_CLOSE
				break;
			}
			TranslateMessage(&msg);//ת����Ӧ�ó������ʶ�����Ϣ
			DispatchMessage(&msg);//����Ϣ�ɷ���ȥ
		}
		Draw();
		SwapBuffers(dc);//��ǰ���������������н���
	}
	return 0;
}