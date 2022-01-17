#include "framework.h"
#include "S03_DXRectangle.h"

S03_DXRectangle::S03_DXRectangle()
{
	SetSceneName("S03_DXRectangle");
	CreateVertexBuffer();
	CreateShader();
	CreateInputLayout();
}

S03_DXRectangle::~S03_DXRectangle()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVsBlob);
	SAFE_RELEASE(m_pPsBlob);
	SAFE_RELEASE(m_pInputLayout);
}
void S03_DXRectangle::Update()
{
	
}

void S03_DXRectangle::Render()
{
	// VS --> PS
	// D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, 
	// D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DeviceContext->Draw(6, 0);

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
void S03_DXRectangle::CreateVertexBuffer()
{
	// 1. ������ �����  --> �ð����
	vertices[0].Position = Vector3(-0.5f, -0.5f, 0.0f);  // 0
	vertices[1].Position = Vector3(-0.5f, +0.5f, 0.0f);  // 1
	vertices[2].Position = Vector3(+0.5f, +0.5f, 0.0f);  // 2
	vertices[3].Position = Vector3(-0.5f, -0.5f, 0.0f);  // 0
	vertices[4].Position = Vector3(+0.5f, +0.5f, 0.0f);  // 2
	vertices[5].Position = Vector3(+0.5f, -0.5f, 0.0f);  // 3


	vertices[0].Color = Color(1.0f, 0.0f, 0.0f, 1.0f);
	vertices[1].Color = Color(1.0f, 0.0f, 0.0f, 1.0f);
	vertices[2].Color = Color(1.0f, 0.0f, 0.0f, 1.0f);
	vertices[3].Color = Color(0.0f, 0.0f, 1.0f, 1.0f);
	vertices[4].Color = Color(0.0f, 0.0f, 1.0f, 1.0f);
	vertices[5].Color = Color(0.0f, 0.0f, 1.0f, 1.0f);



	// CW �ð����(������ ������), CCW �ݽð����(������ ���)�� �Ǵ��ϴ� ��
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
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC)); // typedef �׿� ��� typdef�� �Ȱ�찡 ����
	desc.Usage = D3D11_USAGE_DEFAULT;             // GPU�� �����Ͱ� �Ѿ�� ���� �� �� ����
	desc.ByteWidth = sizeof(Vertex) * 6;
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
void S03_DXRectangle::CreateShader()
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

void S03_DXRectangle::CreateInputLayout()
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


