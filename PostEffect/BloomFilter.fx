//=====================================
//
//�u���[���V�F�[�_[BloomFilter.fx]
//Author:GP12B332 21 ���ԗY��
//
//=====================================

/***************************************
�O���[�o���ϐ�
***************************************/
float threthold;				//���o���邵�����l
float bloomPower;				//�P�x�̃Q�C��

/***************************************
�e�N�X�`���T���v���[
***************************************/
sampler s0 : register(s0) = sampler_state{
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};	

sampler s1 : register(s1) = sampler_state {
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

sampler s2 : register(s2) = sampler_state {
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

/***************************************
���_�V�F�[�_�o�͍\����
***************************************/
struct VS_OUTPUT
{
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
�s�N�Z���V�F�[�_(�P�x���o�p)
***************************************/
float4 PS1(VS_OUTPUT In) : COLOR0
{
	//�l�̖ڂ̋P�x�̎󂯎������l�������W��
	const float3 perception = float3(0.2126f, 0.7152f, 0.0722f);

	//�e�N�X�`�����T���v�����O
	float4 color = tex2D(s0, In.tex);
	
	//�P�x���v�Z
	float brightness = dot(perception, float3(color.rgb));

	//�P�x���������l�����Ȃ獕�A�ȏ�Ȃ�T���v�����O�����F���Q�C�����ĕԂ�
	float4 result = lerp(float4(0.0f, 0.0f, 0.0f, 0.0f), color, step(threthold, brightness));
	return result * bloomPower;
}

/***************************************
�s�N�Z���V�F�[�_(���Z�����p)
***************************************/
float4 PS2(VS_OUTPUT In) : COLOR0
{
	return tex2D(s0, In.tex) + tex2D(s1, In.tex) + tex2D(s2, In.tex);
}

/***************************************
�e�N�j�b�N
***************************************/
technique tech {
	pass P0 {
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS1();
	}

	pass P1 {
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS2();
	}
};