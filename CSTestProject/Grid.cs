using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

class Grid
{
	private class PointMass
	{
		public Vector3 Position;
		public Vector3 Velocity;
		public float InverseMass;

		private Vector3 acceleration;
		private float damping = 0.98f;

		public PointMass(Vector3 position, float invMass)
		{
			Position = position;
			InverseMass = invMass;
		}

		public void ApplyForce(Vector3 force)
		{
			acceleration += force * InverseMass;
		}

		public void IncreaseDamping(float factor)
		{
			damping *= factor;
		}

		public void Update()
		{
			Velocity += acceleration;
			Position += Velocity;
			acceleration = Vector3.Zero;

			if (Velocity.LengthSquared() < 0.001f * 0.001f)
				Velocity = Vector3.Zero;

			Velocity *= damping;
			damping = 0.98f;
		}
	}

	private struct Spring
	{
		public PointMass End1;
		public PointMass End2;
		public float TargetLength;
		public float Stiffness;
		public float Damping;

		public Spring(PointMass end1, PointMass end2, float stiffness, float damping)
		{
			End1 = end1;
			End2 = end2;
			Stiffness = stiffness;
			Damping = damping;
			TargetLength = end1.Position.Distance(end2.Position) * 0.95f;
		}

		public void Update()
		{
			var x = End1.Position - End2.Position;

			float length = x.Length();

			if (length <= TargetLength)
				return;

			x = (x / length) * (length - TargetLength);
			var dv = End2.Velocity - End1.Velocity;
			var force = Stiffness * x - dv * Damping;

			End1.ApplyForce(-force);
			End2.ApplyForce(force);
		}
	}

	const float stiffness = 0.28f;
	const float damping = 0.06f;

	Spring[] springs;
	PointMass[,] points;

	public Grid(Rectangle size, Vector2 spacing)
	{
		var springList = new List<Spring>();

		int numColumns = (int)(size.Width / spacing.X) + 1;
		int numRows = (int)(size.Height / spacing.Y) + 1;
		points = new PointMass[numColumns, numRows];

		PointMass[,] fixedPoints = new PointMass[numColumns, numRows];

		int column = 0, row = 0;
		for (float iy = size.Top; iy <= size.Bottom; iy += spacing.Y)
		{
			for (float ix = size.Left; ix <= size.Right; ix += spacing.X)
			{
				points[column, row] = new PointMass(new Vector3(ix, iy, 0), 1);
				fixedPoints[column, row] = new PointMass(new Vector3(ix, iy, 0), 0);
				column++;
			}
			row++;
			column = 0;
		}

		for (int iy = 0; iy < numRows; iy++)
		{
			for (int ix = 0; ix < numColumns; ix++)
			{
				if (ix == 0 || iy == 0 || ix == numColumns - 1 || iy == numRows - 1)
					springList.Add(new global::Grid.Spring(fixedPoints[ix, iy], points[ix, iy], 0.1f, 0.1f));
				else if (ix % 3 == 0 && iy % 3 == 0)
					springList.Add(new Spring(fixedPoints[ix, iy], points[ix, iy], 0.002f, 0.02f));

				if (ix > 0)
					springList.Add(new Spring(points[ix - 1, iy], points[ix, iy], stiffness, damping));
				if (iy > 0)
					springList.Add(new Spring(points[ix, iy - 1], points[ix, iy], stiffness, damping));
			}
		}

		springs = springList.ToArray();
	}

	public void Update()
	{
		foreach (var spring in springs)
			spring.Update();

		foreach (var mass in points)
			mass.Update();
	}

	public void ApplyDirectedForce(Vector3 force, Vector3 position, float radius)
	{
		foreach (var mass in points)
		{
			if (position.DistanceSquared(mass.Position) < radius * radius)
				mass.ApplyForce(10 * force / (10 + position.Distance(mass.Position)));
		}
	}

	public void ApplyImplosiveForce(float force, Vector3 position, float radius)
	{
		foreach (var mass in points)
		{
			float dist2 = position.DistanceSquared(mass.Position);

			if (dist2 < radius * radius)
			{
				mass.ApplyForce(10 * force * (position - mass.Position) / (100 + dist2));
				mass.IncreaseDamping(0.6f);
			}
		}
	}

	public void ApplyExplosiveForce(float force, Vector3 position, float radius)
	{
		foreach (var mass in points)
		{
			float dist2 = position.Distance(mass.Position);

			if (dist2 < radius * radius)
			{
				mass.ApplyForce(100 * force * (mass.Position - position) / (10000 + dist2));
				mass.IncreaseDamping(0.6f);
			}
		}
	}

	private Vector2 ToVec2(Vector3 v)
	{
		float factor = (v.Z + 2000) / 2000;
		return (new Vector2(v.X, v.Y) - GameRoot.ScreenSize / 2f) * factor + GameRoot.ScreenSize / 2;
	}

	public void Draw(SpriteBatch spriteBatch)
	{
		int width = points.GetLength(0);
		int height = points.GetLength(1);
		Color color = new Color(30, 30, 139, 85);

		for (int iy = 1; iy < height; iy++)
		{
			for (int ix = 1; ix < width; ix++)
			{
				Vector2 left = new Vector2();
				Vector2 up = new Vector2();
				Vector2 p = ToVec2(points[ix, iy].Position);

				if (ix > 1)
				{
					left = ToVec2(points[ix - 1, iy].Position);
					float thickness = iy % 3 == 1 ? 3f : 1f;

					int clampedX = Math.Min(ix + 1, width - 1);
					Vector2 mid = Vector2.CatmullRom(ToVec2(points[ix - 2, iy].Position), left, p, ToVec2(points[clampedX, iy].Position), 0.5f);

					if (mid.DistanceSquared((left + p) / 2) > 1)
					{
						spriteBatch.DrawLine(left, mid, color, thickness);
						spriteBatch.DrawLine(mid, p, color, thickness);
					}
					else
						spriteBatch.DrawLine(left, p, color, thickness);
				}

				if (iy > 1)
				{
					up = ToVec2(points[ix, iy - 1].Position);
					float thickness = ix % 3 == 1 ? 3f : 1f;

					int clampedY = Math.Min(iy + 1, height - 1);
					Vector2 mid = Vector2.CatmullRom(ToVec2(points[ix, iy - 2].Position), up, p, ToVec2(points[ix, clampedY].Position), 0.5f);

					if (mid.DistanceSquared((up + p) / 2) > 1)
					{
						spriteBatch.DrawLine(up, mid, color, thickness);
						spriteBatch.DrawLine(mid, p, color, thickness);
					}
					else
						spriteBatch.DrawLine(up, p, color, thickness);
				}

				if (ix > 1 && iy > 1)
				{
					Vector2 upLeft = ToVec2(points[ix - 1, iy - 1].Position);
					spriteBatch.DrawLine(0.5f * (upLeft + up), 0.5f * (left + p), color, 1f);
					spriteBatch.DrawLine(0.5f * (upLeft + left), 0.5f * (up + p), color, 1f);
				}
			}
		}
	}
}
