#include  "framework.h"
#include  "S10_DXBackgroundDemo.h"
#include  "Object/Background.h"
#include  "Renders/Texture.h"

/////////////////////////////////////////////////////
// 생성자
////////////////////////////////////////////////////
S10_DXBackgroundDemo::S10_DXBackgroundDemo()
{
	m_pBackground = new Background();
	m_pBackground->SetPosition(Vector2(0.0f, -300.0f));

	wstring   strImage = L"/mario.png";
	wstring   shaderFile = L"Texture.fx";
	m_pMario  = new Texture(strImage, shaderFile);
	m_pMario->SetScale(1.5f, 1.5f);

	strImage = L"/Wizard.png";
	m_pWizard = new Texture(strImage, shaderFile);
	m_pWizard->SetPosition(-322.0f, -194.0f);
	m_pWizard->SetScale(3.0f, 3.0f);

}

S10_DXBackgroundDemo::~S10_DXBackgroundDemo()
{
}
////////////////////////////////////////////////////
// Scene Update
//  1. View, Projection Matrix
//  2.  Collision
//  3. Object별로 Update()를 실시
////////////////////////////////////////////////////
void S10_DXBackgroundDemo::Update()
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
		printf("1 Button Down\n");
	if (Mouse->DoubleClick(2))
		printf("3 Button DoubleClick\n");

//	printf("VC : %f %f\n", position.x, position.y);



	//3. 
	m_pBackground->Update(V, P);
	m_pMario->Update(V, P);
	m_pWizard->Update(V, P);
	CAMERA->Update(V, P);

}

void S10_DXBackgroundDemo::Render()
{
	m_pBackground->Render();

	// Render전에 Debug 용으로  ImGUI 를 이용

	Vector2  position = m_pMario->GetPosition();

	ImGui::SliderFloat("Mario coord X:", &position.x, -500, 500);
	ImGui::SliderFloat("Mario coord Y:", &position.y, -500, 500);

	m_pMario->SetPosition(position);




	m_pMario->Render();
	m_pWizard->Render();

	CAMERA->Render();


}


