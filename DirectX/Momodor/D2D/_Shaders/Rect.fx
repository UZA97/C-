#include "00_Global.fx"


float4 PS_Color(VertexOutput input) : SV_TARGET0
{
    return Color;
}


technique11 T0
{
    P_VP(P0, VS, PS_Color)
}