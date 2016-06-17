using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

class Program
{
    static void Main(string[] args)
    {
        GraphicsDevice device = new GraphicsDevice(RenderingAPI.Direct3D11);
        IntPtr context = device.GetContext();

        Window window = new Window(context, "Title", 800, 600);

        SpriteBatch batch = new SpriteBatch(context);

        OrthographicCamera camera = new OrthographicCamera(-400, -300, 800, 600);
        camera.Update();

        Bitmap bmp = new Bitmap(context, "box.png");

        Sprite sprite = new Sprite();
        sprite.SetPosition(100, 100);
        sprite.SetSize(400, 400);
        sprite.SetSrcRect(0, 0, 70, 70);
        sprite.SetColor(Color.Tan);
        sprite.SetTexture(bmp.GetTexture2D());

        WndEvent e = new WndEvent();

        while (!e.IsQuitEvent())
        {
            if (window.GetNextEvent(e))
            {
                window.ProcessEvent(e);
            }
            else
            {
                window.Clear(Color.Red);

                batch.Begin(window, camera);
                batch.Draw(sprite);
                batch.End();

                window.SwapBuffers();
            }
        }
    }
}