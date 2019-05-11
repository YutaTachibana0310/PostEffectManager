//=====================================
//
//�V���b�N�u���[�V�F�[�_[ShockBlur.fx]
//Author:GP12B332 21 ���ԗY��
//
//=====================================

/***************************************
�O���[�o���ϐ�
***************************************/
float2 centerTexel;		//���S�ƂȂ�e�N�Z��
float blurPower;		//�u���[����
float tU;				//1�e�N�Z���̑傫���iX�����j
float tV;				//1�e�N�Z���̑傫���iY�����j

/***************************************
�e�N�X�`���T���v��
***************************************/
sampler s0 : register(s0);

/***************************************
���_�V�F�[�_�o�͍\����
***************************************/
struct VS_OUTPUT {
	float4 pos : POSITION;
	float2 tex : TEXCOORD0;
};

/***************************************
���_�V�F�[�_
***************************************/
VS_OUTPUT VS(
	float4 pos : POSITION,
	float2 tex : TEXCOORD0
) {
	VS_OUTPUT Out;

	Out.pos = pos;
	Out.tex = tex;

	return Out;
}

/***************************************
�s�N�Z���V�F�[�_
***************************************/
float4 PS(VS_OUTPUT In) : COLOR0
{
	float4 Color[10];

	//�u���[����
	float2 dir = centerTexel - In.tex;

	//�������v�Z
	float len = length(dir);

	//���K����1�e�N�Z�����ɂ���
	dir = normalize(dir) * float2(tU, tV);

	//�{�P����v�Z
	dir *= blurPower * len;

	//����
	Color[0] = tex2D(s0, In.tex) * 0.19f;
	Color[1] = tex2D(s0, In.tex + dir) * 0.17f;
	Color[2] = tex2D(s0, In.tex + dir * 2.0f) * 0.15f;
	Color[3] = tex2D(s0, In.tex + dir * 3.0f) * 0.13f;
	Color[4] = tex2D(s0, In.tex + dir * 4.0f) * 0.11f;
	Color[5] = tex2D(s0, In.tex + dir * 5.0f) * 0.09f;
	Color[6] = tex2D(s0, In.tex + dir * 6.0f) * 0.07f;
	Color[7] = tex2D(s0, In.tex + dir * 7.0f) * 0.05f;
	Color[8] = tex2D(s0, In.tex + dir * 8.0f) * 0.03f;
	Color[9] = tex2D(s0, In.tex + dir * 9.0f) * 0.01f;

	return Color[0] + Color[1] + Color[2] + Color[3] + Color[4] + Color[5] + Color[6] + Color[7] + Color[8] + Color[9];
}

/***************************************
�e�N�j�b�N
***************************************/
technique tech
{
	pass P0
	{
		VertexShader = compile vs_1_1 VS();
		PixelShader = compile ps_2_0 PS();
	}
};