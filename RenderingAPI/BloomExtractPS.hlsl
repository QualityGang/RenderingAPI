
// Pixel shader extracts the brighter areas of an image.
// This is the first step in applying a bloom postprocess.

Texture2D    Texture : register(t0);
SamplerState Sampler : register(s0);

cbuffer cbBloomThreshold : register(b0)
{
	float BloomThreshold;
}

struct VSOutput // TOOD: Remove duplicate
{
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD;
	float4 Color    : COLOR;
};

float4 PSMain(VSOutput input) : SV_TARGET
{
	// Look up the original image color.
	float4 c = Texture.Sample(Sampler, input.TexCoord);

	// Adjust it to keep only values brighter than the specified threshold.
	return saturate((c - BloomThreshold) / (1 - BloomThreshold));
}