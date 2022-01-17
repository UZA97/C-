#include "framework.h"
#include "S01_DXLine.h"

/////////////////////////////////////////////////////////////
//  ������ / �Ҹ���
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
		&stride,  // ��������ü�� size
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
// ������ ���������� IA�ܰ�
// DirectX/OpenGL  ---> ���� ��ǥ -1~1�� : Normal��
//            |
//        1   |   2        
//    ----------------    --> �ð���ⱸ�� 
//        0   |   3 
//            |             0 : -0.5f, -0.5f , 0.0f
//                          1 : -0.5f, +0.5f , 0.0f
//                          2 : +0.5f, +0.5f , 0.0f
//                          3 : +0.5f, -0.5f , 0.0f
//////////////////////////////////////////////////////
void S01_DXLine::CreateVertexBuffer()
{
	// 1. ������ �����  --> �ð����
	vertices[0].Poisition = Vector3(-0.5f, -0.5f, 0.0f);
	vertices[1].Poisition = Vector3(-0.5f, +0.5f, 0.0f);
	vertices[2].Poisition = Vector3(+0.5f, +0.5f, 0.0f);
	vertices[3].Poisition = Vector3(+0.5f, -0.5f, 0.0f);

	vertices[0].Color = Color(1.0f, 0.0f, 0.0f, 1.0f);
	vertices[1].Color = Color(0.0f, 1.0f, 0.0f, 1.0f);
	vertices[2].Color = Color(0.0f, 0.0f, 1.0f, 1.0f);
	vertices[3].Color = Color(1.0f, 0.0f, 0.0f, 1.0f);

	// 2. CPU�ڿ��� GPU(IA)�� �Ѱ� �ֱ����� ��ȯ �۾��� �Ѵ� 
	//    --> Buffer, Texture

	D3D11_BUFFER_DESC         desc;

	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));  // �ʱ�ȭ
	desc.Usage = D3D11_USAGE_DEFAULT;              // GPU�� �����Ͱ� �Ѿ�� �����͸� ������ ���ΰ�(����X)
	desc.ByteWidth = sizeof(Vertex) * 4;           // Buffer�� ���� �������� ũ��
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;     // Buffer���� ( ����,�ε���,Texture)

	D3D11_SUBRESOURCE_DATA    data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA)); // �ʱ�ȭ
	data.pSysMem = vertices;                        // vertices �ý��� ����

	HRESULT hr = Device->CreateBuffer(&desc, &data, &this->m_pVertextBuffer);
	assert(SUCCEEDED(hr));
}
//////////////////////////////////////////////////////////
//  1. D3DX11CompileFromFile()�� Effect.hlsl�� ������
//  2. CreateVertexShader()
//  3. CreatePixelShader()
//  4. DeviceContext->VSSetShader()
//  5. DeviceContext->PSSetShader()
/////////////////////////////////////////////////////////
void S01_DXLine::CreateShader()
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
//////////////////////////////////////////////////////////
//  IA�ܰ迡�� VS�� ������ �ѱ涧 ���̴��� ���� ������ ����Ѵ�
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


