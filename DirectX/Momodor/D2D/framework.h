// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once

#define _CRT_SECURE_NO_WARNINGS
#ifdef _DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")   // 	std::wcout << "Window Create" << std::endl;
#endif

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

// Windows 헤더 파일
#include <windows.h>
#include <assert.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <commdlg.h> 
#include <shellapi.h>

// STL
#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <map>
#include <functional>
#include <mutex>
using namespace std;

// DirectX Library
#include <d3d11.h>
#include <D3DX11.h>
#include <D3DX10.h>
#include <D3DX10math.h>
#include <d3dx11effect.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

#pragma comment(lib, "./_Libraries/Effects11d.lib")
#pragma comment(lib, "d3dcompiler.lib")

// Direct 2D Write
#include  <d2d1_2.h>
#include  <dwrite.h>
#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")



//ImGUI 
#include "./_Libraries/imgui.h"
#include "./_Libraries/imguiDx11.h"
#pragma comment(lib, "./_Libraries/imgui.lib")


// MACRO
#define SAFE_DELETE(p)       { if(p) { delete (p); (p) = NULL; }}
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p); (p) = NULL; }}
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p) = NULL; }}
// Typedef
typedef D3DXVECTOR2 Vector2;
typedef D3DXVECTOR3 Vector3;
typedef D3DXMATRIX  Matrix;
typedef D3DXCOLOR   Color;



// User정의 파일 입니다
#define    WINX   640
#define    WINY   480
#include "MainWindow.h"
#include  "System/Time.h"       // 싱글톤 
#include  "System/Keyboard.h"   // 싱글톤
#include  "System/Mouse.h"
#include  "System/DirectWrite.h"
#include  "Base/Scene.h"        // 추상화
#include  "Base/SceneManager.h"
#include  "Base/Camera.h"       // 싱글톤
#include  "Base/GameObject.h"
#include  "Renders/Shader.h"
#include  "Renders/Texture.h"
#include  "Renders/AnimationClip.h"
#include  "Renders/Animation.h"
#include  "Utilities/String.h"
#include  "Utilities/Math.h"
#include  "Object/MoMo.h"

#define  TIMEMANAGER     Time::GetInstance()
#define  SCENEMANAGER    SceneManager::GetInstance()
#define  KEYMANAGER      Keyboard::GetInstance() 
#define  CAMERA          Camera::GetInstance() 
#define  MOMO            MoMo::GetInstance() 

extern  MainWindow *Main;                          // Main Window
extern  ID3D11Device*            Device;           // CPU 핸들링
extern  ID3D11DeviceContext*     DeviceContext;    // GPU 핸들링
extern  IDXGISwapChain*          SwapChain;        // WinAPI Backbuffer 유사한것, front/back frame
extern  ID3D11RenderTargetView*  RTV;              // 그려질 대상(윈도우)
extern  CMouse     *Mouse;