#include "framework.h"
#include "S01_DXLine.h"

/////////////////////////////////////////////////////////////
//  생성자 / 소멸자
////////////////////////////////////////////////////////////
S01_DXLine::S01_DXLine()
{
	SetSceneName("S01_DXLine");
	CreateVertexBuffer();
	CreateShader();
	CreateInputLayout();
	//SetWindowText(Main->GetWindowHandler(), L"S01_DXLine");
}
S01_DXLine::~S01_DXLine()
{
	SAFE_RELEASE(m_pVertextBuffer);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVsBlob);
	SAFE_RELEASE(m_pPsBlob);
	SAFE_RELEASE(m_pInputLayout);
}
void S01_DXLine::Update()
{
	
}

void S01_DXLine::Render()
{
	UINT   stride = sizeof(Vertex);
	UINT   offset = 0;

	// CreateVertextBuffer() --> GPU
	DeviceContext->IASetVertexBuffers(0, // Start slot
		1, // NumBuffers
		&m_pVertextBuffer,
		&stride,  // 보낼구조체의 size
		&offset
	);
	DeviceContext->IASetInputLayout(m_pInputLayout);
	// VS --> PS
	// D3D11_PRIMITIVE_TOPOLOGY_LINELIST, 
	// D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	DeviceContext->Draw(4, 0);





	//printf("Render %s\n", GetSceneName().c_str());
}
//////////////////////////////////////////////////////
// 랜더링 파이프라인 IA단계
// DirectX/OpenGL  ---> 실제 좌표 -1~1값 : Normal값
//            |
//        1   |   2        
//    ----------------    --> 시계방향구조 
//        0   |   3 
//            |             0 : -0.5f, -0.5f , 0.0f
//                          1 : -0.5f, +0.5f , 0.0f
//                          2 : +0.5f, +0.5f , 0.0f
//                          3 : +0.5f, -0.5f , 0.0f
//////////////////////////////////////////////////////
void S01_DXLine::CreateVertexBuffer()
{
	// 1. 정점을 만든다  --> 시계방향
	vertices[0].Poisition = Vector3(-0.5f, -0.5f, 0.0f);
	vertices[1].Poisition = Vector3(-0.5f, +0.5f, 0.0f);
	vertices[2].Poisition = Vector3(+0.5f, +0.5f, 0.0f);
	vertices[3].Poisition = Vector3(+0.5f, -0.5f, 0.0f);

	vertices[0].Color = Color(1.0f, 0.0f, 0.0f, 1.0f);
	vertices[1].Color = Color(0.0f, 1.0f, 0.0f, 1.0f);
	vertices[2].Color = Color(0.0f, 0.0f, 1.0f, 1.0f);
	vertices[3].Color = Color(1.0f, 0.0f, 0.0f, 1.0f);

	// 2. CPU자원을 GPU(IA)에 넘겨 주기위한 변환 작업을 한다 
	//    --> Buffer, Texture

	D3D11_BUFFER_DESC         desc;

	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));  // 초기화
	desc.Usage = D3D11_USAGE_DEFAULT;              // GPU에 데이터가 넘어간후 데이터를 수정할 것인가(수정X)
	desc.ByteWidth = sizeof(Vertex) * 4;           // Buffer에 보낼 데이터의 크기
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;     // Buffer종류 ( 정점,인덱스,Texture)

	D3D11_SUBRESOURCE_DATA    data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA)); // 초기화
	data.pSysMem = vertices;                        // vertices 시스템 번지

	HRESULT hr = Device->CreateBuffer(&desc, &data, &this->m_pVertextBuffer);
	assert(SUCCEEDED(hr));
}
//////////////////////////////////////////////////////////
//  1. D3DX11CompileFromFile()을 Effect.hlsl을 컴파일
//  2. CreateVertexShader()
//  3. CreatePixelShader()
//  4. DeviceContext->VSSetShader()
//  5. DeviceContext->PSSetShader()
/////////////////////////////////////////////////////////
void S01_DXLine::CreateShader()
{
	HRESULT hr;

	// 1. HLSL파일을 컴파일하고 그결과를 Blob에 저장 : Binary를 저장
	{
		hr = D3DX11CompileFromFile(
			L"Effect.hlsl",    // HLSL 파일
			0,                 // MACRO 사용 안함
			0,                 // Include 사용 안함
			"VS",              // HLSL에 정의된 function명과 동일
			"vs_5_0",          // 사용될 프로파일  문법 : vs_5_0
			0,                 // Flag1
			0,                 // Flag2
			0,                 // ThredPump
			&m_pVsBlob,        // Binary Large Object
			0,
			0
		);
		assert(SUCCEEDED(hr));

		hr = D3DX11CompileFromFile(
			L"Effect.hlsl",    // HLSL 파일
			0,                 // MACRO 사용 안함
			0,                 // Include 사용 안함
			"PS",              // HLSL에 정의된 function명과 동일
			"ps_5_0",          // 사용될 프로파일  문법 : vs_5_0
			0,                 // Flag1
			0,                 // Flag2
			0,                 // ThredPump
			&m_pPsBlob,        // Binary Large Object
			0,
			0
		);
		assert(SUCCEEDED(hr));

	}

	// 2. 쉐이더를 생성

	{
		hr = Device->CreateVertexShader(m_pVsBlob->GetBufferPointer(),
			m_pVsBlob->GetBufferSize(), NULL, &this->m_pVertexShader);
		assert(SUCCEEDED(hr));

		hr = Device->CreatePixelShader(m_pPsBlob->GetBufferPointer(),
			m_pPsBlob->GetBufferSize(), NULL, &this->m_pPixelShader);
		assert(SUCCEEDED(hr));

	}

	// 3. 만들어진 Shader를 DeviceContext Set
	DeviceContext->VSSetShader(m_pVertexShader, 0, 0);
	DeviceContext->PSSetShader(m_pPixelShader, 0, 0);

}
//////////////////////////////////////////////////////////
//  IA단계에서 VS로 정보를 넘길때 쉐이더가 받을 정보를 기술한다
//////////////////////////////////////////////////////////
void S01_DXLine::CreateInputLayout()
{
	HRESULT hr;

	D3D11_INPUT_ELEMENT_DESC layoutDesc[]=
	{
		{
			"POSITION",                  // SemanticName
			0,                           // SematicIndex
			DXGI_FORMAT_R32G32B32_FLOAT, // DXGI Format
			0,                           // Input Slot
			0,                           // AlignByteOffset
			D3D11_INPUT_PER_VERTEX_DATA, // Input Slot Class
			0                            // InstanceData StepRate
        },
	    {
			"COLOR",                     // SemanticName
			0,                           // SematicIndex
			DXGI_FORMAT_R32G32B32_FLOAT, // DXGI Format
			0,                           // Input Slot
			12,                          // AlignByteOffset
			D3D11_INPUT_PER_VERTEX_DATA, // Input Slot Class
			0                            // InstanceData StepRate
        }

	};


	hr = Device->CreateInputLayout(layoutDesc, 2,
		m_pVsBlob->GetBufferPointer(),
		m_pVsBlob->GetBufferSize(),
		&this->m_pInputLayout);
	assert(SUCCEEDED(hr));


}


