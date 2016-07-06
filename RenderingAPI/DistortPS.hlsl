Texture2D    Texture : register(t0);
SamplerState Sampler : register(s0);

struct VSOutput
{
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD;
	float4 Color    : COLOR;
};

float4 PSMain(VSOutput input) : SV_TARGET
{
	return Texture.Sample(Sampler, input.TexCoord);
}