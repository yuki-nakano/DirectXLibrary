
cbuffer ConstantBuffer
{
	float4x4 world;
	float4x4 view;
	float4x4 proj;
	float4 LightVector;
	float4 LightColor;
	float4 ambient;
	float4 diffuse;
};

struct PSInput
{
	float4 pos : SV_POSITION;
	float4 nor : NORMAL;
	float4 col : COLOR0;
};

float4 main(PSInput input) : SV_TARGET
{
	float material_ambient_power = 1.0;
	float material_diffuse_power = 1.0;
	float total_power = material_ambient_power + material_diffuse_power;

	if (total_power == 0)
	{
		total_power = 1.0;
	}

	float ambient_power = material_ambient_power / total_power;
	float diffuse_power = material_diffuse_power / total_power;

	float4 ambient_color = ambient * ambient_power;
	float4 diffuse_color = (LightColor * diffuse * input.nor) * diffuse_power;

	// アンビエントカラー + ディフューズカラー
	float4 out_color = ambient_color + diffuse_color;

	return out_color;
}