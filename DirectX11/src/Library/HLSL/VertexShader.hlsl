
struct VSInput
{
	float4 pos : POSITION0;
	float4 col : COLOR0;
};

struct VSOutput
{
	float4 pos : SV_POSITION;
	float4 col : COLOR0;
};


VSOutput main(VSInput input)
{
	VSOutput output = input;

	return output;
}
