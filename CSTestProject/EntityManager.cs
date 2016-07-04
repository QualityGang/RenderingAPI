using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

static class EntityManager
{
    static List<Entity> entities = new List<Entity>();
	static List<Enemy> enemies = new List<Enemy>();
	static List<Bullet> bullets = new List<Bullet>();
	static List<BlackHole> blackHoles = new List<BlackHole>();

    static bool isUpdating;
    static List<Entity> addedEntities = new List<Entity>();

	public static List<BlackHole> BlackHoles { get { return blackHoles; } }

    public static int Count { get { return entities.Count; } }
	public static int BlackHoleCount { get { return blackHoles.Count; } }

	private static void AddEntity(Entity entity)
	{
		entities.Add(entity);

		if (entity is Bullet)
			bullets.Add(entity as Bullet);
		else if (entity is Enemy)
			enemies.Add(entity as Enemy);
		else if (entity is BlackHole)
			blackHoles.Add(entity as BlackHole);
	}

    public static void Add(Entity entity)
    {
		if (!isUpdating)
			AddEntity(entity);
		else
			addedEntities.Add(entity);
    }

	public static void Clear()
	{
		entities.Clear();
		enemies.Clear();
		bullets.Clear();
		blackHoles.Clear();
		addedEntities.Clear();
	}

    public static void Update()
    {
        isUpdating = true;

        foreach (var entity in entities)
            entity.Update();

		HandleCollisions();

        isUpdating = false;

		foreach (var entity in addedEntities)
			AddEntity(entity);

        addedEntities.Clear();

        entities = entities.Where(entity => !entity.IsExpired).ToList();
		bullets = bullets.Where(bullet => !bullet.IsExpired).ToList();
		enemies = enemies.Where(enemy => !enemy.IsExpired).ToList();
		blackHoles = blackHoles.Where(blackHole => !blackHole.IsExpired).ToList();
    }

	private static void HandleCollisions()
	{
		for (int i = 0; i < enemies.Count; i++)
		{
			for (int j = i + 1; j < enemies.Count; j++)
			{
				if (IsColliding(enemies[i], enemies[j]))
				{
					enemies[i].HandleCollision(enemies[j]);
					enemies[j].HandleCollision(enemies[i]);
				}
			}
		}

		for (int i = 0; i < enemies.Count; i++)
		{
			for (int j = 0; j < bullets.Count; j++)
			{
				if (IsColliding(enemies[i], bullets[j]))
				{
					enemies[i].Hit();
					bullets[j].IsExpired = true;
				}
			}
		}

		for (int i = 0; i < enemies.Count; i++)
		{
			if (enemies[i].IsActive && IsColliding(PlayerShip.Instance, enemies[i]))
			{
				PlayerShip.Instance.Kill();
				enemies.ForEach(enemy => enemy.Hit());
				blackHoles.ForEach(blackHole => blackHole.Kill());
				break;
			}
		}

		for (int i = 0; i < blackHoles.Count; i++)
		{
			for (int j = 0; j < enemies.Count; j++)
				if (enemies[j].IsActive && IsColliding(blackHoles[i], enemies[j]))
					enemies[j].Hit();

			for (int j = 0; j < bullets.Count; j++)
			{
				if (IsColliding(blackHoles[i], bullets[j]))
				{
					bullets[j].IsExpired = true;
					blackHoles[i].Hit();
				}
			}

			if (IsColliding(PlayerShip.Instance, blackHoles[i]))
			{
				PlayerShip.Instance.Kill();
				break;
			}
		}
	}

	private static bool IsColliding(Entity a, Entity b)
	{
		float radius = a.Radius + b.Radius;
		return !a.IsExpired && !b.IsExpired && a.Position.DistanceSquared(b.Position) < radius * radius;
	}

	public static IEnumerable<Entity> GetNearbyEntities(Vector2 position, float radius)
	{
		return entities.Where(entity => position.DistanceSquared(entity.Position) < radius * radius);
	}

    public static void Draw(SpriteBatch spriteBatch)
    {
        foreach (var entity in entities)
            entity.Draw(spriteBatch);
    }
}
