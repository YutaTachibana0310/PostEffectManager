//=====================================
//
//���m�g�[���V�F�[�_[MonotoneFilter.fx]
//Author:GP12B332 21 ���ԗY��
//
//=====================================

/***************************************
�O���[�o���ϐ�
***************************************/
float power;		//���m�g�[���̋���

/***************************************
�e�N�X�`���T���v��
***************************************/
sampler tex0 : register(s0);	//�e�N�X�`���T���v���[

/***************************************
���_�V�F�[�_�o�͍\����
***************************************/
struct VS_OUTPUT {
	float4 Pos : POSITION;
	float2 Tex : TEXCOORD0;
};

/***************************************
���_�V�F�[�_
***************************************/
VS_OUTPUT VS(
	float4 Pos : POSITION,
	float4 Col : COLOR0,
	float2 Tex : TEXCOORD0)
{
	VS_OUTPUT Out;

	Out.Pos = Pos;
	Out.Tex = Tex;

	return Out;
}

/***************************************
�s�N�Z���V�F�[�_
***************************************/
float4 PS(VS_OUTPUT In) : COLOR
{
	float4 Out = tex2D(tex0, In.Tex);
	float4 Col = tex2D(tex0, In.Tex);

	//���m�g�[���������F
	Col.rgb = (Col.r + Col.g + Col.b) * 0.333f;

	//���`����
	Out.r = Out.r * (1.0f - power) + Col.r * power;
	Out.g = Out.g * (1.0f - power) + Col.g * power;
	Out.b = Out.b * (1.0f - power) + Col.b * power;

	return Out;
}

/***************************************
�e�N�j�b�N
***************************************/
technique tech {
	pass P0
	{
		VertexShader = compile vs_1_1 VS();
		PixelShader = compile ps_2_0 PS();
	}
}