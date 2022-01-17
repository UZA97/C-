#include  "framework.h"
#include  "S08_DXTextureDemo.h"
#include  "Renders/Shader.h"
#include  "Base/Camera.h"

// 생성자
S08_DXTextureDemo::S08_DXTextureDemo()
{
	wstring strFileName = L"./_Shaders/07_Texture.fx";
	
	m_pShader = new Shader(strFileName);
	CreateVertexBuffer();
}

S08_DXTextureDemo::~S08_DXTextureDemo()
{
	SAFE_DELETE(m_pShader);
	SAFE_RELEASE(this->m_pVertexBuffer);
}


void S08_DXTextureDemo::Update()
{
	Matrix V, P;
	 
	CAMERA->Update();
	V = CAMERA->GetViewMatrix();
	P = CAMERA->GetProjectionMatrix();

	m_pShader->AsMatrix("View")->SetMatrix(V);
	m_pShader->AsMatrix("Projection")->SetMatrix(P);

	Matrix  W, S, T, R;

	D3DXMatrixScaling(&S, 640.0f, 640.0f, 0.0f);
	D3DXMatrixTranslation(&T, 100.0f, 100.0f, 0.0f);
	D3DXMatrixRotationZ(&R, 1.141592f / 180.0f * 45.0f);
	W = S * T;

	m_pShader->AsMatrix("World")->SetMatrix(W);

}

void S08_DXTextureDemo::Render()
{
	UINT   stride = sizeof(Vertex);
	UINT   offset = 0;

	// CreateVertextBuffer() --> GPU
	DeviceContext->IASetVertexBuffers(0, // Start slot
		1, // NumBuffers
		&m_pVertexBuffer,
		&stride,  // 보낼구조체의 size
		&offset
	);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//ImGui::ColorEdit3("Color", (float*)&color);
	//m_pShader->AsVector("Color")->SetFloatVector(color);

	static int mode = 0;
	static float time = 0.0f;

	time = time + TIMEMANAGER->Delta();
	if (time >= 1.0f)
		time = 0.0f;

	ImGui::InputInt("Mode", (int*)&mode);
	ImGui::SliderFloat("Time", &time, 0, 1);

	if (mode > 3 || mode < 0)
		mode = 0;

	m_pShader->AsScalar("Time")->SetFloat(time);
	m_pShader->Draw(0, mode, 6, 0);  // tech, pass
}

//////////////////////////////////////////////
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
///////////////////////////////////////////////
void S08_DXTextureDemo::CreateVertexBuffer()
{
	// 1. 정점을 만든다  --> 시계방향
	vertices[0].Position = Vector3(-0.5f, -0.5f, 0.0f);  // 0
	vertices[1].Position = Vector3(-0.5f, +0.5f, 0.0f);  // 1
	vertices[2].Position = Vector3(+0.5f, -0.5f, 0.0f);  // 3
	vertices[3].Position = Vector3(+0.5f, -0.5f, 0.0f);  // 3
	vertices[4].Position = Vector3(-0.5f, +0.5f, 0.0f);  // 1
	vertices[5].Position = Vector3(+0.5f, +0.5f, 0.0f);  // 2

	// 2. UV 값
	vertices[0].Uv = Vector2(0.0f, 1.0f);
	vertices[1].Uv = Vector2(0.0f, 0.0f);
	vertices[2].Uv = Vector2(1.0f, 1.0f);
	vertices[3].Uv = Vector2(1.0f, 1.0f);
	vertices[4].Uv = Vector2(0.0f, 0.0f);
	vertices[5].Uv = Vector2(1.0f, 0.0f);


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
	HRESULT hr = Device->CreateBuffer(&desc, &data, &this->m_pVertexBuffer);

	assert(SUCCEEDED(hr));

	// Shader Resource View  --> *Image File, Memory.....
	{
		
		HRESULT hr = D3DX11CreateShaderResourceViewFromFile(
			Device, m_ImageFile.c_str(), NULL, NULL, &m_pSRV, NULL);
		assert(SUCCEEDED(hr));

		m_pShader->AsShaderResource("DiffuseMap")->SetResource(m_pSRV);


	}

}
