#include "Walnut/ApplicationGUI.h"
#include "Walnut/EntryPoint.h"

#include "ClientLayer.h"
#include "Embed/Icons.embed"
#include "Embed/IcoMoon.embed"

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Black Ops 3 Practice Tool - Frontend Data: Gobblegum Loadouts";
	spec.IconData = g_AppIcon;
	spec.FontIcon = { (void*)g_IcoMoonTTF, sizeof(g_IcoMoonTTF), 16.0f, 0xE900, 0xEAEA };
	spec.CustomTitlebar = true;
	spec.CenterWindow = true;

	Walnut::Application* app = new Walnut::Application(spec);
	std::shared_ptr<ClientLayer> clientLayer = std::make_shared<ClientLayer>();
	app->PushLayer(clientLayer);
	app->SetMenubarCallback(std::function<void()>(ClientLayer::MenuBar));
	return app;
}