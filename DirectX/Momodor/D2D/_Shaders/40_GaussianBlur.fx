#include "00_Global.fx"

//-----------------------------------------------------------------------------
//PreRender
//-----------------------------------------------------------------------------
struct VertexOutput_GassianBlur
{
    float4 Position : SV_POSITION0;
    float2 Uv : UV0;
};

VertexOutput_GassianBlur VS_GaussianBlur(float4 position : POSITION0)
{
    VertexOutput_GassianBlur output;

    output.Position = position;
    output.Position = WorldPosition(position);
    output.Position = ViewProjection(output.Position);

    output.Uv.x = position.x + 0.5f;
    output.Uv.y = -position.y + 0.5f;

    return output;
}

float4 PS_Diffuse(VertexOutput_GassianBlur input) : SV_TARGET0
{
    return DiffuseMap.Sample(PointSampler, input.Uv);
}

//Mrt - Luminosity
float Threshold = 0.0f;
float4 PS_Luminosity(VertexOutput_GassianBlur input) : SV_TARGET0
{
    float4 diffuse = DiffuseMap.Sample(PointSampler, input.Uv);

    float3 threshold = saturate((diffuse.rgb - Threshold) / (1 - Threshold));

    return float4(threshold, diffuse.a);
}

//Mrt - GaussianBlurX
float2 PxSize;
static const float Weights[13] =
{
    0.0561f, 0.1353f, 0.278f, 0.4868f, 0.7261f, 0.9231f,
    1.0f,
    0.9231f, 0.7261f, 0.4868f, 0.278f, 0.1353f, 0.0561f
};

float4 PS_GaussianBlurX(VertexOutput_GassianBlur input) : SV_TARGET0
{
    float2 uv = input.Uv;
    float u = PxSize.x;

    float sum = 0;
    float4 color = 0;

    for (int i = -6; i <= 6; i++)
    {
        float2 temp = uv + float2(u * (float) i, 0.0f);
        color += DiffuseMap.Sample(LinearSampler, temp) * Weights[6 + i];

        sum += Weights[6 + i];
    }

    color /= sum;

    return float4(color.rgb, 1.0f);
}

//Mrt - GaussianBlurY
float4 PS_GaussianBlurY(VertexOutput_GassianBlur input) : SV_TARGET0
{
    float2 uv = input.Uv;
    float v = PxSize.y;

    float sum = 0;
    float4 color = 0;

    for (int i = -6; i <= 6; i++)
    {
        float2 temp = uv + float2(0.0f, v * (float) i);
        color += DiffuseMap.Sample(LinearSampler, temp) * Weights[6 + i];

        sum += Weights[6 + i];
    }

    color /= sum;

    return float4(color.rgb, 1.0f);
}

//Mrt - Composite
float BlurRatio;
Texture2D LuminosityMap;
Texture2D BlurMap;
float4 PS_Composite(VertexOutput_GassianBlur input) : SV_TARGET0
{
    float4 luminosity = LuminosityMap.Sample(LinearSampler, input.Uv);
    float4 blur = BlurMap.Sample(LinearSampler, input.Uv);
    float4 diffuse = DiffuseMap.Sample(LinearSampler, input.Uv);

    luminosity *= blur;
    
    float4 blurColor = lerp(blur, diffuse, BlurRatio);

    return float4((luminosity + blurColor).rgb, 1.0f);

}

technique11 T0
{
    P_VP(P0, VS_GaussianBlur, PS_Diffuse)
    P_VP(P1, VS_GaussianBlur, PS_Luminosity)
    P_VP(P2, VS_GaussianBlur, PS_GaussianBlurX)
    P_VP(P3, VS_GaussianBlur, PS_GaussianBlurY)
    P_VP(P4, VS_GaussianBlur, PS_Composite)
}