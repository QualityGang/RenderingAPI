using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

static class Extensions
{
    public static float ToAngle(this Vector2 vector)
	{
		return (float)Math.Atan2(vector.Y, vector.X);
	}

	public static Vector2 ScaleTo(this Vector2 vector, float length)
	{
		return vector * (length / vector.Length());
	}

	public static float NextFloat(this Random rand, float minValue, float maxValue)
	{
		return (float)rand.NextDouble() * (maxValue - minValue) + minValue;
	}

	public static Vector2 NextVector2(this Random rand, float minLength, float maxLength)
	{
		double theta = rand.NextDouble() * 2 * Math.PI;
		float length = rand.NextFloat(minLength, maxLength);
		return new Vector2(length * (float)Math.Cos(theta), length * (float)Math.Sin(theta));
	}

	public static void DrawLine(this SpriteBatch spriteBatch, Vector2 start, Vector2 end, Color color, float thickness = 2f)
	{
		Vector2 delta = end - start;

		TextureSize texSize = GameRoot.Context.GetTexture2DSize(Art.Pixel.GetTexture2D());

		Vector2 size = new Vector2(texSize.Width, texSize.Height);
		size *= new Vector2(delta.Length(), thickness);

		Sprite sprite = new Sprite();
		sprite.SetPositionAnchor(0.0f, 0.5f);
		sprite.SetPosition(start.X, start.Y);
		sprite.SetSize(size.X, size.Y);
		sprite.SetSrcRect(0, 0, texSize.Width, texSize.Height);
		sprite.SetAngle((float)(delta.ToAngle() * (180 / Math.PI)));
		sprite.SetColor(color);
		sprite.SetTexture(Art.Pixel.GetTexture2D());

		spriteBatch.Draw(sprite);
	}
}
