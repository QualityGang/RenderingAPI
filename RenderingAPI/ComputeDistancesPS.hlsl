
Texture2D    Texture : register(t0);
SamplerState Sampler : register(s0);

struct VSInput
{
	float4   Position : POSITION;
	float2   TexCoord : TEXCOORD;
	float4   Color    : COLOR;
};

struct VSOutput
{
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD;
	float4 Color    : COLOR;
};

VSOutput VSMain(VSInput input)
{
	VSOutput output;
	output.Position = input.Position;
	output.Color = input.Color;
	output.TexCoord = input.TexCoord;

	return output;
}

float4 PSMain(VSOutput input) : SV_TARGET
{
	return Texture.Sample(Sampler, input.TexCoord) * input.Color;
}