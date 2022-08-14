
cbuffer ConstantBuffer
{
	float4x4 world;
	float4x4 view;
	float4x4 proj;
	float4	LightVector;
	float4	LightColor;
};

struct VSInput
{
	float4 pos : POSITION0;
	float4 tex : TEXTURE;
	float4 nor : NORMAL;
};

struct VSOutput
{
	float4 pos : SV_POSITION;
	float4 tex : TEXTURE;
	float4 col : COLOR0;
};

VSOutput main(VSInput input)
{
	VSOutput output = input;

	output.pos = mul(output.pos, world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, proj);

	float4 normal;
	input.nor.w = 0.0;
	// 頂点の法線にワールド行列を掛け合わせてワールド座標上での法線の向きに変換する
	normal = mul(input.nor, world).xyzw;
	normal = normalize(normal);
	// saturate:引数で指定した値を0～1間での範囲に収める
	float rad = saturate(dot(normal, LightVector));

	// ライトのカラー * 光のあたり加減
	output.col = LightColor * rad;

	return output;
}