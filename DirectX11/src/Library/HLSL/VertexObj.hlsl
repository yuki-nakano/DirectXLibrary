
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
	float4 nor : NORMAL;
};

struct VSOutput
{
	float4 pos : SV_POSITION;
	float4 col : COLOR0;
};

VSOutput main(VSInput input)
{
	VSOutput output = input;

	output.pos = mul(output.pos, world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, proj);

	float4 normal;
	// �ړ����v�Z�ɔ��f�����Ȃ�
	input.nor.w = 0.0;
	// ���_�̖@���Ƀ��[���h�s����|�����킹��
	// ���[���h���W��ł̖@���̌����ɕϊ�����
	normal = mul(input.nor, world).xyzw;
	normal = normalize(normal);
	// saturate => �����Ŏw�肵���l��0�`1�Ԃł͈̔͂Ɏ��߂�
	// dot => ���όv�Z
	float rad = saturate(dot(normal, LightVector));

	// ���C�g�̃J���[ * ���̂��������
	output.col = LightColor * rad;

	return output;
}