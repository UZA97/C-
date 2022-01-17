#include "framework.h"
#include "S04_DXCircle.h"

#define  PI  3.141592f
S04_DXCircle::S04_DXCircle()
{
	SetSceneName("S04_DXCircle");
	CreateVertexBuffer();
	CreateShader();
	CreateInputLayout();
}

S04_DXCircle::~S04_DXCircle()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVsBlob);
	SAFE_RELEASE(m_pPsBlob);
	SAFE_RELEASE(m_pInputLayout);
}
void S04_DXCircle::Update()
{
	// Circle Ŀ����,�۾�����  ������ �ٲ��

	m_Time = m_Time + TIMEMANAGER->Delta();

	if (m_Time >= 0.8f)
		m_Time = 0.0f;
	
	m_nVertexCount = 0;

	for (int i = 0; i < 361; i++)
	{
		float x =  m_Time      * cosf(PI*i / 180.0f);
		float y =  m_Time      * sinf(PI*i / 180.0f);
		float x2 = m_Time*0.8f * cosf(PI*i / 180.0f);
		float y2 = m_Time*0.8f * sinf(PI*i / 180.0f);
		vertices[m_nVertexCount].Position = Vector3(x, y, 0.0f);
		vertices[m_nVertexCount].Color = Color(1.0f, 0.0f, 0.0f, 1.0f);
		m_nVertexCount++;
		vertices[m_nVertexCount].Position = Vector3(x2, y2, 0.0f);
		vertices[m_nVertexCount].Color = Color(0.0f, 0.0f, 1.0f, 1.0f);
		m_nVertexCount++;

	}

	// CreateVertexBuffer()���� desc.Usage = D3D11_USAGE_DYNAMIC ���� 
	// DeviceContext->UpdateSubResouce()�� ���� VertexBuffer�� ������ 
	// ������ �� ����
	// map �� unmap���̿��� �����͸� GPU�� wrire�� ����  ����
	D3D11_MAPPED_SUBRESOURCE  subResource;
	DeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, vertices, sizeof(Vertex)*m_nVertexCount);
	}
	DeviceContext->Unmap(m_pVertexBuffer, 0);

}

void S04_DXCircle::Render()
{
	// VS --> PS
	// D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, 
	// D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	DeviceContext->Draw(m_nVertexCount, 0);

}
//////////////////////////////////////////////
//   user�� cpu(Device)�� �����Ѵ�
//   ������ �ð� �������� �����
// DirectX/OpenGL  ---> ���� ��ǥ -1~1�� : Normal��
//            |
//        1   |   2        
//    ----------------    --> �ð���ⱸ�� 
//        0   |   3 
//            |             0 : -0.5f, -0.5f , 0.0f
//                          1 : -0.5f, +0.5f , 0.0f
//                          2 : +0.5f, +0.5f , 0.0f
//                          3 : +0.5f, -0.5f , 0.0f
///////////////////////////////////////////////
void S04_DXCircle::CreateVertexBuffer()
{
	// 1. ������ �����  --> �ð����

	m_nVertexCount = 0;

	for (int i = 0; i < 361; i++)
	{
		float x  = 0.5f * cosf(PI*i / 180.0f);
		float y  = 0.5f * sinf(PI*i / 180.0f);
		float x2 = 0.4f * cosf(PI*i / 180.0f);
		float y2 = 0.4f * sinf(PI*i / 180.0f);
		vertices[m_nVertexCount].Position = Vector3(x, y, 0.0f);
		vertices[m_nVertexCount].Color = Color(1.0f, 0.0f, 0.0f, 1.0f);
		m_nVertexCount++;
		vertices[m_nVertexCount].Position = Vector3(x2, y2, 0.0f);
		vertices[m_nVertexCount].Color = Color(1.0f, 0.0f, 0.0f, 1.0f);
		m_nVertexCount++;

	}


	D3D11_BUFFER_DESC       desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC)); // typedef �׿� ��� typdef�� �Ȱ�찡 ����
	{ // Read Only
	//	desc.Usage = D3D11_USAGE_DEFAULT;             // GPU�� �����Ͱ� �Ѿ�� ���� �� �� ����
	}
	{ // Read & Write
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	

	desc.ByteWidth = sizeof(Vertex) * m_nVertexCount;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;    // ����Buffer, IndexBuffer, ConstantBuffer

	D3D11_SUBRESOURCE_DATA  data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA)); // memset(&data,0x00,sixeof(...))

	data.pSysMem = vertices; // ������ ���� �������� �ּҰ�

	// �ڿ��� ����̽��� �����,(Buffer,Texture,Constant)
	// com interface�� �Ȱ��� �����Ҷ� DX11������ descrtion�� �ʿ��ϴ�
	// com interface�� �Ȱ��� �����Ѱ��� Release�� �������Ѵ�
	HRESULT hr = Device->CreateBuffer(&desc, &data, &this->m_pVertexBuffer);

	assert(SUCCEEDED(hr));

	UINT   stride = sizeof(Vertex);
	UINT   offset = 0;

	// CreateVertextBuffer() --> GPU
	DeviceContext->IASetVertexBuffers(0, // Start slot
		1, // NumBuffers
		&m_pVertexBuffer,
		&stride,  // ��������ü�� size
		&offset
	);
}
//////////////////////////////////////////////////////////
//  1. D3DX11CompileFromFile()�� Effect.hlsl�� ������
//  2. CreateVertexShader()
//  3. CreatePixelShader()
//  4. DeviceContext->VSSetShader()
//  5. DeviceContext->PSSetShader()
/////////////////////////////////////////////////////////
void S04_DXCircle::CreateShader()
{
	HRESULT hr;

	// 1. HLSL������ �������ϰ� �װ���� Blob�� ���� : Binary�� ����
	{
		hr = D3DX11CompileFromFile(
			L"Effect.hlsl",    // HLSL ����
			0,                 // MACRO ��� ����
			0,                 // Include ��� ����
			"VS",              // HLSL�� ���ǵ� function��� ����
			"vs_5_0",          // ���� ��������  ���� : vs_5_0
			0,                 // Flag1
			0,                 // Flag2
			0,                 // ThredPump
			&m_pVsBlob,        // Binary Large Object
			0,
			0
		);
		assert(SUCCEEDED(hr));

		hr = D3DX11CompileFromFile(
			L"Effect.hlsl",    // HLSL ����
			0,                 // MACRO ��� ����
			0,                 // Include ��� ����
			"PS",              // HLSL�� ���ǵ� function��� ����
			"ps_5_0",          // ���� ��������  ���� : vs_5_0
			0,                 // Flag1
			0,                 // Flag2
			0,                 // ThredPump
			&m_pPsBlob,        // Binary Large Object
			0,
			0
		);
		assert(SUCCEEDED(hr));

	}

	// 2. ���̴��� ����

	{
		hr = Device->CreateVertexShader(m_pVsBlob->GetBufferPointer(),
			m_pVsBlob->GetBufferSize(), NULL, &this->m_pVertexShader);
		assert(SUCCEEDED(hr));

		hr = Device->CreatePixelShader(m_pPsBlob->GetBufferPointer(),
			m_pPsBlob->GetBufferSize(), NULL, &this->m_pPixelShader);
		assert(SUCCEEDED(hr));

	}

	// 3. ������� Shader�� DeviceContext Set
	DeviceContext->VSSetShader(m_pVertexShader, 0, 0);
	DeviceContext->PSSetShader(m_pPixelShader, 0, 0);
}

void S04_DXCircle::CreateInputLayout()
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


