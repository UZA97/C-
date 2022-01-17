#include "framework.h"
#include  "resource.h"
#include "MainWindow.h"
// #include "Base/Camera.h"

// DX Cominterface 및 Global
ID3D11Device*			Device = NULL;
ID3D11DeviceContext*	DeviceContext = NULL;
IDXGISwapChain*			SwapChain = NULL;
ID3D11RenderTargetView* RTV = NULL;
bool                    g_bVisibleCollisionCheck = true;
MainWindow              *Main;
CMouse                  *Mouse = NULL;
// CAudio                 *Audio = NULL;


///////////////////////////////////////////////////////////////////
// Window Program Enetry 함수 
// MainWindow.cpp에 있지만, 
///////////////////////////////////////////////////////////////////
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{

	// MainWindow Class호출
	//Audio = new CAudio();
	Main = new MainWindow();

	Main->CreateInstance(hInstance, 1000, 720);   // Window  Create
//	Main->CreateInstance(hInstance, 0, 0);   // Window  Create

	Main->InitDirect3D();                        // DirectX Init
	Main->InitSceneManager();                    // SceneManger

	ImGui::Create(Main->GetWindowHandler(), Device, DeviceContext);
	ImGui::StyleColorsDark();



	// DirectWrite
	DirectWrite::Create();
	
	Mouse = new CMouse(Main->GetWindowHandler());


	//MOUSE->SetWindowHandler(Main->GetWindowHandler());

	MSG msg;

	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			TIMEMANAGER->Update(60.0f);
		//	TIMEMANAGER->Update();
			Main->Update();
			ImGui::Update();
			Mouse->Update();
		//	Audio->Update();

			Main->Render();
		}
	}

	delete Main;
	ImGui::Delete();
	DirectWrite::Delete();
	SAFE_DELETE(Mouse);
//	SAFE_DELETE(Audio);
	return (int)msg.wParam;
}

/////////////////////////////////////////////////////////////////
//  여기서 부터 MainWindow Class의 cpp파일임
/////////////////////////////////////////////////////////////////
MainWindow::MainWindow()
{
}

MainWindow::~MainWindow()
{
	Delete();
}




void MainWindow::Delete()
{
	SAFE_RELEASE(DeviceContext);
	SAFE_RELEASE(Device);
	SAFE_RELEASE(SwapChain);
	SAFE_RELEASE(RTV);
}

