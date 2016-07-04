using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

class Enemy : Entity
{
	private int timeUntilStart = 60;
	public bool IsActive { get { return timeUntilStart <= 0; } }

	private List<IEnumerator<int>> behaviours = new List<IEnumerator<int>>();

	Random rand = new Random();

	public Enemy(IntPtr image, Vector2 position)
	{
		this.image = image;
		Position = position;
		color = new Color(0);

		TextureSize imageSize = GameRoot.Context.GetTexture2DSize(image);
		Radius = (imageSize.Width + imageSize.Height) / 4.0f;
	}

	public static Enemy CreateSeeker(Vector2 position)
	{
		Enemy enemy = new Enemy(Art.Seeker.GetTexture2D(), position);
		enemy.AddBehaviour(enemy.FollowPlayer());

		return enemy;
	}

	public static Enemy CreateWanderer(Vector2 position)
	{
		Enemy enemy = new Enemy(Art.Wanderer.GetTexture2D(), position);
		enemy.AddBehaviour(enemy.MoveRandomly());
		return enemy;
	}

	public override void Update()
	{
		if (timeUntilStart <= 0)
		{
			ApplyBehaviours();
		}
		else
		{
			timeUntilStart--;
			color = Color.White * (1 - timeUntilStart / 60f);
		}

		Position += Velocity;
		Position.Clamp(Size / 2, GameRoot.ScreenSize - Size / 2);

		Velocity *= 0.8f;
	}

	public void HandleCollision(Enemy other)
	{
		Vector2 d = Position - other.Position;
		Velocity += 10 * d / (d.LengthSquared() + 1);
	}

	public void Hit()
	{
		IsExpired = true;

		float hue1 = rand.NextFloat(0, 6);
		float hue2 = (hue1 + rand.NextFloat(0, 2)) % 6f;
		Color color1 = ColorUtils.HSVToColor(hue1, 0.5f, 1);
		Color color2 = ColorUtils.HSVToColor(hue2, 0.5f, 1);

		for (int i = 0; i < 120; i++)
		{
			float speed = 18f * (1f - 1 / rand.NextFloat(1f, 10f));
			var state = new ParticleState();
			state.Velocity = rand.NextVector2(speed, speed);
			state.Type = ParticleType.Enemy;
			state.LengthMultiplier = 1f;

			Color color = Color.Lerp(color1, color2, rand.NextFloat(0, 1));
			GameRoot.ParticleManager.CreateParticle(Art.LineParticle.GetTexture2D(), Position, color, 190, new Vector2(1.5f, 1.5f), state);
		}
	}

	private void AddBehaviour(IEnumerable<int> behaviour)
	{
		behaviours.Add(behaviour.GetEnumerator());
	}

	private void ApplyBehaviours()
	{
		for (int i = 0; i < behaviours.Count; i++)
		{
			if (!behaviours[i].MoveNext())
				behaviours.RemoveAt(i--);
		}
	}

	IEnumerable<int> FollowPlayer(float acceleration = 1f)
	{
		while (true)
		{
			Velocity += (PlayerShip.Instance.Position - Position).ScaleTo(acceleration);

			if (Velocity != Vector2.Zero)
				Orientation = (float)(Velocity.ToAngle() * (180 / Math.PI));

			yield return 0;
		}
	}

	IEnumerable<int> MoveRandomly()
	{
		float direction = rand.NextFloat(0, (float)(Math.PI * 2));

		while (true)
		{
			direction += rand.NextFloat(-0.1f, 0.1f);
			direction = MathUtils.WrapAngle(direction);

			for (int i = 0; i < 6; i++)
			{
				Velocity += MathUtils.FromPolar(direction, 0.4f);
				Orientation -= 2.8f;

				TextureSize imageSize = GameRoot.Context.GetTexture2DSize(image);
				Rectangle screenBounds = new Rectangle(0, 0, GameRoot.ScreenSize.X, GameRoot.ScreenSize.Y);
				screenBounds.Inflate(new Vector2(-imageSize.Width, -imageSize.Height));

				if (!screenBounds.Overlaps(Position))
					direction = (GameRoot.ScreenSize / 2 - Position).ToAngle() + rand.NextFloat((float)(-Math.PI / 2), (float)(Math.PI / 2));

				yield return 0;
			}
		}
	}
}
