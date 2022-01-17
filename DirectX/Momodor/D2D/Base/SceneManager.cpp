#include "framework.h"
#include "SceneManager.h"
#include  "Lesson/S01_DXLine.h"
#include  "Lesson/S02_DXTriangle.h"
#include  "Lesson/S03_DXRectangle.h"
#include  "Lesson/S04_DXCircle.h"
#include  "Lesson/S05_DXEffect.h"
#include  "Lesson/S06_WVPDemo.h"
#include  "Lesson/S07_SnakeDemo.h"
#include  "Lesson/CloseScene.h"
#include  "Lesson/S08_DXTextureDemo.h"
#include  "Lesson/S09_DXTextureClassDemo.h"
#include  "Lesson/S10_DXBackgroundDemo.h"
#include  "Lesson/S11_DXAniamtionDemo.h"
#include  "Lesson/S12_DXRotationDemo.h"
#include  "Lesson/S13_DXCameraDemo.h"
#include  "Lesson/S14_DXAABBDemo.h"
#include  "Lesson/S16_DXMouseDemo.h"
#include  "Lesson/S18_DXFileDemo.h"
#include  "Lesson/M01_Momodora.h"
#include  "Lesson/M02_Momodora.h"

/////////////////////////////////////////////////
//  생성자
/////////////////////////////////////////////////
SceneManager::SceneManager()
{
	m_cvScenes.push_back(new M01_Momodora());       // itro scene

	// 쓰레드를 통해서 background로 여러개의 신을 Loading

	thread t1(bind(&SceneManager::ThreadStart, this));
	t1.detach();  // wait 없이 쓰레드가 동작 
}

SceneManager::~SceneManager()
{
}
void SceneManager::Delete()
{
}

void SceneManager::ChangeScene(string name)
{
	for (UINT i = 0; i < m_cvScenes.size(); i++)
	{
		m_cvScenes[i]->SetActive(false);
		if(m_cvScenes[i]->GetSceneName() == name)
			m_cvScenes[i]->SetActive(true);
	}

}

Scene * SceneManager::GetScene(string name)
{
	Scene *pScene = nullptr;

	for (UINT i = 0; i < m_cvScenes.size(); i++)
	{
		if (m_cvScenes[i]->GetSceneName() == name)
			pScene = m_cvScenes[i];
	}
	return pScene;
}

Scene * SceneManager::GetCurrentScene()
{
	Scene *pScene = nullptr;

	for (UINT i = 0; i < m_cvScenes.size(); i++)
	{
		if (!m_cvScenes[i]->IsActive())
			continue;
		pScene = m_cvScenes[i];
		break;
	}
	return pScene;
}

////////////////////////////////////////////////////////////////
// 스레드   : 데이터 (int a),코드(소스코드),heap(memory Allocation)을 공유할수 있음       
//           stack은 공유 할 수 없다
// 프로세스 : 전부공유가 가능,  IPC( Inter process Control)규약을 이영하여
//            Share할 수 있도록 정부 만들어야 함 --> 비용,시간
//            * 데이터 : 전역변수 , Static
//            * 코드   : if (m_cvScenes[i]->IsActive())
//            * HEAP   : malloc, new
//            * STACK  : 재귀함수 ( 함수포인터, 지역변수, 매개변수)
// 
////////////////////////////////////////////////////////////////
void SceneManager::ThreadStart()
{
	m_cvScenes.push_back(new M02_Momodora());
	printf("Thead End\n");
}

//////////////////////////////////////////////////
// 각각의 Scene을 Update
/////////////////////////////////////////////////
void SceneManager::Update()
{
	for (UINT i = 0; i < m_cvScenes.size(); i++)
	{
		if (m_cvScenes[i]->IsActive())
			m_cvScenes[i]->Update();
    }
}
//////////////////////////////////////////////////
// 각각의 Scene을 Rendering
/////////////////////////////////////////////////
void SceneManager::Render()
{
	for (UINT i = 0; i < m_cvScenes.size(); i++)
	{
		if (m_cvScenes[i]->IsActive())
			m_cvScenes[i]->Render();
	}
	ImGui::Render();
}