////////////////////////////////////////////////////////////
//  DirectX11 초기화
//  1. D3D11CreateDeviceAndSwapChain()을 이용하여  Device, DeviceContext, SwapChain을
//     생성한다
//     Device         : 자원을 생성하는 역활 ( CPU )
//     DeviceContext  : 그래픽관련          GPU  (그래픽카드 ) 
//     SwapChain      :  GDI에서 Backbuffer처럼  Front/Back buffer를 바꿔주는 역할
//     * com interface
//       1) 직접메모리 할당을 할수 없다 --> new Device() 형태를 쓸수 없다
//          2중 포인터형으로 되어있음
//       2) Release함수로 메모리를 해제해야 한다.
//       3) Description이 필요하다
//
//   2. SwapChain으로 Backbuffer를 얻어와  CreateRenderTargetView를 생성한다
////////////////////////////////////////////////////////////////
void MainWindow::InitDirect3D()
{
	// 1. D3D11CreateDeviceAndSwapChain()을 이용하여  Device, DeviceContext, SwapChain을
    //     생성한다

	// SwapChainDesc --> BufferDesc
	DXGI_MODE_DESC bufferDesc;
	{
		ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));
		bufferDesc.Width = this->GetWidth();      // 화면의 넓이
		bufferDesc.Height = this->GetHeight();    // 화면의 높이
		bufferDesc.RefreshRate.Numerator = 60;    // 분모  --
		bufferDesc.RefreshRate.Denominator = 1;   // 분자  --|  1초에 60회 Refresh 
		bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // DXGI_FORMAT_R8G8B8A8_UNORM
		bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;  // 기본형,비용
		bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;                  // 기본형,비용
	}
	// SwapChainDesc
	DXGI_SWAP_CHAIN_DESC   swapDesc;
	{
		ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
		swapDesc.BufferDesc  = bufferDesc;
		swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapDesc.BufferCount = 1;  // 2D -> 이차원좌표 , 3D -> 3차원좌표 ( Depth Buffer)
		swapDesc.SampleDesc.Count = 1;     // 기본형
		swapDesc.SampleDesc.Quality = 0;   // 기본형  확대/축소 Texture
		swapDesc.OutputWindow = this->GetWindowHandler();
		swapDesc.Windowed = TRUE;
		swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	}


	vector<D3D_FEATURE_LEVEL> feature_level =
	{
		//	D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	D3D_FEATURE_LEVEL pFeatureLevel;

	HRESULT hr = D3D11CreateDeviceAndSwapChain
	(
		NULL,                     // pAdapter   : 비데오카드를 지정하지 않을때는 기본적으로 NULL로 Setting
	    D3D_DRIVER_TYPE_HARDWARE, // DriverType : 그래픽카드가 었는 경우(D3D_DRIVER_TYPE_SOFTWARE) 
	    NULL,                     // Software   : thidy party에서 또는 별도의 dll을 개발하여 사용할때 
	    D3D11_CREATE_DEVICE_BGRA_SUPPORT,  // Flags : 2D + 3D 혼재하여 사용하는 Flag
		feature_level.data(),     // pFeatureLevels : 지원가능한 우선순위를 설정한 데이터
		feature_level.size(),     // FeatureLevels  : 위에 선언한 데이터의 갯수
		D3D11_SDK_VERSION,        // SDKVersion
		&swapDesc,                // pSwapChainDesc  : 위에 선언된 swapDescc
		&SwapChain,               // * ppSwapChain     :  output SwapChain
		&Device,                  // * ppDevice        :  output Divice
	    &pFeatureLevel,           // * pFeatureLevel   :  output FeatureLevel DX--> 
		&DeviceContext            // * ppDeviceContext :  output DeviceContext
		);

	assert(SUCCEEDED(hr));

	//   2. SwapChain으로 Backbuffer를 얻어와  CreateRenderTargetView를 생성한다
	CreateBackBuffer();

}

