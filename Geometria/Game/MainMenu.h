#pragma once
#ifndef MAINMENU_H
#define MAINMENU_H
#include "geometria.h"
#include "Graphics/Externals/DragBorderlessWindow.h"
#include "Game/Scripts/HubManager.h"
#include "geometria/iGUI.h"

struct MainMenu
{
	static void Run()
	{
		Graphics::SetResolution(Vector2(1280, 720));
		//Graphics::SetBorderless(true);
		//Graphics::EnableDraggableBorderless();

		DrawCall* d = SceneManager::MainScene().CreateDrawCall();
		d->sort = DrawCall::AtStartup;
		d->type = DrawCall::UI;
		d->Close();

		Texture* t = new Texture("Game/Textures/main-background-with-blur.png");

		//ImGUIElement* menu = GUIML::NewGUIML("Game/UI/menu.guiml");
		TextureManager::UploadToGPU();
		//RendererCore::AddImGUIElement(*menu, d->Target());

		iWindow* win = new iWindow("Main Window", iWindow::Mode::Canvas);

		win->GetTransform().scale = Vector2(0, 0);
		win->Style()->Window()->ScreenScale() = Vector2(100, 100);

		win->Style()->Window()->ScreenPosition() = Vector2(0.5, 0.5);
		win->Style()->BackgroundImage() = t;

		iText* text = new iText(*win, "This is sample text!");
		text->Style()->Font("EngineResources/Fonts/Raleway-Italic.ttf");

		RendererCore::AddIGUI(*win, d->Target());

		//Empty* hubManager = new Empty();
		//HubManager* h = hubManager->AddScript<HubManager>();
		//h->win = menu;
	}
};
#endif
