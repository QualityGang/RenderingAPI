
#include <iostream>

#if _DEBUG
#include <chrono>
#endif

#include <GraphicsDevice.h>
#include <Window.h>
#include <Bitmap.h>
#include <PrimitiveBatch.h>
#include <SpriteBatch.h>
#include <OrthographicCamera.h>
#include <RenderTexture.h>
#include <ShadowMap.h>
#include <Bloom.h>

#include <DirectXMath.h>

#include <Throw.h>

#pragma comment(lib, "RenderingAPI.lib")

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	GraphicsDevice device(RenderingAPI_Direct3D11);
	GraphicsContext *context = device.getContext();

	Window window(context, "Title", 800, 600);
	//window.setVSyncEnabled(false);

	Bitmap bmp(context, "sunset.png");
	Bitmap bmp2(context, "box.png");

	Sprite sprite;
	sprite.setSize((float)window.getSize().x, (float)window.getSize().y);
	sprite.setSrcRect(FloatRect(0, 0, (float)bmp.getWidth(), (float)bmp.getHeight()));
	sprite.setTexture(bmp.getTexture2D());

	Sprite sprite2;
	sprite2.setPosition(200, 200);
	sprite2.setSize(80, 80);
	sprite2.setSrcRect(FloatRect(0, 0, (float)bmp2.getWidth(), (float)bmp2.getHeight()));
	sprite2.setTexture(bmp2.getTexture2D());

	Sprite sprite3;
	sprite3.setPosition(500, 400);
	sprite3.setSize(80, 80);
	sprite3.setSrcRect(FloatRect(0, 0, (float)bmp2.getWidth(), (float)bmp2.getHeight()));
	sprite3.setTexture(bmp2.getTexture2D());

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
#if _DEBUG
			auto t1 = std::chrono::high_resolution_clock::now();
#endif
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

			window.clear(Color(255, 255, 0, 255));

			static float xy = 0;
			xy += 0.5f;

			sprite2.setPosition(xy, xy);

			batch.begin(window.getRenderTarget(), SpriteSortMode_Deferred, &camera);
			//batch.draw(sprite);
			batch.draw(sprite2);
			batch.draw(sprite3);
			batch.end();

			shadowMap.draw(batch);
			//bloom.apply(batch);

			window.swapBuffers();
#if _DEBUG
			auto t2 = std::chrono::high_resolution_clock::now();
			auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

			OutputDebugString(std::to_string(1000.0 / dt).c_str());
			OutputDebugString("\n");
#endif
		}
	}

	return e.quit.exitCode;
}