#include "00_Global.fx"

float4 PS_Diffuse(VertexOutput input) : SV_TARGET0
{
    return DiffuseMap.Sample(PointSampler, input.Uv);
}

float2 PxSize;
float Intensity;
float4 PS_Edge(VertexOutput input) : SV_TARGET0
{
    float4 center = DiffuseMap.Sample(LinearSampler, input.Uv);
    float4 top = DiffuseMap.Sample(LinearSampler, input.Uv + float2(0, -PxSize.y));
    float4 bottom = DiffuseMap.Sample(LinearSampler, input.Uv + float2(0, +PxSize.y));
    float4 left = DiffuseMap.Sample(LinearSampler, input.Uv + float2(-PxSize.x, 0));
    float4 right = DiffuseMap.Sample(LinearSampler, input.Uv + float2(+PxSize.x, 0));

    float4 edge = center * 4 - top - bottom - left - right;
    //return float4(edge.rgb, 1) * Intensity;

    float3 gray = float3(0.2126f, 0.7152f, 0.0722f);
    float average = dot(edge.rgb, gray);

    //return float4(average, average, average, 1);

    return center + float4(average, average, average, 1) * Intensity;
}

uint BlurCount = 8;
float4 PS_Blur(VertexOutput input) : SV_TARGET0
{
    float2 arr[9] =
    {
        float2(-1, -1), float2(+0, -1), float2(+1, -1),
        float2(-1, +0), float2(+0, +0), float2(+1, +0),
        float2(-1, +1), float2(+0, +1), float2(+1, +1)
    };


    float3 color = 0;
    for (uint blur = 1; blur < BlurCount; blur++)
    {
        for (int i = 0; i < 9; i++)
        {
            float x = arr[i].x * PxSize.x * (float) blur;
            float y = arr[i].y * PxSize.y * (float) blur;

            float2 uv = input.Uv + float2(x, y);
            color += DiffuseMap.Sample(LinearSampler, uv).rgb;
        }
    }

    color /= (BlurCount - 1) * 9;

    return float4(color, 1.0f);
}


float3x3 ColorToHerculesiMatrix = float3x3
(
    0.393f, 0.769f, 0.189f, //R
    0.349f, 0.686f, 0.168f, //G
    0.272f, 0.534f, 0.131f //B
);
float4 PS_Sepia(VertexOutput input) : SV_TARGET0
{
    float4 diffuse = DiffuseMap.Sample(PointSampler, input.Uv);

    float3 result = mul(transpose(ColorToHerculesiMatrix), diffuse.rgb);

    return float4(result, 1.0f);
}

float Power = 2;
float2 Scale = float2(0.2, 0.2);
float4 PS_Vignette(VertexOutput input) : SV_TARGET0
{
    float4 diffuse = DiffuseMap.Sample(PointSampler, input.Uv);

    float radius = length((input.Uv - 0.5f) * 2 / Scale);
    float vignette = pow(abs(radius + 1e-6), Power);

    return saturate(1 - vignette) * diffuse;
}

uint RadialBlurCount = 8;
float RadialBlurRadius = 0.6f;
float RadialBlurAmount = 0.04f;
float2 RadialCenter = float2(0.5f, 0.5f);
float4 PS_RadialBlur(VertexOutput input) : SV_TARGET0
{
    float2 radius = input.Uv - RadialCenter;
    float r = max(length(radius), 1e-6f);
    radius /= r;

    r = 2 * r / RadialBlurRadius;
    r = saturate(r);

    float2 delta = radius * r * r * RadialBlurAmount / RadialBlurCount;
    //delta = -delta;

    float4 color = 0;
    for (uint i = 0; i < RadialBlurCount; i++)
    {
        color += DiffuseMap.Sample(LinearSampler, input.Uv);
        input.Uv -= delta;
    }
    color /= RadialBlurCount;

    return float4(color.rgb, 1);
}


technique11 T0
{
    P_VP(P0, VS, PS_Diffuse)
    P_VP(P1, VS, PS_Edge)
    P_VP(P2, VS, PS_Blur)
    P_VP(P3, VS, PS_Sepia)
    P_VP(P4, VS, PS_Vignette)
    P_VP(P5, VS, PS_RadialBlur)
}