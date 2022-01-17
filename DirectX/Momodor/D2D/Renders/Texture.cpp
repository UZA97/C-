#include  "framework.h"
#include  "Texture.h"
#include  "Renders/Shader.h"
#include  <io.h>

//////////////////////////////////////////////////////////
// Image가 1장으로 구성되어 있는 경우
// ex) background
//////////////////////////////////////////////////////////
Texture::Texture(wstring strImageFile, wstring strShader)
{
	wstring  tmpImageFile  = L"./_Textures/" + strImageFile;
	wstring  tmpShaderFile = L"./_Shaders/"  + strShader;

	m_pShader = new Shader(tmpShaderFile);
	CreateShaderResourceView(tmpImageFile);
	CreateVertexBuffer(0.0f, 0.0f, 0.0f, 0.0f);

}
//////////////////////////////////////////////////////////
// Image가 1장내에 여러 컷으로 구성되어 있는 경우
// ex) Player
//////////////////////////////////////////////////////////
Texture::Texture(wstring strImageFile, wstring strShader, int startX, int startY, int endX, int endY)
{
	wstring  tmpImageFile = L"./_Textures/" + strImageFile;
	wstring  tmpShaderFile = L"./_Shaders/" + strShader;

	m_pShader = new Shader(tmpShaderFile);
	CreateShaderResourceView(tmpImageFile);
	CreateVertexBuffer((float)startX, (float)startY, (float)endX, (float)endY);
}
//////////////////////////////////////////////////////////
//  소멸자
//////////////////////////////////////////////////////////
Texture::~Texture()
{
	SAFE_RELEASE(this->m_pVertexBuffer);
	SAFE_RELEASE(this->m_pSRV);
	SAFE_DELETE(m_pShader);

}
//////////////////////////////////////////////////////////
// Update
/////////////////////////////////////////////////////////
void Texture::Update(Matrix V, Matrix P)
{
	m_pShader->AsMatrix("View")->SetMatrix(V); // Scene에서 만들어진 View matrix
	m_pShader->AsMatrix("Projection")->SetMatrix(P);
	UpdateWorld();
}
//////////////////////////////////////////////////////////
// Texture World Matrix
/////////////////////////////////////////////////////////
void Texture::UpdateWorld()
{
	Matrix W, T, S, R;

	D3DXMatrixScaling(&S, m_Scale.x*m_SpriteSize.x, m_Scale.y*m_SpriteSize.y, 0.0f);
	D3DXMatrixTranslation(&T, m_Position.x, m_Position.y, 0.0f);
	D3DXMatrixRotationYawPitchRoll(&R, m_Rotation.y*3.141692f / 180.0f,
		m_Rotation.x*3.141692f / 180.0f,
		m_Rotation.z*3.141692f / 180.0f);
	W = S * R * T;
	m_pShader->AsMatrix("World")->SetMatrix(W);
}
//////////////////////////////////////////////////////////////
// UV값을 변경하는 Logic
//////////////////////////////////////////////////////////////
void Texture::UpDateBlock()
{
	Vertex    vertices[6];

	// 1. 정점을 만든다  --> 시계방향
	vertices[0].Position = Vector3(-0.5f, -0.5f, 0.0f);  // 0
	vertices[1].Position = Vector3(-0.5f, +0.5f, 0.0f);  // 1
	vertices[2].Position = Vector3(+0.5f, -0.5f, 0.0f);  // 3
	vertices[3].Position = Vector3(+0.5f, -0.5f, 0.0f);  // 3
	vertices[4].Position = Vector3(-0.5f, +0.5f, 0.0f);  // 1
	vertices[5].Position = Vector3(+0.5f, +0.5f, 0.0f);  // 2

	// 2. UV 값
	float startX = m_SpriteOffset.x /  m_ImageSize.x;
	float startY = m_SpriteOffset.y /  m_ImageSize.y;
	float endX   = (m_SpriteOffset.x + m_SpriteSize.x) / m_ImageSize.x;
	float endY   = (m_SpriteOffset.y + m_SpriteSize.y) / m_ImageSize.y;


	vertices[0].Uv = Vector2(startX, endY);
	vertices[1].Uv = Vector2(startX, startY);
	vertices[2].Uv = Vector2(endX, endY);
	vertices[3].Uv = Vector2(endX, endY);
	vertices[4].Uv = Vector2(startX, startY);
	vertices[5].Uv = Vector2(endX, startY);
	
	// 버퍼생성시 CpU Access write option일 경우
	// map 과 unmap 사이에서 updata를 할수가 있다

	D3D11_MAPPED_SUBRESOURCE subResource;
	DeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, vertices, sizeof(Vertex) * 6);
	}
	DeviceContext->Unmap(m_pVertexBuffer, 0);

}

