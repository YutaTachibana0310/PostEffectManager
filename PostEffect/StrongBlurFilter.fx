//=====================================
//
//�X�g�����O�u���[�V�F�[�_[StrongBlur.fx]
//Author:GP12B332 21 ���ԗY��
//
//=====================================

/***************************************
�O���[�o���ϐ�
***************************************/
float texelU[9];			//X�����ׂ̗̃e�N�Z���ʒu
float texelV[9];			//Y�����ׂ̗̃e�N�Z���ʒu

sampler s0 : register(s0);	//�e�N�X�`���T���v���[

/******************************************
���_�V�F�[�_�o�͍\����
*******************************************/
struct VS_OUTPUT
{
	float4 pos : POSITION;
	float2 tex : TEXCOORD0;
};

/******************************************
���_�V�F�[�_
*******************************************/
VS_OUTPUT VS(
	float4 pos : POSITION,
	float2 tex : TEXCOORD0
){
	VS_OUTPUT Out;
	Out.pos = pos;
	Out.tex = tex;
	return Out;
}

/******************************************
���ӂ̃s�N�Z���J���[����������������֐�
*******************************************/
float4 GetPixelColor(float2 texel[10])
{
	float4 p0 = tex2D(s0, texel[0]) * 0.19f;
	float4 p1 = tex2D(s0, texel[1]) * 0.17f;
	float4 p2 = tex2D(s0, texel[2]) * 0.15f;
	float4 p3 = tex2D(s0, texel[3]) * 0.13f;
	float4 p4 = tex2D(s0, texel[4]) * 0.11f;
	float4 p5 = tex2D(s0, texel[5]) * 0.09f;
	float4 p6 = tex2D(s0, texel[6]) * 0.07f;
	float4 p7 = tex2D(s0, texel[7]) * 0.05f;
	float4 p8 = tex2D(s0, texel[8]) * 0.03f;
	float4 p9 = tex2D(s0, texel[9]) * 0.01f;

	return p0 + p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;
}

/******************************************
�s�N�Z���V�F�[�_�i�E�����u���[�j
*******************************************/
float4 PS1(VS_OUTPUT In) : COLOR0
{
	float2 texel[10];

	texel[0] = In.tex;
	texel[1] = In.tex + float2(texelU[0], 0.0f);
	texel[2] = In.tex + float2(texelU[1], 0.0f);
	texel[3] = In.tex + float2(texelU[2], 0.0f);
	texel[4] = In.tex + float2(texelU[3], 0.0f);
	texel[5] = In.tex + float2(texelU[4], 0.0f);
	texel[6] = In.tex + float2(texelU[5], 0.0f);
	texel[7] = In.tex + float2(texelU[6], 0.0f);
	texel[8] = In.tex + float2(texelU[7], 0.0f);
	texel[9] = In.tex + float2(texelU[8], 0.0f);

	return GetPixelColor(texel);
}

/******************************************
�s�N�Z���V�F�[�_�i�������u���[�j
*******************************************/
float4 PS2(VS_OUTPUT In) : COLOR0
{
	float2 texel[10];

	texel[0] = In.tex;
	texel[1] = In.tex + float2(-texelU[0], 0.0f);
	texel[2] = In.tex + float2(-texelU[1], 0.0f);
	texel[3] = In.tex + float2(-texelU[2], 0.0f);
	texel[4] = In.tex + float2(-texelU[3], 0.0f);
	texel[5] = In.tex + float2(-texelU[4], 0.0f);
	texel[6] = In.tex + float2(-texelU[5], 0.0f);
	texel[7] = In.tex + float2(-texelU[6], 0.0f);
	texel[8] = In.tex + float2(-texelU[7], 0.0f);
	texel[9] = In.tex + float2(-texelU[8], 0.0f);

	return GetPixelColor(texel);
}

/******************************************
�s�N�Z���V�F�[�_�i������u���[�j
*******************************************/
float4 PS3(VS_OUTPUT In) : COLOR0
{
	float2 texel[10];

	texel[0] = In.tex;
	texel[1] = In.tex + float2(0.0f, -texelV[0]);
	texel[2] = In.tex + float2(0.0f, -texelV[1]);
	texel[3] = In.tex + float2(0.0f, -texelV[2]);
	texel[4] = In.tex + float2(0.0f, -texelV[3]);
	texel[5] = In.tex + float2(0.0f, -texelV[4]);
	texel[6] = In.tex + float2(0.0f, -texelV[5]);
	texel[7] = In.tex + float2(0.0f, -texelV[6]);
	texel[8] = In.tex + float2(0.0f, -texelV[7]);
	texel[9] = In.tex + float2(0.0f, -texelV[8]);

	return GetPixelColor(texel);
}

/******************************************
�s�N�Z���V�F�[�_�i�������u���[�j
*******************************************/
float4 PS4(VS_OUTPUT In) : COLOR0
{
	float2 texel[10];

	texel[0] = In.tex;
	texel[1] = In.tex + float2(0.0f, texelV[0]);
	texel[2] = In.tex + float2(0.0f, texelV[1]);
	texel[3] = In.tex + float2(0.0f, texelV[2]);
	texel[4] = In.tex + float2(0.0f, texelV[3]);
	texel[5] = In.tex + float2(0.0f, texelV[4]);
	texel[6] = In.tex + float2(0.0f, texelV[5]);
	texel[7] = In.tex + float2(0.0f, texelV[6]);
	texel[8] = In.tex + float2(0.0f, texelV[7]);
	texel[9] = In.tex + float2(0.0f, texelV[8]);

	return GetPixelColor(texel);
}

/******************************************
�e�N�j�b�N
*******************************************/
technique tech
{
	pass P0
	{
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS1();
	}

	pass P1
	{
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS2();
	}

	pass P2
	{
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS3();
	}

	pass P3
	{
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS4();
	}

};