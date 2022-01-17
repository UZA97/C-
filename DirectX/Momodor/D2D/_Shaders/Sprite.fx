cbuffer CB_PerFrame
{
    matrix View;
    matrix Projection;
}

matrix World;

Texture2D DiffuseMap;

//Types
//-----------------------------------------------------------------------------
struct VertexInput
{
    float4 Position : POSITION0;
    float2 Uv : UV0;
};

struct VertexOutput
{
    float4 Position : SV_POSITION0;
    float2 Uv : UV0;
};


//VertexShader
//-----------------------------------------------------------------------------
VertexOutput VS(VertexInput input)
{
    VertexOutput output;

    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    output.Uv = input.Uv;

    return output;
}

//Pixelhader
//-----------------------------------------------------------------------------
SamplerState PointSampler
{
    Filter = MIN_MAG_MIP_POINT;

    AddressU = Mirror;
    AddressV = Mirror;
};

SamplerState LinearSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
};

RasterizerState CullMode_None
{
    CullMode = None;
    DepthClipEnable = false;
};

float4 PS(VertexOutput input) : SV_TARGET0
{
    return DiffuseMap.Sample(PointSampler, input.Uv);
}


//----------------
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



//Pipeline
//-----------------------------------------------------------------------------
technique11 T0
{
    pass P0
    {
        
        SetRasterizerState(CullMode_None);
        SetBlendState(AlphaBlend, float4(0, 0, 0, 0), 0xFF);
        
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}