//=====================================
//
//�����o�[�g�Ɩ��V�F�[�_[LambertLight.fx]
//Author:GP12B332 21 ���ԗY��
//
//=====================================

/***************************************
�O���[�o���ϐ�
***************************************/
float4x4 mtxView;			//�r���[�s��
float4x4 mtxProjection;		//�v���W�F�N�V�����s��
float4x4 mtxWorld;			//���[���h�ϊ��s��
float4 lightDir[3];			//���s�����̕����x�N�g��
float4 ambient[3];			//����
float zf;					//���ߎˉe�s��̍ŉ��ߋ���

/***************************************
�e�N�X�`���T���v��
***************************************/
sampler s0 : register(s0);

/***************************************
���_�V�F�[�_�o�͍\����
***************************************/
struct VS_OUTPUT
{
	float4 pos		: POSITION;
	float4 col		: COLOR0;
	float2 tex		: TEXCOORD0;
	float4 posWVP	: TEXCOORD1;
};

/***************************************
���_�V�F�[�_
***************************************/
VS_OUTPUT VS(
	float4 pos		: POSITION,
	float4 normal : NORMAL,
	float2 tex : TEXCOORD0)
{
	VS_OUTPUT Out;

	Out.pos = mul(mul(mul(pos, mtxWorld), mtxView), mtxProjection);
	Out.tex = tex;

	int i;
	for (i = 0; i < 3; i++)
	{
		float3 L = lightDir[i].xyz;
		float3 N = normalize(normal.xyz);
		Out.col += max(ambient[i], dot(N, L));
	}

	Out.col = saturate(Out.col);
	
	Out.posWVP = Out.pos;

	return Out;
}

/***************************************
�s�N�Z���V�F�[�_�o�͍\����
***************************************/
struct PS_OUTPUT
{
	float4 col1 : COLOR0;
	float4 col2 : COLOR1;
};

/***************************************
�s�N�Z���V�F�[�_
***************************************/
PS_OUTPUT PS(VS_OUTPUT In)
{
	PS_OUTPUT Out;

	//�F�����i�[
	Out.col1 = tex2D(s0, In.tex);
	Out.col1.rgb = Out.col1.rgb * In.col.rgb;

	//Z�l�����i�[
	Out.col2 = In.posWVP.z / zf;

	return Out;
}

/***************************************
�e�N�j�b�N
***************************************/
technique tech
{
	pass P0 {
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS();
	}
};