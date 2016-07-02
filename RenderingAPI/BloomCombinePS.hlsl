
// Pixel shader combines the bloom image with the original
// scene, using tweakable intensity levels and saturation.
// This is the final step in applying a bloom postprocess.

Texture2D    BloomTexture : register(t0);
Texture2D    BaseTexture  : register(t1);
SamplerState Sampler      : register(s0);

struct VSOutput // TOOD: Remove duplicate
{
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD;
	float4 Color    : COLOR;
};

cbuffer cbIntensityAndSaturation : register(b0)
{
	float BloomIntensity;
	float BaseIntensity;

	float BloomSaturation;
	float BaseSaturation;
}

float4 AdjustSaturation(float4 color, float saturation)
{
	// The constants 0.3, 0.59, and 0.11 are chosen because the
	// human eye is more sensitive to green light, and less to blue.
	float grey = dot(color.rgb, float3(0.3, 0.59, 0.11));

	return lerp(grey, color, saturation);
}

float4 PSMain(VSOutput input) : SV_TARGET
{
	// Look up the bloom and original base image colors.
	float4 bloom = BloomTexture.Sample(Sampler, input.TexCoord);
	float4 base  = BaseTexture.Sample(Sampler, input.TexCoord);

	// Adjust color saturation and intensity.
	bloom = AdjustSaturation(bloom, BloomSaturation) * BloomIntensity;
	base  = AdjustSaturation(base,  BaseSaturation)  * BaseIntensity;

	// Darken down the base image in areas where there is a lot of bloom,
	// to prevent things looking excessively burned-out.
	base *= (1 - saturate(bloom));

	// Combine the two images.
	return base + bloom;
}