#include "00_Global.fx"

float4 LineColor = float4(0, 1, 0, 1);

struct VertexInput_Collider
{
    float4 Position : POSITION0;
};

struct VertexOutput_Collider
{
    float4 Position : SV_POSITION0;
};

VertexOutput_Collider VS_Collider(VertexInput_Collider input)
{
    VertexOutput_Collider output;

    output.Position = WorldPosition(input.Position);
    output.Position = ViewProjection(output.Position);

    return output;
}

float4 PS(VertexOutput_Collider input) : SV_TARGET0
{
    return LineColor;
}

technique11 T0
{
    P_VP(P0, VS_Collider, PS)
}