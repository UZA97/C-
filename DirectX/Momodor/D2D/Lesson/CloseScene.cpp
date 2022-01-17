#include  "framework.h"
#include "CloseScene.h"
#include  "Object/Rect.h"
#include  "Object/Snake.h"
#include  "Base/Camera.h"

#define  BOX_SIZE  20.0f
#define  MAPX      44
#define  MAPY      30
CloseScene::CloseScene()
{
	this->m_strSceneName = "CloseScene";
	Main->SetWindowSize(960, 720);
	this->SetActive(false);


	// 荐流 Block 积己

	Rect *pRect = new Rect();
	pRect->SetScale(BOX_SIZE, BOX_SIZE * MAPY);
	pRect->SetColor(0.3f, 0.5f, 0.65f);
	pRect->SetPosition(BOX_SIZE * MAPX *0.5 + BOX_SIZE * 0.5, 0.0);
	m_cvBlocks.push_back(pRect);

	pRect = new Rect();
	pRect->SetScale(BOX_SIZE, BOX_SIZE * MAPY);
	pRect->SetColor(0.3f, 0.5f, 0.65f);
	pRect->SetPosition(-BOX_SIZE * MAPX *0.5 - BOX_SIZE * 0.5, 0.0);
	m_cvBlocks.push_back(pRect);

	// 荐乞 Block 积己

	pRect = new Rect();
	pRect->SetScale(BOX_SIZE*MAPX, BOX_SIZE);
	pRect->SetColor(0.3f, 0.5f, 0.65f);
	pRect->SetPosition(0.0, BOX_SIZE*MAPY*0.5 + BOX_SIZE * 0.5);
	m_cvBlocks.push_back(pRect);

	pRect = new Rect();
	pRect->SetScale(BOX_SIZE*MAPX, BOX_SIZE);
	pRect->SetColor(0.3f, 0.5f, 0.65f);
	pRect->SetPosition(0.0, -BOX_SIZE * MAPY*0.5 - BOX_SIZE * 0.5);
	m_cvBlocks.push_back(pRect);

}

CloseScene::~CloseScene()
{


}
void CloseScene::Update()
{
	SetWindowText(Main->GetWindowHandler(), L"CloseScene");

	Matrix V, P;

	CAMERA->Update();
	V = CAMERA->GetViewMatrix();
	P = CAMERA->GetProjectionMatrix();



	for (auto block : m_cvBlocks)
		block->Update(V, P);

}

void CloseScene::Render()
{
	for (UINT i = 0; i < m_cvBlocks.size(); i++)
		m_cvBlocks[i]->Render();
}


