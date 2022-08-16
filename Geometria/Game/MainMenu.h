#pragma once
#ifndef MAINMENU_H
#define MAINMENU_H
#include "geometria.h"
#include "Graphics/Externals/DragBorderlessWindow.h"
#include "Game/Scripts/HubManager.h"

struct MainMenu
{
	static void Run()
	{
		Graphics::SetResolution(Vector2(1280, 720));
		Graphics::SetBorderless(true);
		Graphics::EnableDraggableBorderless();

		DrawCall* d = SceneManager::MainScene().CreateDrawCall();
		d->sort = DrawCall::AtStartup;
		d->type = DrawCall::UI;
		d->Close();

		ImGUIElement* menu = GUIML::NewGUIML("Game/UI/menu.guiml");
		TextureManager::UploadToGPU();
		RendererCore::AddImGUIElement(*menu, d->Target());

		Empty* hubManager = new Empty();
		HubManager* h = hubManager->AddScript<HubManager>();
		h->win = menu;
	}
};
#endif