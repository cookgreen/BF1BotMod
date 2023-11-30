#include "includes.h"

std::unique_ptr<BFConsole> m_pConsole = std::make_unique<BFConsole>();

#define WindowName "Battlefield Console"

void BFConsole::Run()
{
	ImGui::Begin(WindowName, FALSE, 0);
}