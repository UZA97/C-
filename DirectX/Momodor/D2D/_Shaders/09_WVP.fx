///////////////////////////////////////////////////////////////
//  CB : World, View, Projection
//////////////////////////////////////////////////////////////
matrix World;
matrix View;
matrix Projection;



//////////////////////////////////////////////////////////////////
// 정점에 대한 Structure
/////////////////////////////////////////////////////////////////
struct VertexInput
{
    float4 Position : POSITION0;
    float4 Color : COLOR0;
};
struct VertexOutput
{
    float4 Position : SV_POSITION0;
    float4 Color : COLOR0;
};
////////////////////////////////////////////////////////////////
//  Vertex Shader
//  mul() 곱하기
//  정점생성(IA) -> VS input -> 계산 -> VS output -> PS
//                   ------------------------  (-1~1)
//////////////////////////////////////////////////////////////
VertexOutput VS(VertexInput input)
{
    VertexOutput output;

    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    output.Color = input.Color;

    return output;
}

//Pixelhader
float3 Color;   // SetFloat..()
//-----------------------------------------------------------------------------
float4 PS(VertexOutput input) : SV_TARGET0
{
	return float4(Color, 1);
}

technique11 T0
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}