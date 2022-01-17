#include "00_Global.fx"


SamplerState Address_Wrap
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = WRAP;
    AddressV = WRAP;
};

SamplerState Address_Mirror
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = MIRROR;
    AddressV = MIRROR;
};

SamplerState Address_CLMAP
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = CLAMP;
    AddressV = CLAMP;
};

SamplerState Address_BORDER
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = BORDER;
    AddressV = MIRROR;

    BorderColor = float4(0, 0, 1, 1);
};

uint Type;
float4 PS_Point(VertexOutput input) : SV_TARGET0
{

    if (Type == 0)
        return DiffuseMap.Sample(Address_Wrap, input.Uv);
    else if (Type == 1)
        return DiffuseMap.Sample(Address_Mirror, input.Uv);
    else if (Type == 2)
        return DiffuseMap.Sample(Address_CLMAP, input.Uv);
    else if (Type == 3)
        return DiffuseMap.Sample(Address_BORDER, input.Uv);

    return float4(1, 1, 1, 1);
}

technique11 T0
{
    P_VP(P0, VS, PS_Point)
}