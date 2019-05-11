//=====================================
//
//ランバート照明シェーダ[LambertLight.fx]
//Author:GP12B332 21 立花雄太
//
//=====================================

/***************************************
グローバル変数
***************************************/
float4x4 mtxView;			//ビュー行列
float4x4 mtxProjection;		//プロジェクション行列
float4x4 mtxWorld;			//ワールド変換行列
float4 lightDir[3];			//平行光源の方向ベクトル
float4 ambient[3];			//環境光
float zf;					//遠近射影行列の最遠近距離

/***************************************
テクスチャサンプラ
***************************************/
sampler s0 : register(s0);

/***************************************
頂点シェーダ出力構造体
***************************************/
struct VS_OUTPUT
{
	float4 pos		: POSITION;
	float4 col		: COLOR0;
	float2 tex		: TEXCOORD0;
	float4 posWVP	: TEXCOORD1;
};

/***************************************
頂点シェーダ
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
ピクセルシェーダ出力構造体
***************************************/
struct PS_OUTPUT
{
	float4 col1 : COLOR0;
	float4 col2 : COLOR1;
};

/***************************************
ピクセルシェーダ
***************************************/
PS_OUTPUT PS(VS_OUTPUT In)
{
	PS_OUTPUT Out;

	//色情報を格納
	Out.col1 = tex2D(s0, In.tex);
	Out.col1.rgb = Out.col1.rgb * In.col.rgb;

	//Z値情報を格納
	Out.col2 = In.posWVP.z / zf;

	return Out;
}

/***************************************
テクニック
***************************************/
technique tech
{
	pass P0 {
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS();
	}
};