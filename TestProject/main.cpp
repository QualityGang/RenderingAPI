
#include <sstream>

#include <GraphicsDevice.h>
#include <Window.h>
#include <Bitmap.h>
#include <PrimitiveBatch.h>
#include <SpriteBatch.h>
#include <OrthographicCamera.h>
#include <RenderTexture.h>
#include <ShadowMap.h>
#include <Bloom.h>
#include <DebugConsole.h>

#include <DirectXMath.h>

#include <Throw.h>

#pragma comment(lib, "RenderingAPI.lib")

Sprite sprite2;

void Print(const char *args)
{
	OutputDebugString("Print: ");
	OutputDebugString(args);
	OutputDebugString("\n");
}
ConsoleCommand(Print, print);

void MoveBox(const char *args)
{
	std::stringstream ss(args);
	float x, y;
	ss >> x >> y;
	sprite2.setPosition(x, y);
}
ConsoleCommand(MoveBox, move_box);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	GraphicsDevice device(RenderingAPI_Direct3D11);
	GraphicsContext *context = device.getContext();

	Window window(context, "Title", 800, 600);
	//window.setVSyncEnabled(false);

	Bitmap bmp(context, "sunset.png");
	Bitmap bmp2(context, "dtest.png");

	Font font("arial.ttf", 40);
	FontAtlas atlas(context, FA_ALLLOWERCASE FA_ALLUPPERCASE FA_ALLNUMBERS "_", font);

	uint8_t pixels[] = { 0, 0, 0, 255 };
	Bitmap bmp3(context, 1, 1, pixels);

	Sprite sprite;
	sprite.setSize((float)window.getSize().x, (float)window.getSize().y);
	sprite.setSrcRect(FloatRect(0, 0, (float)bmp.getWidth(), (float)bmp.getHeight()));
	sprite.setTexture(bmp.getTexture2D());

	Sprite sprite2;
	sprite2.setSize((float)window.getSize().x, (float)window.getSize().y);
	sprite2.setSrcRect(FloatRect(0, 0, (float)bmp2.getWidth(), (float)bmp2.getHeight()));
	sprite2.setTexture(bmp2.getTexture2D());

	BloomSettings settings;
	settings.bloomThreshold = 0.25f;
	settings.blurAmount = 4.0f;
	settings.bloomIntensity = 2.0f;
	settings.baseIntensity = 1.0f;
	settings.bloomSaturation = 1.5f;
	settings.baseSaturation = 1.0f;

	ShadowMap shadowMap(context, ShadowMapSize::Size512);
	shadowMap.setRenderTarget(window.getRenderTarget());

	Bloom bloom(context);
	bloom.setSettings(settings);
	bloom.setRenderTarget(window.getRenderTarget());
	//bloom.setShowFilter(Bloom::BlurredBothWays);

	SpriteBatch batch(context);

	OrthographicCamera camera(window.getSize().x / 2.0f, window.getSize().y / 2.0f, (float)window.getSize().x, (float)window.getSize().y);

	WndEvent e;

	while (e.type != WndEvent::Quit)
	{
		if (window.getNextEvent(&e))
		{
			window.processEvent(e);
		}
		else
		{
			static uint32_t prevWidth = window.getSize().x;
			static uint32_t prevHeight = window.getSize().y;

			if (prevWidth != window.getSize().x || prevHeight != window.getSize().y)
			{
				prevWidth = window.getSize().x;
				prevHeight = window.getSize().y;

				camera.setViewport((float)prevWidth, (float)prevHeight);
				shadowMap.setRenderTarget(window.getRenderTarget());
				bloom.setRenderTarget(window.getRenderTarget());
			}

			if (window.isKeyPressed(Key::W))
				camera.setPosition(camera.getX(), camera.getY() - 1);
			if (window.isKeyPressed(Key::S))
				camera.setPosition(camera.getX(), camera.getY() + 1);
			if (window.isKeyPressed(Key::A))
				camera.setPosition(camera.getX() - 1, camera.getY());
			if (window.isKeyPressed(Key::D))
				camera.setPosition(camera.getX() + 1, camera.getY());

			camera.update();

			Color bg = Color(100, 255, 0, 255);
			window.clear(bg);

			batch.begin(window.getRenderTarget(), SpriteSortMode_Deferred, &camera);
			batch.draw(sprite2);
			batch.end();

			shadowMap.draw(batch,bg);
			//bloom.apply(batch);

			DebugConsole::Draw(context, window, batch, bmp3.getTexture2D(), atlas);

			window.swapBuffers();
		}
	}

	return e.quit.exitCode;
}