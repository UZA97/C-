#include  "framework.h"
#include  "Rect.h"
#include  "Renders/Shader.h"
#include  "Collider/Collider.h"

///////////////////////////////////////////////
//  ������
///////////////////////////////////////////////
Rect::Rect()
{
	wstring  strFileName = L"./_Shaders/10_Rect.fx";
	m_pShader = new Shader(strFileName); // VS,PS,BLOB,........
	CreateVertexBuffer();
}
Rect::Rect(Vector2 position, Vector2 scale)
{
	wstring  strFileName = L"./_Shaders/10_Rect.fx";
	m_pShader = new Shader(strFileName); // VS,PS,BLOB,........
	CreateVertexBuffer();
	SetPosition(position);
	SetScale(scale);
}

Rect::~Rect()
{
	SAFE_DELETE(m_pShader);
	SAFE_RELEASE(m_pVertexBuffer);
}

// Shader�� Constant Buffer�� Matrix�� �Ѱ�����
void Rect::Update(Matrix V, Matrix P)
{
	m_pShader->AsMatrix("View")->SetMatrix(V);
	m_pShader->AsMatrix("Projection")->SetMatrix(P);
	UpdateWorld();
}

void Rect::Render()
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
	m_pShader->AsVector("Color")->SetFloatVector(color);
	m_pShader->Draw(0, 0, 6, 0);

}
////////////////////////////////////////////////////////
// Clipping �˰��� --> ��������� Ŭ���� �˰���
// Window(Rectangle) <-> Line���� Ŭ����
//  1. OuterCode()
//  2. y = ax+ b
//  loop 1,2�� �ݺ�
//
//    1001 | 1000 | 1010
//    -----*------*-----
//    0001 | 0000 | 0010
//    -----*------*-----
//    0101 | 0100 | 0110
//
////////////////////////////////////////////////////////
int Rect::OuterCode(Vector2 BoxMin, Vector2 BoxMax, Vector2 position)
{
	const  int TOP    = 8; // 1000
	const  int BOTTOM = 4; // 0100
	const  int LEFT   = 2; // 0010
	const  int RIGHT  = 1; // 0001

	int Value = 0;

	if (position.y > BoxMax.y) Value |= TOP;
	if (position.y < BoxMin.y) Value |= BOTTOM;
	if (position.x > BoxMax.x) Value |= RIGHT;
	if (position.x < BoxMin.x) Value |= LEFT;


	return 	Value;
}
bool Rect::IsPtInRect(Vector2 position)
{
	if (m_Rotation.z != 0.0)
		return IsPtInRegion(position);

	Vector2 BoxMin = Vector2(-m_Scale.x*0.5f + m_Position.x, -m_Scale.y*0.5f + m_Position.y);
	Vector2 BoxMax = Vector2( m_Scale.x*0.5f + m_Position.x,  m_Scale.y*0.5f + m_Position.y);

	if (OuterCode(BoxMin, BoxMax, position) == 0)
		return true;
	return false;

	// 
	if (BoxMax.x >= position.x && BoxMin.x <= position.x &&
		BoxMax.y >= position.y && BoxMin.x <= position.y)
		return true;
}

bool Rect::IsPtInRegion(Vector2 position)
{
	Matrix A = m_World;   // scale, trans, rotate
	Vector2 pos[5];
	vector<Vector2*>  cvPolygons;

	// CW�̰� close polygon
	pos[0] = Vector2(-0.5f, -0.5f);
	pos[1] = Vector2(+0.5f, -0.5f);
	pos[2] = Vector2(+0.5f, +0.5f);
	pos[3] = Vector2(-0.5f, +0.5f);
	pos[4] = Vector2(-0.5f, -0.5f);

	for (int i = 0; i < 5; i++)
	{
		D3DXVec2TransformCoord(&pos[i], &pos[i], &A);
		cvPolygons.push_back(new Vector2(pos[i]));
	}

	bool value =  Collider::IsPtInRegion(cvPolygons, position);

	for (UINT i = 0; i < cvPolygons.size(); i++)
		delete cvPolygons[i];

	cvPolygons.erase(cvPolygons.begin(), cvPolygons.end());


	return value;
}

void Rect::CreateVertexBuffer()
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
	vertices[3].Color = Color(1.0f, 0.0f, 0.0f, 1.0f);
	vertices[4].Color = Color(1.0f, 0.0f, 0.0f, 1.0f);
	vertices[5].Color = Color(1.0f, 0.0f, 0.0f, 1.0f);



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

void Rect::UpdateWorld()
{
	Matrix  W, S, T, R;

	D3DXMatrixScaling(&S, m_Scale.x, m_Scale.y, 0.0f);
	D3DXMatrixTranslation(&T, m_Position.x, m_Position.y, 0.0f);
	D3DXMatrixRotationYawPitchRoll(&R, m_Rotation.y*3.141692f / 180.0f,
		m_Rotation.x*3.141692f / 180.0f,
		m_Rotation.z*3.141692f / 180.0f);
	m_World = W = S * R * T;
	m_pShader->AsMatrix("World")->SetMatrix(W);
}




