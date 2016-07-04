using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

static class Input
{
    public static Vector2 MousePosition = new Vector2();

    public static void Update()
    {
        MousePosition = GameRoot.Window.GetMousePosition();
    }

    public static bool IsKeyPressed(Key key)
    {
        return GameRoot.Window.IsKeyPressed(key);
    }

    public static Vector2 GetMovementDirection()
    {
        Vector2 direction = new Vector2();

        if (IsKeyPressed(Key.A))
            direction.X -= 1;
        if (IsKeyPressed(Key.D))
            direction.X += 1;
        if (IsKeyPressed(Key.W))
            direction.Y -= 1;
        if (IsKeyPressed(Key.S))
            direction.Y += 1;

        if (direction.LengthSquared() > 1)
            direction.Normalize();

        return direction;
    }

    public static Vector2 GetAimDirection()
    {
		Vector2 direction = MousePosition - PlayerShip.Instance.Position;

		if (direction == Vector2.Zero)
			return direction;

		direction.Normalize();
		return direction;
    }
}
