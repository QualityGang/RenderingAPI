using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

struct Color
{
    public static readonly Color Clear      = new Color(0, 0, 0, 0);
    public static readonly Color Black      = new Color(0, 0, 0, 255);
                                            
    public static readonly Color White      = new Color(0xffffffff);
    public static readonly Color LightGray  = new Color(0xbfbfbfff);
    public static readonly Color Gray       = new Color(0x7f7f7fff);
    public static readonly Color DarkGray   = new Color(0x3f3f3fff);
                  
    public static readonly Color Blue       = new Color(0, 0, 255, 255);
	public static readonly Color LightBlue  = new Color(0xffadd8e6);
    public static readonly Color Navy       = new Color(0, 0, 127, 255);
    public static readonly Color Royal      = new Color(0x4169e1ff);
    public static readonly Color Slate      = new Color(0x708090ff);
    public static readonly Color Sky        = new Color(0x87ceebff);
    public static readonly Color Cyan       = new Color(0, 255, 255, 255);
    public static readonly Color Teal       = new Color(0, 127, 127, 255);
                  
    public static readonly Color Green      = new Color(0x00ff00ff);
	public static readonly Color LightGreen = new Color(0xff90ee90);
    public static readonly Color Chartreuse = new Color(0x7fff00ff);
    public static readonly Color Lime       = new Color(0x32cd32ff);
    public static readonly Color Forest     = new Color(0x228b22ff);
    public static readonly Color Olive      = new Color(0x6b8e23ff);
                  
    public static readonly Color Yellow     = new Color(0xffff00ff);
    public static readonly Color Gold       = new Color(0xffd700ff);
    public static readonly Color Goldenrod  = new Color(0xdaa520ff);
    public static readonly Color Orange     = new Color(0xffa500ff);
                  
    public static readonly Color Brown      = new Color(0x8b4513ff);
    public static readonly Color Tan        = new Color(0xd2b48cff);
    public static readonly Color Firebrick  = new Color(0xb22222ff);
                  
    public static readonly Color Red        = new Color(0xff0000ff);
    public static readonly Color Scarlet    = new Color(0xff341cff);
    public static readonly Color Coral      = new Color(0xff7f50ff);
    public static readonly Color Salmon     = new Color(0xfa8072ff);
    public static readonly Color Pink       = new Color(0xff69b4ff);
    public static readonly Color Magenta    = new Color(255, 0, 255, 255);
                                           
    public static readonly Color Purple     = new Color(0xa020f0ff);
    public static readonly Color Violet     = new Color(0xee82eeff);
    public static readonly Color Maroon     = new Color(0xb03060ff);

    public byte R, G, B, A;

    public Color(byte r, byte g, byte b, byte a)
    {
        R = r;
        G = g;
        B = b;
        A = a;
    }

	public Color(float r, float g, float b, float a)
	{
		R = (byte)(r * 255);
		G = (byte)(g * 255);
		B = (byte)(b * 255);
		A = (byte)(a * 255);
	}

    public Color(uint rgba8888)
    {
        R = (byte)((rgba8888 & 0xFF000000) >> 24);
        G = (byte)((rgba8888 & 0x00FF0000) >> 16);
        B = (byte)((rgba8888 & 0x0000FF00) >>  8);
        A = (byte)((rgba8888 & 0x000000FF) >>  0);
    }

	public static Color Lerp(Color value1, Color value2, Single amount)
	{
		byte Red   = (byte)MathUtils.Lerp(value1.R, value2.R, amount);
		byte Green = (byte)MathUtils.Lerp(value1.G, value2.G, amount);
		byte Blue  = (byte)MathUtils.Lerp(value1.B, value2.B, amount);
		byte Alpha = (byte)MathUtils.Lerp(value1.A, value2.A, amount);

		return new Color(Red, Green, Blue, Alpha);
	}

	public static Color operator*(Color color, float scale)
	{
		byte r = (byte)(color.R * scale);
		byte g = (byte)(color.G * scale);
		byte b = (byte)(color.B * scale);
		byte a = (byte)(color.A * scale);

		return new global::Color(r, g, b, a);
	}
}
