#pragma once
#include "Walnut/Layer.h"

class ClientLayer : public Walnut::Layer
{
public:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUIRender() override;

	static void MenuBar();
private:
	bool m_AboutPopupOpen = false;
};