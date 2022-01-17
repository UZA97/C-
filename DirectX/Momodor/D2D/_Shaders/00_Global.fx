//-----------------------------------------------------------------------------
// Common
//-----------------------------------------------------------------------------
cbuffer CB_PerFrame
{
    matrix View;
    matrix Projection;
}

matrix World;

Texture2D DiffuseMap;
float4 Color;

//VertexInput
struct VertexInput
{
    float4 Position : POSITION0;
    float2 Uv : UV0;
};

//VertexOutput
struct VertexOutput
{
    float4 Position : SV_POSITION0;
    float4 oPosition : POSITION1;
    float3 wPosition : POSITION2;
    float2 Uv : UV0;
};

//Functions
float4 WorldPosition(float4 position)
{
    return mul(position, World);
}

float4 ViewProjection(float4 position)
{
    position = mul(position, View);
    return mul(position, Projection);
}

//VertexShader
VertexOutput VS(VertexInput input)
{
    VertexOutput output;

    output.oPosition = input.Position;

    output.Position = WorldPosition(input.Position);
    output.wPosition = output.Position;

    output.Position = ViewProjection(output.Position);

    output.Uv = input.Uv;

    return output;
}

//-----------------------------------------------------------------------------
// States
//-----------------------------------------------------------------------------
SamplerState PointSampler
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Wrap;
    AddressV = Wrap;
};

SamplerState LinearSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

RasterizerState CullMode_None
{
    CullMode = None;
    DepthClipEnable = false;
};

BlendState AlphaBlend
{
    BlendEnable[0] = true;
    DestBlend[0] = INV_SRC_ALPHA;
    SrcBlend[0] = SRC_ALPHA;
    BlendOp[0] = Add;
    
    SrcBlendAlpha[0] = One;
    DestBlendAlpha[0] = One;
    RenderTargetWriteMask[0] = 0x0F;
};

//Macro Pass
#define P_VP(number, vs, ps) \
pass number \
{ \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
\
    SetRasterizerState(CullMode_None); \
    SetBlendState(AlphaBlend, float4(0, 0, 0, 0), 0xFF); \
}