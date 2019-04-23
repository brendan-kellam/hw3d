#include "App.h"
#include "Box.h"
#include <memory>
#include <iostream>
#include <sstream>

App::App()
	:
	wnd( 800,600,"The Donkey Fart Box" )
{
	std::mt19937 rng( std::random_device{}() );
	std::uniform_real_distribution<float> adist(0.0f, DirectX::XM_PI * 2.0f);
	std::uniform_real_distribution<float> ddist( 0.0f, DirectX::XM_PI * 2.0f );
	std::uniform_real_distribution<float> odist( 0.0f, DirectX::XM_PI * 0.3f );
	std::uniform_real_distribution<float> rdist( 0.0f, DirectX::XM_PI*10);
	
	for( auto i = 0; i < 4; i++ )
	{
		boxes.push_back( std::make_unique<Box>(
			wnd.Gfx(),rng,adist,
			ddist,odist,rdist
		) );
	}
}

int App::Go()
{
	while( true )
	{
		// process all messages pending, but to not block for new messages
		if( const auto ecode = Window::ProcessMessages() )
		{
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}
		DoFrame();
	}
}

App::~App()
{}

void App::DoFrame()
{
	auto dt = timer.Mark();
	wnd.Gfx().ClearBuffer( 0.07f,0.0f,0.12f );

	std::ostringstream stream;
	stream << "FPS: " << wnd.Gfx().GetFPS();
	wnd.SetTitle(stream.str());

	HandleInput(dt);

	// Update the boxes
	for( auto& b : boxes )
	{
		b->Update( dt );
		b->Draw( wnd.Gfx() );
	}
	wnd.Gfx().EndFrame();
}

static int lastMouseX = 0, lastMouseY = 0;

void App::HandleInput(float dt)
{
	Camera& cam = wnd.Gfx().camera;

	// Handle mouse input
	auto mouseEvent = wnd.mouse.Read();
	if (mouseEvent.has_value()) 
	{
		Mouse::Event& ev = mouseEvent.value();
		if (ev.GetType() == Mouse::Event::Type::Move) 
		{
			int mouseX = ev.GetPosX();
			int mouseY = ev.GetPosY();

			if (ev.RightIsPressed()) 
			{
				float dPitch = static_cast<float>(mouseY - lastMouseY);
				float dYaw = static_cast<float>(mouseX - lastMouseX);
				cam.AdjustRotation(dPitch * 0.01f, dYaw * 0.01f, 0.0f);
			}

			lastMouseX = mouseX;
			lastMouseY = mouseY;
		}
	}

	using namespace DirectX;

	if (wnd.kbd.KeyIsPressed('W'))
	{
		cam.AdjustPosition(cam.GetForwardVector() * cam.GetTranslationSpeed() * dt);
	}
	if (wnd.kbd.KeyIsPressed('S'))
	{
		cam.AdjustPosition(cam.GetBackwardVector() * cam.GetTranslationSpeed() * dt);
	}
	if (wnd.kbd.KeyIsPressed('A'))
	{
		cam.AdjustPosition(cam.GetLeftVector() * cam.GetTranslationSpeed() * dt);
	}
	if (wnd.kbd.KeyIsPressed('D'))
	{
		cam.AdjustPosition(cam.GetRightVector() * cam.GetTranslationSpeed() * dt);
	}
	if (wnd.kbd.KeyIsPressed(VK_SPACE)) {
		cam.AdjustPosition(cam.GetUpwardVector() * cam.GetTranslationSpeed() * dt);
	}
	if (wnd.kbd.KeyIsPressed('Z')) {
		cam.AdjustPosition(cam.GetDownwardVector() * cam.GetTranslationSpeed() * dt);
	}

}
