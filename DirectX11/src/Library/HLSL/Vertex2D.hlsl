
cbuffer ConstantBuffer
{
	float4x4 WorldMatrix;
	float4 Viewport;
};

struct VSInput
{
	float4 pos : POSITION0;
	float4 col : COLOR0;
};

struct VSOutput
{
	float4 pos : SV_POSITION0;
	float4 col : COLOR0;
};

VSOutput main(VSInput input)
{
	VSOutput output = input;

	float4x4 world = WorldMatrix;

	world[3][0] *= 2.0f;
	world[3][1] *= -2.0f;

	float4 localPos = input.pos;
	localPos *= 2.0f;
	localPos.w = 1.0f;

	output.pos = mul(localPos, world);

	float projectionPosX = output.pos.x / Viewport.x;
	float projectionPosY = output.pos.y / Viewport.y;

	// Œ´“_‚ÌˆÚ“®
	float offsetX = -1.0f - (-abs(localPos.x) * world[0][0]) / Viewport.x;
	float offsetY = 1.0f - (abs(localPos.y) * world[1][1]) / Viewport.y;

	output.pos.x = projectionPosX + offsetX;
	output.pos.y = projectionPosY + offsetY;

	return output;
}
