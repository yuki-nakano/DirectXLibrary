
struct PSInput
{
	float4 pos	: SV_POSITION;
	float2 tex	: TEXCOORD0;
};

Texture2D myTexture :register(t0);
SamplerState mySampler :register(s0);

float4 main(PSInput input) : SV_TARGET
{
	return myTexture.Sample(mySampler, input.tex);
}