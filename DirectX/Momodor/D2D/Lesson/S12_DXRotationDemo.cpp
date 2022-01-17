#include  "framework.h"
#include  "S12_DXRotationDemo.h"
#include  "Object/Background.h"
#include  "Object/Marco.h"

/////////////////////////////////////////////////////
// 생성자
////////////////////////////////////////////////////
S12_DXRotationDemo::S12_DXRotationDemo()
{
	m_pBackground = new Background();
	m_pBackground->SetPosition(Vector2(0.0f, -300.0f));

	wstring   strImage = L"/mario.png";
	wstring   shaderFile = L"Texture.fx";


	strImage = L"/Wizard.png";
	m_pWizard = new Texture(strImage, shaderFile);
	m_pWizard->SetPosition(70.0f, -191.0f);
	m_pWizard->SetScale(5.0f, 5.0f);


	m_pMarco = new Marco();
	m_pMarco->SetScale(2.5f, 2.5f);
	m_pMarco->SetPosition(-420.0f, -195.0f);
}

S12_DXRotationDemo::~S12_DXRotationDemo()
{
}
////////////////////////////////////////////////////
// Scene Update
//  1. View, Projection Matrix
//  2.  Collision
//  3. Object별로 Update()를 실시
////////////////////////////////////////////////////
void S12_DXRotationDemo::Update()
{
	Matrix V, P;

	//1. View, Projection Matrix 
	CAMERA->Update();
	V = CAMERA->GetViewMatrix();
	P = CAMERA->GetProjectionMatrix();

	//2. Collsion, 키보드......

	Vector2 position = Mouse->GetPosition();
	CAMERA->WCtoVC(position);

	if (Mouse->Down(0))
	{
		printf("1 Button Down\n");
		printf("%f %f\n", position.x, position.y);
	}
		
	if (Mouse->DoubleClick(2))
		printf("3 Button DoubleClick\n");


	//3. Rotation Test
	m_pWizard->SetRotation(0.0f, 0.0f, Math::ToDegree(sinf(TIMEMANAGER->GetRunning()*Math::PI)));

	printf("%f\n", TIMEMANAGER->GetRunning());

	m_pBackground->Update(V, P);
	m_pWizard->Update(V, P);
	CAMERA->Update(V, P);
	m_pMarco->Update(V, P);
}

void S12_DXRotationDemo::Render()
{
	m_pBackground->Render();
	CAMERA->Render();

	m_pWizard->Render();
	m_pMarco->Render();
}


