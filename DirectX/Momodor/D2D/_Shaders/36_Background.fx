#include "00_Global.fx"

float4 PS_Diffuse(VertexOutput input) : SV_TARGET0
{
    return DiffuseMap.Sample(PointSampler, input.Uv);
}

float4 PS_GrayScale(VertexOutput input) : SV_TARGET0
{
    float4 diffuse = DiffuseMap.Sample(PointSampler, input.Uv);
    float gray = (diffuse.r + diffuse.g + diffuse.b) * 0.333333f;

    return float4(gray, gray, gray, diffuse.a);
}


float4 PS_GrayScale2(VertexOutput input) : SV_TARGET0
{
    float4 diffuse = DiffuseMap.Sample(PointSampler, input.Uv);
    float3 gray = float3(0.2126f, 0.7152f, 0.0722f);
    float average = dot(diffuse.rgb, gray);

    return float4(average, average, average, diffuse.a);
}

float4 PS_Tone(VertexOutput input) : SV_TARGET0
{
    float4 diffuse = DiffuseMap.Sample(PointSampler, input.Uv);
    float3 tone = diffuse.rgb * (1 - input.Uv.x);

    return float4(tone, diffuse.a) * 2.0f;
}

float RunningTime;
float2 WiggleOffset;
float2 WiggleAmout;
float4 PS_Wiggle(VertexOutput input) : SV_TARGET0
{
    float2 uv = input.Uv;
    uv.x += sin(RunningTime + uv.x * WiggleOffset.x) * WiggleAmout.x;
    uv.y += cos(RunningTime + uv.y * WiggleOffset.y) * WiggleAmout.y;

    return DiffuseMap.Sample(PointSampler, uv);
}

float4 PS_Distortion(VertexOutput input) : SV_TARGET0
{
    float4 diffuse = DiffuseMap.Sample(PointSampler, input.Uv);
    float4 distortion = diffuse;

    distortion -= DiffuseMap.Sample(PointSampler, input.Uv + sin(RunningTime) * 0.01f).r;
    distortion += DiffuseMap.Sample(PointSampler, input.Uv + sin(RunningTime) * 0.02f).g;
    distortion += DiffuseMap.Sample(PointSampler, input.Uv + sin(RunningTime) * 0.03f).b;

    distortion *= 0.1f;

    return diffuse + distortion;
}

float4 PS_Print(VertexOutput input) : SV_TARGET0
{
    float4 diffuse = DiffuseMap.Sample(PointSampler, input.Uv);
    diffuse -= DiffuseMap.Sample(PointSampler, input.Uv - 0.001f);
    diffuse += DiffuseMap.Sample(PointSampler, input.Uv + 0.001f);
    diffuse.rgb = (diffuse.r + diffuse.g + diffuse.b) * 0.333333f;

    return diffuse;

}

technique11 T0
{
    P_VP(P0, VS, PS_Diffuse)
    P_VP(P1, VS, PS_GrayScale)
    P_VP(P2, VS, PS_GrayScale2)
    P_VP(P3, VS, PS_Tone)
    P_VP(P4, VS, PS_Wiggle)
    P_VP(P5, VS, PS_Distortion)
    P_VP(P6, VS, PS_Print)
}