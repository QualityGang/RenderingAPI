
#include <GraphicsDevice.h>
#include <Window.h>
#include <SpriteBatch.h>
#include <OrthographicCamera.h>

#include "Chip8.h"

#pragma comment(lib, "RenderingAPI.lib")

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	GraphicsDevice device(RenderingAPI_Direct3D11);
	GraphicsContext *context = device.getContext();

	Window window(context, "Title", 800, 600);
	//window.setVSyncEnabled(false);

	SpriteBatch batch(context);

	Bitmap whitePixel(context, 1, 1, nullptr);
	whitePixel.setPixel(0, Color(255, 255, 255, 255));

	OrthographicCamera camera(64, 32, false);
	camera.setPosition(-32, -16);
	camera.update();

	Chip8 chip;
	chip.load("pong2.c8");

	Sprite sprite;
	sprite.setSize(1, 1);
	sprite.setSrcRect(FloatRect(0, 0, (float)whitePixel.getWidth(), (float)whitePixel.getHeight()));
	sprite.setTexture(whitePixel.getTexture2D());

	WndEvent e;

	while (e.type != WndEvent::Quit)
	{
		if (window.getNextEvent(&e))
		{
			window.processEvent(e);
		}
		else
		{
			chip.setKeyPressed(0x1, window.isKeyPressed(Key::D1));
			chip.setKeyPressed(0x2, window.isKeyPressed(Key::D2));
			chip.setKeyPressed(0x3, window.isKeyPressed(Key::D3));
			chip.setKeyPressed(0xC, window.isKeyPressed(Key::D4));

			chip.setKeyPressed(0x4, window.isKeyPressed(Key::Q));
			chip.setKeyPressed(0x5, window.isKeyPressed(Key::W));
			chip.setKeyPressed(0x6, window.isKeyPressed(Key::E));
			chip.setKeyPressed(0xD, window.isKeyPressed(Key::R));

			chip.setKeyPressed(0x7, window.isKeyPressed(Key::A));
			chip.setKeyPressed(0x8, window.isKeyPressed(Key::S));
			chip.setKeyPressed(0x9, window.isKeyPressed(Key::D));
			chip.setKeyPressed(0xE, window.isKeyPressed(Key::F));

			chip.setKeyPressed(0xA, window.isKeyPressed(Key::Z));
			chip.setKeyPressed(0x0, window.isKeyPressed(Key::X));
			chip.setKeyPressed(0xB, window.isKeyPressed(Key::C));
			chip.setKeyPressed(0xF, window.isKeyPressed(Key::V));

			chip.cycle();

			window.clear(Color(0, 0, 0, 255));

			char gfx[64 * 32];
			chip.getDisplay(gfx);

			batch.begin(window.getRenderTarget(), camera);

			for (int iy = 0; iy < 32; iy++)
			{
				for (int ix = 0; ix < 64; ix++)
				{
					sprite.setPosition((float)ix, (float)iy);

					if (gfx[iy * 64 + ix] != 0)
						batch.draw(sprite);
				}
			}

			batch.end();

			window.swapBuffers();
		}
	}

	return 0;
}