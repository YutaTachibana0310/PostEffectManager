float texelU[5];	//X方向の隣のテクセル位置
float texelV[5];	//Y方向の隣のテクセル位置
int blurPower;		//ブラーの強さ

sampler s0 : register(s);	//テクスチャサンプラー

/*********************************
頂点シェーダ出力構造体
*********************************/
struct VS_OUTPUT
{
	float4 pos	: POSITION;
	float2 tex	: TEXCOORD0;
};

/*********************************
頂点シェーダ
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
//X方向にぼかすピクセルシェーダ(弱)
*********************************/
float4 PS1(VS_OUTPUT In) : COLOR0
{
	//テクセルを取得
	float2 texel0 = In.tex + float2(-texelU[0], 0.0f);
	float2 texel1 = In.tex + float2(-texelU[1], 0.0f);
	float2 texel2 = In.tex + float2(-texelU[2], 0.0f);
	float2 texel3 = In.tex + float2(-texelU[3], 0.0f);
	float2 texel4 = In.tex + float2(-texelU[4], 0.0f);

	float2 texel5 = In.tex + float2(texelU[0], 0.0f);
	float2 texel6 = In.tex + float2(texelU[1], 0.0f);
	float2 texel7 = In.tex + float2(texelU[2], 0.0f);
	float2 texel8 = In.tex + float2(texelU[3], 0.0f);
	float2 texel9 = In.tex + float2(texelU[4], 0.0f);

	//それぞれのテクセルのカラー情報を取得し合成
	float4 p0 = tex2D(s0, In.tex) * 0.20f;
	
	float4 p1 = tex2D(s0, texel0) * 0.12f;
	float4 p2 = tex2D(s0, texel1) * 0.10f;
	float4 p3 = tex2D(s0, texel2) * 0.08f;
	float4 p4 = tex2D(s0, texel3) * 0.06f;
	float4 p5 = tex2D(s0, texel4) * 0.04f;

	float4 p6 = tex2D(s0, texel5) * 0.12f;
	float4 p7 = tex2D(s0, texel6) * 0.10f;
	float4 p8 = tex2D(s0, texel7) * 0.08f;
	float4 p9 = tex2D(s0, texel8) * 0.06f;
	float4 p10 = tex2D(s0, texel9) * 0.04f;

	return p0 + p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9 + p10;
}

/*********************************
//Y方向にぼかすピクセルシェーダ(弱)
*********************************/
float4 PS2(VS_OUTPUT In) : COLOR0
{
	//テクセルを取得
	float2 texel0 = In.tex + float2(0.0f, -texelV[0]);
	float2 texel1 = In.tex + float2(0.0f, -texelV[1]);
	float2 texel2 = In.tex + float2(0.0f, -texelV[2]);
	float2 texel3 = In.tex + float2(0.0f, -texelV[3]);
	float2 texel4 = In.tex + float2(0.0f, -texelV[4]);

	float2 texel5 = In.tex + float2(0.0f, texelV[0]);
	float2 texel6 = In.tex + float2(0.0f, texelV[1]);
	float2 texel7 = In.tex + float2(0.0f, texelV[2]);
	float2 texel8 = In.tex + float2(0.0f, texelV[3]);
	float2 texel9 = In.tex + float2(0.0f, texelV[4]);

	//それぞれのテクセルのカラー情報を取得し合成
	float4 p0 = tex2D(s0, In.tex) * 0.20f;

	float4 p1 = tex2D(s0, texel0) * 0.12f;
	float4 p2 = tex2D(s0, texel1) * 0.10f;
	float4 p3 = tex2D(s0, texel2) * 0.08f;
	float4 p4 = tex2D(s0, texel3) * 0.06f;
	float4 p5 = tex2D(s0, texel4) * 0.04f;

	float4 p6 = tex2D(s0, texel5) * 0.12f;
	float4 p7 = tex2D(s0, texel6) * 0.10f;
	float4 p8 = tex2D(s0, texel7) * 0.08f;
	float4 p9 = tex2D(s0, texel8) * 0.06f;
	float4 p10 = tex2D(s0, texel9) * 0.04f;

	return p0 + p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9 + p10;
}

/*********************************
テクニック
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
}