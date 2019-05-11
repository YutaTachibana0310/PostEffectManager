//=====================================
//
//�u���[�V�F�[�_[BlufFilter.fx]
//Author:GP12B332 21 ���ԗY��
//
//=====================================

/***************************************
�O���[�o���ϐ�
***************************************/
float texelU[5];	//X�����ׂ̗̃e�N�Z���ʒu
float texelV[5];	//Y�����ׂ̗̃e�N�Z���ʒu
int blurPower;		//�u���[�̋���

sampler s0 : register(s);	//�e�N�X�`���T���v���[

/*********************************
���_�V�F�[�_�o�͍\����
*********************************/
struct VS_OUTPUT
{
	float4 pos	: POSITION;
	float2 tex	: TEXCOORD0;
};

/*********************************
���_�V�F�[�_
*********************************/
VS_OUTPUT VS(
	float4 pos	: POSITION,
	float2 tex : TEXCOORD0
) {
	VS_OUTPUT Out;

	Out.pos = pos;
	Out.tex = tex;

	return Out;
}

/*********************************
//�s�N�Z���J���[�擾����
*********************************/
float4 GetPixelColor(float2 texel[11])
{
	float4 p0 = tex2D(s0, texel[0]) * 0.20f;
	
	float4 p1 = tex2D(s0, texel[1]) * 0.12f;
	float4 p2 = tex2D(s0, texel[2]) * 0.10f;
	float4 p3 = tex2D(s0, texel[3]) * 0.08f;
	float4 p4 = tex2D(s0, texel[4]) * 0.06f;
	float4 p5 = tex2D(s0, texel[5]) * 0.04f;
	
	float4 p6 = tex2D(s0, texel[6]) * 0.12f;
	float4 p7 = tex2D(s0, texel[7]) * 0.10f;
	float4 p8 = tex2D(s0, texel[8]) * 0.08f;
	float4 p9 = tex2D(s0, texel[9]) * 0.06f;
	float4 p10 = tex2D(s0, texel[10]) * 0.04f;

	return p0 + p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;
}

/*********************************
//X�����ɂڂ����s�N�Z���V�F�[�_(��)
*********************************/
float4 PS1(VS_OUTPUT In) : COLOR0
{
	float2 texel[11];

	//�e�N�Z�����擾
	texel[0] = In.tex;

	texel[1] = In.tex + float2(-texelU[0], 0.0f);
	texel[2] = In.tex + float2(-texelU[1], 0.0f);
	texel[3] = In.tex + float2(-texelU[2], 0.0f);
	texel[4] = In.tex + float2(-texelU[3], 0.0f);
	texel[5] = In.tex + float2(-texelU[4], 0.0f);

	texel[6] = In.tex + float2(texelU[0], 0.0f);
	texel[7] = In.tex + float2(texelU[1], 0.0f);
	texel[8] = In.tex + float2(texelU[2], 0.0f);
	texel[9] = In.tex + float2(texelU[3], 0.0f);
	texel[10] = In.tex + float2(texelU[4], 0.0f);

	//���ꂼ��̃e�N�Z���̃J���[�����擾������
	return GetPixelColor(texel);
}

/*********************************
//Y�����ɂڂ����s�N�Z���V�F�[�_(��)
*********************************/
float4 PS2(VS_OUTPUT In) : COLOR0
{
	float2 texel[11];

	//�e�N�Z�����擾
	texel[0] = In.tex;

	texel[1] = In.tex + float2(0.0f, -texelV[0]);
	texel[2] = In.tex + float2(0.0f, -texelV[1]);
	texel[3] = In.tex + float2(0.0f, -texelV[2]);
	texel[4] = In.tex + float2(0.0f, -texelV[3]);
	texel[5] = In.tex + float2(0.0f, -texelV[4]);

	texel[6] = In.tex + float2(0.0f, texelV[0]);
	texel[7] = In.tex + float2(0.0f, texelV[1]);
	texel[8] = In.tex + float2(0.0f, texelV[2]);
	texel[9] = In.tex + float2(0.0f, texelV[3]);
	texel[10] = In.tex + float2(0.0f, texelV[4]);

	//���ꂼ��̃e�N�Z���̃J���[�����擾������
	return GetPixelColor(texel);
}

/*********************************
//�΂ߕ����ɂڂ����s�N�Z���V�F�[�_(�E��A����)
*********************************/
float4 PS3(VS_OUTPUT In) : COLOR0
{
	float2 texel[11];

	//�e�N�Z�����擾
	texel[0] = In.tex;

	texel[1] = In.tex + float2(-texelU[0], -texelV[0]);
	texel[2] = In.tex + float2(-texelU[1], -texelV[1]);
	texel[3] = In.tex + float2(-texelU[2], -texelV[2]);
	texel[4] = In.tex + float2(-texelU[3], -texelV[3]);
	texel[5] = In.tex + float2(-texelU[4], -texelV[4]);

	texel[6] = In.tex + float2(texelU[0], texelV[0]);
	texel[7] = In.tex + float2(texelU[1], texelV[1]);
	texel[8] = In.tex + float2(texelU[2], texelV[2]);
	texel[9] = In.tex + float2(texelU[3], texelV[3]);
	texel[10] = In.tex + float2(texelU[4], texelV[4]);

	return GetPixelColor(texel);

}

/*********************************
//�΂ߕ����ɂڂ����s�N�Z���V�F�[�_(�E���A����)
*********************************/
float4 PS4(VS_OUTPUT In) : COLOR0
{
	float2 texel[11];

	texel[0] = In.tex;

	texel[1] = In.tex + float2(-texelU[0], texelV[0]);
	texel[2] = In.tex + float2(-texelU[1], texelV[1]);
	texel[3] = In.tex + float2(-texelU[2], texelV[2]);
	texel[4] = In.tex + float2(-texelU[3], texelV[3]);
	texel[5] = In.tex + float2(-texelU[4], texelV[4]);

	texel[6] = In.tex + float2(texelU[0], -texelV[0]);
	texel[7] = In.tex + float2(texelU[1], -texelV[1]);
	texel[8] = In.tex + float2(texelU[2], -texelV[2]);
	texel[9] = In.tex + float2(texelU[3], -texelV[3]);
	texel[10] = In.tex + float2(texelU[4], -texelV[4]);

	return GetPixelColor(texel);
}

/*********************************
�e�N�j�b�N
*********************************/
technique tech
{
	pass P0
	{
		VertexShader = compile vs_1_1 VS();
		PixelShader = compile ps_2_0 PS1();
	}

	pass P1
	{
		VertexShader = compile vs_1_1 VS();
		PixelShader = compile ps_2_0 PS2();
	}

	pass P2
	{
		VertexShader = compile vs_1_1 VS();
		PixelShader = compile ps_2_0 PS3();
	}

	pass P3
	{
		VertexShader = compile vs_1_1 VS();
		PixelShader = compile ps_2_0 PS4();
	}
}