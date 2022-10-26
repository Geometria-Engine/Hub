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
		Graphics::SetResolution(Vector2(640, 480));
		//Graphics::SetBorderless(true);
		//Graphics::EnableDraggableBorderless();

		DrawCall* d = SceneManager::MainScene().CreateDrawCall();
		d->sort = DrawCall::AtStartup;
		d->type = DrawCall::UI;
		d->Close();

		Texture* t = new Texture("Game/Textures/main-background-with-blur.png");

		Texture* logo = new Texture("Game/Textures/logo2.png");

		//ImGUIElement* menu = GUIML::NewGUIML("Game/UI/menu.guiml");
		TextureManager::UploadToGPU();
		//RendererCore::AddImGUIElement(*menu, d->Target());

		iWindow* win = new iWindow("Main Window", iWindow::Mode::Canvas);

		win->GetTransform().scale = Vector2(0, 0);
		win->Style()->ScreenScale() = Vector2(100, 100);

		win->Style()->ScreenPosition() = Vector2(0.5, 0.5);
		win->Style()->BackgroundImage() = t;

		win->BeginTree
		(
			iValue* v = new iValue(*win, "Screen Scale X", &win->Style()->ScreenScale().x);
			iValue* v2 = new iValue(*win, "Screen Scale Y", &win->Style()->ScreenScale().y);
		
			iWindow* secWin = new iWindow(*win, "Second Window", iWindow::Mode::Canvas);
			
			secWin->Style()->ScreenPosition() = Vector2(0.5, 0.5);
			secWin->Style()->ScreenScale() = Vector2(50, 50);
		
			secWin->BeginTree
			(
				iText* text = new iText(*secWin, StringAPI::LoremIpsum::Original());
				text->Style()->Font()->Wrap() = iFont::WordWrap::BreakSpace;
				text->Style()->Font()->Size() = 60;
		
				iValue* textSize = new iValue(*secWin, "Change Size", &text->Style()->Font()->Size());
				iValue* alignText = new iValue(*secWin, "Align", &text->Style()->Font()->Align());
			)
		)

		RendererCore::AddIGUI(*win, d->Target());

		//Empty* hubManager = new Empty();
		//HubManager* h = hubManager->AddScript<HubManager>();
		//h->win = menu;
	}
};
#endif
