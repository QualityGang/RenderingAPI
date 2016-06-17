
#include <iostream>
#include <chrono>
#include <vld.h>

#include <GraphicsDevice.h>
#include <Window.h>
#include <Bitmap.h>
#include <SpriteBatch.h>
#include <OrthographicCamera.h>
#include <RenderTexture.h>

#include <DirectXMath.h>

#include <Throw.h>

#pragma comment(lib, "RenderingAPI.lib")

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	GraphicsDevice device(RenderingAPI_Direct3D11);
	GraphicsContext *context = device.getContext();

	Window window(context, "Title", 800, 600);
	//window.setVSyncEnabled(false);

	Bitmap bmp(context, "box.png");

	Sprite sprite;
	sprite.setSize(300, 300);
	sprite.setSrcRect(FloatRect(0, 0, (float)bmp.getWidth(), (float)bmp.getHeight()));
	sprite.setTexture(bmp.getTexture2D());

	Sprite sprite2;
	sprite2.setPosition(200, 400);
	sprite2.setSize(300, 300);
	sprite2.setSrcRect(FloatRect(0, 0, (float)bmp.getWidth(), (float)bmp.getHeight()));
	sprite2.setColor(Color(0, 200, 200, 255));
	sprite2.setTexture(bmp.getTexture2D());

	RenderTexture renTex(context, 600, 600);
	context->clearRenderTarget(renTex.getRenderTarget(), 0, Color(255, 255, 255, 255));

	Sprite sprite3;
	sprite3.setPosition(600, 100);
	sprite3.setSize(600, 600);
	sprite3.setSrcRect(FloatRect(0, 0, (float)600, (float)600));
	sprite3.setTexture(renTex.getTexture2D());

	Font font("arial.ttf", 48);
	FontAtlas atlas(context, FA_ALLLOWERCASE FA_ALLUPPERCASE FA_ALLNUMBERS "'!?", font);

	Text text;
	text.str = "Hey there!\nWhat's up?";
	text.position.x = 800;
	text.position.y = 800;
	text.position.z = 0;
	text.lineGap = 5;
	text.color.set(0, 0, 0, 255);

	OrthographicCamera camera((float)window.getSize().x, (float)window.getSize().y, false);
	camera.setPosition((float)window.getSize().x / -2.0f, (float)window.getSize().y / -2.0f);

	SpriteBatch batch(context);

	WndEvent e;

	while (e.type != WndEvent::Quit)
	{
		if (window.getNextEvent(&e))
		{
			window.processEvent(e);
		}
		else
		{
			static const float camZoomSpeed = 0.015f;
			static const float camMoveSpeed = 6.000f;
			static const float camRotSpeed  = 1.000f;
			
#if _DEBUG
			auto t1 = std::chrono::high_resolution_clock::now();
#endif

			if (window.isKeyPressed(Key::Q))
				camera.setZoom(camera.getZoom() - camZoomSpeed);

			if (window.isKeyPressed(Key::E))
				camera.setZoom(camera.getZoom() + camZoomSpeed);

			if (window.isKeyPressed(Key::W))
				camera.setPosition(camera.getX(), camera.getY() - camMoveSpeed);

			if (window.isKeyPressed(Key::S))
				camera.setPosition(camera.getX(), camera.getY() + camMoveSpeed);

			if (window.isKeyPressed(Key::A))
				camera.setPosition(camera.getX() - camMoveSpeed, camera.getY());

			if (window.isKeyPressed(Key::D))
				camera.setPosition(camera.getX() + camMoveSpeed, camera.getY());

			if (window.isKeyPressed(Key::R))
				camera.setAngle(camera.getAngle() + camRotSpeed);

			if (window.isKeyPressed(Key::T))
				camera.setAngle(camera.getAngle() - camRotSpeed);
			
			if (camera.getViewportWidth() != (float)window.getSize().x
				|| camera.getViewportHeight() != (float)window.getSize().y)
			{
				camera.setPosition(camera.getX() + (float)camera.getViewportWidth() / 2.0f, camera.getY() + (float)camera.getViewportHeight() / 2.0f);
				camera.setViewport((float)window.getSize().x, (float)window.getSize().y);
				camera.setPosition(camera.getX() - (float)window.getSize().x / 2.0f, camera.getY() - (float)window.getSize().y / 2.0f);
			}

			camera.update();

			POINT mp = window.getMousePosition();
			DirectX::XMFLOAT4 mouse((float)mp.x, (float)mp.y, 0.0f, 1.0f);
			camera.unproject(window, mouse);

			if (mouse.x > sprite.getX() && mouse.x < sprite.getX() + sprite.getWidth()
				&& mouse.y > sprite.getY() && mouse.y < sprite.getY() + sprite.getHeight())
			{
				sprite.setColor(Color(0, 255, 0, 255));
			}
			else
			{
				sprite.setColor(Color(255, 255, 255, 255));
			}

			window.clear(Color(255, 0, 0, 255));

			batch.begin(window.getRenderTarget(), camera);
			batch.draw(sprite);
			batch.draw(sprite2);
			batch.draw(sprite3);
			batch.draw(text, atlas);
			batch.end();

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