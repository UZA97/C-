#include  "framework.h"
#include  "Collider.h"
#include  "Renders/Shader.h"
#include  "Collider/clipper.hpp"

//http://www.angusj.com/delphi/clipper.php
using namespace clipper;
enum  poly_color_type{ pctSubject,pctClip, pctSolution};
ClipType  ct = ctIntersection; // ctIntersection(교집합), ctUnion(합집합),ctDifference,ctXor
PolyFillType pft = pftNonZero;

///////////////////////////////////////////////
//  생성자
///////////////////////////////////////////////
Collider::Collider()
{
	wstring  strFileName = L"./_Shaders/22_Collider.fx";
	m_pShader = new Shader(strFileName); // VS,PS,BLOB,........
	CreateVertexBuffer();
}

Collider::~Collider()
{
	SAFE_DELETE(m_pShader);
	SAFE_RELEASE(m_pVertexBuffer);
}

// Shader에 Constant Buffer인 Matrix를 넘겨진다
void Collider::Update(Matrix V, Matrix P)
{
	m_pShader->AsMatrix("View")->SetMatrix(V);
	m_pShader->AsMatrix("Projection")->SetMatrix(P);
	UpdateWorld();
}

void Collider::Render()
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
	m_pShader->Draw(0, 0, 5, 0);

}
////////////////////////////////////////////////////////////
// AABB 알고리즘
////////////////////////////////////////////////////////////
bool Collider::AaBB(Collider * A, Collider * B)
{
	if (A->GetRotation().z != 0.0 || B->GetRotation().z != 0.0)
		return ClippingPolygon(A, B);

	float  AminX = A->GetPosition().x - A->GetScale().x*0.5f;
	float  AminY = A->GetPosition().y - A->GetScale().y*0.5f;
	float  AmaxX = A->GetPosition().x + A->GetScale().x*0.5f;
	float  AmaxY = A->GetPosition().y + A->GetScale().y*0.5f;

	float  BminX = B->GetPosition().x - B->GetScale().x*0.5f;
	float  BminY = B->GetPosition().y - B->GetScale().y*0.5f;
	float  BmaxX = B->GetPosition().x + B->GetScale().x*0.5f;
	float  BmaxY = B->GetPosition().y + B->GetScale().y*0.5f;

	if (AmaxX < BminX || AminX > BmaxX)  return false;
	if (AmaxY < BminY || AminY > BmaxY)  return false;
	return true;
}
////////////////////////////////////////////////////////////
// Matrix를 이용하여 AABB
////////////////////////////////////////////////////////////
bool Collider::AaBB(Matrix A, Matrix B)
{
	// world1영역
	float halfWidth  = fabsf(A._11*0.5f);
	float halfHeight = fabsf(A._22*0.5f);

	float leftA   = A._41 - halfWidth;
	float rightA  = A._41 + halfWidth;
	float topA    = A._42 + halfHeight;
	float bottomA = A._42 - halfHeight;

	// world2영역
	      halfWidth  = fabsf(B._11*0.5f);
	      halfHeight = fabsf(B._22*0.5f);

	float leftB   = B._41 - halfWidth;
	float rightB  = B._41 + halfWidth;
	float topB    = B._42 + halfHeight;
	float bottomB = B._42 - halfHeight;

	// 충돌 Check

	if (rightA < leftB || leftA > rightA)
		return false;
	if( topA < bottomB || bottomA > topB)
		return false;
	return true;
}
////////////////////////////////////////////////////////////
// Polygon Intersection 알고리즘
////////////////////////////////////////////////////////////
bool Collider::ClippingPolygon(Collider * A, Collider * B)
{
	Matrix mA = A->GetWorld();   // scale, trans, rotate
	Matrix mB = B->GetWorld();   // scale, trans, rotate

	Vector2 posA[5];
	Vector2 posB[5];

	// CW이고 close polygon
	posA[0] = Vector2(-0.5f,-0.5f);
	posA[1] = Vector2(+0.5f,-0.5f);
	posA[2] = Vector2(+0.5f,+0.5f);
	posA[3] = Vector2(-0.5f,+0.5f);
	posA[4] = Vector2(-0.5f,-0.5f);

	posB[0] = Vector2(-0.5f, -0.5f);
	posB[1] = Vector2(+0.5f, -0.5f);
	posB[2] = Vector2(+0.5f, +0.5f);
	posB[3] = Vector2(-0.5f, +0.5f);
	posB[4] = Vector2(-0.5f, -0.5f);

	for (int i = 0; i < 5; i++)
	{
		//printf(" [%d]  %f %f\n", i, posA[i].x, posA[i].y);
		D3DXVec2TransformCoord(&posA[i], &posA[i], &mA);
		D3DXVec2TransformCoord(&posB[i], &posB[i], &mB);
		//printf(" [%d]  %f %f\n", i, posA[i].x, posA[i].y);
	}

	// cliiper 사용

	// 1. 클리펀 엔진 변수 설정 및 폴리곤 변수
	Clipper  Engine; 
	Polygons clip, sub, solution;

	clip.resize(1);     // Polygon이 다수 있을때는 그 갯수 만큼
	sub.resize(1);      // Polygon이 다수 있을때는 그 갯수 만큼

	clip[0].resize(5);  // Polygon내의 원소 갯수
	sub[0].resize(5);   // Polygon내의 원소 갯수

	for (int i = 0; i < 5; i++)
	{
		clip[0][i].X = (int)(posA[i].x*1000.0f);
		clip[0][i].Y = (int)(posA[i].y*1000.0f);

		sub[0][i].X = (int)(posB[i].x*1000.0f);
		sub[0][i].Y = (int)(posB[i].y*1000.0f);
	}

	Engine.AddPolygons(sub, ptSubject);
	Engine.AddPolygons(clip, ptClip);
	Engine.Execute(ct, solution, pft, pft);

	int nCount = 0;

	if (solution.size() == 0)
	{
	//	printf(" 겹쳐지지 않음\n");
		return false;
	}
		
	// 출력 방법


	for (UINT i = 0; i < solution.size(); ++i)
	{

		for (UINT j = 0; j < solution[i].size(); j++)
		{
		//	printf("[%d] X=%f Y=%f\n", nCount,(solution[i][j].X / 1000.0f), (solution[i][j].Y / 1000.0f));
		//	nCount++;
		}

	}


//	printf(" 겹침\n");
	return true;
}
bool Collider::ClippingPolygon(Matrix A, Matrix B)
{
	return false;
}
/////////////////////////////////////////////////////////////////
// 영역안에 한점이 주어졌을때 Check하는 방법
//  Window winding number rule
////////////////////////////////////////////////////////////////
bool Collider::IsPtInRegion(vector<Vector2*> m_cvPolygons, Vector2 position)
{
	int  cn = 0;  // Counting number, crossing number

	for (UINT i = 0; i < (m_cvPolygons.size() - 1); i++)
	{
		if (((m_cvPolygons[i]->y <= position.y) && (m_cvPolygons[i + 1]->y > position.y)) ||
			((m_cvPolygons[i]->y > position.y) && (m_cvPolygons[i + 1]->y <= position.y)))
		{
			float vt = (float)(position.y - m_cvPolygons[i]->y) / 
				              (m_cvPolygons[i + 1]->y - m_cvPolygons[i]->y);
			float it = m_cvPolygons[i]->x + vt * (m_cvPolygons[i + 1]->x - m_cvPolygons[i]->x);

			if (position.x < it)
				++cn;
		}

	}

	if ((cn & 1) == 0)
		return false;
	return true;
}

