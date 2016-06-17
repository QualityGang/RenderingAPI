using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

class Bitmap
{
    private IntPtr nativeBitmap;

    public Bitmap(IntPtr context, string fileName)
    {
        nativeBitmap = Bitmap_Create(context, fileName);
    }

    ~Bitmap()
    {
        Bitmap_Release(nativeBitmap);
    }

    public IntPtr GetTexture2D()
    {
        return Bitmap_GetTexture2D(nativeBitmap);
    }

    [DllImport("RenderingAPI.dll")]
    private static extern IntPtr Bitmap_Create(IntPtr context, [MarshalAs(UnmanagedType.LPStr)] string fileName);

    [DllImport("RenderingAPI.dll")]
    private static extern void Bitmap_Release(IntPtr bmp);

    [DllImport("RenderingAPI.dll")]
    private static extern IntPtr Bitmap_GetTexture2D(IntPtr bmp);
}