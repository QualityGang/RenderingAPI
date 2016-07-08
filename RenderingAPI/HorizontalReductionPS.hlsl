Texture2D    Texture : register(t0);
SamplerState Sampler : register(s0);

struct VSOutput
{
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD;
	float4 Color    : COLOR;
};

cbuffer cbReductionData : register(b0)
{
	float2 TextureDimensions;
}

float4 PSMain(VSOutput input) : SV_TARGET
{
	float2 color = Texture.Sample(Sampler, input.TexCoord);
	float2 colorR = Texture.Sample(Sampler, input.TexCoord + float2(TextureDimensions.x,0));
	float2 result = min(color, colorR);
	return float4(result,0,1);
}