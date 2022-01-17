#include  "framework.h"
#include  "S05_DXEffect.h"
#include  "Renders/Shader.h"

///////////////////////////////////////////////
//  생성자
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
		&stride,  // 보낼구조체의 size
		&offset
	);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ImGui::ColorEdit3("Color", (float*)&color);

	m_pShader->AsVector("Color")->SetFloatVector(color);
	m_pShader->Draw(0, 0, 6, 0);

}

void S05_DXEffect::CreateVertexBuffer()
{
	// 1. 정점을 만든다  --> 시계방향
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
	desc.ByteWidth = sizeof(Vertex) * 6;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;    // 정점Buffer, IndexBuffer, ConstantBuffer

	D3D11_SUBRESOURCE_DATA  data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA)); // memset(&data,0x00,sixeof(...))

	data.pSysMem = vertices; // 정점에 대한 데이터의 주소값

	// 자원은 디바이스로 만든다,(Buffer,Texture,Constant)
	// com interface로 된것은 생성할때 DX11에서는 descrtion이 필요하다
	// com interface로 된것은 생성한것은 Release로 지워야한다
	HRESULT hr = Device->CreateBuffer(&desc, &data, &this->m_pVertexBuffer);

	assert(SUCCEEDED(hr));
}


