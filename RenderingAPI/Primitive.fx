


struct VSInput
{
	float4 Position : POSITION;
	float4 Color    : COLOR;
};

struct VSOutput
{
	float4 Position : SV_POSITION;
	float4 Color    : COLOR;
};

VSOutput VSMain(VSInput input)
{
	VSOutput output;
	output.Position = input.Position;
	output.Color    = input.Color;

	return output;
}

float4 PSMain(VSOutput input) : SV_TARGET
{
	return input.Color;
}