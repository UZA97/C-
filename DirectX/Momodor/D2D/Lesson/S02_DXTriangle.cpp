#include "framework.h"
#include "S02_DXTriangle.h"

S02_DXTriangle::S02_DXTriangle()
{
	SetSceneName("S02_DXTriangle");
	CreateVertexBuffer();
	CreateShader();
	CreateInputLayout();
}

S02_DXTriangle::~S02_DXTriangle()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVsBlob);
	SAFE_RELEASE(m_pPsBlob);
	SAFE_RELEASE(m_pInputLayout);
}
void S02_DXTriangle::Update()
{
	
}

void S02_DXTriangle::Render()
{
	// VS --> PS
	// D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, 
	// D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	DeviceContext->Draw(3, 0);

}
//////////////////////////////////////////////
//   user가 cpu(Device)에 생성한다
//   정점은 시계 방향으로 만든다
// DirectX/OpenGL  ---> 실제 좌표 -1~1값 : Normal값
//            |
//        1   |   2        
//    ----------------    --> 시계방향구조 
//        0   |   3 
//            |             0 : -0.5f, -0.5f , 0.0f
//                          1 : -0.5f, +0.5f , 0.0f
//                          2 : +0.5f, +0.5f , 0.0f
//                          3 : +0.5f, -0.5f , 0.0f
///////////////////////////////////////////////
void S02_DXTriangle::CreateVertexBuffer()
{
	// 1. 정점을 만든다  --> 시계방향
	vertices[0].Position = Vector3(-0.5f, -0.5f, 0.0f);
	vertices[1].Position = Vector3(-0.5f, +0.5f, 0.0f);
	vertices[2].Position = Vector3(+0.5f, +0.5f, 0.0f);

	vertices[0].Color = Color(1.0f, 0.0f, 0.0f, 1.0f);
	vertices[1].Color = Color(1.0f, 0.0f, 0.0f, 1.0f);
	vertices[2].Color = Color(1.0f, 0.0f, 0.0f, 1.0f);

	// CW 시계방향(면적이 음수값), CCW 반시계방향(면적이 양수)을 판단하는 법
	// Clock Wise     Counter Clock Wise
	{
		double area = 0;
		for (int i = 0; i < 3; i += 2)
			area += vertices[i + 1].Position.x * 
			        (vertices[i + 2].Position.y - vertices[i].Position.y) + 
			         vertices[i + 1].Position.y * 
			        (vertices[i].Position.x - vertices[i + 2].Position.x);
		area = area * 0.5f;
	}

	D3D11_BUFFER_DESC       desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC)); // typedef 및에 계속 typdef로 된경우가 많음
	desc.Usage = D3D11_USAGE_DEFAULT;             // GPU로 데이터가 넘어간후 변경 할 수 없음
	desc.ByteWidth = sizeof(Vertex) * 3;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;    // 정점Buffer, IndexBuffer, ConstantBuffer

	D3D11_SUBRESOURCE_DATA  data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA)); // memset(&data,0x00,sixeof(...))

	data.pSysMem = vertices; // 정점에 대한 데이터의 주소값

	// 자원은 디바이스로 만든다,(Buffer,Texture,Constant)
	// com interface로 된것은 생성할때 DX11에서는 descrtion이 필요하다
	// com interface로 된것은 생성한것은 Release로 지워야한다
	HRESULT hr = Device->CreateBuffer(&desc, &data, &this->m_pVertexBuffer);

	assert(SUCCEEDED(hr));

	UINT   stride = sizeof(Vertex);
	UINT   offset = 0;

	// CreateVertextBuffer() --> GPU
	DeviceContext->IASetVertexBuffers(0, // Start slot
		1, // NumBuffers
		&m_pVertexBuffer,
		&stride,  // 보낼구조체의 size
		&offset
	);
}
//////////////////////////////////////////////////////////
//  1. D3DX11CompileFromFile()을 Effect.hlsl을 컴파일
//  2. CreateVertexShader()
//  3. CreatePixelShader()
//  4. DeviceContext->VSSetShader()
//  5. DeviceContext->PSSetShader()
/////////////////////////////////////////////////////////
void S02_DXTriangle::CreateShader()
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

void S02_DXTriangle::CreateInputLayout()
{
	HRESULT hr;

	D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
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

	DeviceContext->IASetInputLayout(m_pInputLayout);
}


