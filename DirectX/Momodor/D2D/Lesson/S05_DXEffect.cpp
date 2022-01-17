#include  "framework.h"
#include  "S05_DXEffect.h"
#include  "Renders/Shader.h"

///////////////////////////////////////////////
//  ������
///////////////////////////////////////////////
S05_DXEffect::S05_DXEffect()
{
	wstring  strFileName = L"./_Shaders/Effect.fx";
	m_pShader = new Shader(strFileName); // VS,PS,BLOB,........
	CreateVertexBuffer();

}

S05_DXEffect::~S05_DXEffect()
{
	SAFE_DELETE(m_pShader);
	SAFE_RELEASE(m_pVertexBuffer);
}


void S05_DXEffect::Update()
{
}

void S05_DXEffect::Render()
{
	UINT   stride = sizeof(Vertex);
	UINT   offset = 0;

	// CreateVertextBuffer() --> GPU
	DeviceContext->IASetVertexBuffers(0, // Start slot
		1, // NumBuffers
		&m_pVertexBuffer,
		&stride,  // ��������ü�� size
		&offset
	);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ImGui::ColorEdit3("Color", (float*)&color);

	m_pShader->AsVector("Color")->SetFloatVector(color);
	m_pShader->Draw(0, 0, 6, 0);

}

void S05_DXEffect::CreateVertexBuffer()
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
}


