Texture2D    Texture		: register(t0);
SamplerState Sampler		: register(s0);

cbuffer cbReductionData : register(b0)
{
	float2 TextureDimensions;
}

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
};

float4 PSMain(VSOutput input) : SV_TARGET
{
	// Calculate the minimum of this color and the pixel to the left; Basically calculating the first hit distance of each ray
	float2 color = Texture.Sample(Sampler, input.TexCoord);
	float2 colorL = Texture.Sample(Sampler, input.TexCoord - float2(TextureDimensions.x,0));
	float2 result = min(color, colorL);
	return float4(result,0,1);
}