void MainWindow::InitSceneManager()
{
	SCENEMANAGER->GetInstance();
}
/////////////////////////////////////////////////////////////////
//   2. SwapChain으로 Backbuffer를 얻어와  CreateRenderTargetView를 생성한다
////////////////////////////////////////////////////////////////
void MainWindow::CreateBackBuffer()
{
	// Get Backbuffer
	ID3D11Texture2D* BackBuffer;
	               // com interface --> typelib ( Automation기법  --> Excel 
	HRESULT hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);
	assert(SUCCEEDED(hr));
	
	// Create RenderTargetView
	hr = Device->CreateRenderTargetView(BackBuffer, NULL, &RTV);
	assert(SUCCEEDED(hr));
	SAFE_RELEASE(BackBuffer);

	// RenderTargetView -> OMSet  PipeLine Stage : IA->VS->PS->RS->OM 
	DeviceContext->OMSetRenderTargets(1, &RTV, NULL);

	// Create ViewPort
	{
		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = (float)m_nWidth;
		viewport.Height = (float)m_nHeight;

		DeviceContext->RSSetViewports(1, &viewport);
	}
}
void MainWindow::DeleteBackBuffer()
{
	SAFE_RELEASE(RTV);

}
void MainWindow::SetWindowSize(int Width, int Height)
{
	RECT rect = { 0,0,(LONG)Width,(LONG)Height };
	UINT centerX = GetSystemMetrics(SM_CXSCREEN) - Width / 2;
	UINT centerY = GetSystemMetrics(SM_CYSCREEN) - Height / 2;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	MoveWindow(m_hWnd, 10, 10, rect.right - rect.left,
		rect.bottom - rect.top, TRUE);
}
void MainWindow::OnSize()
{
	RECT rect;
	GetClientRect(this->GetWindowHandler(), &rect);

	m_nWidth  = rect.right  - rect.left;
	m_nHeight = rect.bottom - rect.top;

	if (Device != NULL)
	{
		ImGui::Invalidate();
		DeleteBackBuffer();
		DirectWrite::DeleteBackBuffer();

		HRESULT hr = SwapChain->ResizeBuffers(0, m_nWidth, m_nHeight, DXGI_FORMAT_UNKNOWN, 0);
		assert(SUCCEEDED(hr));
		DirectWrite::CreateBackBuffer();
		CreateBackBuffer();
		ImGui::Validate();

	}

}
void MainWindow::Update()
{
	SCENEMANAGER->Update();

}
void MainWindow::Render()
{
	// 3D Render
	Color bgColor = Color(0.0f, 0.0f, 0.0f, 1.0f);
	DeviceContext->ClearRenderTargetView(RTV, (float*)bgColor);

	// another rendering
	SCENEMANAGER->Render();

	SwapChain->Present(0, 0);
}
/////////////////////////////////////////////////////////////////
//  레즈스트에 등록하고 Window를 생성
/////////////////////////////////////////////////////////////////
void MainWindow::CreateInstance(HINSTANCE hInstance, int width, int height)
{
	m_nWidth = width;
	m_nHeight = height;

	// 레즈스트리 등록
	{
		WNDCLASSEXW wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_D2D));
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	//	if (m_nWidth == 0 && m_nHeight == 0)
			wcex.lpszMenuName = NULL;
	//	else
	//	    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_D2D);
		wcex.lpszClassName = L"D2DGame";
		wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
		RegisterClassExW(&wcex);
	}

	// Window를 생성

	m_hInstance = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	if (m_nWidth == 0 && m_nHeight == 0)
	{
		m_hWnd = CreateWindowW(L"D2DGame", L"D2DGame", WS_POPUP,
			CW_USEDEFAULT, CW_USEDEFAULT, 
			GetSystemMetrics(SM_CXSCREEN), 
			GetSystemMetrics(SM_CYSCREEN)-0, 
			nullptr, nullptr, hInstance, nullptr);

		m_nWidth  = GetSystemMetrics(SM_CXSCREEN);
		m_nHeight = GetSystemMetrics(SM_CYSCREEN) - 0;

	}
	else
	{
		m_hWnd = CreateWindowW(L"D2DGame", L"D2DGame", WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, nullptr, hInstance, nullptr);
		RECT rect = { 0,0,(LONG)width,(LONG)height };
		UINT centerX = GetSystemMetrics(SM_CXSCREEN) - width / 2;
		UINT centerY = GetSystemMetrics(SM_CYSCREEN) - height / 2;
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
		MoveWindow(m_hWnd, 10, 10, rect.right - rect.left,
			rect.bottom - rect.top, TRUE);
	}

	UpdateWindow(m_hWnd);
	ShowWindow(m_hWnd, SW_SHOWDEFAULT);
	

}



/////////////////////////////////////////////////////////////////
//  Callback 
/////////////////////////////////////////////////////////////////
LRESULT MainWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui::WndProc(hWnd, message, wParam, lParam))
		return true;
	if (Mouse != NULL)
		Mouse->WndProc(message, wParam, lParam);

	switch (message)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		case VK_TAB:
			if (g_bVisibleCollisionCheck)
				g_bVisibleCollisionCheck = false;
			else
				g_bVisibleCollisionCheck = true;
			break;
		}
		break;
	case WM_COMMAND:
	{

	}
	break;
	case WM_PAINT:
	{

	}
	break;

	case WM_SIZE:
		        Main->OnSize();
     break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);;
}
