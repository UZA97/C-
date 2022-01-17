///////////////////////////////////////////////////////
//    Vertext Type Define
//    * High Level Shader Langauge 에서 사용
//    HLSL 프로그램 파일은 전역 변수, 타입 정의, 정점 , 픽셀,
//    geometry shader로 구성할 수 있음 
//
//    IA->VS->RS->PS->OM
///////////////////////////////////////////////////////


///////////// // GLOBALS // ///////////// 
// cbuffer MatrixBuffer
// { 
// 	matrix worldMatrix; 
// 	matrix viewMatrix; 
// 	matrix projectionMatrix;
// };

////////////////////////////////////////////////////////
// Vertex input typedefine
// float4         : x,y,z,w를 가지는 shader사용되는 변수
// POSITION,COLOR : GPU에 이 변수를 사용할 것을 정의 (SemanticName)
// 같은 Type을 더 사용하려면 0,1,2...를 사용한다 
////////////////////////////////////////////////////////
struct VertexInput
{
    float4 Position : POSITION0;     
    float4 Color :    COLOR0;
};
struct PixelInput
{
    float4 Position : SV_POSITION0;
    float4 Color :    COLOR0;
};
////////////////////////////////////////////////////////
//  Vertex Shader 출력 프로그램
////////////////////////////////////////////////////////
PixelInput VS(VertexInput input)
{
    PixelInput output;

	// 현재는 정규화된 좌표만을 사용하기 때문에 아래 function은 (예) 만 보여줍니다.
	{
		// 올바르게 행렬 연산을 하기 위하여 position 벡터를 w까지 있는 4성분이 있는 것으로 사용합니다. 
	//	input.position.w = 1.0f;
		// 정점의 위치를 월드, 뷰, 투영의 순으로 계산합니다. 
	//	output.position = mul(input.position, worldMatrix);
	//	output.position = mul(output.position, viewMatrix);
	//	output.position = mul(output.position, projectionMatrix);
	}

	output.Position = input.Position;
	output.Color = input.Color;
    return output;
}
////////////////////////////////////////////////////////
// Pixel Shader 출력 프로그램
////////////////////////////////////////////////////////
float4 PS(PixelInput input) : SV_TARGET0
{
	//return float4(1, 1, 0, 1);
    return input.Color;
}
