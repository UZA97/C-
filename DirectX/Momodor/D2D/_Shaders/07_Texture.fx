////////////////////////////////////////////////////////////////////////////////////////
//   World, View, Projection Matrix
//   각 Object ( Player->Animation->AnimationClip->Texture
//                                                                     (Shader link)
//                  Update(V,P)
////////////////////////////////////////////////////////////////////////////////////////
matrix View;
matrix Projection;
matrix World;

Texture2D DiffuseMap;

//Types
//-----------------------------------------------------------------------------
struct VertexInput
{
	float4 Position : POSITION0;
	float2 Uv : UV0;
};

struct VertexOutput
{
	float4 Position : SV_POSITION0;
	float2 Uv : UV0;
};


//VertexShader
//-----------------------------------------------------------------------------
VertexOutput VS(VertexInput input)
{
	VertexOutput output;

	output.Position = mul(input.Position, World);
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);

	output.Uv = input.Uv;

	return output;
}

//Pixelhader
//-----------------------------------------------------------------------------
SamplerState PointSampler
{
	Filter = MIN_MAG_MIP_POINT;

    AddressU = Mirror;
	 AddressV = Mirror;
};

float4 PS_Point(VertexOutput input) : SV_TARGET0
{
	float4 color = DiffuseMap.Sample(PointSampler, input.Uv);
	//  clip(color.a - 0.9f);

	//color.a = 0.9;
	//	return color;

    if( color.r > 0.88f && color.b > 0.88f)   // bmp파일일 경우 
	     discard;
	//  return color;
	  return DiffuseMap.Sample(PointSampler, input.Uv);
}


//----------------
SamplerState LinearSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
};

float4 PS_Linear(VertexOutput input) : SV_TARGET0
{
	return DiffuseMap.Sample(LinearSampler, input.Uv);
}
//----------------

float Time = 0;
float4 PS_Lerp(VertexOutput input) : SV_Target
{
	float4 diffuse = DiffuseMap.Sample(PointSampler, input.Uv);

	float4 red = float4(0.5f, 0.5f, 0.0f, diffuse.a);
	float4 color = lerp(diffuse, red, Time);

	return color;
}


//----------------
BlendState AlphaBlend
{
	BlendEnable[0] = true;
	DestBlend[0] = INV_SRC_ALPHA;
	SrcBlend[0] = SRC_ALPHA;
	BlendOp[0] = Add;

	SrcBlendAlpha[0] = One;
	DestBlendAlpha[0] = One;
	RenderTargetWriteMask[0] = 0x0F;
};

BlendState AlphaBlend_AlphaToCoverage
{
	AlphaToCoverageEnable = true;

	BlendEnable[0] = true;
	DestBlend[0] = INV_SRC_ALPHA;
	SrcBlend[0] = SRC_ALPHA;
	BlendOp[0] = Add;

	SrcBlendAlpha[0] = One;
	DestBlendAlpha[0] = One;
	RenderTargetWriteMask[0] = 0x0F;
};



//Pipeline
//-----------------------------------------------------------------------------
technique11 T0
{
	pass P0
	{
		SetBlendState(AlphaBlend, float4(0, 0, 0, 0), 0xFF);
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS_Point()));
	}

	pass P1
	{
		SetBlendState(AlphaBlend, float4(0, 0, 0, 0), 0xFF);
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS_Linear()));
	}

	pass P2
	{
		SetBlendState(AlphaBlend_AlphaToCoverage, float4(0, 0, 0, 0), 0xFF);
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS_Linear()));
	}

	pass P3
	{
		SetBlendState(AlphaBlend_AlphaToCoverage, float4(1, 0, 0, 0), 0xFF);
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS_Lerp()));
	}
}