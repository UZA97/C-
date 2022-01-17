float4 Color;


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

VertexOutput VS(VertexInput input)
{
    VertexOutput output;

    output.Position = input.Position;
    output.Color = input.Color;

    return output;
}

float4 PS_VertexColor(VertexOutput input) : SV_TARGET0
{
    return input.Color;
}

float4 PS_Red(VertexOutput input) : SV_TARGET0
{
    return float4(1, 0, 0, 1);
}

float4 PS_CustomColor(VertexOutput input) : SV_TARGET
{
    return Color;
}

technique11 T0
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS_VertexColor()));
    }

    pass P1
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS_Red()));
    }

    pass P2
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS_CustomColor()));
    }
}