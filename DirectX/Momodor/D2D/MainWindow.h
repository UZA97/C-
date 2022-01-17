#pragma once
class MainWindow
{
	// Variable
public:
	HDC       GetHDC() { return m_hDC; }
	int       GetWidth() { return m_nWidth; };
	int       GetHeight() { return m_nHeight; };
	HWND      GetWindowHandler() { return m_hWnd; };
	Vector2   GetMousePosition() { return m_MousePos; }
	void      SetWindowSize(int Width, int Height);
	void      SetMousePosition(Vector2 pos) { m_MousePos = pos; }
	void      OnSize();
private:
	HINSTANCE  m_hInstance;     // 프로그램 Instance Handler
	HWND       m_hWnd;          // Window Handle
	HDC        m_hDC;
	int        m_nWidth = 0;
	int        m_nHeight = 0;
	Vector2    m_MousePos = Vector2(0, 0);
	// Function
public:
	void  Delete();
	void  InitDirect3D();
	void  InitSceneManager();
	void  Update();
	void  Render();
	void  CreateInstance(HINSTANCE hInstance, int width, int height); // 레즈스트리 등록,Window생성
	void  CreateBackBuffer();
	void  DeleteBackBuffer();

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
public:  // 생성자, 소멸자
	MainWindow();
	~MainWindow();
};

