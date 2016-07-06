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
	float u0 = (input.TexCoord.x * 2) - 1;
	float v0 = (input.TexCoord.y * 2) - 1;

	v0 = v0 * abs(u0);

	v0 = (v0 + 1) / 2;

	float2 newCoords = float2(input.TexCoord.x, v0);

	float horizontal = Texture.Sample(Sampler, newCoords).r;
	float vertical = Texture.Sample(Sampler, newCoords.yx).r;

	return float4(horizontal,vertical,0,1);
}