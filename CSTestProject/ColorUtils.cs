using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

static class ColorUtils
{
	public static Color HSVToColor(float h, float s, float v)
	{
		if (h == 0 && s == 0)
			return new Color(v, v, v, 1.0f);

		float c = s * v;
		float x = c * (1 - Math.Abs(h % 2 - 1));
		float m = v - c;

		if (h < 1) return new Color(c + m, x + m, m, 1.0f);
		else if (h < 2) return new Color(x + m, c + m, m, 1.0f);
		else if (h < 3) return new Color(m, c + m, x + m, 1.0f);
		else if (h < 4) return new Color(m, x + m, c + m, 1.0f);
		else if (h < 5) return new Color(x + m, m, c + m, 1.0f);
		else return new Color(c + m, m, x + m, 1.0f);
	}
}
