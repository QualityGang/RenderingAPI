using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

class Window : IDisposable
{
    private IntPtr nativeWindow;

    public Window(GraphicsContext context, string title, int width, int height)
    {
        nativeWindow = Window_Create(context.GetNativePtr(), title, width, height);
    }

    ~Window()
    {
	}

    public void Clear(Color color)
    {
        Window_Clear(nativeWindow, color);
    }

    public void SwapBuffers()
    {
        Window_SwapBuffers(nativeWindow);
    }

    public bool GetNextEvent(WndEvent e)
    {
        return Window_GetNextEvent(nativeWindow, e.GetNativePtr());
    }

    public void ProcessEvent(WndEvent e)
    {
         Window_ProcessEvent(nativeWindow, e.GetNativePtr());
    }

    public Vector2 GetSize()
    {
        int width, height;
        Window_GetSize(nativeWindow, out width, out height);
        return new Vector2(width, height);
    }

    public Vector2 GetMousePosition()
    {
        int x, y;
        Window_GetMousePosition(nativeWindow, out x, out y);
        return new Vector2(x, y);
    }

    public IntPtr GetRenderTarget()
    {
        return Window_GetRenderTarget(nativeWindow);
    }

    public bool IsKeyPressed(Key key)
    {
        return Window_IsKeyPressed(nativeWindow, key);
    }

	public void Dispose()
	{
		Window_Release(nativeWindow);
		nativeWindow = IntPtr.Zero;
		GC.SuppressFinalize(this);
	}
    
    [DllImport("RenderingAPI.dll")]
    private static extern IntPtr Window_Create(IntPtr context, [MarshalAs(UnmanagedType.LPStr)] string title, int width, int height);

    [DllImport("RenderingAPI.dll")]
    private static extern void Window_Release(IntPtr window);

    [DllImport("RenderingAPI.dll")]
    private static extern void Window_Clear(IntPtr window, Color color);

    [DllImport("RenderingAPI.dll")]
    private static extern void Window_SwapBuffers(IntPtr window);

    [DllImport("RenderingAPI.dll")]
    private static extern bool Window_GetNextEvent(IntPtr window, IntPtr e);

    [DllImport("RenderingAPI.dll")]
    private static extern void Window_ProcessEvent(IntPtr window, IntPtr e);

    [DllImport("RenderingAPI.dll")]
    private static extern void Window_GetMousePosition(IntPtr window, out int x, out int y);

    [DllImport("RenderingAPI.dll")]
    private static extern void Window_GetSize(IntPtr window, out int width, out int height);

    [DllImport("RenderingAPI.dll")]
    private static extern IntPtr Window_GetRenderTarget(IntPtr window);

    [DllImport("RenderingAPI.dll")]
    [return: MarshalAs(UnmanagedType.I1)]
    private static extern bool Window_IsKeyPressed(IntPtr window, Key key);
}
