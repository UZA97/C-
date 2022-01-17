////////////////////////////////////////////////////////////////////////
//   Shader :  물체의  색상과 표면 상태를 결정짓는 속성
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
//    텍스쳐가 아닌 일반 정점에 대한 속성은  x,y,z,w(동차)과 색상으로
//    생각 할수 있다.  
//    CPU->GPU로 보내는 데이터의 구조체를 선언 
//
//    Vertext Type Define
//       * High Level Shader Langauge 에서 사용
//         HLSL 프로그램 파일은 전역 변수, 타입 정의, 정점 , 픽셀,
//         geometry shader로 구성할 수 있음 
//
//        IA->VS->RS->PS->OM
////////////////////////////////////////////////////////////////////////
cbuffer CB_PerFrame            // Constant Bufffer를 사용함으로 가져올수 있다  
{
    matrix View;
    matrix Projection;
}
matrix World;
//=======================================================================
//   Texture Global 
//=======================================================================
Texture2D    DiffuseMap;                  //  텍스쳐 자원으로서 모델에 텍스쳐를 그리때 사용, Shader Class에서 선언
SamplerState PointSampler                 //  셰이딩이 이루어질 때 어떻게 텍스쳐의 픽셀이 씌여지는 지를 수정할 수있게 함.
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Mirror;
    AddressV = Mirror;
};
SamplerState LinearSampler             //  셰이딩이 이루어질 때 어떻게 텍스쳐의 픽셀이 씌여지는 지를 수정할 수있게 함.
{
    Filter = MIN_MAG_MIP_LINEAR;
};

//=======================================================================
//   입출력 데이터 Type Define
//=======================================================================
struct VertexInput                     // DeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
{
    float4 Position : POSITION0;       // C에서 처럼 Type을 디파인하여 쓰지만 변수뒤에 GPU가 알아 볼수 있도록 POSITION(씨멘틱)을 부여한다
    float2 Uv : UV0;                   // Semantic(의미) : 
};

struct VertexOutput
{
    float4 Position : SV_POSITION0;
    float2 Uv : UV0;
};

//=======================================================================
//   VS Stage
//=======================================================================
VertexOutput VS(VertexInput input)
{
    VertexOutput output;

    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    output.Uv = input.Uv;

    return output;
}

//=======================================================================
//   PS Stage
//=======================================================================
float4 PS_Point(VertexOutput input) : SV_TARGET0
{
  //  float4 color = DiffuseMap.Sample(PointSampler, input.Uv);
    
    
 //   return color;
    
    return DiffuseMap.Sample(PointSampler, input.Uv);
}
float4 PS_Linear(VertexOutput input) : SV_TARGET0
{
    return DiffuseMap.Sample(LinearSampler, input.Uv);
}
float Time = 0;
float4 PS_Lerp(VertexOutput input) : SV_Target
{
    float4 diffuse = DiffuseMap.Sample(PointSampler, input.Uv);

    float4 red = float4(1, 0, 0, diffuse.a);
    float4 color = lerp(diffuse, red, Time);

    return color;
}

//=======================================================================
//   OM Stage
//   DestBlend  :  이미 그려져 있는 픽셀의 색상
//   SrcBlend   :  현재그려야하는 픽셀
//    blendStateDescription.RenderTarget[0].BlendEnable = TRUE; 
//    blendStateDescription.RenderTarget[0].SrcBlend =D3D11_BLEND_SRC_ALPHA; 
//    blendStateDescription.RenderTarget[0].DestBlend =D3D11_BLEND_INV_SRC_ALPHA; 
//    blendStateDescription.RenderTarget[0].BlendOp =D3D11_BLEND_OP_ADD; 
//    blendStateDescription.RenderTarget[0].SrcBlendAlpha =D3D11_BLEND_ONE; 
//    blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO; 
//    blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD; 
//    blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;
//=======================================================================
BlendState AlphaBlend     // https://docs.microsoft.com/en-us/windows/win32/api/d3d11/ne-d3d11-d3d11_blend
{
    BlendEnable[0] = true;
    DestBlend[0] = INV_SRC_ALPHA;      // 소스의 a 값을 역전 시킨것 ( 1-a )  투명하지 않은 Pixel은 a 값이 작겠죠
    SrcBlend[0] = SRC_ALPHA;           // 현 Image의 a 값 사용
    BlendOp[0] = ADD;                  

    SrcBlendAlpha[0] = ONE;
    DestBlendAlpha[0] = ONE;
    RenderTargetWriteMask[0] = 0x0F;
};

BlendState AlphaBlend_AlphaToCoverage
{
    AlphaToCoverageEnable = true;

    BlendEnable[0] = true;
    DestBlend[0] = INV_SRC_ALPHA;
    SrcBlend[0] = SRC_ALPHA;
    BlendOp[0] = Add;

    SrcBlendAlpha[0] = One;
    DestBlendAlpha[0] = One;
    RenderTargetWriteMask[0] = 0x0F;
};
RasterizerState CullMode_None
{
    CullMode = None;
    DepthClipEnable = false;
};
//=======================================================================
//   Pipe Line
//=======================================================================
technique11 T0
{
    pass P0
    {
        SetRasterizerState(CullMode_None);
        SetBlendState(AlphaBlend, float4(0, 0, 0, 0), 0xFF);
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS_Point()));
    }

    pass P1
    {
        SetBlendState(AlphaBlend, float4(0, 0, 0, 0), 0xFF);

        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS_Linear()));
    }

    pass P2
    {
        SetBlendState(AlphaBlend_AlphaToCoverage, float4(0, 0, 0, 0), 0xFF);

        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS_Linear()));
    }

    pass P3
    {
        SetBlendState(AlphaBlend_AlphaToCoverage, float4(0, 0, 0, 0), 0xFF);

        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS_Lerp()));
    }
}