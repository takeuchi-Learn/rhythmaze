#include "Basic.hlsli"

Texture2D<float4> tex : register(t0);  	// 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      	// 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
	float4 texcolor = float4(tex.Sample(smp, input.uv));
	float3 normalLight = normalize(light);
	normalLight = normalize(float3(0,-1,0));

	float lightPower = 0.75f;

	// 光源へのベクトルと法線ベクトルの内積
	float diffuse = saturate(dot(-normalLight, input.normal)) * lightPower;
	// アンビエント光を0.3として計算
	float brightness = diffuse + 0.2f;
	// テクスチャとシェーディングによる色を合成
	return float4(texcolor.rgb * brightness, texcolor.a) * color;
}
