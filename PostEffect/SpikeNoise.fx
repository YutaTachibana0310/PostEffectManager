//=====================================
//
//�X�p�C�N�m�C�Y�V�F�[�_[SpikeNoise.fx]
//Author:GP12B332 21 ���ԗY��
//
//=====================================

/***************************************
�O���[�o���ϐ�
***************************************/
float length;		//�m�C�Y�̋���

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
	float2 tex : TEXCOORD
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
	//float y = (int)(In.tex.y * 50.0f) * 0.02f * 20.0f;
	float y = In.tex.y * 50.0f ;
	In.tex.x += (sin(10 * y) + cos(15 * y)) * 0.02f *length;
	
	return tex2D(s0, In.tex);
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