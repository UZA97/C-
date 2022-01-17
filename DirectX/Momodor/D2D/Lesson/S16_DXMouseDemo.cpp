#include  "framework.h"
#include  "S16_DXMouseDemo.h"
#include  "Object/Rect.h"
#include  "Collider/Collider.h"
#include  "Object/Marker.h"
#include  "Object/Button.h"
#include  "Object/CustomCursor.h"

////////////////////////////////////////////////////////////
//  생성자
///////////////////////////////////////////////////////////
S16_DXMouseDemo::S16_DXMouseDemo()
{
	wstring strImageFie   = L"/Sonic/SonicStage.png";
	wstring strShaderFile = L"Sprite.fx";

	Main->SetWindowSize(800, 600);

	// background, rect create
	m_pBackground = new Texture(strImageFie, strShaderFile);

	m_pRect1 = new Rect(Vector2(-200.0f,100.0f),Vector2(100.0f, 100.0f));
	m_pRect2 = new Rect(Vector2(0.0f, 0.0f), Vector2(50.0f, 50.0f));

	m_pRect1->SetRotation(0.0f, 0.0f, 45.0f);

	m_pButton = new Button();
	m_pButton->SetPosition(Vector2(-300.0f, -50.0f));
	m_pButton->SetCallback(bind(&S16_DXMouseDemo::OnButton, this));

	//ShowCursor(false);
	m_pCustomCursor = new CustomCursor(L"/cursor/cursor-charge-3.png", L"Sprite.fx");



	function<void(void)>      exec1;    // void AAA()
	function<int(void)>       exec2;    // int AAA()
	function<void(int)>   exec3;    // int AAA()

	exec1 = bind(&S16_DXMouseDemo::OnButton, this);
	//exec1();

	exec2 = bind(&S16_DXMouseDemo::Test, this);

	int aaa = exec2();
	printf("%d\n", aaa);

	// exec3 = bind(&S16_DXMouseDemo::Test2, this, placeholders::_1);  


	CAMERA->SetMoveSpeed(400.0f, 400.0f);



	
}

S16_DXMouseDemo::~S16_DXMouseDemo()
{
	SAFE_DELETE(m_pBackground);
	SAFE_DELETE(m_pRect1);
	SAFE_DELETE(m_pRect2);
	SAFE_DELETE(m_pButton);
}



void S16_DXMouseDemo::Update()
{
	//1 . View,Pojection
	Matrix V, P;
	CAMERA->Update();
	V = CAMERA->GetViewMatrix();
	P = CAMERA->GetProjectionMatrix();

	//2. 키보드, collision

	Vector2 position = Mouse->GetPosition();  // Window
	CAMERA->WCtoVC(position);                 // View Coord

	

	if (Mouse->Press(0))
	{
		if (m_pRect1->IsPtInRect(position) == true)
		//if (m_pRect1->IsPtInRegion(position) == true)
		{
			m_pRect1->SetPosition(position);
			if (Collider::AaBB(m_pRect1->GetMatrix(), m_pRect2->GetMatrix()))
			{
				m_pRect1->SetColor(1.0f, 1.0f, 0.0f, 0.8f);
				m_pRect2->SetColor(1.0f, 0.0f, 0.0f);
			}
			else
			{
				m_pRect1->SetColor(0.0f, 1.0f, 0.0f);
				m_pRect2->SetColor(0.0f, 1.0f, 0.0f);
			}

		}
	}

	// marker
	if (Mouse->DoubleClick(0))
		m_cvMarkers.push_back(new Marker(position, Vector2(1.0f, 1.0f)));



	//3. 신에 있는 해당 Object Update()

	m_pBackground->Update(V, P);
	m_pRect1->Update(V, P);
	m_pRect2->Update(V, P);
	m_pButton->Update(V, P);
	m_pCustomCursor->Update(V, P);

	for (auto a : m_cvMarkers)
		a->Update(V, P);

	CAMERA->Update(V, P);
	

}

void S16_DXMouseDemo::Render()
{



	Vector2 mouse = Mouse->GetPosition();
	CAMERA->WCtoVC(mouse);


	m_pBackground->Render();
	m_pRect1->Render();
	m_pRect2->Render();
	m_pButton->Render();


	for (auto a : m_cvMarkers)
		a->Render();

	CAMERA->Render();
	m_pCustomCursor->Render();

	// 2D Render
	DirectWrite::GetDC()->BeginDraw();
	{
		RECT  rect = { 0,0,500,200 };
		wstring str = L"FPS(ImGui) : " + to_wstring(ImGui::GetIO().Framerate);
		DirectWrite::RenderText(str, rect);

		RECT  rect2 = { 0,20,500,200 };
		wstring str2 = L"Mouse [" + to_wstring(mouse.x) + L"," + to_wstring(mouse.y) + L"]";
		DirectWrite::RenderText(str2, rect2);

	}
	DirectWrite::GetDC()->EndDraw();


}

void S16_DXMouseDemo::OnButton()
{
	MessageBox(Main->GetWindowHandler(), L"Test", L"Test", MB_OK);
}

int S16_DXMouseDemo::Test()
{
	return 5;
}

int S16_DXMouseDemo::Test2(int a, int b)
{

	return a+b;
}


