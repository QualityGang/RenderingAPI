using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

struct Rectangle
{
	public float X, Y, Width, Height;

	public float Left   { get { return X;          } }
	public float Right  { get { return X + Width;  } }
	public float Top    { get { return Y;          } }
	public float Bottom { get { return Y + Height; } }

	public Rectangle(float x, float y, float width, float height)
	{
		X = x;
		Y = y;
		Width = width;
		Height = height;
	}

	public bool Overlaps(Vector2 vector)
	{
		return X <= vector.X && vector.X < X + Width
			&& Y <= vector.Y && vector.Y < Y + Height;
	}

	public void Inflate(Vector2 vector)
	{
		X -= vector.X;
		Y -= vector.Y;
		Width += vector.X * 2;
		Height += vector.Y * 2;
	}
}