void Texture::Render()
{
	UINT   stride = sizeof(Vertex);
	UINT   offset = 0;

	UpdateWorld();
	UpDateBlock();  // Sprite Size변경

	// CreateVertextBuffer() --> GPU
	DeviceContext->IASetVertexBuffers(0, // Start slot
		1, // NumBuffers
		&m_pVertexBuffer,
		&stride,  // 보낼구조체의 size
		&offset
	);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


//	m_pShader->AsScalar("Time")->SetFloat(time);
	m_pShader->Draw(m_nTech, m_nPass, 6, 0);  // tech, pass
}
void Texture::SetFloatConstant(string strName, float delta)
{
	m_pShader->AsScalar(strName.c_str())->SetFloat(delta);
}
Matrix Texture::GetWorld()
{
	Matrix W, T, S, R;

	D3DXMatrixScaling(&S, m_Scale.x*m_SpriteSize.x, m_Scale.y*m_SpriteSize.y, 0.0f);
	D3DXMatrixTranslation(&T, m_Position.x, m_Position.y, 0.0f);
	D3DXMatrixRotationYawPitchRoll(&R, m_Rotation.y*3.141692f / 180.0f,
		m_Rotation.x*3.141692f / 180.0f,
		m_Rotation.z*3.141692f / 180.0f);
	W = S * R * T;

	return W;
}
Vector2 Texture::GetRealTextureSize()
{
	Vector2 size = Vector2(m_SpriteSize.x*m_Scale.x, m_SpriteSize.y*m_Scale.y);
	return   size;
}
/////////////////////////////////////////////////////////////
// Image File을 이용하여 m_pSRV를 생성
/////////////////////////////////////////////////////////////
void Texture::CreateShaderResourceView(wstring strImageFile)
{
	m_strImageFile = strImageFile;

	string str = String::ToString(m_strImageFile);

	if (_access(str.c_str(), 00) != 0)
	{
		str = " file not found  " + str;
		MessageBoxA(NULL, str.c_str(), "file not found Error", MB_OK);
		return;
	}


	HRESULT hr = D3DX11CreateShaderResourceViewFromFile(
		Device, strImageFile.c_str(), NULL, NULL, &m_pSRV, NULL);
	assert(SUCCEEDED(hr));

	m_pShader->AsShaderResource("DiffuseMap")->SetResource(m_pSRV);
}
////////////////////////////////////////////////////////////
// 정점 Buffer생성
//
//   user가 cpu(Device)에 생성한다
//   정점은 시계 방향으로 만든다
// DirectX/OpenGL  ---> 실제 좌표 -1~1값 : Normal값
//            |
//        1   |   2        
//    ----------------    --> 시계방향구조 
//        0   |   3 
//            |             0 : -0.5f, -0.5f , 0.0f
//                          1 : -0.5f, +0.5f , 0.0f
//                          2 : +0.5f, +0.5f , 0.0f
//                          3 : +0.5f, -0.5f , 0.0f
////////////////////////////////////////////////////////////
void Texture::CreateVertexBuffer(float startX, float startY, float endX, float endY)
{
	Vertex    vertices[6];

	// 1. 정점을 만든다  --> 시계방향
	vertices[0].Position = Vector3(-0.5f, -0.5f, 0.0f);  // 0
	vertices[1].Position = Vector3(-0.5f, +0.5f, 0.0f);  // 1
	vertices[2].Position = Vector3(+0.5f, -0.5f, 0.0f);  // 3
	vertices[3].Position = Vector3(+0.5f, -0.5f, 0.0f);  // 3
	vertices[4].Position = Vector3(-0.5f, +0.5f, 0.0f);  // 1
	vertices[5].Position = Vector3(+0.5f, +0.5f, 0.0f);  // 2

	// 2. UV 값

	D3DX11_IMAGE_INFO  ImageInfo;

	HRESULT hr = D3DX11GetImageInfoFromFile(m_strImageFile.c_str(), NULL, &ImageInfo, NULL);

	float TextureWidth  = (float)ImageInfo.Width;
	float TextureHeight = (float)ImageInfo.Height;

	m_ImageSize    = Vector2(TextureWidth, TextureHeight);
	m_TextureSize  = Vector2(endX - startX, endY - startY);
	m_SpriteOffset = Vector2(startX, startY);
	m_SpriteSize   = Vector2(endX - startX, endY - startY);

	// 비율계산
	startX = (startX > 0) ? startX  / TextureWidth  : 0.0f;
	startY = (startY > 0) ? startY  / TextureHeight : 0.0f;
	endX   = (endX > 0) ?   endX    / TextureWidth  : 1.0f;
	endY   = (endY > 0) ?   endY    / TextureHeight : 1.0f;

	if ((endX - startX) == 1.0) // full
	{
		m_TextureSize.x = TextureWidth;
		m_SpriteSize.x = TextureWidth;
	}
	if ((endY - startY) == 1.0) // full
	{
		m_TextureSize.y = TextureHeight;
		m_SpriteSize.y  = TextureHeight;
	}


	vertices[0].Uv = Vector2(startX, endY);
	vertices[1].Uv = Vector2(startX, startY);
	vertices[2].Uv = Vector2(endX,   endY);
	vertices[3].Uv = Vector2(endX,   endY);
	vertices[4].Uv = Vector2(startX, startY);
	vertices[5].Uv = Vector2(endX,   startY);


	D3D11_BUFFER_DESC       desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));   // typedef 및에 계속 typdef로 된경우가 많음
	desc.Usage = D3D11_USAGE_DYNAMIC;               // GPU로 데이터가 넘어간후 변경 할 수 있음
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.ByteWidth = sizeof(Vertex) * 6;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;    // 정점Buffer, IndexBuffer, ConstantBuffer

	D3D11_SUBRESOURCE_DATA  data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA)); // memset(&data,0x00,sixeof(...))

	data.pSysMem = vertices; // 정점에 대한 데이터의 주소값

	// 자원은 디바이스로 만든다,(Buffer,Texture,Constant)
	// com interface로 된것은 생성할때 DX11에서는 descrtion이 필요하다
	// com interface로 된것은 생성한것은 Release로 지워야한다
	hr = Device->CreateBuffer(&desc, &data, &this->m_pVertexBuffer);

	assert(SUCCEEDED(hr));
}

