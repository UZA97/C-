#include "00_Global.fx"

//-----------------------------------------------------------------------------
//PreRender
//-----------------------------------------------------------------------------
struct VertexOutput_Mrt
{
    float4 Position : SV_POSITION0;
    float2 Uv : UV0;
};

VertexOutput_Mrt VS_Mrt(float4 position : POSITION0)
{
    VertexOutput_Mrt output;

    output.Position = position;
    output.Position =  WorldPosition(position);
    output.Position = ViewProjection(output.Position);

    output.Uv.x = position.x + 0.5f;
    output.Uv.y = -position.y + 0.5f;

    return output;
}

float2 PxSize;
float4 PS_PreRender(VertexOutput_Mrt input) : SV_TARGET0
{
    float Strenth = 0.5f;
    int IntraceValue = 5;

    float4 diffuse = DiffuseMap.Sample(LinearSampler, input.Uv);

    float height = 1.0f / PxSize.y;
    int value = (int) ((input.Uv.y * height) % IntraceValue);

    [flatten]
    if (value)
    {
        float3 grayScale = float3(0.2126f, 0.7152f, 0.0722f);
        float3 average = dot(diffuse.rgb, grayScale);


        diffuse.rgb = lerp(diffuse.rgb, diffuse.rgb * average, Strenth);
    }

    return diffuse;
    
}

//-----------------------------------------------------------------------------
//Mrt
//-----------------------------------------------------------------------------
struct PixelOutput
{
    float4 Color0 : SV_TARGET0;
    float4 Color1 : SV_TARGET1;
    float4 Color2 : SV_TARGET2;
};

float4 ColorToVignette(float4 color, float2 uv)
{
    float Power = 12.0f;
    float2 Scale = float2(0.17f, 0.25f);
  
    float radius = length((uv - 0.5f) * 2 / Scale);
    float vignette = pow(abs(radius + 1e-6), Power);

    return saturate(1 - vignette) * color;
}

float4 ColorToMonoTone(float4 color, float2 uv)
{
    color.rgb = (color.r + color.g + color.b) * 0.3333f;

    if (color.r < 0.3f || color.r > 0.9f)
        color.r = 0;
    else
        color.r = 1;

    if (color.g < 0.3f || color.g > 0.9f)
        color.g = 0;
    else
        color.g = 1;

    if (color.b < 0.3f || color.b > 0.9f)
        color.b = 0;
    else
        color.b = 1;

    return color;

}

PixelOutput PS_Mrt(VertexOutput_Mrt input)
{
    PixelOutput output;

    float4 color = DiffuseMap.Sample(LinearSampler, input.Uv);

    output.Color0 = color;
    output.Color1 = ColorToVignette(color, input.Uv) * float4(0.1f, 0.75f, 0.2f, 1.0f);
    output.Color2 = ColorToMonoTone(color, input.Uv) + float4(1, 0, 0, 1);

    return output;
}

technique11 T0
{
    P_VP(P0, VS_Mrt, PS_PreRender)
    P_VP(P1, VS_Mrt, PS_Mrt)
}