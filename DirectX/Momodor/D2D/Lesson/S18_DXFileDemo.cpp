#include  "framework.h"
#include  "S18_DXFileDemo.h"
#include  "Object/Rect.h"
#include  "Collider/Collider.h"
#include  "Object/Marker.h"
#include  "Object/Button.h"
#include  "Object/CustomCursor.h"
#include  "Utilities/Xml.h"

////////////////////////////////////////////////////////////
//  생성자
///////////////////////////////////////////////////////////
S18_DXFileDemo::S18_DXFileDemo()
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
	m_pButton->SetCallback(bind(&S18_DXFileDemo::OnButton, this));

	//ShowCursor(false);
	m_pCustomCursor = new CustomCursor(L"/cursor/cursor-charge-3.png", L"Sprite.fx");



	function<void(void)>      exec1;    // void AAA()
	function<int(void)>       exec2;    // int AAA()
	function<void(int)>   exec3;    // int AAA()

	exec1 = bind(&S18_DXFileDemo::OnButton, this);
	//exec1();

	exec2 = bind(&S18_DXFileDemo::Test, this);

	int aaa = exec2();
	printf("%d\n", aaa);

	 exec3 = bind(&S18_DXFileDemo::Test2, this, placeholders::_1);  

	 int k = 7;
	 int aaa2;
	 exec3(k);
	// int aaa2 = exec3(7);
	// printf("%d\n", exec3(k));


	CAMERA->SetMoveSpeed(400.0f, 400.0f);



	
}

S18_DXFileDemo::~S18_DXFileDemo()
{
	SAFE_DELETE(m_pBackground);
	SAFE_DELETE(m_pRect1);
	SAFE_DELETE(m_pRect2);
	SAFE_DELETE(m_pButton);
}



void S18_DXFileDemo::Update()
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

void S18_DXFileDemo::Render()
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

	// ImGui

	if (ImGui::Button("Save Ascii"))
		SaveAscii();
	ImGui::SameLine();
	if (ImGui::Button("Read Ascii"))
		ReadAscii();

	if (ImGui::Button("Save Binary"))
		SaveBinary();
	ImGui::SameLine();
	if (ImGui::Button("Read Binary"))
		ReadBinary();

	if (ImGui::Button("Write XML"))
		WriteXML();
	ImGui::SameLine();
	if (ImGui::Button("Read XML"))
		ReadXML();


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

void S18_DXFileDemo::OnButton()
{
	MessageBox(Main->GetWindowHandler(), L"Test", L"Test", MB_OK);
}

int S18_DXFileDemo::Test()
{
	return 5;
}

int S18_DXFileDemo::Test2(int a)
{

	return a;
}

void S18_DXFileDemo::SaveAscii()
{
	FILE *fp;

	fopen_s(&fp, "./save.txt", "w");    // fp = fopen("./save.txt","w");

	fprintf(fp, "Count %d\n", m_cvMarkers.size());
	for (UINT i = 0; i < this->m_cvMarkers.size(); i++)
	{
		fprintf(fp, "%f %f\n", m_cvMarkers[i]->GetPosition().x, m_cvMarkers[i]->GetPosition().y);
	}
	fclose(fp);

	system("Notepad.exe ./save.txt");
}

void S18_DXFileDemo::ReadAscii()
{
	FILE  *fp;
	char  buf[1000];

	// 기존데이터 지우기

	for (unsigned int i = 0; i < m_cvMarkers.size(); i++)
		delete m_cvMarkers[i];

	m_cvMarkers.erase(m_cvMarkers.begin(), m_cvMarkers.end());

	fopen_s(&fp, "./Save.txt", "r");

	// 1st line read
	fgets(buf, 1000, fp);
	int nCount = 0;

	buf[strlen(buf) - 1] = '\0';
	sscanf(buf, "%*s %d", &nCount);
	printf("%s  --> %d\n", buf, nCount);

	for (int i = 0; i < nCount; i++)
	{
		fgets(buf, 1000, fp);     // fscanf("%f %f",&X,&Y);
		float X, Y;
		sscanf(buf,"%f %f", &X, &Y);
		printf("%f  %f\n", X, Y);

		m_cvMarkers.push_back(new Marker(Vector2(X, Y), Vector2(1.0f, 1.0f)));
	}


	//fread(buf, sizeof(char), 1000, fp);

	fclose(fp);
}

void S18_DXFileDemo::SaveBinary()
{
	FILE *fp;
	GAME_FILE  title;
	Vector2    DATA[500];

	fopen_s(&fp, "./Save.bin", "wb");   // c+ 형태

	strcpy_s(title.Verion, "Game Version");
	title.nCount = this->m_cvMarkers.size();

	fwrite(&title, sizeof(GAME_FILE), 1, fp);

	for (UINT i = 0; i < m_cvMarkers.size(); i++)
		DATA[i] = m_cvMarkers[i]->GetPosition();

	fwrite(DATA, sizeof(Vector2), m_cvMarkers.size(), fp);

	fclose(fp);

}

