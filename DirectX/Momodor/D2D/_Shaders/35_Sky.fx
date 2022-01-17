#include "00_Global.fx"

struct SkyDesc
{
    float4 Center;
    float4 Apex;
    float Height;
};

SkyDesc Sky;



float4 PS(VertexOutput input) : SV_TARGET0
{
    float y = saturate(input.oPosition.y) *2.0f;
    
    return lerp(Sky.Center, Sky.Apex, y * Sky.Height);
}

technique11 T0
{
    P_VP(P0, VS, PS)
}