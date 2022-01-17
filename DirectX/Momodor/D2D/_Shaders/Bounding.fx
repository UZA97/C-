cbuffer CB_PerFrame
{
    matrix View;
    matrix Projection;
}
matrix World;

//Types
//-----------------------------------------------------------------------------
struct VertexInput
{
    float4 Position : POSITION0;
};

struct VertexOutput
{
    float4 Position : SV_POSITION0;
};


//VertexShader
//-----------------------------------------------------------------------------
VertexOutput VS(VertexInput input)
{
    VertexOutput output;

    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);
    return output;
}
//Pixelhader
float3 Color;   // SetFloat..()
//-----------------------------------------------------------------------------
float4 PS(VertexOutput input) : SV_TARGET0
{
    return float4(0, 1, 0, 1);
  //  return float4(Color, 1);
}

//Pipeline
//-----------------------------------------------------------------------------
technique11 T0
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}