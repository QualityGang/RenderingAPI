using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

class PlayerShip : Entity
{
	const float speed = 8;

	const int cooldownFrames = 6;
	int cooldownRemaining = 0;

	int framesUntilRespawn = 0;
	public bool IsDead { get { return framesUntilRespawn > 0; } }

	static Random rand = new Random();

    private static PlayerShip instance;
    public static PlayerShip Instance
    {
        get
        {
            if (instance == null)
                instance = new PlayerShip();

            return instance;
        }
    }

    private PlayerShip()
    {
        image = Art.Player.GetTexture2D();
        Position = GameRoot.ScreenSize / 2;
        Radius = 10;
    }

    public override void Update()
    {
		if (IsDead)
		{
			if (--framesUntilRespawn == 0)
				GameRoot.Grid.ApplyDirectedForce(new Vector3(0, 0, 5000), new Vector3(Position, 0), 50);

			return;
		}

		Velocity = speed * Input.GetMovementDirection();
		Position += Velocity;
		Position.Clamp(Size / 2, GameRoot.ScreenSize - Size / 2);

		if (Input.GetAimDirection().LengthSquared() > 0)
			Orientation = (float)(Input.GetAimDirection().ToAngle() * (180 / Math.PI));

		var aim = Input.GetAimDirection();

		if (aim.LengthSquared() > 0 && cooldownRemaining <= 0)
		{
			cooldownRemaining = cooldownFrames;
			float aimAngle = aim.ToAngle();
			Quaternion aimQuat = Quaternion.CreateFromYawPitchRoll(0, 0, aimAngle);
			
			float randomSpread = rand.NextFloat(-0.04f, 0.04f) + rand.NextFloat(-0.04f, 0.04f);
			Vector2 vel = MathUtils.FromPolar(aimAngle + randomSpread, 11.0f);

			Vector2 offset = new Vector2(25, -8);
			offset.Transform(aimQuat);
			EntityManager.Add(new Bullet(Position + offset, vel));

			offset = new Vector2(25, 8);
			offset.Transform(aimQuat);
			EntityManager.Add(new Bullet(Position + offset, vel));
		}

		if (cooldownRemaining > 0)
			cooldownRemaining--;
    }

	public void Kill()
	{
		framesUntilRespawn = 120;
		Position = GameRoot.ScreenSize / 2;
		EnemySpawner.Reset();

		Color yellow = new Color(0.8f, 0.8f, 0.4f, 1.0f);

		for (int i = 0; i < 1200; i++)
		{
			float speed = 18f * (1f - 1 / rand.NextFloat(1f, 10f));
			Color color = Color.Lerp(Color.White, yellow, rand.NextFloat(0, 1));
			var state = new ParticleState()
			{
				Velocity = rand.NextVector2(speed, speed),
				Type = ParticleType.None,
				LengthMultiplier = 1
			};

			GameRoot.ParticleManager.CreateParticle(Art.LineParticle.GetTexture2D(), Position, color, 190, new Vector2(1.5f), state);
		}
	}

	public override void Draw(SpriteBatch spriteBatch)
	{
		if (!IsDead)
			base.Draw(spriteBatch);
	}
}
