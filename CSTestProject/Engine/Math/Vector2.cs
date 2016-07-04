using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

struct Vector2 : IEquatable<Vector2>
{
    public static readonly Vector2 Zero = new Vector2(0);
	public static readonly Vector2 One  = new Vector2(1);

    public float X, Y;

    public Vector2(float x, float y)
    {
        X = x;
        Y = y;
    }

    public Vector2(float value)
    {
        X = value;
        Y = value;
    }

	public float Length()
	{
		return (float)Math.Sqrt(X * X + Y * Y);
	}

	public float LengthSquared()
	{
		return X * X + Y * Y;
	}

	public void Normalize()
	{
		float val = 1.0f / Length();
		X *= val;
		Y *= val;
	}

	public float Distance(Vector2 vector)
	{
		float dx = X - vector.X;
		float dy = Y - vector.Y;
		return (float)Math.Sqrt(dx * dx + dy * dy);
	}

	public float DistanceSquared(Vector2 vector)
	{
		float dx = X - vector.X;
		float dy = Y - vector.Y;
		return dx * dx + dy * dy;
	}
	
	public void Transform(Quaternion quat)
	{
		Quaternion v = new Quaternion(X, Y, 0, 0), i, t;
		i = Quaternion.Inverse(quat);
		t = Quaternion.Multiply(quat, v);
		v = Quaternion.Multiply(t, i);

		X = v.X;
		Y = v.Y;
	}

	public void Clamp(Vector2 min, Vector2 max)
	{
		X = Math.Min(Math.Max(min.X, X), max.X);
		Y = Math.Min(Math.Max(min.Y, Y), max.Y);
	}

	public static Vector2 CatmullRom(Vector2 value1, Vector2 value2, Vector2 value3, Vector2 value4, float amount)
	{
		return new Vector2(
			MathUtils.CatmullRom(value1.X, value2.X, value3.X, value4.X, amount),
			MathUtils.CatmullRom(value1.Y, value2.Y, value3.Y, value4.Y, amount));
	}

	public static Vector2 operator-(Vector2 value)
    {
        value.X = -value.X;
        value.Y = -value.Y;
        return value;
    }

    public static bool operator==(Vector2 value1, Vector2 value2)
    {
        return value1.X == value2.X && value1.Y == value2.Y;
    }

    public static bool operator!=(Vector2 value1, Vector2 value2)
    {
        return value1.X != value2.X || value1.Y != value2.Y;
    }

    public static Vector2 operator+(Vector2 value1, Vector2 value2)
    {
        value1.X += value2.X;
        value1.Y += value2.Y;
        return value1;
    }

    public static Vector2 operator-(Vector2 value1, Vector2 value2)
    {
        value1.X -= value2.X;
        value1.Y -= value2.Y;
        return value1;
    }

    public static Vector2 operator*(Vector2 value1, Vector2 value2)
    {
        value1.X *= value2.X;
        value1.Y *= value2.Y;
        return value1;
    }

    public static Vector2 operator*(Vector2 value, float scaleFactor)
    {
        value.X *= scaleFactor;
        value.Y *= scaleFactor;
        return value;
    }

    public static Vector2 operator*(float scaleFactor, Vector2 value)
    {
        value.X *= scaleFactor;
        value.Y *= scaleFactor;
        return value;
    }

    public static Vector2 operator/(Vector2 value1, Vector2 value2)
    {
		value1.X /= value2.X;
        value1.Y /= value2.Y;
        return value1;
    }

    public static Vector2 operator/(Vector2 value1, float divider)
    {
        float factor = 1 / divider;
        value1.X *= factor;
        value1.Y *= factor;
        return value1;
    }

	public override bool Equals(object obj)
	{
		if (obj is Vector2)
			return Equals((Vector2)obj);

		return false;
	}

	public bool Equals(Vector2 other)
	{
		return X == other.X && Y == other.Y;
	}

	public override int GetHashCode()
	{
		return X.GetHashCode() + Y.GetHashCode();
	}

	public override string ToString()
	{
		return X.ToString() + " " + Y.ToString();
	}
}
