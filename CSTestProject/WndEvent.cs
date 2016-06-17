using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

class WndEvent
{
    private IntPtr nativeEvent;

    public WndEvent()
    {
        nativeEvent = WndEvent_Create();
    }

    ~WndEvent()
    {
        WndEvent_Release(nativeEvent);
    }

    public bool IsQuitEvent()
    {
        return WndEvent_IsQuitEvent(nativeEvent);
    }

    internal IntPtr GetNativePtr()
    {
        return nativeEvent;
    }

    [DllImport("RenderingAPI.dll")]
    private static extern IntPtr WndEvent_Create();

    [DllImport("RenderingAPI.dll")]
    private static extern void WndEvent_Release(IntPtr e);

    [DllImport("RenderingAPI.dll")]
    [return:MarshalAs(UnmanagedType.I1)]
    private static extern bool WndEvent_IsQuitEvent(IntPtr e);
}