void S18_DXFileDemo::ReadBinary()
{
	FILE *fp;
	GAME_FILE  title;
	Vector2    DATA[500];

	// 기존데이터 지우기

	for (unsigned int i = 0; i < m_cvMarkers.size(); i++)
		delete m_cvMarkers[i];

	m_cvMarkers.erase(m_cvMarkers.begin(), m_cvMarkers.end());



	fopen_s(&fp, "./Save.bin", "rb");   // c+ 형태

	fread(&title, sizeof(GAME_FILE), 1, fp);


	if (strcmp(title.Verion, "Game Version") != 0)
	{
		fclose(fp);
		return;

	}

	for (UINT i = 0; i < title.nCount; i++)
	{
		float X, Y;
		fread(&X, sizeof(float), 1, fp);
		fread(&Y, sizeof(float), 1, fp);
		m_cvMarkers.push_back(new Marker(Vector2(X, Y), Vector2(1.0f, 1.0f)));
	}

//	fread(DATA, sizeof(Vector2), title.nCount, fp);

	


	fclose(fp);

}

void S18_DXFileDemo::WriteXML()
{
	WriteXML2();


	FILE *fp;

	fopen_s(&fp, "./test.xml", "w");
	if (fp == NULL)
	{
		MessageBox(Main->GetWindowHandler(), L"파일 저장시 문제가 발생", L"Game", MB_OK);
		return;
	}

	fprintf(fp, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fprintf(fp, "<SGA08>\n");

	for (UINT i = 0; i < m_cvMarkers.size(); i++)
	{
		char TAB = '\t';
		fprintf(fp, "%c <ITEM>\n",TAB);
		fprintf(fp, "%c%c<X>%f</X>\n", TAB, TAB, m_cvMarkers[i]->GetPosition().x);
		fprintf(fp, "%c%c<Y>%f</Y>\n", TAB, TAB, m_cvMarkers[i]->GetPosition().y);


		fprintf(fp, "%c </ITEM>\n", TAB);
	}

	fprintf(fp, "</SGA08>\n");


	fclose(fp);

	system("NotePad.exe ./test.xml");
}

void S18_DXFileDemo::WriteXML2()
{
	Xml::XMLDocument*    doc  = new Xml::XMLDocument();
	Xml::XMLDeclaration* decl = doc->NewDeclaration();

	doc->LinkEndChild(decl);

	Xml::XMLElement *root = doc->NewElement("SGA08");
	doc->LinkEndChild(root);

	Xml::XMLElement *node = doc->NewElement("ITEM");
	root->LinkEndChild(node);

	Xml::XMLElement *node2 = doc->NewElement("X");
	root->LinkEndChild(node2);
	node2->SetText("23.56");

	doc->SaveFile("./test2.xml");
	SAFE_DELETE(doc);


}

void S18_DXFileDemo::ReadXML()
{
	Xml::XMLDocument reader;
	reader.LoadFile("./test.xml");
	if (reader.Error() == true)
	{
		printf("%s\n", reader.ErrorStr());
		return;
	}
		
	// parsing
	Xml::XMLElement* root = NULL;
	Xml::XMLElement* node = NULL;
	Xml::XMLElement* Childnode = NULL;

	for (node = reader.FirstChildElement(); node; node=node->NextSiblingElement())
	{
		string strTemp = node->Value();
		printf("--> %s\n", strTemp.c_str()); // doc name
		for (Childnode = node->FirstChildElement(); Childnode; Childnode = Childnode->NextSiblingElement())
		{
			string strTemp2 = Childnode->Value();
			if (strTemp2 != "ITEM") continue;
			ParsingItem(Childnode);

			printf("--> %s\n", strTemp2.c_str()); // ITEMDESC name
		}



		
	}


	printf("...................\n");
}

void S18_DXFileDemo::ParsingItem(Xml::XMLElement * node)
{
	Xml::XMLElement* Childnode = NULL;
	float X = 0;
	float Y = 0;


	for (Childnode = node->FirstChildElement(); Childnode; Childnode = Childnode->NextSiblingElement())
	{
		string strTemp = Childnode->Value();
		if (strTemp == "X")
		{
			string temp2 = Childnode->GetText(); // 값
			X = stof(temp2);
		}

		if (strTemp == "Y")
		{
			string temp2 = Childnode->GetText(); // 값
			Y = stof(temp2);
	     }
	}

	printf("X= %f Y=%f\n", X, Y);

	m_cvMarkers.push_back(new Marker(Vector2(X, Y), Vector2(1.0f, 1.0f)));

	
}


