#include "GUIWindow.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    GUIWindow::hInst = hInstance;
    GUIWindow::Run();

    return 0;
}