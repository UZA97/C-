#include "00_Global.fx"

float Time;
VertexOutput VS_Rotate(VertexInput input)
{
    VertexOutput output;

    input.Position.x += cos(Time * 2.0f);
    input.Position.y += sin(Time * 2.0f);

    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    output.Uv = input.Uv;

    return output;
}

float4 PS(VertexOutput input) : SV_TARGET0
{
    return DiffuseMap.Sample(PointSampler, input.Uv);
}


technique11 T0
{
    P_VP(P0, VS, PS)
    P_VP(P1, VS_Rotate, PS)
}