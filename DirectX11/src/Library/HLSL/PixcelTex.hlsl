
struct PSInput
{
	float4 pos	: SV_POSITION;
	float4 col	: COLOR0;
	float2 tex	: TEXCOORD0;
};

Texture2D myTexture :register(t0);
SamplerState mySampler :register(s0);

float4 main(PSInput input) : SV_TARGET
{
	float4 result = myTexture.Sample(mySampler, input.tex);
	result.a *= input.col[3];

	return result;
}