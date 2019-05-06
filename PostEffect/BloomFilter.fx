float threthold;				//抽出するしきい値
float bloomPower;				//輝度のゲイン

sampler s0 : register(s0);		//テクスチャサンプラー

/***************************************
頂点シェーダ出力構造体
***************************************/
struct VS_OUTPUT
{
	float4 pos : POSITION;
	float2 tex : TEXCOORD0;
};

/***************************************
頂点シェーダ
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
ピクセルシェーダ
***************************************/
float4 PS(VS_OUTPUT In) : COLOR0
{
	//人の目の輝度の受け取り方を考慮した係数
	const float3 perception = float3(0.2126f, 0.7152f, 0.0722f);

	//テクスチャをサンプリング
	float4 color = tex2D(s0, In.tex);
	
	//輝度を計算
	float brightness = dot(perception, float3(color.rgb));

	//輝度がしきい値未満なら黒、以上ならサンプリングした色をゲインして返す
	float4 result = lerp(float4(0.0f, 0.0f, 0.0f, 0.0f), color, step(threthold, brightness));
	return result * bloomPower;
}

/***************************************
テクニック
***************************************/
technique tech {
	pass P0 {
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS();
	}
};