bool Collider::IsPtInRect(Vector2 position)
{

	Vector2 BoxMin = Vector2(-m_Scale.x*0.5f + m_Position.x, -m_Scale.y*0.5f + m_Position.y);
	Vector2 BoxMax = Vector2(m_Scale.x*0.5f + m_Position.x, m_Scale.y*0.5f + m_Position.y);

	if (BoxMax.x >= position.x && BoxMin.x <= position.x &&
		BoxMax.y >= position.y && BoxMin.x <= position.y)
		return true;
	else
		return false;
}

bool Collider::IntersectionLine(Vector2 AP1, Vector2 AP2, Vector2 BP1, Vector2 BP2, Vector2 & out)
{
	double t;
	double s;
	double under = (BP2.y - BP1.y)*(AP2.x - AP1.x) - (BP2.x - BP1.x)*(AP2.y - AP1.y);
	if (under == 0) return false;

	double _t = (BP2.x - BP1.x)*(AP1.y - BP1.y) - (BP2.y - BP1.y)*(AP1.x - BP1.x);
	double _s = (AP2.x - AP1.x)*(AP1.y - BP1.y) - (AP2.y - AP1.y)*(AP1.x - BP1.x);

	t = _t / under;
	s = _s / under;

	if (t<0.0 || t>1.0 || s<0.0 || s>1.0) return false;
	if (_t == 0 && _s == 0) return false;

	out.x = AP1.x + t * (double)(AP2.x - AP1.x);
	out.y = AP1.y + t * (double)(AP2.y - AP1.y);

	return true;
}

/////////////////////////////////////////////////////////////
//
//     1    |   (3)
//          |
//    -------------
//     0    |    2         0 : -0.5f, -0.5f, 0.0f
//          |              1 : -0.5f, +0.5f, 0.0f
//                         2 : +0.5f, -0.5f, 0.0f
////////////////////////////////////////////////////////////
void Collider::CreateVertexBuffer()
{
	Vertex         vertices[10];
	// 1. 정점을 만든다  --> 시계방향
	vertices[0].Position = Vector3(-0.5f, -0.5f, 0.0f);  // 0
	vertices[1].Position = Vector3(-0.5f, +0.5f, 0.0f);  // 1
	vertices[2].Position = Vector3(+0.5f, +0.5f, 0.0f);  // 3
	vertices[3].Position = Vector3(+0.5f, -0.5f, 0.0f);  // 2
	vertices[4].Position = Vector3(-0.5f, -0.5f, 0.0f);  // 0


	D3D11_BUFFER_DESC       desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC)); // typedef 및에 계속 typdef로 된경우가 많음
	desc.Usage = D3D11_USAGE_DEFAULT;             // GPU로 데이터가 넘어간후 변경 할 수 없음
	desc.ByteWidth = sizeof(Vertex) * 5;
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

void Collider::UpdateWorld()
{
	
	Matrix W, T, S, R;

	D3DXMatrixScaling(&S, m_Scale.x, m_Scale.y, 0.0f);
	D3DXMatrixTranslation(&T, m_Position.x, m_Position.y, 0.0f);
	D3DXMatrixRotationYawPitchRoll(&R, m_Rotation.y*3.141692f / 180.0f,
		m_Rotation.x*3.141692f / 180.0f,
		m_Rotation.z*3.141692f / 180.0f);
	W = S * R * T;
	m_pShader->AsMatrix("World")->SetMatrix(W);
	m_WorldMatrix = W;

}


