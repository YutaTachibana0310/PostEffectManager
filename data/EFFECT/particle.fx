//=====================================
//
//�p�[�e�B�N���V�F�[�_[Particle.fx]
//Author:GP12B332 21 ���ԗY��
//
//=====================================

/***************************************
�O���[�o���ϐ�
***************************************/
float4x4 mtxView;		//�r���[�s��
float4x4 mtxProj;		//�v���W�F�N�V�����s��
float4x4 mtxInvView;	//�r���[�t�s��

/**************************************************************
�e�N�X�`���T���v���[�i0�Ԃ�SetTexture�����e�N�X�`�����g�p����
***************************************************************/
sampler s0 : register(s0);

/**************************************************************
���_�V�F�[�_�o�͍\����
***************************************************************/
struct VS_OUTPUT {
	float4 pos : POSITION;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
};

/**************************************************************
���_�V�F�[�_
***************************************************************/
VS_OUTPUT vsMain(
	float3 pos : POSITION,
	float2 localUV : TEXCOORD0,
	float3 worldPos : TEXCOORD1,
	float3 worldRot : TEXCOORD2,
	float3 worldScl : TEXCOORD3,
	float2 texUV : TEXCOORD4
) {
	VS_OUTPUT Out;

	Out.pos = float4(pos, 1.0f);

	//scale
	Out.pos.x = Out.pos.x * worldScl.x;
	Out.pos.y = Out.pos.y * worldScl.y;
	Out.pos.z = Out.pos.z * worldScl.z;

	//rotX
	float4 tmpPos = Out.pos;
	Out.pos.y = tmpPos.y * cos(worldRot.x) - tmpPos.z * sin(worldRot.x);
	Out.pos.z = tmpPos.y * sin(worldRot.x) + tmpPos.z * sin(worldRot.x);

	//rotY
	tmpPos = Out.pos;
	Out.pos.x = tmpPos.x * cos(worldRot.y) + tmpPos.z * sin(worldRot.y);
	Out.pos.z = tmpPos.x * -sin(worldRot.y) + tmpPos.z * cos(worldRot.y);

	//rotZ
	tmpPos = Out.pos;
	Out.pos.x = tmpPos.x * cos(worldRot.z) - tmpPos.y * sin(worldRot.z);
	Out.pos.y = tmpPos.x * sin(worldRot.z) + tmpPos.y * cos(worldRot.z);

	//InvView
	Out.pos = mul(Out.pos, mtxInvView);

	//Translate
	Out.pos.x = Out.pos.x + worldPos.x;
	Out.pos.y = Out.pos.y + worldPos.y;
	Out.pos.z = Out.pos.z + worldPos.z;

	//view & projection
	Out.pos = mul(Out.pos, mtxView);
	Out.pos = mul(Out.pos, mtxProj);

	//texUV
	Out.uv = localUV + texUV;

	//Color
	Out.color = float4(1.0f, 1.0f, 1.0f, 1.0f);

	return Out;
}

/**************************************************************
�s�N�Z���V�F�[�_
***************************************************************/
float4 psMain(VS_OUTPUT In) : COLOR0
{
	//�e�N�X�`������J���[���擾
	return tex2D(s0, In.uv) * In.color;
}

/**************************************************************
�e�N�j�b�N
***************************************************************/
technique tech {
	pass p0 {
		VertexShader = compile vs_2_0 vsMain();
		PixelShader = compile ps_2_0 psMain();
	}
};