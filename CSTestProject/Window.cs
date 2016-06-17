using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

class Window
{
    private IntPtr nativeWindow;

    public Window(IntPtr context, string title, int width, int height)
    {
        nativeWindow = Window_Create(context, title, width, height);
    }

    ~Window()
    {
        Window_Release(nativeWindow);
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

    internal IntPtr GetRenderTarget()
    {
        return Window_GetRenderTarget(nativeWindow);
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
    private static extern IntPtr Window_GetRenderTarget(IntPtr window);
}
