#include  "framework.h"
#include  "Line.h"
#include  "Renders/Shader.h"

///////////////////////////////////////////////////
// 생성자
///////////////////////////////////////////////////
Line::Line()
{
	wstring strShader = L"./_Shaders/22_collider.fx";
	m_pShader = new Shader(strShader);
}
Line::Line(Vector2 position1, Vector2 position2)
{
	wstring strShader = L"./_Shaders/22_collider.fx";
	m_pShader = new Shader(strShader);

	AddVertex(position1.x, position1.y);
	AddVertex(position2.x, position2.y);
	End();
}
///////////////////////////////////////////////////
// 소멸자
///////////////////////////////////////////////////
Line::~Line()
{
	SAFE_DELETE(m_pShader);
	SAFE_RELEASE(m_pVertexBuffer);

}


void Line::Update(Matrix V, Matrix P)
{
	m_pShader->AsMatrix("View")->SetMatrix(V);
	m_pShader->AsMatrix("Projection")->SetMatrix(P);
	UpdateWorld();
}
///////////////////////////////////////////////////
// Render
///////////////////////////////////////////////////
void Line::Render()
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
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	m_pShader->AsVector("LineColor")->SetFloatVector(color);
	m_pShader->Draw(0, 0, m_cvVertexDBs.size(), 0);
}

void Line::AddVertex(float x, float y)
{
	m_cvVertexDBs.push_back(new Vector2(x, y));
}

void Line::CreateVertexBuffer()
{
	m_pVertices = new Vertex[m_cvVertexDBs.size()];

	for (UINT i = 0; i < m_cvVertexDBs.size(); i++)
	{
		m_pVertices[i].Position = Vector3(m_cvVertexDBs[i]->x, m_cvVertexDBs[i]->y, 0.0f);  // 0
	}


	D3D11_BUFFER_DESC       desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC)); // typedef 및에 계속 typdef로 된경우가 많음
	desc.Usage = D3D11_USAGE_DEFAULT;             // GPU로 데이터가 넘어간후 변경 할 수 없음
	desc.ByteWidth = sizeof(Vertex) * m_cvVertexDBs.size();
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;    // 정점Buffer, IndexBuffer, ConstantBuffer

	D3D11_SUBRESOURCE_DATA  data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA)); // memset(&data,0x00,sixeof(...))

	data.pSysMem = m_pVertices; // 정점에 대한 데이터의 주소값

	// 자원은 디바이스로 만든다,(Buffer,Texture,Constant)
	// com interface로 된것은 생성할때 DX11에서는 descrtion이 필요하다
	// com interface로 된것은 생성한것은 Release로 지워야한다
	HRESULT hr = Device->CreateBuffer(&desc, &data, &this->m_pVertexBuffer);

	assert(SUCCEEDED(hr));

	delete m_pVertices;
}

void Line::UpdateWorld()
{
	Matrix W, T, S, R;

	D3DXMatrixScaling(&S, 1.0f, 1.0f, 0.0f);
	D3DXMatrixTranslation(&T, 0.0f, 0.0f, 0.0f);

	W = S * T;
	m_pShader->AsMatrix("World")->SetMatrix(W);
}


