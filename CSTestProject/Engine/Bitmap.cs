using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

class Bitmap : IDisposable
{
    private IntPtr nativeBitmap;

	public Bitmap(GraphicsContext context, uint width, uint height)
	{
		nativeBitmap = Bitmap_Create(context.GetNativePtr(), width, height);
	}

    public Bitmap(GraphicsContext context, string fileName)
    {
        nativeBitmap = Bitmap_CreateFromFile(context.GetNativePtr(), fileName);
    }

    ~Bitmap()
    {
	}

	public void SetPixel(uint index, Color color)
	{
		Bitmap_SetPixel_I(nativeBitmap, index, color);
	}

	public void SetPixel(uint x, uint y, Color color)
	{
		Bitmap_SetPixel_XY(nativeBitmap, x, y, color);
	}

    public IntPtr GetTexture2D()
    {
        return Bitmap_GetTexture2D(nativeBitmap);
    }

	public void Dispose()
	{
		Bitmap_Release(nativeBitmap);
		nativeBitmap = IntPtr.Zero;
		GC.SuppressFinalize(this);
	}

	[DllImport("RenderingAPI.dll")]
	private static extern IntPtr Bitmap_Create(IntPtr context, uint width, uint height);

	[DllImport("RenderingAPI.dll")]
    private static extern IntPtr Bitmap_CreateFromFile(IntPtr context, [MarshalAs(UnmanagedType.LPStr)] string fileName);

    [DllImport("RenderingAPI.dll")]
    private static extern void Bitmap_Release(IntPtr bmp);

	[DllImport("RenderingAPI.dll")]
	private static extern void Bitmap_SetPixel_I(IntPtr bmp, uint index, Color color);

	[DllImport("RenderingAPI.dll")]
	private static extern void Bitmap_SetPixel_XY(IntPtr bmp, uint x, uint y, Color color);

	[DllImport("RenderingAPI.dll")]
    private static extern IntPtr Bitmap_GetTexture2D(IntPtr bmp);
}