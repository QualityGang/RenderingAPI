#pragma once

enum SpriteEffect
{
	SpriteEffect_None           = 0,
	SpriteEffect_FlipVertical   = (1U << 0),
	SpriteEffect_FlipHorizontal = (1U << 1),
	SpriteEffect_FlipBoth = SpriteEffect_FlipVertical | SpriteEffect_FlipHorizontal
};
