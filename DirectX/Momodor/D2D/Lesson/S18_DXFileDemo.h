#pragma once
// World 
// View
// Project
// 향후 Class로 변경

#include  "Utilities/Xml.h"
struct GAME_FILE
{
	char  Verion[128];
	int   nCount;
};
class S18_DXFileDemo : public Scene
{
public:
	void  Update() override;   // Update(Matrix v, Matrix P) --> Object에서 사용
	void  Render() override;

public:
	void  OnButton();
	int   Test();
	int   Test2(int a);
	void  SaveAscii();
	void  ReadAscii();
	void  SaveBinary();
	void  ReadBinary();
	void  WriteXML();
	void  WriteXML2();
	void  ReadXML();
	void  ParsingItem(Xml::XMLElement* node);


private:
	class Texture     *m_pBackground = nullptr;
	class Rect        *m_pRect1      = nullptr;
	class Rect        *m_pRect2      = nullptr;
	class Button      *m_pButton     = nullptr;
	class CustomCursor *m_pCustomCursor = nullptr;
	vector<class Marker*>  m_cvMarkers;
public:
	S18_DXFileDemo();
	~S18_DXFileDemo();
};
