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
//  ������
/////////////////////////////////////////////////
SceneManager::SceneManager()
{
	m_cvScenes.push_back(new M01_Momodora());       // itro scene

	// �����带 ���ؼ� background�� �������� ���� Loading

	thread t1(bind(&SceneManager::ThreadStart, this));
	t1.detach();  // wait ���� �����尡 ���� 
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
// ������   : ������ (int a),�ڵ�(�ҽ��ڵ�),heap(memory Allocation)�� �����Ҽ� ����       
//           stack�� ���� �� �� ����
// ���μ��� : ���ΰ����� ����,  IPC( Inter process Control)�Ծ��� �̿��Ͽ�
//            Share�� �� �ֵ��� ���� ������ �� --> ���,�ð�
//            * ������ : �������� , Static
//            * �ڵ�   : if (m_cvScenes[i]->IsActive())
//            * HEAP   : malloc, new
//            * STACK  : ����Լ� ( �Լ�������, ��������, �Ű�����)
// 
////////////////////////////////////////////////////////////////
void SceneManager::ThreadStart()
{
	m_cvScenes.push_back(new M02_Momodora());
	printf("Thead End\n");
}

//////////////////////////////////////////////////
// ������ Scene�� Update
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
// ������ Scene�� Rendering
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



