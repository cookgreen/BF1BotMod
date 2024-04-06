#include "includes.h"

std::unique_ptr<BFConsole> m_pConsole = std::make_unique<BFConsole>();

#define WindowName "Battlefield Console"

void BFConsole::Run()
{
	//Create a bf1 console so that we can input some useful commands
	ImGui::Begin(WindowName, FALSE, 0);
	ImGui::End();
}