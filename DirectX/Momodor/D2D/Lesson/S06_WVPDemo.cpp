#include  "framework.h"
#include  "S06_WVPDemo.h"
#include  "Renders/Shader.h"
#include  "Base/Camera.h"
#include  "Object/Rect.h"

/////////////////////////////////////////////////
//  생성자/소멸자
////////////////////////////////////////////////
S06_WVPDemo::S06_WVPDemo()
{
	Matrix W, V, P;    // World --> 객체의  Scale, 위치, Rotation 대한 matrix

	// 행렬의 초기화
	D3DXMatrixIdentity(&W);

	printf("%4.2f %4.2f %4.2f %4.2f\n", W._11, W._12, W._13, W._14);
	printf("%4.2f %4.2f %4.2f %4.2f\n", W._21, W._22, W._23, W._24);
	printf("%4.2f %4.2f %4.2f %4.2f\n", W._31, W._32, W._33, W._34);
	printf("%4.2f %4.2f %4.2f %4.2f\n", W._41, W._42, W._43, W._44);

	// 행렬에서의 위치 이동
	//Matrix W;
	D3DXMatrixTranslation(&W, 10.0f, 20.0f, 30.f);  // 행렬초기화 & 위치이동 Matrix

	printf("%4.2f %4.2f %4.2f %4.2f\n", W._11, W._12, W._13, W._14);
	printf("%4.2f %4.2f %4.2f %4.2f\n", W._21, W._22, W._23, W._24);
	printf("%4.2f %4.2f %4.2f %4.2f\n", W._31, W._32, W._33, W._34);
	printf("%4.2f %4.2f %4.2f %4.2f\n", W._41, W._42, W._43, W._44);


	Vector3 position = Vector3(20000.0f, 30000.0f, 40000.0f);  // 210,320,430

	Matrix S,T,R;

	D3DXMatrixScaling(&S, 0.05f, 0.05f, 0.0f);
	D3DXMatrixTranslation(&T, 100.0f, 200.0f, 300.f);
	D3DXMatrixRotationYawPitchRoll(&R, 0.0f, 0.0f, 3.141592f*90.0f * 45.0f);

	W = S * T *R;

	D3DXVec3TransformCoord(&position, &position, &W);

	// Rect 

	m_pRect = new Rect();
	m_pRect->SetScale(50.0f, 80.0f);
	m_pRect->SetPosition(100.0f, 200.0f);


}

S06_WVPDemo::~S06_WVPDemo()
{
}
void S06_WVPDemo::Update()
{
	Matrix W, V, P;

	CAMERA->Update();
	V = CAMERA->GetViewMatrix();
	P = CAMERA->GetProjectionMatrix();


	// W = V * P;
	// printf("%f %f\n", Main->GetWidth()*P._11, Main->GetHeight()*P._22);
	// printf("%f %f\n", P._11, P._22);


	static  float delta = 0.0f;
	delta = delta + TIMEMANAGER->Delta();
	m_pRect->SetPosition(m_pRect->GetPosition().x + delta,
		m_pRect->GetPosition().y + delta);





	m_pRect->Update(V, P);


}

void S06_WVPDemo::Render()
{
	m_pRect->Render();
}


