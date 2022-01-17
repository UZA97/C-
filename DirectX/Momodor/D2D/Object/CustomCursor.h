#pragma once
class CustomCursor : public GameObject
{
public: 
	void   Update(Matrix V, Matrix P)  override;
	void   Render() override;

public:

	
private:
	class  Texture  *m_pTexture = nullptr;

public:
	CustomCursor(wstring strImageFile, wstring strShaderFile);
	~CustomCursor();
};