#include "00_Global.fx"

float4 PS_Diffuse(VertexOutput input) : SV_TARGET0
{
    return DiffuseMap.Sample(PointSampler, input.Uv);
}

float4 AdjustSaturation(float4 color, float saturation)
{
    float4 gray = float4(0.2126f, 0.7152f, 0.0722f, 1.0f);
    float average = dot(color, gray);


    return lerp(average, color, saturation);
}

float BaseSaturation = 2.0f;
float BaseIntesity = 2.0f;
float BloomSaturation = 1.0f;
float BloomIntesity = 2.0f;
float4 PS_Bloom(VertexOutput input) : SV_TARGET0
{
    float4 diffuse = DiffuseMap.Sample(LinearSampler, input.Uv);
    float4 bloom = diffuse;
    
    diffuse = AdjustSaturation(diffuse, BaseSaturation) * BaseIntesity;
    bloom = AdjustSaturation(bloom, BloomSaturation) * BloomIntesity;

    diffuse *= (1 - saturate(bloom));

    return diffuse + bloom;

}

float3 Mix(float3 color)
{
    float4 gold = float4(1.0f, 0.66f, 0.33f, 1.0f);
    float3 mix = abs(frac(color.rrr + gold.rgb) * 6.0f - gold.rrr);
    return color.b * lerp(gold.rrr, saturate(mix - gold.rrr), gold.g);
}


float4 PS_ColorOverlay(VertexOutput input) : SV_TARGET0
{
    float3 diffuse = DiffuseMap.Sample(LinearSampler, input.Uv).rgb;
    float alpha = DiffuseMap.Sample(LinearSampler, input.Uv).a;

    float3 uvColor = float3(input.Uv.x, 1.0f, 1.0f);

    return float4(Mix(uvColor) * 0.5f + diffuse.rgb, alpha);
}

technique11 T0
{
    P_VP(P0, VS, PS_Diffuse)
    P_VP(P1, VS, PS_Bloom)
    P_VP(P2, VS, PS_ColorOverlay)
}