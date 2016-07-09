

Texture2D    Texture : register(t0);
SamplerState Sampler : register(s0);

cbuffer cbDistanceData : register(b0)
{
	float4 BackgroundColor;
}

struct VSOutput
{
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD;
	float4 Color    : COLOR;
};

float4 PSMain(VSOutput input) : SV_TARGET
{
	float4 color = Texture.Sample(Sampler, input.TexCoord);
	float distance = (color != BackgroundColor)  ? length(input.TexCoord - 0.5f) : 1.0f;
	return float4(distance,0,0,1);
}