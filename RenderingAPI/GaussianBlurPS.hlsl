
// Pixel shader applies a one dimensional gaussian blur filter.
// This is used twice by the bloom postprocess, first to
// blur horizontally, and then again to blur vertically.

#define SAMPLE_COUNT 15

Texture2D    Texture : register(t0);
SamplerState Sampler : register(s0);

cbuffer cbOffsetsAndWeights : register(b0)
{
	float2 SampleOffsets[SAMPLE_COUNT];
	float  SampleWeights[SAMPLE_COUNT];
}

struct VSOutput // TOOD: Remove duplicate
{
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD;
	float4 Color    : COLOR;
};

float4 PSMain(VSOutput input) : SV_TARGET
{
	float4 c = 0;

	// Combine a number of weighted image filter taps.
	for (int i = 0; i < SAMPLE_COUNT; i++)
		c += Texture.Sample(Sampler, input.TexCoord + SampleOffsets[i]) * SampleWeights[i];

	return c;
}