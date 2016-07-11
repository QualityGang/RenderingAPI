

Texture2D    Texture : register(t0);
SamplerState Sampler : register(s0);

cbuffer cbDistanceData : register(b0)
{
	float BackgroundColor;
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

	// Compute distance from center
	float distance = (color != BackgroundColor)  ? length(input.TexCoord - 0.5f) : 1.0f;
	// Save it to the Red channel
	return float4(distance,0,0,1);
}