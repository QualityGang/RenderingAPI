using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

static class EnemySpawner
{
	static Random rand = new Random();
	static float inverseSpawnChance = 60;
	static float inverseBlackHoleChance = 600;

	public static void Update()
	{
		if (!PlayerShip.Instance.IsDead && EntityManager.Count < 200)
		{
			if (rand.Next((int)inverseSpawnChance) == 0)
				EntityManager.Add(Enemy.CreateSeeker(GetSpawnPosition()));

			if (rand.Next((int)inverseSpawnChance) == 0)
				EntityManager.Add(Enemy.CreateWanderer(GetSpawnPosition()));

			if (EntityManager.BlackHoleCount < 2 && rand.Next((int)inverseBlackHoleChance) == 0)
				EntityManager.Add(new BlackHole(GetSpawnPosition()));
		}

		if (inverseSpawnChance > 20)
			inverseSpawnChance -= 0.005f;
	}

	private static Vector2 GetSpawnPosition()
	{
		Vector2 position;

		do
			position = new Vector2(rand.Next((int)GameRoot.ScreenSize.X), rand.Next((int)GameRoot.ScreenSize.Y));
		while (position.DistanceSquared(PlayerShip.Instance.Position) < 250 * 250);

		return position;
	}

	public static void Reset()
	{
		inverseSpawnChance = 60;
	}
}
