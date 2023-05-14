#include "GUIWindow.h"
#include "../Fonts/Icons.h"
#include "PlayerOptions.h"
#include "ZombieOptions.h"
#include "PowerupOptions.h"
#include "EggStepOptions.h"
#include "Craftables.h"
#include "Blockers.h"
#include "MapOptions.h"
#include "Resources.h"
#include "ImageHelp.h"
#include "Helper.h"
#include "Memhelp.h"
#include "KeyBinds.h"
#include "json.h"
#include "../Resource.h"

#include <string>
#include <iostream>
#include <fstream>
#include <functional>
#include <Psapi.h>
#include <unordered_set>
#define CURL_STATICLIB
#include <curl/curl.h>
#include <miniz/miniz.h>

#define SAMELINE ImGui::SameLine()

using namespace GUIWindow;
using namespace ImageHelp;
using namespace ZombieCalc;
using namespace SOECodeGuide;
using namespace GKValveSolver;
using namespace IceCodePractice;
using namespace KeyBinds;
using namespace JSON;
using namespace Autosplits;

// Forward function declarations
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void SearchForGame(); 
bool UpdateAvailable();
bool PerformUpdate();
bool BeginFrame();
bool RenderFrame();
bool EndFrame();

void GobblegumLoadoutPtr();
void AutosplitsPtr();
void PracticePatchesPtr();
void SettingsPtr();
void PlayerOptionsPtr();
void ZombieOptionsPtr();
void RoundOptionsPtr();
void PowerupOptionsPtr();
void EggStepOptionsPtr();
void CraftableOptionsPtr();
void BlockerOptionsPtr();
void MapOptionsPtr();
void ZombieCalculatorPtr();
void CodeGuidesPtr();
void GKValveSolverPtr();

void PresetSelectionFunc(int input);
void SwapSelectionFunc(int input);

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// JSON Settings
bool steamPathFound = false;

// Function pointer list for main UI content
std::vector<std::function<void()>> funcList;

// ImGUI data
ImVec4 clear_color = ImVec4(0.1f, 0.1f, 0.1f, 1.00f);
ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoDocking;
ImGuiWindowFlags dockFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoScrollbar;
ImGuiViewport* viewport;
ImFont* mainFont;
ImFont* sidebarFont;
ImFont* titleFont;

// Main data
static bool mainDocked = false;
static bool subDocked = false;
static bool updateAvailable = false;
static bool updateFailed = false;
static bool doUpdateAvailable = false;
static bool doUpdateFailed = false;
static bool injectResponse = false;
static bool injectResponseWait = false;
static int sidebarCurrentItem = 0;
static ImVec4 fakeColor = { 0, 0, 0, 0 };
static std::string internalVersion = "Beta-v0.2.0";

// Gum data
static int chooseGumPreset = 0;
static std::vector<int> classicGumList = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18 };
static std::vector<int> megaGumList = { 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34,
                                        35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
                                        51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62};
static std::vector<int> gumSearchList = { };
static bool showGumPresetExists = false;
static bool gumPopupBoolCheck = false;
static const char* gumSelectMenu = "";

// Practice patch data
static std::vector<std::string> zodPracticeList = { "None", "Soft Patch", "Infinite Beast", "Ovums Practice", "Flags Practice" };
static std::vector<std::string> factoryPracticeList = { "None", "Flytrap Practice" };
static std::vector<std::string> castlePracticeList = { "None", "Soft Patch", "Wisp 2 Practice", "Keeper Practice", "Lightning Bow Shots", "Fire Bow Shots" };
static std::vector<std::string> islandPracticeList = { "None", "Soft Patch" };
static std::vector<std::string> stalingradPracticeList = { "None", "Soft Patch", "Lockdown Practice", "Challenges Practice", "Gersh Quote Skips", "Boss Quote Skip" };
static std::vector<std::string> genesisPracticeList = { "None", "Soft Patch", "Bones Practice", "Boss 1 Practice", "Boss 2 Practice", "Basketball Practice", "Squid Shards Practice"};
static std::vector<std::string> moonPracticeList = { "None" };
static std::vector<std::string> tombPracticeList = { "None", "Wind Orb Practice" };
static int practicePatchIndexes[8] = { 0 };
static int inactivePracticePatchIndexes[8] = { 0 };

// Player options data
static std::string weaponSelectName = "none";
static int perkSelectIndex = 0;
static int gobblegumClassicSelectIndex = 0;
static int gobblegumMegaSelectIndex = 0;
static int pointInput = 0;

// Round options data
static int roundInput = 1;
static int zombieCount = 0;

// Autosplits data
static bool showAutosplitPresetExists = false;
static bool autosplitPopupBoolCheck = false;
static bool addSplitView = false;
static bool showChangeMapError = false;
static int currentAddSplitIndex = 0;
static int addSplitRound = -1;
static int mapError = 0;

// Memory reading data
DWORD pID;
uintptr_t* baseAddr;
uintptr_t* mapNameAddr;
uintptr_t* roundAddr;
HANDLE pHandle;
static bool procFound = false;
static int currentRound = 0;
static char readMap[13];
int error = 0;

// Keybind data
std::vector<int> assignedKeys;

namespace GUIWindow
{
    void Setup()
    {
        TCHAR buf[256];
        GetCurrentDirectoryA(256, buf);
        selfDirectory = buf;

        LogFile("-------------Practice Tool Startup-------------", true);

        wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), LoadIcon(hInst, MAKEINTRESOURCE(IDI_BO3PRACTICETOOL)), NULL, NULL, NULL, "BO3PT", NULL};
        RegisterClassExA(&wc);
        RECT rect;
        SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
        hWnd = CreateWindowExA(WS_EX_OVERLAPPEDWINDOW, "BO3PT", "BO3 Practice Tool", WS_OVERLAPPEDWINDOW, rect.right / 4, rect.bottom / 4, 1280, 720, 0, 0, NULL, 0);

        InitImgList();

        if (!CreateDeviceD3D(hWnd))
        {
            CleanupDeviceD3D();
            UnregisterClassA(wc.lpszClassName, wc.hInstance);
            return;
        }

        InitImgTextures();

        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui::GetStyle().ScaleAllSizes(1.25f);
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        mainFont = io.Fonts->AddFontFromFileTTF("./Fonts/DroidSans.ttf", 20);
        ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
        static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
        io.Fonts->AddFontFromFileTTF("./Fonts/" FONT_ICON_FILE_NAME_FAS, 16.0f, &icons_config, icons_ranges);
        sidebarFont = io.Fonts->AddFontFromFileTTF("./Fonts/DroidSans.ttf", 24);
        io.Fonts->AddFontFromFileTTF("./Fonts/" FONT_ICON_FILE_NAME_FAS, 20.0f, &icons_config, icons_ranges);
        titleFont = io.Fonts->AddFontFromFileTTF("./Fonts/DroidSans.ttf", 28);
        io.Fonts->AddFontFromFileTTF("./Fonts/" FONT_ICON_FILE_NAME_FAS, 24.0f, &icons_config, icons_ranges);
        ImGui_ImplWin32_Init(hWnd);
        ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

        if (!DoesPathExist(selfDirectory + "\\settings.json"))
            WriteEmptyJson(selfDirectory + "\\settings.json");
        Document doc = ReadJsonFromFile(selfDirectory + "\\settings.json");
        
        LogFile("Searching for BO3 directory");
        if (doc.HasMember("Steam Path") && doc["Steam Path"].IsString())
        {
            bo3Directory = doc["Steam Path"].GetString();
            LogFile("BO3 directory found");
        }
        else
            LogFile("BO3 directory not saved - prompting user for input");
        if (DoesPathExist(bo3Directory))
        {
            steamPathFound = true;
            VerifyFileStructure();
        }
        pID = MemHelp::GetProcessIdByName("BlackOps3.exe");
        if (pID != 0)
        {
            baseAddr = MemHelp::GetModuleBaseAddress(pID, "BlackOps3.exe");
            mapNameAddr = baseAddr + 0x179DF840 / 8;
            roundAddr = baseAddr + 0x1140DC30 / 8;
            std::stringstream log;
            log << "Base Address: " << baseAddr << "\n";
            log << "Map Address: " << mapNameAddr << "\n";
            log << "Round Address: " << roundAddr << "\n";
            LogFile(log.str());
            pHandle = OpenProcess(PROCESS_ALL_ACCESS, false, pID);
            if (pHandle != INVALID_HANDLE_VALUE)
                procFound = true;
        }
        else
        {
            auto thread = std::thread(SearchForGame);
            thread.detach();
        }
        LogFile("Writing presets");
        BGB::WritePresetToGame(BGB::inactivePreset, bo3Directory + "\\Practice Tool\\Settings\\Active Gum Preset.txt");
        WritePracticePatches(inactivePracticePatchIndexes);
        updateAvailable = UpdateAvailable();
        LogFile("Setup Finished");
    }

    void Run()
    {
        Setup();
        if (steamPathFound)
            InitVariables();
        // Init function list
        {
            funcList.push_back(std::function<void()>(GobblegumLoadoutPtr));
            funcList.push_back(std::function<void()>(AutosplitsPtr));
            funcList.push_back(std::function<void()>(PracticePatchesPtr));
            funcList.push_back(std::function<void()>(SettingsPtr));
            funcList.push_back(std::function<void()>(PlayerOptionsPtr));
            funcList.push_back(std::function<void()>(ZombieOptionsPtr));
            funcList.push_back(std::function<void()>(RoundOptionsPtr));
            funcList.push_back(std::function<void()>(PowerupOptionsPtr));
            funcList.push_back(std::function<void()>(EggStepOptionsPtr));
            funcList.push_back(std::function<void()>(CraftableOptionsPtr));
            funcList.push_back(std::function<void()>(BlockerOptionsPtr));
            funcList.push_back(std::function<void()>(MapOptionsPtr));
            funcList.push_back(std::function<void()>(ZombieCalculatorPtr));
            funcList.push_back(std::function<void()>(CodeGuidesPtr));
            funcList.push_back(std::function<void()>(GKValveSolverPtr));
        }

        ShowWindow(hWnd, SW_SHOWDEFAULT);
        UpdateWindow(hWnd);

        ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& style = ImGui::GetStyle();
        style.FrameRounding = 6.0f;
        style.PopupRounding = 6.0f;
        style.WindowBorderSize = 0.0f;
        style.Colors[2].w = 0.6f;

        keyboardRegistered = RegisterRawInput(hWnd, 0x06);
        keyboardRegistered = RegisterRawInput(hWnd, 0x02);

        while (!done)
        {
            if (!BeginFrame())
            {
                done = true;
                break;
            }

            if (!RenderFrame())
            {
                done = true;
                break;
            }

            if (!EndFrame())
            {
                done = true;
                break;
            }
        }

        Destroy();
    }

    void Destroy()
    {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();

        CleanupDeviceD3D();
        ::DestroyWindow(hWnd);
        ::UnregisterClassA(wc.lpszClassName, wc.hInstance);
    }

    bool CreateDeviceD3D(HWND hWnd)
    {
        // Setup swap chain
        DXGI_SWAP_CHAIN_DESC sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.BufferCount = 2;
        sd.BufferDesc.Width = 0;
        sd.BufferDesc.Height = 0;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = hWnd;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        UINT createDeviceFlags = 0;
        //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
        D3D_FEATURE_LEVEL featureLevel;
        const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
        if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
            return false;

        CreateRenderTarget();
        return true;
    }

    void CleanupDeviceD3D()
    {
        CleanupRenderTarget();
        if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
        if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
        if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
    }

    void CreateRenderTarget()
    {
        ID3D11Texture2D* pBackBuffer;
        g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
        g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
        pBackBuffer->Release();
    }

    void CleanupRenderTarget()
    {
        if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
    }

    LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
            return true;

        switch (msg)
        {
        case WM_SIZE:
            if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
            {
                CleanupRenderTarget();
                g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
                CreateRenderTarget();
                mainDocked = false;
            }
            return 0;
        case WM_SYSCOMMAND:
            if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
                return 0;
            break;
        case WM_DESTROY:
            done = true;
            LogFile("-------------Practice Tool Ended---------------");
            ::PostQuitMessage(0);
            return 0;
        case WM_DPICHANGED:
            if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
            {
                const RECT* suggested_rect = (RECT*)lParam;
                ::SetWindowPos(hWnd, NULL, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
            }
            break;
        case WM_INPUT:
            UINT dwSize = 0;
            GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
            LPBYTE lpb = new BYTE[dwSize];
            if (lpb == NULL)
                break;
            if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
                NLog("GetRawInputData does not return correct size!");
            RAWINPUT* raw = (RAWINPUT*)lpb;
            // handle keyboard input
            if (raw->header.dwType == RIM_TYPEKEYBOARD)
            {
                int key = raw->data.keyboard.VKey;
                // Check invalid key
                if (key == 0xff)
                    break;
                // Check key pressed or released
                if (raw->data.keyboard.Message == WM_KEYDOWN || raw->data.keyboard.Message == WM_SYSKEYDOWN)
                {
                    if (keyMap[key])
                        keyMapHeld[key] = true;
                    keyMap[key] = true;
                    if (!keyMapHeld[key])
                        usedKeys.push_back(key);
                }
                else if (raw->data.keyboard.Message == WM_KEYUP || raw->data.keyboard.Message == WM_SYSKEYUP)
                {
                    keyMapHeld[key] = false;
                    keyMap[key] = false;
                    usedKeys.erase(std::remove(usedKeys.begin(), usedKeys.end(), key), usedKeys.end());
                }
                else
                    break;
                // New hotkey registration
                if (registerHotKey)
                {
                    // Catch system key press events
                    if (raw->data.keyboard.Message == WM_KEYDOWN || raw->data.keyboard.Message == WM_SYSKEYDOWN)
                    {
                        // If user was inputting mouse controls, override it with keyboard controls. This should never actually happen because mouse buttons can't be chained
                        if (hotkeyToAssign->second.type)
                        {
                            hotkeyToAssign->second.type = 0;
                            hotkeyToAssign->second.keyNames = "";
                            hotkeyToAssign->second.keys = { };
                            usedKeys = { };
                            totalNumKeys = 0;
                            initialKeyToRelease = 0;
                            assignedKeys = { };
                        }
                        // Escape clears the current hotkey
                        if (key == VK_ESCAPE)
                        {
                            registerHotKey = false;
                            hotkeyToAssign->second.keyNames = "";
                            hotkeyToAssign->second.keys = { };
                            usedKeys = { };
                            totalNumKeys = 0;
                            initialKeyToRelease = 0;
                            assignedKeys = { };
                            ValidateKeybind(*hotkeyToAssign, true);
                        }
                        // Make sure key isn't held down and we haven't hit the max combo length already
                        else if (!keyMapHeld[key] && totalNumKeys < 3)
                        {
                            // Check to make sure key isn't already used in the sequence
                            if (std::find(assignedKeys.begin(), assignedKeys.end(), key) != assignedKeys.end())
                                break;
                            // Check if this is the first key in the sequence
                            if (!initialKeyToRelease)
                            {
                                // Check for modifier key
                                if (key != VK_SHIFT && key != VK_CONTROL && key != VK_MENU)
                                {
                                    registerHotKey = false;
                                    hotkeyToAssign->second.keyNames = keyDictionary[key];
                                    hotkeyToAssign->second.keys = { key };
                                    totalNumKeys = 0;
                                    initialKeyToRelease = 0;
                                    assignedKeys = { };
                                    ValidateKeybind(*hotkeyToAssign, true);
                                    break;
                                }
                                else
                                {
                                    modifiersPressed++;
                                    initialKeyToRelease = key;
                                    assignedKeys.push_back(key);
                                }
                                if (registerHotKey)
                                    totalNumKeys++;
                                hotkeyToAssign->second.keyNames = keyDictionary[key];
                                hotkeyToAssign->second.keys = { key };
                            }
                            else
                            {
                                // Check for modifier key to limit ctrl + shift + alt as a keybind - max length we're supporting is 3
                                if (key == VK_SHIFT || key == VK_CONTROL || key == VK_MENU)
                                {
                                    modifiersPressed++;
                                    totalNumKeys++;
                                    hotkeyToAssign->second.keyNames += "+" + keyDictionary[key];
                                    hotkeyToAssign->second.keys.push_back(key);
                                    if (modifiersPressed < 3)
                                        break;
                                }
                                registerHotKey = false;
                                modifiersPressed = 0;
                                totalNumKeys++;
                                hotkeyToAssign->second.keyNames += "+" + keyDictionary[key];
                                hotkeyToAssign->second.keys.push_back(key);
                                totalNumKeys = 0;
                                initialKeyToRelease = 0;
                                assignedKeys = { };
                                ValidateKeybind(*hotkeyToAssign, true);
                            }
                        }
                    }
                    // Catach system key release events
                    else if (raw->data.keyboard.Message == WM_KEYUP || raw->data.keyboard.Message == WM_SYSKEYUP)
                    {
                        // Wait for first key in the sequence to be released, signaling the end of the hotkey - only triggered if modifier keys are detected
                        if (key == initialKeyToRelease)
                        {
                            registerHotKey = false;
                            initialKeyToRelease = 0;
                            modifiersPressed = 0;
                            totalNumKeys = 0;
                            for (const int& usedKey : usedKeys)
                            {
                                keyMap[usedKey] = false;
                                keyMapHeld[usedKey] = false;
                            }
                            assignedKeys = { };
                            ValidateKeybind(*hotkeyToAssign, true);
                        }
                    }
                    break;
                }
                // If we make it here, we can safely assume that we have received a key press or release event, and that a new keybind is not being registered
                // Therefore, we can check if the newest key was pressed, and run a keybind event to search for a match given the pressed keys
                if (keyMap[key] && !keyMapHeld[key])
                    CheckAndRunKeybind(0);
            }
            // handle mouse input
            else if (raw->header.dwType == RIM_TYPEMOUSE && raw->data.mouse.usButtonFlags)
            {
                int key = raw->data.mouse.usButtonFlags;
                if (key == 0x400 || key == 0x800)
                {
                    if (((short)(unsigned short)raw->data.mouse.usButtonData) < 0)
                        key *= -1;
                }
                bool down = true;
                // Check if the mouse button is in the supported button dictionaries, else break the check
                if (buttonDictionaryDown.find(key) != buttonDictionaryDown.end())
                {
                    buttonMap[key] = true;
                    usedButtons.push_back(key);
                }
                else if (buttonDictionaryUp.find(key) != buttonDictionaryUp.end())
                {
                    buttonMap[key / 2] = false;
                    buttonMapHeld[key / 2] = false;
                    usedButtons.erase(std::remove(usedButtons.begin(), usedButtons.end(), key), usedButtons.end());
                    down = false;
                }
                else
                    break;
                // New
                if (registerHotKey)
                {
                    // If user was inputting keyboard controls, override it with mouse controls
                    if (!hotkeyToAssign->second.type)
                    {
                        hotkeyToAssign->second.type = 1;
                        hotkeyToAssign->second.keyNames = "";
                        hotkeyToAssign->second.keys = { };
                        usedKeys = { };
                        totalNumKeys = 0;
                        initialKeyToRelease = 0;
                        assignedKeys = { };
                    }
                    // If button pressed, assign that to the current hotkey being changed. We don't need to handle button up events because the max length of a mouse keybind is 1
                    if (down)
                    {
                        registerHotKey = false;
                        hotkeyToAssign->second.keyNames = buttonDictionaryDown[key];
                        hotkeyToAssign->second.keys = { key };
                        totalNumKeys = 0;
                        initialKeyToRelease = 0;
                        assignedKeys = { };
                        ValidateKeybind(*hotkeyToAssign, true);
                        break;
                    }
                }
                // Check for a matching keybind
                if (buttonMap[key] && !buttonMapHeld[key])
                    CheckAndRunKeybind(1);
            }
            break;
        }
        return ::DefWindowProc(hWnd, msg, wParam, lParam);
    }
}

void SearchForGame()
{
    HWND codHWND;
    DWORD tempID = 0;
    appStatus = "Status: Inactive";
    enabled = false;
    LogFile("Started async process - looking for BO3");
    for (;;)
    {
        codHWND = FindWindow("CoDBlackOps", NULL);
        if (codHWND != NULL)
        {
            char windowFilename[256] = "";
            GetWindowThreadProcessId(codHWND, &tempID);
            HANDLE tempHandle = OpenProcess(PROCESS_ALL_ACCESS, false, tempID);
            GetModuleFileNameExA(tempHandle, NULL, windowFilename, 256);
            std::string name = windowFilename;
            if (name.find("BlackOps3.exe") != name.npos)
            {
                pID = MemHelp::GetProcessIdByName("BlackOps3.exe");
                baseAddr = MemHelp::GetModuleBaseAddress(pID, "BlackOps3.exe");
                mapNameAddr = baseAddr + 0x179DF840 / 8;
                roundAddr = baseAddr + 0x1140DC30 / 8;
                pHandle = OpenProcess(PROCESS_ALL_ACCESS, false, pID);
                if (pHandle != INVALID_HANDLE_VALUE)
                {
                    procFound = true;
                    LogFile("Ending async process - BO3 found");
                    return;
                }
            }
            CloseHandle(tempHandle);
        }
        Sleep(1000);
    }
}

bool UpdateAvailable()
{
    if (DoesPathExist(selfDirectory + "/BO3 Practice Tool.old.exe"))
    {
        if (std::filesystem::remove(selfDirectory + "/BO3 Practice Tool.old.exe"))
            LogFile("Deleting old exe");
        else
            LogFile("Couldn't remove old exe with error code: " + std::error_code(errno, std::system_category()).message());
    }
    LogFile("Checking for updates");

    CURL* curl = curl_easy_init();
    CURLcode res;
    std::string buffer;
    std::string tagName;
    
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.github.com/repos/joshr520/BO3-Practice-Tool/releases/latest");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "BO3-Practice-Tool");
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        if (res != CURLE_OK)
        {
            LogFile("curl GET failed with error code: " + std::to_string(res));
            return 0;
        }
        Document get = ReadJsonFromString(buffer);
        if (get.HasParseError())
        {
            LogFile("JSON returned from get invalid");
            return 0;
        }
        if (!GetStringFromJsonArray("assets", "browser_download_url", get, downloadURL))
            return 0;
        if (!GetStringFromJson("tag_name", get, tagName))
            return 0;

        if (!CheckVersions(tagName, internalVersion))
        {
            LogFile("New version not available");
            return 0;
        }
        LogFile("New version detected");
    }

    return 1;
}

bool PerformUpdate()
{
    std::string ptexe;

    if (DownloadAndExtractZip({ "BO3 Practice Tool", "GSC", "Resource Images" }))
        return 0;
    return 1;
}

bool BeginFrame()
{
    MSG msg;
    while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
    {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
        if (msg.message == WM_QUIT)
            return false;
    }

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    return true;
}

bool RenderFrame()
{
    viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::PushFont(titleFont);
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.2f, 0.2f, 0.2f, 1.f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("BO3 Practice Tool", NULL, flags);
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
    if (done)
    {
        ImGui::End();
        return 0;
    }

    ImGuiID dockID = ImGui::GetID("Dockspace");
    if (!mainDocked)
    {
        mainDocked = true;

        ImGui::DockBuilderRemoveNode(dockID);
        ImGui::DockBuilderAddNode(dockID);

        // Make the dock node's size and position to match the viewport
        ImGui::DockBuilderSetNodeSize(dockID, ImGui::GetMainViewport()->WorkSize);
        ImGui::DockBuilderSetNodePos(dockID, ImGui::GetMainViewport()->WorkPos);

        ImGuiID dock_main_id = dockID;
        ImGuiID dock_left_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.2f, nullptr, &dock_main_id);
        ImGuiID dock_right_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.2f, nullptr, &dock_main_id);
        ImGuiID dock_down_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.625f, nullptr, &dock_right_id);

        ImGui::DockBuilderDockWindow("##Sidebar", dock_left_id);
        ImGui::DockBuilderDockWindow("##Body", dock_right_id);
        ImGui::DockBuilderDockWindow("##Gum Context Menu", dock_down_id);

        ImGui::DockBuilderFinish(dock_main_id);
    }
    ImGui::DockSpace(dockID, ImVec2(viewport->Size.x, viewport->Size.y), ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_PassthruCentralNode);

    ImGui::PopFont();
    ImGui::Begin("##Sidebar", 0, dockFlags);
    {
        if (!steamPathFound || !procFound)
            ImGui::BeginDisabled();
        // Create sidebar options
        {
            ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(appStatus.c_str()).x) / 2 + 10);
            ImGui::Text(appStatus.c_str());
            if (CreateButton("Toggle Status", ImVec2(ImGui::GetContentRegionAvail().x, 50.0f), &enabled, true))
            {
                if (currentMap == ".")
                {
                    enabled = false;
                    appStatus = "Status: Inactive";
                }
                else
                {
                    appStatus = (appStatus == "Status: Inactive") ? "Status: Active" : "Status: Inactive";
                    if (appStatus == "Status: Inactive")
                    {
                        BGB::writeGums = false;
                        BGB::WritePresetToGame(BGB::inactivePreset, bo3Directory + "\\Practice Tool\\Settings\\Active Gum Preset.txt");
                        WritePracticePatches(inactivePracticePatchIndexes);
                        ResetToggles();
                    }
                    else
                        WritePracticePatches(practicePatchIndexes);
                    auto injectThread = std::thread(InjectTool, enabled, std::ref(injectResponse));
                    injectThread.detach();
                }
            }
            ImGui::Separator();
            if (!steamPathFound || !procFound)
                ImGui::EndDisabled();
            ImGui::PushFont(sidebarFont);
            ImGui::Text("Frontend"); ImGui::Separator(3.5f);
            ImGui::PopFont();
            
            const char* sidebarItems[] = { "Gobblegum Loadout", "Autosplits", "Practice Patches", "Settings", "Player Options", "Zombie Options", "Round Options", "Powerup Options", "Egg Step Options",
                "Craftable Options", "Blocker Options", "Map Options", "Zombie Calculator", "Code Guides", "GK Valve Solver"};
            const char* sidebarPreview = sidebarItems[sidebarCurrentItem];
            static int frontendItems = 4;
            static int inGameItems = frontendItems + 8;
            static int resourceItems = inGameItems + 3;
            // Frontend
            for (int i = 0; i < frontendItems; i++)
            {
                const bool is_selected = sidebarCurrentItem == i;
                if (ImGui::Selectable(sidebarItems[i], is_selected))
                    sidebarCurrentItem = i;
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            } ImGui::Spacing(); ImGui::Separator();
            if (!steamPathFound || !procFound)
                ImGui::BeginDisabled();
            ImGui::PushFont(sidebarFont);
            ImGui::Text("In Game"); ImGui::Separator(3.5f);
            ImGui::PopFont();
            // In Game
            if (appStatus != "Status: Active" || currentMap == "core_frontend")
            {
                if (sidebarCurrentItem > 3 && sidebarCurrentItem < 11)
                    sidebarCurrentItem = 0;
                ImGui::BeginDisabled();
            }
            for (int i = frontendItems; i < inGameItems; i++)
            {
                const bool is_selected = sidebarCurrentItem == i;
                if (ImGui::Selectable(sidebarItems[i], is_selected))
                    sidebarCurrentItem = i;
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            } ImGui::Spacing(); ImGui::Separator();
            if (appStatus != "Status: Active" || currentMap == "core_frontend")
                ImGui::EndDisabled();
            if (!steamPathFound || !procFound)
                ImGui::EndDisabled();
            ImGui::PushFont(sidebarFont);
            ImGui::Text("Resources"); ImGui::Separator(3.5f);
            ImGui::PopFont();
            // Resources
            for (int i = inGameItems; i < resourceItems; i++)
            {
                const bool is_selected = sidebarCurrentItem == i;
                if (ImGui::Selectable(sidebarItems[i], is_selected))
                    sidebarCurrentItem = i;
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            } ImGui::Spacing(); ImGui::Separator();
        }
    }
    ImGui::End();

    ImGui::Begin("##Body", 0, dockFlags);
    
    if (updateAvailable)
    {
        ImGui::OpenPopup("Update Available");
        updateAvailable = false;
        doUpdateAvailable = true;
    }
    else if (updateFailed)
    {
        ImGui::OpenPopup("Update Failed");
        updateFailed = false;
        doUpdateFailed = true;
    }
    if (doUpdateAvailable)
    {
        ImVec2 windowPos = ImGui::GetWindowPos();
        ImVec2 windowSize = ImGui::GetWindowSize();
        ImGui::SetNextWindowPos(ImVec2(windowPos.x + windowSize.x / 2 - ImGui::CalcItemWidth() / 3, windowPos.y + 120), ImGuiCond_Always);
        if (ImGui::BeginPopupModal("Update Available", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::TextWrapped("A new update is available, would you like to update?");
            if (ImGui::Button("OK", ImVec2(120, 0)))
            {
                if (!PerformUpdate())
                    updateFailed = true;
                ImGui::CloseCurrentPopup();
                doUpdateAvailable = false;
            } SAMELINE;
            if (ImGui::Button("Exit", ImVec2(120, 0)))
            {
                ImGui::CloseCurrentPopup();
                doUpdateAvailable = false;
            }
            ImGui::EndPopup();
        }
    }
    else if (doUpdateFailed)
    {
        ImVec2 windowPos = ImGui::GetWindowPos();
        ImVec2 windowSize = ImGui::GetWindowSize();
        ImGui::SetNextWindowPos(ImVec2(windowPos.x + windowSize.x / 2 - ImGui::CalcItemWidth() / 3, windowPos.y + 120), ImGuiCond_Always);
        if (ImGui::BeginPopupModal("Update Failed", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::TextWrapped("There was an error updating the program, check log.txt for more info");
            if (ImGui::Button("OK", ImVec2(240, 0)))
                ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }
    }
    if (injectResponse)
    {
        ImGui::OpenPopup("Injection Failed");
        injectResponse = false;
        injectResponseWait = true;
    }
    else if (injectResponseWait)
    {
        ImVec2 windowPos = ImGui::GetWindowPos();
        ImVec2 windowSize = ImGui::GetWindowSize();
        ImGui::SetNextWindowPos(ImVec2(windowPos.x + windowSize.x / 2 - ImGui::CalcItemWidth() / 3, windowPos.y + 120), ImGuiCond_Always);
        if (ImGui::BeginPopupModal("Injection Failed", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::TextWrapped("1 or more required files is missing, would you like to redownload them? (Add an exclusion in your antivirus to stop this from happening)");
            if (ImGui::Button("Download Files", ImVec2(125, 0)))
            {
                injectResponseWait = false;
                ImGui::CloseCurrentPopup();
                DownloadAndExtractZip({ "GSC" });
                VerifyFileStructure();
            }  SAMELINE;
            if (ImGui::Button("Exit", ImVec2(125, 0)))
            {
                injectResponseWait = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }
    if (!steamPathFound)
    {
        ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionAvail().x / 2 - ImGui::CalcTextSize("Select BO3 EXE").x / 2, ImGui::GetContentRegionAvail().y / 2 - ImGui::CalcTextSize("Select BO3 EXE").y / 2 - 50.0f));
        ImGui::Text("Select BO3 EXE");
        ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x / 2 - ImGui::CalcTextSize("Select BO3 EXE").x / 2);
        if (CreateButton("Browse", ImVec2(ImGui::CalcTextSize("Select BO3 EXE").x, 50.0f)))
        {
            OPENFILENAMEA ofn;
            char szFile[260];

            ZeroMemory(&ofn, sizeof(ofn));
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = NULL;
            ofn.lpstrFile = szFile;
            ofn.lpstrFile[0] = '\0';
            ofn.nMaxFile = sizeof(szFile);
            ofn.lpstrFilter = "exe\0*.exe\0All\0*.*";
            ofn.nFilterIndex = 1;
            ofn.lpstrFileTitle = NULL;
            ofn.nMaxFileTitle = 0;
            ofn.lpstrInitialDir = NULL;
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

            GetOpenFileNameA(&ofn);

            bo3Directory = szFile;

            if (bo3Directory.empty() || bo3Directory.substr(bo3Directory.length() - 14, bo3Directory.length()) != "\\BlackOps3.exe")
                ImGui::OpenPopup("Directory Error");
            else
            {
                bo3Directory = bo3Directory.substr(0, bo3Directory.length() - 14);
                steamPathFound = true;
                Document data = ReadJsonFromFile(selfDirectory + "\\settings.json");
                ModifyJsonString(data, "Steam Path", bo3Directory);
                WriteJson(data, selfDirectory + "\\settings.json");
                VerifyFileStructure();
                InitVariables();
            }
        }

        if (bo3Directory.empty() || bo3Directory.substr(bo3Directory.length() - 14, bo3Directory.length()) != "\\BlackOps3.exe")
        {
            ImVec2 windowPos = ImGui::GetWindowPos();
            ImVec2 windowSize = ImGui::GetWindowSize();
            ImGui::SetNextWindowPos(ImVec2(windowPos.x + windowSize.x / 2 - ImGui::CalcItemWidth() / 3, windowPos.y + 120), ImGuiCond_Always);
            if (ImGui::BeginPopupModal("Directory Error", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::TextWrapped("Incorrect File Chosen");
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::CalcTextSize("Incorrect File Chosen").x / 2 - 60);
                if (ImGui::Button("OK", ImVec2(120, 0)))
                    ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
            }
        }
    }
    else
        funcList[sidebarCurrentItem]();

    ImGui::End();
    ImGui::End();

    if (procFound)
    {
        char* prevMap = readMap;
        int prevRound = currentRound;

        if (pHandle != INVALID_HANDLE_VALUE && !ReadProcessMemory(pHandle, mapNameAddr, &readMap, sizeof(readMap), NULL))
        {
            procFound = false;
            error = GetLastError();
            LogFile("Failed to read game memory with error code: " + error);
            CloseHandle(pHandle);
            pHandle = NULL;
            ResetToggles();
        }
        if (pHandle && pHandle != INVALID_HANDLE_VALUE && !ReadProcessMemory(pHandle, roundAddr, &currentRound, sizeof(currentRound), NULL))
        {
            procFound = false;
            error = GetLastError();
            LogFile("Failed to read game memory with error code: " + error);
            CloseHandle(pHandle);
            pHandle = NULL;
            ResetToggles();
        }
        currentMap = readMap;

        if ((strcmp(readMap, "core_frontend") && !strcmp(prevMap, "core_frontend")) || (!currentRound && prevRound))
        {
            ResetToggles();
            LogFile("Map change or restart detected, resetting toggles");
        }
    }

    return true;
}

bool EndFrame()
{
    ImGui::Render();
    const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
    g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
    g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }

    g_pSwapChain->Present(1, 0); // Present with vsync
    //g_pSwapChain->Present(0, 0); // Present without vsync

    return true;
}

void GobblegumLoadoutPtr()
{
    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));
    if (!BGB::showGumSelection)
    {
        ImGui::BeginGroup();
        if (CreateButton(ICON_FA_FILE_CIRCLE_PLUS " New Preset", ImVec2(150.0f, 25.0f)))
            ImGui::OpenPopup("New Gum Preset");
        if (CreateButton(ICON_FA_FILE_CIRCLE_MINUS " Delete Preset", ImVec2(150.0f, 25.0f)))
        {
            BGB::DeleteGumPreset(BGB::gumPresets[BGB::currentPreset].presetName);
            if (BGB::writeGums && appStatus == "Status: Active")
                BGB::WritePresetToGame(BGB::gumPresets[BGB::currentPreset], bo3Directory + "\\Practice Tool\\Settings\\Active Gum Preset.txt");
        } ImGui::EndGroup();
        SAMELINE;
        ImGui::PopStyleVar();
        ImGui::BeginGroup();
        // new preset creation
        if (ImGui::BeginPopup("New Gum Preset"))
        {
            ImGui::SetKeyboardFocusHere();
            char presetInput[32] = "";
            ImGui::SetNextItemWidth(200);
            if (ImGui::InputText("New Preset Name", presetInput, IM_ARRAYSIZE(presetInput), ImGuiInputTextFlags_EnterReturnsTrue))
            {
                if (std::string(presetInput).empty() || BGB::CheckPresetExists(presetInput))
                {
                    showGumPresetExists = true;
                    gumPopupBoolCheck = true;
                }
                else
                {
                    BGB::CreateNewGumPreset(presetInput);
                    if (BGB::writeGums && appStatus == "Status: Active")
                        BGB::WritePresetToGame(BGB::gumPresets[BGB::currentPreset], bo3Directory + "\\Practice Tool\\Settings\\Active Gum Preset.txt");
                }
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        if (showGumPresetExists)
        {
            if (gumPopupBoolCheck)
            {
                ImGui::OpenPopup("Gum Preset Already Exists");
                gumPopupBoolCheck = false;
            }
            ImVec2 windowPos = ImGui::GetWindowPos();
            ImVec2 windowSize = ImGui::GetWindowSize();
            ImGui::SetNextWindowPos(ImVec2(windowPos.x + windowSize.x / 2 - ImGui::CalcItemWidth() / 3, windowPos.y + 120), ImGuiCond_Always);
            if (ImGui::BeginPopupModal("Gum Preset Already Exists", &showGumPresetExists, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::TextWrapped("Gum Preset Already Exists");
                if (ImGui::Button("Close", ImVec2(120.0f, 25.0f)))
                    ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
            }
        }
        // presets dropdown
        ImGui::SetNextItemWidth(250);
        std::string previousPreset = BGB::gumPresets[BGB::currentPreset].presetName;
        if (ImGui::BeginCombo("Gum Presets", BGB::gumPresets[BGB::currentPreset].presetName.c_str(), ImGuiComboFlags_HeightRegular))
        {
            for (int i = 0; i < BGB::gumPresets.size(); i++)
            {
                const bool is_selected = BGB::currentPreset == i;
                if (ImGui::Selectable(BGB::gumPresets[i].presetName.c_str(), is_selected))
                    BGB::currentPreset = i;
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        } SAMELINE;
        if (previousPreset != BGB::gumPresets[BGB::currentPreset].presetName)
        {
            if (BGB::writeGums && appStatus == "Status: Active")
                BGB::WritePresetToGame(BGB::gumPresets[BGB::currentPreset], bo3Directory + "\\Practice Tool\\Settings\\Active Gum Preset.txt");
        }
        if (appStatus != "Status: Active")
            ImGui::BeginDisabled();
        if (ImGui::Checkbox("Active", &BGB::writeGums))
        {
            if (BGB::writeGums && appStatus == "Status: Active")
                BGB::WritePresetToGame(BGB::gumPresets[BGB::currentPreset], bo3Directory + "\\Practice Tool\\Settings\\Active Gum Preset.txt");
            else
                BGB::WritePresetToGame(BGB::inactivePreset, bo3Directory + "\\Practice Tool\\Settings\\Active Gum Preset.txt");
        }
        if (appStatus != "Status: Active")
            ImGui::EndDisabled();
        // preset gum image buttons
        CreateGumImages(BGB::gumPresets[BGB::currentPreset].presetGums, ImVec2(128.0f, 128.0f), 5, "Selection", PresetSelectionFunc);
        ImGui::EndGroup();
    }
    else
    {
        ImGui::PopStyleVar();
        // swap gum selection menu
        if (CreateButton(ICON_FA_ARROW_LEFT, ImVec2(50.0f, 25.0f)))
            BGB::showGumSelection = false;
        SAMELINE;
        if (ImGui::BeginTabBar("Gum Type Choice"))
        {
            if (ImGui::BeginTabItem("Classics"))
            {
                static char searchText[64] = "";
                if (!strcmp(gumSelectMenu, "Megas"))
                    strcpy_s(searchText, "");
                gumSelectMenu = "Classics";
                if (!strcmp(searchText, ""))
                    gumSearchList = classicGumList;
                if (ImGui::InputText("Gobblegum Search", searchText, IM_ARRAYSIZE(searchText)))
                {
                    gumSearchList = GumSearch(classicGumList, searchText);
                }
                if (CreateGumImages(gumSearchList, ImVec2(145.0f, 145.0f), 6, "Swap", SwapSelectionFunc))
                    strcpy_s(searchText, "");
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Megas"))
            {
                static char searchText[64] = "";
                if (!strcmp(gumSelectMenu, "Classics"))
                    strcpy_s(searchText, "");
                gumSelectMenu = "Megas";
                if (!strcmp(searchText, ""))
                    gumSearchList = megaGumList;
                if (ImGui::InputText("Gobblegum Search", searchText, IM_ARRAYSIZE(searchText)))
                {
                    gumSearchList = GumSearch(megaGumList, searchText);
                }
                if (CreateGumImages(gumSearchList, ImVec2(145.0f, 145.0f), 6, "Swap", SwapSelectionFunc))
                    strcpy_s(searchText, "");
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
    }

    ImGui::Begin("##Gum Context Menu", 0, dockFlags);
    ImGui::Image((ImTextureID)bgbImgList[BGB::gumContextIndex].imgTexture, ImVec2(196.0f, 196.0f));
    SAMELINE;
    ImVec2 contPos = ImGui::GetCursorPos();
    ImGui::PushFont(sidebarFont);
    ImGui::Text(bgbImgList[BGB::gumContextIndex].imgRelativePath.c_str());
    ImGui::SetCursorPos(ImVec2(contPos.x, contPos.y + 30.0f));
    ImGui::TextWrapped(BGB::gumDescriptions[BGB::gumContextIndex].c_str());
    ImGui::PopFont();
    ImGui::End();
}

void AutosplitsPtr()
{
    if (addSplitView)
    {
        if (CreateButton(ICON_FA_ARROW_LEFT, ImVec2(50.0f, 25.0f)))
            addSplitView = false;
        SAMELINE;
        ImGui::SetNextItemWidth(200);
        if (ImGui::InputInt("Round to split", &addSplitRound))
        {
            if (addSplitRound < -1)
                addSplitRound = -1;
            else if (addSplitRound > 255)
                addSplitRound = 255;
        }
        SAMELINE;
        HelpMarker(R"(How to use:
- Select an item to add to your layout
- Input a number from -1-255 in the "Round to split" input box.
    - -1 means the split will complete when the task is completed, with no attachment to any round
    - 0 means the split will wait for the task to complete, and then wait for the current round to end
    - 1-255 means the split will wait for the task to complete, and then wait for the given round to pass
- Click the "Add Split" button to add the split to your layout)");

        switch (splitPresets[currentPreset].map)
        {
        case 0:
        {
            // row 1
            {
                if (CreateListBox("##Ritual Splits", soeRitualSplits, soeSplits[0], ImVec2(150.0f, 126.0f)))
                {

                }
                SAMELINE;
                if (CreateButton("Add Split##Rituals", ImVec2(100.0f, 25.0f)))
                {
                    if (std::find_if(splitPresets[currentPreset].splits.begin(), splitPresets[currentPreset].splits.end(), [&](const auto& pair)
                        {
                            return pair.first == soeRitualSplits[soeSplits[0]];
                        }) == splitPresets[currentPreset].splits.end())
                    {
                        splitPresets[currentPreset].splits.push_back({ soeRitualSplits[soeSplits[0]], addSplitRound });
                        splitPresets[currentPreset].numSplits++;
                        WriteAutosplitPreset(splitPresets[currentPreset]);
                        addSplitView = false;
                    }
                }
                SAMELINE;
                if (CreateListBox("##Egg Splits", soeEggSplits, soeSplits[1], ImVec2(150.0f, 126.0f)))
                {

                }
                SAMELINE;
                if (CreateButton("Add Split##Eggs", ImVec2(100.0f, 25.0f)))
                {
                    if (std::find_if(splitPresets[currentPreset].splits.begin(), splitPresets[currentPreset].splits.end(), [&](const auto& pair)
                        {
                            return pair.first == soeEggSplits[soeSplits[1]];
                        }) == splitPresets[currentPreset].splits.end())
                    {
                        splitPresets[currentPreset].splits.push_back({ soeEggSplits[soeSplits[1]], addSplitRound });
                        splitPresets[currentPreset].numSplits++;
                        WriteAutosplitPreset(splitPresets[currentPreset]);
                        addSplitView = false;
                    }
                }
            }
            DummySpace(0.0f, 25.0f);
            // row 2
            {
                if (CreateListBox("##Ovum Splits", soeOvumSplits, soeSplits[2], ImVec2(150.0f, 101.0f)))
                {

                }
                SAMELINE;
                if (CreateButton("Add Split##Ovums", ImVec2(100.0f, 25.0f)))
                {
                    if (std::find_if(splitPresets[currentPreset].splits.begin(), splitPresets[currentPreset].splits.end(), [&](const auto& pair)
                        {
                            return pair.first == soeOvumSplits[soeSplits[2]];
                        }) == splitPresets[currentPreset].splits.end())
                    {
                        splitPresets[currentPreset].splits.push_back({ soeOvumSplits[soeSplits[2]], addSplitRound });
                        splitPresets[currentPreset].numSplits++;
                        WriteAutosplitPreset(splitPresets[currentPreset]);
                        addSplitView = false;
                    }
                }
                SAMELINE;
                if (CreateListBox("##Flag Splits", soeFlagSplits, soeSplits[3], ImVec2(200.0f, 151.0f)))
                {

                }
                SAMELINE;
                if (CreateButton("Add Split##Flags", ImVec2(100.0f, 25.0f)))
                {
                    if (std::find_if(splitPresets[currentPreset].splits.begin(), splitPresets[currentPreset].splits.end(), [&](const auto& pair)
                        {
                            return pair.first == soeFlagSplits[soeSplits[3]];
                        }) == splitPresets[currentPreset].splits.end())
                    {
                        splitPresets[currentPreset].splits.push_back({ soeFlagSplits[soeSplits[3]], addSplitRound });
                        splitPresets[currentPreset].numSplits++;
                        WriteAutosplitPreset(splitPresets[currentPreset]);
                        addSplitView = false;
                    }
                }
            }
            break;
        }
        case 2:
        {
            // row 1
            {
                if (CreateListBox("##Dragon Splits", deDragonSplits, deSplits[0], ImVec2(150.0f, 101.0f)))
                {

                }
                SAMELINE;
                if (CreateButton("Add Split##Dragon", ImVec2(100.0f, 25.0f)))
                {
                    if (std::find_if(splitPresets[currentPreset].splits.begin(), splitPresets[currentPreset].splits.end(), [&](const auto& pair)
                        {
                            return pair.first == deDragonSplits[deSplits[0]];
                        }) == splitPresets[currentPreset].splits.end())
                    {
                        splitPresets[currentPreset].splits.push_back({ deDragonSplits[deSplits[0]], addSplitRound });
                        splitPresets[currentPreset].numSplits++;
                        WriteAutosplitPreset(splitPresets[currentPreset]);
                        addSplitView = false;
                    }
                }
                SAMELINE;
                if (CreateListBox("##Lightning Bow Splits", deLightningBowSplits, deSplits[1], ImVec2(150.0f, 126.0f)))
                {

                }
                SAMELINE;
                if (CreateButton("Add Split##Lightning Bow", ImVec2(100.0f, 25.0f)))
                {
                    if (std::find_if(splitPresets[currentPreset].splits.begin(), splitPresets[currentPreset].splits.end(), [&](const auto& pair)
                        {
                            return pair.first == deLightningBowSplits[deSplits[1]];
                        }) == splitPresets[currentPreset].splits.end())
                    {
                        splitPresets[currentPreset].splits.push_back({ deLightningBowSplits[deSplits[1]], addSplitRound });
                        splitPresets[currentPreset].numSplits++;
                        WriteAutosplitPreset(splitPresets[currentPreset]);
                        addSplitView = false;
                    }
                }
                SAMELINE;
                if (CreateListBox("##Fire Bow Splits", deFireBowSplits, deSplits[2], ImVec2(150.0f, 126.0f)))
                {

                }
                SAMELINE;
                if (CreateButton("Add Split##Fire Bow", ImVec2(100.0f, 25.0f)))
                {
                    if (std::find_if(splitPresets[currentPreset].splits.begin(), splitPresets[currentPreset].splits.end(), [&](const auto& pair)
                        {
                            return pair.first == deFireBowSplits[deSplits[2]];
                        }) == splitPresets[currentPreset].splits.end())
                    {
                        splitPresets[currentPreset].splits.push_back({ deFireBowSplits[deSplits[2]], addSplitRound });
                        splitPresets[currentPreset].numSplits++;
                        WriteAutosplitPreset(splitPresets[currentPreset]);
                        addSplitView = false;
                    }
                }
            }
            DummySpace(0.0f, 25.0f);
            // row 2
            {
                if (CreateListBox("##Void Bow Splits", deVoidBowSplits, deSplits[3], ImVec2(150.0f, 151.0f)))
                {

                }
                SAMELINE;
                if (CreateButton("Add Split##Void Bow", ImVec2(100.0f, 25.0f)))
                {
                    if (std::find_if(splitPresets[currentPreset].splits.begin(), splitPresets[currentPreset].splits.end(), [&](const auto& pair)
                        {
                            return pair.first == deVoidBowSplits[deSplits[3]];
                        }) == splitPresets[currentPreset].splits.end())
                    {
                        splitPresets[currentPreset].splits.push_back({ deVoidBowSplits[deSplits[3]], addSplitRound });
                        splitPresets[currentPreset].numSplits++;
                        WriteAutosplitPreset(splitPresets[currentPreset]);
                        addSplitView = false;
                    }
                }
                SAMELINE;
                if (CreateListBox("##Wolf Bow Splits", deWolfBowSplits, deSplits[4], ImVec2(150.0f, 126.0f)))
                {

                }
                SAMELINE;
                if (CreateButton("Add Split##Wolf Bow", ImVec2(100.0f, 25.0f)))
                {
                    if (std::find_if(splitPresets[currentPreset].splits.begin(), splitPresets[currentPreset].splits.end(), [&](const auto& pair)
                        {
                            return pair.first == deWolfBowSplits[deSplits[4]];
                        }) == splitPresets[currentPreset].splits.end())
                    {
                        splitPresets[currentPreset].splits.push_back({ deWolfBowSplits[deSplits[4]], addSplitRound });
                        splitPresets[currentPreset].numSplits++;
                        WriteAutosplitPreset(splitPresets[currentPreset]);
                        addSplitView = false;
                    }
                }
                SAMELINE;
                if (CreateListBox("##Wisp Splits", deWispSplits, deSplits[5], ImVec2(150.0f, 51.0f)))
                {

                }
                SAMELINE;
                if (CreateButton("Add Split##Wisp", ImVec2(100.0f, 25.0f)))
                {
                    if (std::find_if(splitPresets[currentPreset].splits.begin(), splitPresets[currentPreset].splits.end(), [&](const auto& pair)
                        {
                            return pair.first == deWispSplits[deSplits[5]];
                        }) == splitPresets[currentPreset].splits.end())
                    {
                        splitPresets[currentPreset].splits.push_back({ deWispSplits[deSplits[5]], addSplitRound });
                        splitPresets[currentPreset].numSplits++;
                        WriteAutosplitPreset(splitPresets[currentPreset]);
                        addSplitView = false;
                    }
                }
            }
            DummySpace(0.0f, 25.0f);
            // row 3
            {
                if (CreateListBox("##Simon Splits", deSimonSplits, deSplits[6], ImVec2(150.0f, 151.0f)))
                {

                }
                SAMELINE;
                if (CreateButton("Add Split##Simon", ImVec2(100.0f, 25.0f)))
                {
                    if (std::find_if(splitPresets[currentPreset].splits.begin(), splitPresets[currentPreset].splits.end(), [&](const auto& pair)
                        {
                            return pair.first == deSimonSplits[deSplits[6]];
                        }) == splitPresets[currentPreset].splits.end())
                    {
                        splitPresets[currentPreset].splits.push_back({ deSimonSplits[deSplits[6]], addSplitRound });
                        splitPresets[currentPreset].numSplits++;
                        WriteAutosplitPreset(splitPresets[currentPreset]);
                        addSplitView = false;
                    }
                }
                SAMELINE;
                if (CreateListBox("##Keeper Splits", deKeeperSplits, deSplits[7], ImVec2(150.0f, 151.0f)))
                {

                }
                SAMELINE;
                if (CreateButton("Add Split##Keeper", ImVec2(100.0f, 25.0f)))
                {
                    if (std::find_if(splitPresets[currentPreset].splits.begin(), splitPresets[currentPreset].splits.end(), [&](const auto& pair)
                        {
                            return pair.first == deKeeperSplits[deSplits[7]];
                        }) == splitPresets[currentPreset].splits.end())
                    {
                        splitPresets[currentPreset].splits.push_back({ deKeeperSplits[deSplits[7]], addSplitRound });
                        splitPresets[currentPreset].numSplits++;
                        WriteAutosplitPreset(splitPresets[currentPreset]);
                        addSplitView = false;
                    }
                }
                SAMELINE;
                if (CreateListBox("##Boss Splits", deBossSplits, deSplits[8], ImVec2(150.0f, 51.0f)))
                {

                }
                SAMELINE;
                if (CreateButton("Add Split##Boss", ImVec2(100.0f, 25.0f)))
                {
                    if (std::find_if(splitPresets[currentPreset].splits.begin(), splitPresets[currentPreset].splits.end(), [&](const auto& pair)
                        {
                            return pair.first == deBossSplits[deSplits[8]];
                        }) == splitPresets[currentPreset].splits.end())
                    {
                        splitPresets[currentPreset].splits.push_back({ deBossSplits[deSplits[8]], addSplitRound });
                        splitPresets[currentPreset].numSplits++;
                        WriteAutosplitPreset(splitPresets[currentPreset]);
                        addSplitView = false;
                    }
                }
            }
            break;
        }
        case 3:
        {
            // row 1
            {
                if (CreateListBox("##Skull Splits", znsSkullSplits, znsSplits[0], ImVec2(150.0f, 126.0f)))
                {

                }
                SAMELINE;
                if (CreateButton("Add Split##Skull", ImVec2(100.0f, 25.0f)))
                {
                    if (std::find_if(splitPresets[currentPreset].splits.begin(), splitPresets[currentPreset].splits.end(), [&](const auto& pair)
                        {
                            return pair.first == znsSkullSplits[znsSplits[0]];
                        }) == splitPresets[currentPreset].splits.end())
                    {
                        splitPresets[currentPreset].splits.push_back({ znsSkullSplits[znsSplits[0]], addSplitRound });
                        splitPresets[currentPreset].numSplits++;
                        WriteAutosplitPreset(splitPresets[currentPreset]);
                        addSplitView = false;
                    }
                }
                SAMELINE;
                if (CreateListBox("##ZNS Blocker Splits", znsBlockerSplits, znsSplits[1], ImVec2(150.0f, 51.0f)))
                {

                }
                SAMELINE;
                if (CreateButton("Add Split##ZNS Blocker", ImVec2(100.0f, 25.0f)))
                {
                    if (std::find_if(splitPresets[currentPreset].splits.begin(), splitPresets[currentPreset].splits.end(), [&](const auto& pair)
                        {
                            return pair.first == znsBlockerSplits[znsSplits[1]];
                        }) == splitPresets[currentPreset].splits.end())
                    {
                        splitPresets[currentPreset].splits.push_back({ znsBlockerSplits[znsSplits[1]], addSplitRound });
                        splitPresets[currentPreset].numSplits++;
                        WriteAutosplitPreset(splitPresets[currentPreset]);
                        addSplitView = false;
                    }
                }
                SAMELINE;
                if (CreateListBox("##ZNS WW Splits", znsWWSplits, znsSplits[2], ImVec2(150.0f, 51.0f)))
                {

                }
                SAMELINE;
                if (CreateButton("Add Split##ZNS WW", ImVec2(100.0f, 25.0f)))
                {
                    if (std::find_if(splitPresets[currentPreset].splits.begin(), splitPresets[currentPreset].splits.end(), [&](const auto& pair)
                        {
                            return pair.first == znsWWSplits[znsSplits[2]];
                        }) == splitPresets[currentPreset].splits.end())
                    {
                        splitPresets[currentPreset].splits.push_back({ znsWWSplits[znsSplits[2]], addSplitRound });
                        splitPresets[currentPreset].numSplits++;
                        WriteAutosplitPreset(splitPresets[currentPreset]);
                        addSplitView = false;
                    }
                }
            }
            DummySpace(0.0f, 25.0f);
            // row 2
            {
                if (CreateListBox("##ZNS EE Splits", znsEESplits, znsSplits[3], ImVec2(150.0f, 126.0f)))
                {

                }
                SAMELINE;
                if (CreateButton("Add Split##ZNS EE", ImVec2(100.0f, 25.0f)))
                {
                    if (std::find_if(splitPresets[currentPreset].splits.begin(), splitPresets[currentPreset].splits.end(), [&](const auto& pair)
                        {
                            return pair.first == znsEESplits[znsSplits[3]];
                        }) == splitPresets[currentPreset].splits.end())
                    {
                        splitPresets[currentPreset].splits.push_back({ znsEESplits[znsSplits[3]], addSplitRound });
                        splitPresets[currentPreset].numSplits++;
                        WriteAutosplitPreset(splitPresets[currentPreset]);
                        addSplitView = false;
                    }
                }
            }
            break;
        }
        case 4:
        {
            // row 1
            {
                if (CreateListBox("##GK PAP Splits", gkPAPSplits, gkSplits[0], ImVec2(150.0f, 76.0f)))
                {

                }
                SAMELINE;
                if (CreateButton("Add Split##GK PAP", ImVec2(100.0f, 25.0f)))
                {
                    if (std::find_if(splitPresets[currentPreset].splits.begin(), splitPresets[currentPreset].splits.end(), [&](const auto& pair)
                        {
                            return pair.first == gkPAPSplits[gkSplits[0]];
                        }) == splitPresets[currentPreset].splits.end())
                    {
                        splitPresets[currentPreset].splits.push_back({ gkPAPSplits[gkSplits[0]], addSplitRound });
                        splitPresets[currentPreset].numSplits++;
                        WriteAutosplitPreset(splitPresets[currentPreset]);
                        addSplitView = false;
                    }
                }
                SAMELINE;
                if (CreateListBox("##Gauntlet Splits", gkGauntletSplits, gkSplits[1], ImVec2(150.0f, 101.0f)))
                {

                }
                SAMELINE;
                if (CreateButton("Add Split##Gauntlet", ImVec2(100.0f, 25.0f)))
                {
                    if (std::find_if(splitPresets[currentPreset].splits.begin(), splitPresets[currentPreset].splits.end(), [&](const auto& pair)
                        {
                            return pair.first == gkGauntletSplits[gkSplits[1]];
                        }) == splitPresets[currentPreset].splits.end())
                    {
                        splitPresets[currentPreset].splits.push_back({ gkGauntletSplits[gkSplits[1]], addSplitRound });
                        splitPresets[currentPreset].numSplits++;
                        WriteAutosplitPreset(splitPresets[currentPreset]);
                        addSplitView = false;
                    }
                }
                SAMELINE;
                if (CreateListBox("##ZNS WW Splits", gkDragonSplits, gkSplits[2], ImVec2(150.0f, 76.0f)))
                {

                }
                SAMELINE;
                if (CreateButton("Add Split##ZNS WW", ImVec2(100.0f, 25.0f)))
                {
                    if (std::find_if(splitPresets[currentPreset].splits.begin(), splitPresets[currentPreset].splits.end(), [&](const auto& pair)
                        {
                            return pair.first == gkDragonSplits[gkSplits[2]];
                        }) == splitPresets[currentPreset].splits.end())
                    {
                        splitPresets[currentPreset].splits.push_back({ gkDragonSplits[gkSplits[2]], addSplitRound });
                        splitPresets[currentPreset].numSplits++;
                        WriteAutosplitPreset(splitPresets[currentPreset]);
                        addSplitView = false;
                    }
                }
            }
            DummySpace(0.0f, 25.0f);
            // row 2
            {
                if (CreateListBox("##GK Lockdown Splits", gkLockdownSplits, gkSplits[3], ImVec2(150.0f, 51.0f)))
                {

                }
                SAMELINE;
                if (CreateButton("Add Split##GK Lockdown", ImVec2(100.0f, 25.0f)))
                {
                    if (std::find_if(splitPresets[currentPreset].splits.begin(), splitPresets[currentPreset].splits.end(), [&](const auto& pair)
                        {
                            return pair.first == gkLockdownSplits[gkSplits[3]];
                        }) == splitPresets[currentPreset].splits.end())
                    {
                        splitPresets[currentPreset].splits.push_back({ gkLockdownSplits[gkSplits[3]], addSplitRound });
                        splitPresets[currentPreset].numSplits++;
                        WriteAutosplitPreset(splitPresets[currentPreset]);
                        addSplitView = false;
                    }
                }
                SAMELINE;
                if (CreateListBox("##GK Challenge Splits", gkChallengeSplits, gkSplits[4], ImVec2(150.0f, 76.0f)))
                {

                }
                SAMELINE;
                if (CreateButton("Add Split##GK Challenge", ImVec2(100.0f, 25.0f)))
                {
                    if (std::find_if(splitPresets[currentPreset].splits.begin(), splitPresets[currentPreset].splits.end(), [&](const auto& pair)
                        {
                            return pair.first == gkChallengeSplits[gkSplits[4]];
                        }) == splitPresets[currentPreset].splits.end())
                    {
                        splitPresets[currentPreset].splits.push_back({ gkChallengeSplits[gkSplits[4]], addSplitRound });
                        splitPresets[currentPreset].numSplits++;
                        WriteAutosplitPreset(splitPresets[currentPreset]);
                        addSplitView = false;
                    }
                }
            }
            break;
        }
        case 5:
        {
            // row 1
            {
                if (CreateListBox("##Rev Start Splits", revStartSplits, revSplits[0], ImVec2(150.0f, 126.0f)))
                {

                }
                SAMELINE;
                if (CreateButton("Add Split##Rev Start", ImVec2(100.0f, 25.0f)))
                {
                    if (std::find_if(splitPresets[currentPreset].splits.begin(), splitPresets[currentPreset].splits.end(), [&](const auto& pair)
                        {
                            return pair.first == revStartSplits[revSplits[0]];
                        }) == splitPresets[currentPreset].splits.end())
                    {
                        splitPresets[currentPreset].splits.push_back({ revStartSplits[revSplits[0]], addSplitRound });
                        splitPresets[currentPreset].numSplits++;
                        WriteAutosplitPreset(splitPresets[currentPreset]);
                        addSplitView = false;
                    }
                }
                SAMELINE;
                if (CreateListBox("##Reel Splits", revReelSplits, revSplits[1], ImVec2(150.0f, 151.0f)))
                {

                }
                SAMELINE;
                if (CreateButton("Add Split##Reel", ImVec2(100.0f, 25.0f)))
                {
                    if (std::find_if(splitPresets[currentPreset].splits.begin(), splitPresets[currentPreset].splits.end(), [&](const auto& pair)
                        {
                            return pair.first == revReelSplits[revSplits[1]];
                        }) == splitPresets[currentPreset].splits.end())
                    {
                        splitPresets[currentPreset].splits.push_back({ revReelSplits[revSplits[1]], addSplitRound });
                        splitPresets[currentPreset].numSplits++;
                        WriteAutosplitPreset(splitPresets[currentPreset]);
                        addSplitView = false;
                    }
                }
                SAMELINE;
                if (CreateListBox("##Rev Egg Splits", revEggSplits, revSplits[2], ImVec2(150.0f, 126.0f)))
                {

                }
                SAMELINE;
                if (CreateButton("Add Split##Rev Egg", ImVec2(100.0f, 25.0f)))
                {
                    if (std::find_if(splitPresets[currentPreset].splits.begin(), splitPresets[currentPreset].splits.end(), [&](const auto& pair)
                        {
                            return pair.first == revEggSplits[revSplits[2]];
                        }) == splitPresets[currentPreset].splits.end())
                    {
                        splitPresets[currentPreset].splits.push_back({ revEggSplits[revSplits[2]], addSplitRound });
                        splitPresets[currentPreset].numSplits++;
                        WriteAutosplitPreset(splitPresets[currentPreset]);
                        addSplitView = false;
                    }
                }
            }
            DummySpace(0.0f, 25.0f);
            // row 2
            {
                if (CreateListBox("##Rev Rune Splits", revRuneSplits, revSplits[3], ImVec2(150.0f, 126.0f)))
                {

                }
                SAMELINE;
                if (CreateButton("Add Split##Rev Rune", ImVec2(100.0f, 25.0f)))
                {
                    if (std::find_if(splitPresets[currentPreset].splits.begin(), splitPresets[currentPreset].splits.end(), [&](const auto& pair)
                        {
                            return pair.first == revRuneSplits[revSplits[3]];
                        }) == splitPresets[currentPreset].splits.end())
                    {
                        splitPresets[currentPreset].splits.push_back({ revRuneSplits[revSplits[3]], addSplitRound });
                        splitPresets[currentPreset].numSplits++;
                        WriteAutosplitPreset(splitPresets[currentPreset]);
                        addSplitView = false;
                    }
                }
                SAMELINE;
                if (CreateListBox("##Rev End Splits", revEndSplits, revSplits[4], ImVec2(150.0f, 76.0f)))
                {

                }
                SAMELINE;
                if (CreateButton("Add Split##Rev End", ImVec2(100.0f, 25.0f)))
                {
                    if (std::find_if(splitPresets[currentPreset].splits.begin(), splitPresets[currentPreset].splits.end(), [&](const auto& pair)
                        {
                            return pair.first == revEndSplits[revSplits[4]];
                        }) == splitPresets[currentPreset].splits.end())
                    {
                        splitPresets[currentPreset].splits.push_back({ revEndSplits[revSplits[4]], addSplitRound });
                        splitPresets[currentPreset].numSplits++;
                        WriteAutosplitPreset(splitPresets[currentPreset]);
                        addSplitView = false;
                    }
                }
            }
            break;
        }
        case 12:
        {
            ImGui::Text("Coming Soon");
            break;
        }
        case 13:
        {
            // row 1
            {
                if (CreateListBox("##Tomb Staff Splits", tombStaffSplits, tombSplits[0], ImVec2(150.0f, 101.0f)))
                {

                }
                SAMELINE;
                if (CreateButton("Add Split##Tomb Staff", ImVec2(100.0f, 25.0f)))
                {
                    if (std::find_if(splitPresets[currentPreset].splits.begin(), splitPresets[currentPreset].splits.end(), [&](const auto& pair)
                        {
                            return pair.first == tombStaffSplits[tombSplits[0]];
                        }) == splitPresets[currentPreset].splits.end())
                    {
                        splitPresets[currentPreset].splits.push_back({ tombStaffSplits[tombSplits[0]], addSplitRound });
                        splitPresets[currentPreset].numSplits++;
                        WriteAutosplitPreset(splitPresets[currentPreset]);
                        addSplitView = false;
                    }
                }
                SAMELINE;
                if (CreateListBox("##Tomb End Splits", tombEndSplits, tombSplits[1], ImVec2(150.0f, 76.0f)))
                {

                }
                SAMELINE;
                if (CreateButton("Add Split##Tomb End", ImVec2(100.0f, 25.0f)))
                {
                    if (std::find_if(splitPresets[currentPreset].splits.begin(), splitPresets[currentPreset].splits.end(), [&](const auto& pair)
                        {
                            return pair.first == tombEndSplits[tombSplits[1]];
                        }) == splitPresets[currentPreset].splits.end())
                    {
                        splitPresets[currentPreset].splits.push_back({ tombEndSplits[tombSplits[1]], addSplitRound });
                        splitPresets[currentPreset].numSplits++;
                        WriteAutosplitPreset(splitPresets[currentPreset]);
                        addSplitView = false;
                    }
                }
            }
            break;
        }
        default:
            ImGui::Text("No additional splits supported for this map");
            break;
        }
    }
    else
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));
        ImGui::BeginGroup();
        if (CreateButton(ICON_FA_FILE_CIRCLE_PLUS " New Preset", ImVec2(150.0f, 25.0f)))
            ImGui::OpenPopup("New Autosplits Preset");
        if (CreateButton(ICON_FA_FILE_CIRCLE_MINUS " Delete Preset", ImVec2(150.0f, 25.0f)))
        {
            DeleteAutosplitPreset(splitPresets[currentPreset].presetName);
            WriteAutosplitPreset(splitPresets[currentPreset]);
        } ImGui::EndGroup();
        SAMELINE;
        ImGui::PopStyleVar();
        ImGui::BeginGroup();
        // new preset creation
        if (ImGui::BeginPopup("New Autosplits Preset"))
        {
            ImGui::SetKeyboardFocusHere();
            char presetInput[32] = "";
            ImGui::SetNextItemWidth(200);
            if (ImGui::InputText("New Preset Name", presetInput, IM_ARRAYSIZE(presetInput), ImGuiInputTextFlags_EnterReturnsTrue))
            {
                if (std::string(presetInput).empty() || DoesPathExist(presetInput))
                {
                    showAutosplitPresetExists = true;
                    autosplitPopupBoolCheck = true;
                }
                else
                    CreateNewAutosplitPreset(presetInput);
                ImGui::CloseCurrentPopup();
            }
            if (showAutosplitPresetExists)
            {
                if (autosplitPopupBoolCheck)
                {
                    ImGui::OpenPopup("Autosplit Preset Already Exists");
                    autosplitPopupBoolCheck = false;
                }
                ImVec2 windowPos = ImGui::GetWindowPos();
                ImVec2 windowSize = ImGui::GetWindowSize();
                ImGui::SetNextWindowPos(ImVec2(windowPos.x + windowSize.x / 2 - ImGui::CalcItemWidth() / 3, windowPos.y + 120), ImGuiCond_Always);
                if (ImGui::BeginPopupModal("Autosplit Preset Already Exists", &showAutosplitPresetExists, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize))
                {
                    ImGui::TextWrapped("Autosplit Preset Already Exists");
                    if (ImGui::Button("Close", ImVec2(120.0f, 25.0f)))
                        ImGui::CloseCurrentPopup();
                    ImGui::EndPopup();
                }
            }
            ImGui::EndPopup();
        }
        // presets dropdown
        ImGui::SetNextItemWidth(250);
        std::string previousPreset = splitPresets[currentPreset].presetName;
        if (ImGui::BeginCombo("Autosplit Presets", splitPresets[currentPreset].presetName.c_str(), ImGuiComboFlags_HeightRegular))
        {
            for (int i = 0; i < splitPresets.size(); i++)
            {
                const bool is_selected = currentPreset == i;
                if (ImGui::Selectable(splitPresets[i].presetName.c_str(), is_selected))
                    currentPreset = i;
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        SAMELINE;
        if (previousPreset != splitPresets[currentPreset].presetName)
            WriteAutosplitPreset(splitPresets[currentPreset]);
        if (appStatus != "Status: Active")
            ImGui::BeginDisabled();
        if (ImGui::Checkbox("Active", &writeSplits))
        {
            WriteAutosplitPreset(splitPresets[currentPreset]);
        }
        if (appStatus != "Status: Active")
            ImGui::EndDisabled();
        ImGui::EndGroup();
        if (splitPresets[0].presetName != "No Presets Available")
        {
            SAMELINE;
            if (ImGui::Checkbox("In Game Timer", &splitPresets[currentPreset].igt))
            {
                WriteAutosplitPreset(splitPresets[currentPreset]);
            }
            ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 160.0f, ImGui::GetCursorPosY() - 30.0f));
            ImGui::BeginGroup();
            if (ImGui::Checkbox("In Game Round Timer", &splitPresets[currentPreset].igrt))
            {
                WriteAutosplitPreset(splitPresets[currentPreset]);
            }
            SAMELINE;
            ImGui::SetNextItemWidth(200);
            int previousMap = splitPresets[currentPreset].map;
            if (ImGui::BeginCombo("##Select A Map", MapOptions::mapList[splitPresets[currentPreset].map].c_str()))
            {
                for (int i = 0; i < MapOptions::mapList.size(); i++)
                {
                    const bool is_selected = splitPresets[currentPreset].map == i;
                    if (ImGui::Selectable(MapOptions::mapList[i].c_str(), is_selected))
                        splitPresets[currentPreset].map = i;
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            if (previousMap != splitPresets[currentPreset].map)
            {
                if (splitPresets[currentPreset].splits.size())
                {
                    showChangeMapError = true;
                    mapError = splitPresets[currentPreset].map;
                    splitPresets[currentPreset].map = previousMap;
                    ImGui::OpenPopup("Change Map Error");
                }
                else
                    WriteAutosplitPreset(splitPresets[currentPreset]);
            }
            SAMELINE;
            ImGui::SetNextItemWidth(250);
            int previousType = splitPresets[currentPreset].splitType;
            if (ImGui::BeginCombo("##Select Split Type", generalSplitData[splitPresets[currentPreset].splitType].c_str()))
            {
                for (int i = 0; i < generalSplitData.size(); i++)
                {
                    const bool is_selected = splitPresets[currentPreset].splitType == i;
                    if (ImGui::Selectable(generalSplitData[i].c_str(), is_selected))
                        splitPresets[currentPreset].splitType = i;
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            if (previousType != splitPresets[currentPreset].splitType)
                WriteAutosplitPreset(splitPresets[currentPreset]);
            SAMELINE;

            HelpMarker(R"(How to use:
- Select if you want an in game timer (displays total game time with precision down to the second)
- Select if you want an in game round timer (displays current round time in seconds::milliseconds)
- Select the map to choose the splits for (defaults to SOE)
- Select the type of autosplits you want to setup. This determines the ending point of the splits. If you choose "Egg Autosplit" and add no other splits, it will only split once the egg is completed
- If you've selected "Split on every X Round", a text box will appear for you to type the interval into)");

            if (showChangeMapError)
            {
                ImVec2 windowPos = ImGui::GetWindowPos();
                ImVec2 windowSize = ImGui::GetWindowSize();
                ImGui::SetNextWindowPos(ImVec2(windowPos.x + windowSize.x / 2 - ImGui::CalcItemWidth() / 3, windowPos.y + 120), ImGuiCond_Always);
                if (ImGui::BeginPopupModal("Change Map Error", &showChangeMapError, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize))
                {
                    ImGui::TextWrapped("Changing the map will invalidate the current selected extra splits. Press continue to delete the current splits or press close and create a new layout instead.");
                    if (ImGui::Button("Continue", ImVec2(120.0f, 25.0f)))
                    {
                        splitPresets[currentPreset].map = mapError;
                        splitPresets[currentPreset].splits = {  };
                        splitPresets[currentPreset].numSplits = 0;
                        WriteAutosplitPreset(splitPresets[currentPreset]);
                        ImGui::CloseCurrentPopup();
                    }
                    SAMELINE;
                    if (ImGui::Button("Close", ImVec2(120.0f, 25.0f)))
                        ImGui::CloseCurrentPopup();
                    ImGui::EndPopup();
                }
            }

            ImGui::EndGroup();
            if (ImGui::BeginTable("Splits", 4, ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_Borders))
            {
                // lol idk how else to center names in a header
                ImGui::TableSetupColumn("                       Split Names", ImGuiTableColumnFlags_WidthFixed, 300.0f);
                ImGui::TableSetupColumn("     Round", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                ImGui::TableSetupColumn("Add/Remove", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                ImGui::TableSetupColumn("Move Layer", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                ImGui::TableHeadersRow();

                for (int row = 0; row < splitPresets[currentPreset].numSplits + 1; row++)
                {
                    ImGui::TableNextRow();
                    for (int column = 0; column < 4; column++)
                    {
                        ImGui::TableSetColumnIndex(column);

                        if (row < splitPresets[currentPreset].numSplits)
                        {
                            switch (column)
                            {
                            case 0:
                            {
                                ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x / 2 - ImGui::CalcTextSize(splitPresets[currentPreset].splits[row].first.c_str()).x / 2, ImGui::GetCursorPosY() + 2.5f));
                                ImGui::Text(splitPresets[currentPreset].splits[row].first.c_str());
                                break;
                            }
                            case 1:
                            {
                                ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x / 2 - ImGui::CalcTextSize(std::to_string(splitPresets[currentPreset].splits[row].second).c_str()).x / 2, ImGui::GetCursorPosY() + 2.5f));
                                ImGui::Text(std::to_string(splitPresets[currentPreset].splits[row].second).c_str());
                                break;
                            }
                            case 2:
                            {
                                if (ImGui::Button(std::string(ICON_FA_CIRCLE_MINUS " Remove##" + std::to_string(row)).c_str(), ImVec2(100.0f, 25.0f)))
                                {
                                    splitPresets[currentPreset].splits.erase(splitPresets[currentPreset].splits.begin() + row);
                                    splitPresets[currentPreset].numSplits--;
                                    WriteAutosplitPreset(splitPresets[currentPreset]);
                                    row--;
                                }
                                break;
                            }
                            case 3:
                            {
                                if (row != splitPresets[currentPreset].numSplits - 1)
                                {
                                    if (ImGui::Button(std::string(ICON_FA_CIRCLE_ARROW_DOWN "##MoveSplit" + std::to_string(row)).c_str(), ImVec2(45.0f, 25.0f)))
                                    {
                                        std::iter_swap(splitPresets[currentPreset].splits.begin() + row, splitPresets[currentPreset].splits.begin() + row + 1);
                                        WriteAutosplitPreset(splitPresets[currentPreset]);
                                    }
                                    SAMELINE;
                                }
                                if (row != 0)
                                {
                                    if (row == splitPresets[currentPreset].numSplits - 1)
                                    {
                                        DummySpace(45.0f, 0.0f);
                                        SAMELINE;
                                    }
                                    if (ImGui::Button(std::string(ICON_FA_CIRCLE_ARROW_UP "##MoveSplit" + std::to_string(row)).c_str(), ImVec2(45.0f, 25.0f)))
                                    {
                                        std::iter_swap(splitPresets[currentPreset].splits.begin() + row, splitPresets[currentPreset].splits.begin() + row - 1);
                                        WriteAutosplitPreset(splitPresets[currentPreset]);
                                    }
                                }
                                break;
                            }
                            default:
                                break;
                            }
                        }
                        else
                        {
                            switch (column)
                            {
                            case 2:
                            {
                                if (ImGui::Button(std::string(ICON_FA_CIRCLE_PLUS " Add##" + std::to_string(row)).c_str(), ImVec2(100.0f, 25.0f)))
                                {
                                    addSplitView = true;
                                }
                            }
                            default:
                                break;
                            }
                        }
                    }
                }

                ImGui::EndTable();
            }
            if (splitPresets[currentPreset].splitType == 1)
            {
                SAMELINE;
                ImGui::SetNextItemWidth(150);
                if (ImGui::InputInt("Interval", &splitPresets[currentPreset].roundInterval))
                {
                    if (splitPresets[currentPreset].roundInterval < 1)
                        splitPresets[currentPreset].roundInterval = 1;
                    else if (splitPresets[currentPreset].roundInterval > 255)
                        splitPresets[currentPreset].roundInterval = 255;
                    WriteAutosplitPreset(splitPresets[currentPreset]);
                }
            }
        }
    }
}

void PracticePatchesPtr()
{
    ImGui::BeginGroup();
    {
        ImGui::Text(ICON_FA_LOCATION_DOT " Shadows of Evil");
        if (CreateListBox("##SOE", zodPracticeList, practicePatchIndexes[0], ImVec2(200.0f, 200.0f)))
        {
            if (appStatus == "Status: Active")
                WritePracticePatches(practicePatchIndexes);
            else
                WritePracticePatches(inactivePracticePatchIndexes);
        }
    }
    ImGui::EndGroup();
    SAMELINE;
    DummySpace(15.0f, 0.0f);
    SAMELINE;
    ImGui::BeginGroup();
    {
        ImGui::Text(ICON_FA_LOCATION_DOT " The Giant");
        if (CreateListBox("##Giant", factoryPracticeList, practicePatchIndexes[1], ImVec2(200.0f, 200.0f)))
        {
            if (appStatus == "Status: Active")
                WritePracticePatches(practicePatchIndexes);
            else
                WritePracticePatches(inactivePracticePatchIndexes);
        }
    }
    ImGui::EndGroup();
    SAMELINE;
    DummySpace(15.0f, 0.0f);
    SAMELINE;
    ImGui::BeginGroup();
    {
        ImGui::Text(ICON_FA_LOCATION_DOT " Der Eisendrache");
        if (CreateListBox("##DE", castlePracticeList, practicePatchIndexes[2], ImVec2(200.0f, 200.0f)))
        {
            if (appStatus == "Status: Active")
                WritePracticePatches(practicePatchIndexes);
            else
                WritePracticePatches(inactivePracticePatchIndexes);
        }
    }
    ImGui::EndGroup();
    SAMELINE;
    DummySpace(15.0f, 0.0f);
    SAMELINE;
    ImGui::BeginGroup();
    {
        ImGui::Text(ICON_FA_LOCATION_DOT " Zetsubou No Shima");
        if (CreateListBox("##ZNS", islandPracticeList, practicePatchIndexes[3], ImVec2(200.0f, 200.0f)))
        {
            if (appStatus == "Status: Active")
                WritePracticePatches(practicePatchIndexes);
            else
                WritePracticePatches(inactivePracticePatchIndexes);
        }
    }
    ImGui::EndGroup();
    ImGui::BeginGroup();
    {
        ImGui::Text(ICON_FA_LOCATION_DOT " Gorod Krovi");
        if (CreateListBox("##GK", stalingradPracticeList, practicePatchIndexes[4], ImVec2(200.0f, 200.0f)))
        {
            if (appStatus == "Status: Active")
                WritePracticePatches(practicePatchIndexes);
            else
                WritePracticePatches(inactivePracticePatchIndexes);
        }
    }
    ImGui::EndGroup();
    SAMELINE;
    DummySpace(15.0f, 0.0f);
    SAMELINE;
    ImGui::BeginGroup();
    {
        ImGui::Text(ICON_FA_LOCATION_DOT " Revelations");
        if (CreateListBox("##Rev", genesisPracticeList, practicePatchIndexes[5], ImVec2(200.0f, 200.0f)))
        {
            if (appStatus == "Status: Active")
                WritePracticePatches(practicePatchIndexes);
            else
                WritePracticePatches(inactivePracticePatchIndexes);
        }
    }
    ImGui::EndGroup();
    SAMELINE;
    DummySpace(15.0f, 0.0f);
    SAMELINE;
    {
        ImGui::BeginGroup();
        ImGui::Text(ICON_FA_LOCATION_DOT " Moon");
        if (CreateListBox("##Moon", moonPracticeList, practicePatchIndexes[6], ImVec2(200.0f, 200.0f)))
        {
            if (appStatus == "Status: Active")
                WritePracticePatches(practicePatchIndexes);
            else
                WritePracticePatches(inactivePracticePatchIndexes);
        }
    }
    ImGui::EndGroup();
    SAMELINE;
    DummySpace(15.0f, 0.0f);
    SAMELINE;
    {
        ImGui::BeginGroup();
        ImGui::Text(ICON_FA_LOCATION_DOT " Origins");
        if (CreateListBox("##Origins", tombPracticeList, practicePatchIndexes[7], ImVec2(200.0f, 200.0f)))
        {
            if (appStatus == "Status: Active")
                WritePracticePatches(practicePatchIndexes);
            else
                WritePracticePatches(inactivePracticePatchIndexes);
        }
    }
    ImGui::EndGroup();
}

void SettingsPtr()
{
    ImGui::PushFont(titleFont);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x / 2 - ImGui::CalcTextSize("Keybinds").x / 2);
    ImGui::Text("Keybinds");
    ImGui::PopFont();
    ImGui::BeginGroup();
    int count = 0;
    float cursorPosY = ImGui::GetCursorPosY();
    for (std::pair<const std::string, HotKeyBind>& hotkey : hotkeyDefs)
    {
        ImVec2 size = { max(150.0f, ImGui::CalcTextSize(hotkey.second.keyNames.c_str()).x), 25.0f };
        if (count > 12)
        {
            ImGui::SetCursorPosX(375.0f);
            ImGui::Text(hotkey.first.c_str());
            SAMELINE;
            ImGui::SetCursorPosX(575.0f);
            if (CreateButton(hotkey.second.keyNames + "##" + hotkey.first, size) && !registerHotKey)
            {
                AssignHotKey(hotkey.first, hotkey);
            }
            count++;
            continue;
        }
        ImGui::Text(hotkey.first.c_str());
        SAMELINE;
        ImGui::SetCursorPosX(200.0f);
        if (CreateButton(hotkey.second.keyNames + "##" + hotkey.first, size) && !registerHotKey)
        {
            AssignHotKey(hotkey.first, hotkey);
        }
        count++;
        if (count > 12)
        {
            ImGui::EndGroup();
            ImGui::SetCursorPosY(cursorPosY);
            ImGui::BeginGroup();
        }
    }
    ImGui::EndGroup();
}

void PlayerOptionsPtr()
{
    if (ImGui::BeginTabBar("Player Options Tabs"))
    {
        if (ImGui::BeginTabItem(ICON_FA_GEAR " General Options"))
        {
            if (CreateButton("Godmode", ImVec2(100.0f, 25.0f), &PlayerOptions::godActive, true, fakeColor, true))
                NotifyGame({ 0, 11, PlayerOptions::godActive });
            SAMELINE;
            if (CreateButton("Infinite Ammo", ImVec2(150.0f, 25.0f), &PlayerOptions::infAmmoActive, true, fakeColor, true))
                NotifyGame({ 0, 12, PlayerOptions::infAmmoActive });
            SAMELINE;
            ImGui::SetNextItemWidth(175.0f);
            if (ImGui::SliderInt("Timescale", &PlayerOptions::timescaleInt, 1, 10))
                NotifyGame({ 0, 13, PlayerOptions::timescaleInt });
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem(ICON_FA_GEAR " Weapon Options"))
        {
            if (CreateButton(ICON_FA_CIRCLE_PLUS " Give Selected Weapon", ImVec2(200.0f, 25.0f)))
            {
                std::vector<int> notify = { 0, 1 };
                std::vector weaponNums = GetWeaponIndex(currentMap, weaponSelectName);
                notify.insert(notify.end(), weaponNums.begin(), weaponNums.end());
                notify.push_back(Weapons::upgradedWeapon);
                NotifyGame(notify);
            }
            SAMELINE;
            if (CreateButton(ICON_FA_CIRCLE_MINUS " Take Current Weapon", ImVec2(200.0f, 25.0f)))
                NotifyGame({ 0, 2, 0 });
            SAMELINE;
            if (CreateButton(ICON_FA_CIRCLE_MINUS " Take Alt Weapon", ImVec2(200.0f, 25.0f)))
                NotifyGame({ 0, 2, 7 });
            SAMELINE;
            if (CreateButton(ICON_FA_CIRCLE_MINUS " Take Hero Weapon", ImVec2(200.0f, 25.0f)))
                NotifyGame({ 0, 2, 6 });
            SAMELINE;
            ImGui::Checkbox("Upgraded", &Weapons::upgradedWeapon);
            if (CreateButton(ICON_FA_CIRCLE_MINUS " Take Lethals", ImVec2(158.0f, 25.0f)))
                NotifyGame({ 0, 2, 1 });
            SAMELINE;
            if (CreateButton(ICON_FA_CIRCLE_MINUS " Take Tacticals", ImVec2(158.0f, 25.0f)))
                NotifyGame({ 0, 2, 2 });
            SAMELINE;
            if (CreateButton(ICON_FA_CIRCLE_MINUS " Take Shield Slot", ImVec2(158.0f, 25.0f)))
                NotifyGame({ 0, 2, 3 });
            SAMELINE;
            if (CreateButton(ICON_FA_CIRCLE_MINUS " Reset Melee", ImVec2(158.0f, 25.0f)))
                NotifyGame({ 0, 2, 4 });
            SAMELINE;
            if (CreateButton(ICON_FA_CIRCLE_MINUS " Take Mine Slot", ImVec2(158.0f, 25.0f)))
                NotifyGame({ 0, 2, 5 });
            // Can't use API helper function for listboxes because we need custom control over the selection variable
            // All listboxes for the weapons will act as a single listbox over several listbox containers
            ImGui::BeginChild(ImGui::GetID("Weapon List 1"), ImVec2(310.0f, ImGui::GetContentRegionAvail().y - 40.0f));
            {
                if (ImGui::CollapsingHeader("Assault Rifles"))
                {
                    if (ImGui::ListBoxHeader("##ARs", ImVec2(300, Weapons::weaponList[currentMap + "_ar"].size() * 25 + 1.0f)))
                    {
                        for (int i = 0; i < Weapons::weaponList[currentMap + "_ar"].size(); i++)
                        {
                            const bool is_selected = (weaponSelectName == Weapons::weaponList[currentMap + "_ar"][i]);
                            if (ImGui::Selectable(Weapons::weaponList[currentMap + "_ar"][i].c_str(), is_selected))
                                weaponSelectName = Weapons::weaponList[currentMap + "_ar"][i];
                            if (is_selected)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::ListBoxFooter();
                    }
                }
                if (ImGui::CollapsingHeader("Submachine Guns"))
                {
                    if (ImGui::ListBoxHeader("##SMGs", ImVec2(300, Weapons::weaponList[currentMap + "_smg"].size() * 25 + 1.0f)))
                    {
                        for (int i = 0; i < Weapons::weaponList[currentMap + "_smg"].size(); i++)
                        {
                            const bool is_selected = (weaponSelectName == Weapons::weaponList[currentMap + "_smg"][i]);
                            if (ImGui::Selectable(Weapons::weaponList[currentMap + "_smg"][i].c_str(), is_selected))
                                weaponSelectName = Weapons::weaponList[currentMap + "_smg"][i];
                            if (is_selected)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::ListBoxFooter();
                    }
                }
                if (ImGui::CollapsingHeader("Light Machine Guns"))
                {
                    if (ImGui::ListBoxHeader("##LMGs", ImVec2(300, Weapons::weaponList[currentMap + "_lmg"].size() * 25 + 1.0f)))
                    {
                        for (int i = 0; i < Weapons::weaponList[currentMap + "_lmg"].size(); i++)
                        {
                            const bool is_selected = (weaponSelectName == Weapons::weaponList[currentMap + "_lmg"][i]);
                            if (ImGui::Selectable(Weapons::weaponList[currentMap + "_lmg"][i].c_str(), is_selected))
                                weaponSelectName = Weapons::weaponList[currentMap + "_lmg"][i];
                            if (is_selected)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::ListBoxFooter();
                    }
                }
                if (ImGui::CollapsingHeader("Shotguns"))
                {
                    if (ImGui::ListBoxHeader("##Shotguns", ImVec2(300, Weapons::weaponList[currentMap + "_shotgun"].size() * 25 + 1.0f)))
                    {
                        for (int i = 0; i < Weapons::weaponList[currentMap + "_shotgun"].size(); i++)
                        {
                            const bool is_selected = (weaponSelectName == Weapons::weaponList[currentMap + "_shotgun"][i]);
                            if (ImGui::Selectable(Weapons::weaponList[currentMap + "_shotgun"][i].c_str(), is_selected))
                                weaponSelectName = Weapons::weaponList[currentMap + "_shotgun"][i];
                            if (is_selected)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::ListBoxFooter();
                    }
                }
            }
            ImGui::EndChild();
            SAMELINE;
            ImGui::BeginChild(ImGui::GetID("Weapon List 2"), ImVec2(310.0f, ImGui::GetContentRegionAvail().y - 40.0f));
            {
                if (ImGui::CollapsingHeader("Snipers"))
                {
                    if (ImGui::ListBoxHeader("##Snipers", ImVec2(300, Weapons::weaponList[currentMap + "_sniper"].size() * 25 + 1.0f)))
                    {
                        for (int i = 0; i < Weapons::weaponList[currentMap + "_sniper"].size(); i++)
                        {
                            const bool is_selected = (weaponSelectName == Weapons::weaponList[currentMap + "_sniper"][i]);
                            if (ImGui::Selectable(Weapons::weaponList[currentMap + "_sniper"][i].c_str(), is_selected))
                                weaponSelectName = Weapons::weaponList[currentMap + "_sniper"][i];
                            if (is_selected)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::ListBoxFooter();
                    }
                }
                if (ImGui::CollapsingHeader("Pistols"))
                {
                    if (ImGui::ListBoxHeader("##Pistols", ImVec2(300, Weapons::weaponList[currentMap + "_pistol"].size() * 25 + 1.0f)))
                    {
                        for (int i = 0; i < Weapons::weaponList[currentMap + "_pistol"].size(); i++)
                        {
                            const bool is_selected = (weaponSelectName == Weapons::weaponList[currentMap + "_pistol"][i]);
                            if (ImGui::Selectable(Weapons::weaponList[currentMap + "_pistol"][i].c_str(), is_selected))
                                weaponSelectName = Weapons::weaponList[currentMap + "_pistol"][i];
                            if (is_selected)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::ListBoxFooter();
                    }
                }
                if (ImGui::CollapsingHeader("Launchers"))
                {
                    if (ImGui::ListBoxHeader("##Launchers", ImVec2(300, Weapons::weaponList[currentMap + "_launcher"].size() * 25 + 1.0f)))
                    {
                        for (int i = 0; i < Weapons::weaponList[currentMap + "_launcher"].size(); i++)
                        {
                            const bool is_selected = (weaponSelectName == Weapons::weaponList[currentMap + "_launcher"][i]);
                            if (ImGui::Selectable(Weapons::weaponList[currentMap + "_launcher"][i].c_str(), is_selected))
                                weaponSelectName = Weapons::weaponList[currentMap + "_launcher"][i];
                            if (is_selected)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::ListBoxFooter();
                    }
                }
                if (ImGui::CollapsingHeader("Melee"))
                {
                    if (ImGui::ListBoxHeader("##Melee", ImVec2(300, Weapons::weaponList[currentMap + "_melee"].size() * 25 + 1.0f)))
                    {
                        for (int i = 0; i < Weapons::weaponList[currentMap + "_melee"].size(); i++)
                        {
                            const bool is_selected = (weaponSelectName == Weapons::weaponList[currentMap + "_melee"][i]);
                            if (ImGui::Selectable(Weapons::weaponList[currentMap + "_melee"][i].c_str(), is_selected))
                                weaponSelectName = Weapons::weaponList[currentMap + "_melee"][i];
                            if (is_selected)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::ListBoxFooter();
                    }
                }
            }
            ImGui::EndChild();
            SAMELINE;
            ImGui::BeginChild(ImGui::GetID("Weapon List 3"), ImVec2(310.0f, ImGui::GetContentRegionAvail().y - 40.0f));
            {
                if (ImGui::CollapsingHeader("Special"))
                {
                    if (ImGui::ListBoxHeader("##Special", ImVec2(300, Weapons::weaponList[currentMap + "_special"].size() * 25 + 1.0f)))
                    {
                        for (int i = 0; i < Weapons::weaponList[currentMap + "_special"].size(); i++)
                        {
                            const bool is_selected = (weaponSelectName == Weapons::weaponList[currentMap + "_special"][i]);
                            if (ImGui::Selectable(Weapons::weaponList[currentMap + "_special"][i].c_str(), is_selected))
                                weaponSelectName = Weapons::weaponList[currentMap + "_special"][i];
                            if (is_selected)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::ListBoxFooter();
                    }
                }
                if (ImGui::CollapsingHeader("Equipment"))
                {
                    if (ImGui::ListBoxHeader("##Equipment", ImVec2(300, Weapons::weaponList[currentMap + "_equipment"].size() * 25 + 1.0f)))
                    {
                        for (int i = 0; i < Weapons::weaponList[currentMap + "_equipment"].size(); i++)
                        {
                            const bool is_selected = (weaponSelectName == Weapons::weaponList[currentMap + "_equipment"][i]);
                            if (ImGui::Selectable(Weapons::weaponList[currentMap + "_equipment"][i].c_str(), is_selected))
                                weaponSelectName = Weapons::weaponList[currentMap + "_equipment"][i];
                            if (is_selected)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::ListBoxFooter();
                    }
                }
                if (ImGui::CollapsingHeader("Hero"))
                {
                    if (ImGui::ListBoxHeader("##Hero", ImVec2(300, Weapons::weaponList[currentMap + "_hero"].size() * 25 + 1.0f)))
                    {
                        for (int i = 0; i < Weapons::weaponList[currentMap + "_hero"].size(); i++)
                        {
                            const bool is_selected = (weaponSelectName == Weapons::weaponList[currentMap + "_hero"][i]);
                            if (ImGui::Selectable(Weapons::weaponList[currentMap + "_hero"][i].c_str(), is_selected))
                                weaponSelectName = Weapons::weaponList[currentMap + "_hero"][i];
                            if (is_selected)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::ListBoxFooter();
                    }
                }
            }
            ImGui::EndChild();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem(ICON_FA_GEAR " Point Options"))
        {
            if (CreateButton(ICON_FA_CIRCLE_PLUS " Max Score", ImVec2(125.0f, 25.0f)))
                NotifyGame({ 0, 0, 4194303 });
            SAMELINE;
            if (CreateButton(ICON_FA_CIRCLE_MINUS " Reset Score", ImVec2(125.0f, 25.0f)))
                NotifyGame({ 0, 0, 500 });
            ImGui::SetNextItemWidth(200.0f);
            if (ImGui::InputInt("Set Points", &pointInput, 1000, 10000, ImGuiInputTextFlags_EnterReturnsTrue))
            {
                if (pointInput < 0)
                    pointInput = 0;
                else if (pointInput > 4194303)
                    pointInput = 4194303;
            }
            SAMELINE;
            if (CreateButton(ICON_FA_ARROW_RIGHT " Send##Point Input", ImVec2(80.0f, 25.0f)))
                NotifyGame({ 0, 0, pointInput });
            SAMELINE;
            HelpMarker("Increment +- 1000\nCtrl + Click increment +- 10000");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem(ICON_FA_GEAR " Perk Options"))
        {
            if (CreateButton(ICON_FA_CIRCLE_PLUS " Give All Perks", ImVec2(145.f, 25.f)))
                NotifyGame({ 0, 7 });
            SAMELINE;
            if (CreateButton(ICON_FA_CIRCLE_MINUS " Take All Perks", ImVec2(145.f, 25.f)))
                NotifyGame({ 0, 8 });
            if (CreateButton(ICON_FA_CIRCLE_PLUS " Give Perk", ImVec2(145.f, 25.f)))
                NotifyGame({ 0, 9, perkSelectIndex });
            SAMELINE;
            if (CreateButton(ICON_FA_CIRCLE_MINUS " Take Perk", ImVec2(145.f, 25.f)))
                NotifyGame({ 0, 10, perkSelectIndex });
            CreateListBox("##Perk List", Perks::perksList[currentMap], perkSelectIndex, ImVec2(300.f, 300.f));
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem(ICON_FA_GEAR " Gobblegum Options"))
        {
            if (CreateButton(ICON_FA_CIRCLE_PLUS " Give Classic Gum", ImVec2(175.0f, 25.0f)))
                NotifyGame({ 0, 3, gobblegumClassicSelectIndex });
            SAMELINE;
            if (CreateButton(ICON_FA_CIRCLE_PLUS " Give Mega Gum", ImVec2(175.0f, 25.0f)))
                NotifyGame({ 0, 3, gobblegumMegaSelectIndex + 19 });
            SAMELINE;
            if (CreateButton(ICON_FA_CIRCLE_MINUS " Take Gum", ImVec2(125.0f, 25.0f)))
                NotifyGame({ 0, 4 });
            SAMELINE;
            if (CreateButton(ICON_FA_CIRCLE_MINUS " Take Gum Charge", ImVec2(175.0f, 25.0f)))
                NotifyGame({ 0, 5 });
            SAMELINE;
            if (CreateButton(ICON_FA_CIRCLE_MINUS " Activate Gum", ImVec2(175.0f, 25.0f)))
                NotifyGame({ 0, 6 });
            CreateListBox("##Classic Gums List", BGB::classicList, gobblegumClassicSelectIndex, ImVec2(300.0f, ImGui::GetContentRegionAvail().y - 40.0f));
            SAMELINE;
            CreateListBox("##Mega Gums List", BGB::megaList, gobblegumMegaSelectIndex, ImVec2(300.0f, ImGui::GetContentRegionAvail().y - 40.0f));
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
}

void ZombieOptionsPtr()
{
    if (CreateButton("Ignore Players", ImVec2(140.0F, 25.0f), &ZombieOptions::zombiesIgnore, true, fakeColor, true))
        NotifyGame({ 1, 0, ZombieOptions::zombiesIgnore });
    SAMELINE;
    if (CreateButton("Kill Horde", ImVec2(140.0F, 25.0f), NULL))
        NotifyGame({ 1, 1 });
    SAMELINE;
    if (CreateButton("Freeze Zombies", ImVec2(140.0F, 25.0f), &ZombieOptions::zombiesFreeze, true, fakeColor, true))
        NotifyGame({ 1, 2, ZombieOptions::zombiesFreeze });
    SAMELINE;
    if (CreateButton("Toggle Spawning", ImVec2(140.0F, 25.0f), &ZombieOptions::zombiesSpawn, true, fakeColor, true))
        NotifyGame({ 1, 3, ZombieOptions::zombiesSpawn });
    DummySpace(0, 5);
    ImGui::Text("Zombie Speed Options");
    SAMELINE;
    HelpMarker("Only 1 option may be active at once");
    DummySpace(0, 5);

    if (CreateButton("Walk", ImVec2(100.0f, 25.0f), &ZombieOptions::zombieSpeedWalk, true, fakeColor, true))
    {
        ZombieOptions::zombieSpeedRun = false;
        ZombieOptions::zombieSpeedSprint = false;
        if (ZombieOptions::zombieSpeedWalk)
            NotifyGame({ 1, 4, 0 });
        else
            NotifyGame({ 1, 4, 3 });
    }
    SAMELINE;
    if (CreateButton("Run", ImVec2(100.0f, 25.0f), &ZombieOptions::zombieSpeedRun, true, fakeColor, true))
    {
        ZombieOptions::zombieSpeedWalk = false;
        ZombieOptions::zombieSpeedSprint = false;
        if (ZombieOptions::zombieSpeedRun)
            NotifyGame({ 1, 4, 1 });
        else
            NotifyGame({ 1, 4, 3 });
    }
    SAMELINE;
    if (CreateButton("Sprint", ImVec2(100.0f, 25.0f), &ZombieOptions::zombieSpeedSprint, true, fakeColor, true))
    {
        ZombieOptions::zombieSpeedWalk = false;
        ZombieOptions::zombieSpeedRun = false;
        if (ZombieOptions::zombieSpeedSprint)
            NotifyGame({ 1, 4, 2 });
        else
            NotifyGame({ 1, 4, 3 });
    }

}

void RoundOptionsPtr()
{
    if (CreateButton("End Round", ImVec2(120.0f, 25.0f)))
        NotifyGame({ 2, 0 });
    SAMELINE;
    if (CreateButton("Restart Round", ImVec2(120.0f, 25.0f)))
        NotifyGame({ 2, 1 });
    ImGui::SetNextItemWidth(125.0f);
    if (ImGui::InputInt("Set Round", &roundInput, 1, 10, ImGuiInputTextFlags_EnterReturnsTrue))
    {
        if (roundInput < 1)
            roundInput = 1;
        else if (roundInput > 255)
            roundInput = 255;
    }
    SAMELINE;
    if (CreateButton(ICON_FA_ARROW_RIGHT " Send##Round Input", ImVec2(80.0f, 25.0f)))
        NotifyGame({ 2, 1, roundInput });
    SAMELINE;
    HelpMarker("Increment +- 1\nCtrl + Click increment +- 10");
    ImGui::SetNextItemWidth(100.0f);
    if (ImGui::InputInt("Set Zombie Count", &zombieCount, NULL, NULL, ImGuiInputTextFlags_EnterReturnsTrue))
    {
        if (zombieCount < 0)
            zombieCount = 0;
        else if (zombieCount > INT_MAX)
            zombieCount = INT_MAX;
    }
    SAMELINE;
    if (CreateButton(ICON_FA_ARROW_RIGHT " Send##Zombie Count", ImVec2(80.0f, 25.0f)))
        NotifyGame({ 2, 2, zombieCount });
}

void PowerupOptionsPtr()
{
    CreateListBox("##Powerup List", PowerupOptions::powerupList[currentMap], PowerupOptions::powerupListIndex, ImVec2(300, (PowerupOptions::powerupList[currentMap].size() * 25.0f + 1.0f)));
    SAMELINE;
    ImGui::BeginGroup();
    if (CreateButton("Give Powerup", ImVec2(125.0f, 25.0f)))
        NotifyGame({ 3, 0, PowerupOptions::powerupListIndex, PowerupOptions::instaGrab });
    ImGui::Checkbox("Insta Grab", &PowerupOptions::instaGrab);
    ImGui::EndGroup();
}

void EggStepOptionsPtr()
{
    if (ImGui::BeginTabBar("Egg Step Options Tabs"))
    {
        if (ImGui::BeginTabItem("SOE"))
        {
            std::vector<std::string> items = EggStepOptions::zodRitualSteps;
            int size = (int)EggStepOptions::zodRitualSteps.size();
            ImVec2 boxSize = ImVec2(300.0f, 150.0f);
            // Ritual List
            {
                ImGui::BeginGroup();
                ImGui::Text(ICON_FA_GEAR " Ritual Options");
                if (ImGui::ListBoxHeader("##Ritual Options", boxSize))
                {
                    for (int i = 0; i < items.size(); i++)
                    {
                        const bool is_selected = (EggStepOptions::zodRitualIndex == i);
                        if (ImGui::Selectable(items[i].c_str(), is_selected))
                            EggStepOptions::zodRitualIndex = i;
                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::ListBoxFooter();
                }
                SAMELINE;
                ImGui::BeginGroup();
                if (CreateButton("Complete Step##Ritual", ImVec2(125.0f, 25.0f)))
                    NotifyGame({ 4, 0, EggStepOptions::zodRitualIndex });
                if (CreateButton("Complete All Steps##Ritual", ImVec2(155.0f, 25.0f)))
                    NotifyGame({ 4, 0, 4 });
                ImGui::EndGroup();
            }
            // Egg List
            {
                items = EggStepOptions::zodEggSteps;
                size = (int)EggStepOptions::zodEggSteps.size();
                ImGui::BeginGroup();
                ImGui::Text(ICON_FA_GEAR " Egg Options");
                if (ImGui::ListBoxHeader("##Egg Options", boxSize))
                {
                    for (int i = 0; i < items.size(); i++)
                    {
                        const bool is_selected = (EggStepOptions::zodEggIndex == i);
                        if (ImGui::Selectable(items[i].c_str(), is_selected))
                            EggStepOptions::zodEggIndex = i;
                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::ListBoxFooter();
                }
                SAMELINE;
                ImGui::BeginGroup();
                if (CreateButton("Complete Step##Egg", ImVec2(125.0f, 25.0f)))
                    NotifyGame({ 4, 2, EggStepOptions::zodEggIndex });
                if (CreateButton("Complete All Steps##Egg", ImVec2(155.0f, 25.0f)))
                    NotifyGame({ 4, 1 });
                ImGui::EndGroup();
            }
            ImGui::EndGroup();
            ImGui::EndGroup();
            // Ovum List
            {
                SAMELINE;
                items = EggStepOptions::zodOvumSteps;
                size = (int)EggStepOptions::zodOvumSteps.size();
                ImGui::BeginGroup();
                ImGui::Text(ICON_FA_GEAR " Ovum Options");
                if (ImGui::ListBoxHeader("##Ovum Options", boxSize))
                {
                    for (int i = 0; i < items.size(); i++)
                    {
                        const bool is_selected = (EggStepOptions::zodOvumIndex == i);
                        if (ImGui::Selectable(items[i].c_str(), is_selected))
                            EggStepOptions::zodOvumIndex = i;
                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::ListBoxFooter();
                }
                SAMELINE;
                ImGui::BeginGroup();
                if (CreateButton("Complete Step##Ovum", ImVec2(125.0f, 25.0f)))
                    NotifyGame({ 4, 4, EggStepOptions::zodOvumIndex });
                if (CreateButton("Complete All Steps##Ovum", ImVec2(155.0f, 25.0f)))
                    NotifyGame({ 4, 3 });
                ImGui::EndGroup();
            }
            // Flag List
            {
                items = EggStepOptions::zodFlagSteps;
                size = (int)EggStepOptions::zodFlagSteps.size();
                ImGui::BeginGroup();
                ImGui::Text(ICON_FA_GEAR " Flag Options");
                if (ImGui::ListBoxHeader("##Flag Options", boxSize))
                {
                    for (int i = 0; i < items.size(); i++)
                    {
                        const bool is_selected = (EggStepOptions::zodFlagIndex == i);
                        if (ImGui::Selectable(items[i].c_str(), is_selected))
                            EggStepOptions::zodFlagIndex = i;
                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::ListBoxFooter();
                }
                SAMELINE;
                ImGui::BeginGroup();
                if (CreateButton("Complete Step##Flag", ImVec2(125.0f, 25.0f)))
                    NotifyGame({ 4, 6, EggStepOptions::zodFlagIndex });
                if (CreateButton("Complete All Steps##Flag", ImVec2(155.0f, 25.0f)))
                    NotifyGame({ 4, 5 });
                ImGui::EndGroup();
            }
            ImGui::EndGroup();
            ImGui::EndGroup();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("DE"))
        {
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("ZNS"))
        {
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("GK"))
        {
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Rev"))
        {
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Origins"))
        {
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}

void CraftableOptionsPtr()
{
    int craftSize = (int)Craftables::craftList[currentMap].size();
    if (craftSize > 0)
    {
        if (CreateButton("Pickup Every Part", ImVec2(150.0f, 25.0f)))
            NotifyGame({ 5, 0 });
        for (int i = 0; i < craftSize; i++)
        {
            if (ImGui::CollapsingHeader(Craftables::craftNames[currentMap][i].c_str()))
            {
                std::string listName = "##Craftables" + currentMap + std::to_string(i);
                CreateListBox(listName.c_str(), Craftables::craftList[currentMap][i], Craftables::craftComboIndexes[currentMap][i], ImVec2(300.f, min(151.0f, Craftables::craftList[currentMap][i].size() * 25 + 1.0f)));
                SAMELINE;
                ImGui::BeginGroup();
                if (CreateButton("Pickup Part##" + std::to_string(i), ImVec2(150.0f, 25.0f)))
                    NotifyGame({ 5, 1, i, Craftables::craftComboIndexes[currentMap][i] });
                if (CreateButton("Pickup All Parts##" + std::to_string(i), ImVec2(150.0f, 25.0f)))
                    NotifyGame({ 5, 1, i, 0, 1 });
                ImGui::EndGroup();
            }
        }
    }
    else
        ImGui::Text("No Craftables For Current Map");
}

void BlockerOptionsPtr()
{
    if (CreateButton("Open All Doors", ImVec2(150.0f, 25.0f)))
        NotifyGame({ 6, 0 });
    SAMELINE;
    if (CreateButton("Global Power On", ImVec2(150.0f, 25.0f)))
        NotifyGame({ 6, 2 });
    SAMELINE;
    if (CreateButton("Open All Barriers", ImVec2(150.0f, 25.0f)))
        NotifyGame({ 6, 5, 0 });
    SAMELINE;
    if (CreateButton("Close All Barriers", ImVec2(150.0f, 25.0f)))
        NotifyGame({ 6, 5, 1 });
    switch (hashstr(currentMap.c_str()))
    {
    case hashstr("zm_zod"):
    {
        if (CreateButton("Open Selected Door", ImVec2(200.0f, 25.0f)))
            NotifyGame({ 6, 1, Blockers::zodDoorIndex });
        SAMELINE;
        if (CreateButton("Activate Selected Power", ImVec2(200.0f, 25.0f)))
            NotifyGame({ 6, 3, Blockers::zodPowerIndex });
        SAMELINE;
        if (CreateButton("Smash Selected Smashable", ImVec2(225.0f, 25.0f)))
            NotifyGame({ 6, 4, Blockers::zodSmashablesIndex });
        ImGui::BeginGroup();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 250.0f / 2 - ImGui::CalcTextSize("Doors List").x / 2);
        ImGui::Text("Doors List");
        CreateListBox("##SOE Doors", Blockers::zodDoorItems, Blockers::zodDoorIndex, ImVec2(250.0f, min(Blockers::zodDoorItems.size() * 25.0f + 1, 401.0f)));
        ImGui::EndGroup();
        SAMELINE;
        ImGui::BeginGroup();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 250.0f / 2 - ImGui::CalcTextSize("Power List").x / 2);
        ImGui::Text("Power List");
        CreateListBox("##SOE Power", Blockers::zodPowerItems, Blockers::zodPowerIndex, ImVec2(250.0f, min(Blockers::zodPowerItems.size() * 25.0f + 1, 401.0f)));
        ImGui::EndGroup();
        SAMELINE;
        ImGui::BeginGroup();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 250.0f / 2 - ImGui::CalcTextSize("Smashables List").x / 2);
        ImGui::Text("Smashables List");
        CreateListBox("##SOE Smashables", Blockers::zodSmashablesItems, Blockers::zodSmashablesIndex, ImVec2(250.0f, min(Blockers::zodSmashablesItems.size() * 25.0f + 1, 401.0f)));
        ImGui::EndGroup();
        break;
    }
    case hashstr("zm_factory"):
    {
        if (CreateButton("Open Selected Door", ImVec2(175.0f, 25.0f)))
            NotifyGame({ 6, 1, Blockers::factoryDoorIndex });
        SAMELINE;
        if (CreateButton("Activate West TP", ImVec2(150.0f, 25.0f)))
            NotifyGame({ 6, 4, 0 });
        SAMELINE;
        if (CreateButton("Activate East TP", ImVec2(150.0f, 25.0f)))
            NotifyGame({ 6, 4, 1 });
        SAMELINE;
        if (CreateButton("Activate South TP", ImVec2(150.0f, 25.0f)))
            NotifyGame({ 6, 4, 2 });
        ImGui::BeginGroup();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 250.0f / 2 - ImGui::CalcTextSize("Doors List").x / 2);
        ImGui::Text("Doors List");
        CreateListBox("##Factory Doors", Blockers::factoryDoorItems, Blockers::factoryDoorIndex, ImVec2(250.0f, min(Blockers::factoryDoorItems.size() * 25.0f + 1, 401.0f)));
        ImGui::EndGroup();
        break;
    }
    case hashstr("zm_castle"):
    {
        if (CreateButton("Open Selected Door", ImVec2(175.0f, 25.0f)))
            NotifyGame({ 6, 1, Blockers::castleDoorIndex });
        SAMELINE;
        if (CreateButton("Activate Selected Landing Pad", ImVec2(250.0f, 25.0f)))
            NotifyGame({ 6, 4, Blockers::castleLanderIndex });
        ImGui::BeginGroup();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 350.0f / 2 - ImGui::CalcTextSize("Doors List").x / 2);
        ImGui::Text("Doors List");
        CreateListBox("##Castle Doors", Blockers::castleDoorItems, Blockers::castleDoorIndex, ImVec2(350.0f, min(Blockers::castleDoorItems.size() * 25.0f + 1, 401.0f)));
        ImGui::EndGroup();
        SAMELINE;
        ImGui::BeginGroup();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 250.0f / 2 - ImGui::CalcTextSize("Landing Pads").x / 2);
        ImGui::Text("Landing Pads");
        CreateListBox("##Castle Landing Pads", Blockers::castleLanderItems, Blockers::castleLanderIndex, ImVec2(250.0f, min(Blockers::castleLanderItems.size() * 25.0f + 1, 401.0f)));
        ImGui::EndGroup();
        break;
    }
    case hashstr("zm_island"):
    {
        if (CreateButton("Open Selected Door", ImVec2(175.0f, 25.0f)))
            NotifyGame({ 6, 1, Blockers::islandDoorIndex });
        ImGui::BeginGroup();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 300.0f / 2 - ImGui::CalcTextSize("Doors List").x / 2);
        ImGui::Text("Doors List");
        CreateListBox("##Island Doors", Blockers::islandDoorItems, Blockers::islandDoorIndex, ImVec2(300.0f, min(Blockers::islandDoorItems.size() * 25.0f + 1, 401.0f)));
        ImGui::EndGroup();
        break;
    }
    case hashstr("zm_stalingrad"):
    {
        if (CreateButton("Open Selected Door", ImVec2(175.0f, 25.0f)))
            NotifyGame({ 6, 1, Blockers::stalingradDoorIndex });
        ImGui::BeginGroup();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 300.0f / 2 - ImGui::CalcTextSize("Doors List").x / 2);
        ImGui::Text("Doors List");
        CreateListBox("##Stalingrad Doors", Blockers::stalingradDoorItems, Blockers::stalingradDoorIndex, ImVec2(300.0f, min(Blockers::stalingradDoorItems.size() * 25.0f + 1, 401.0f)));
        ImGui::EndGroup();
        break;
    }
    case hashstr("zm_genesis"):
    {
        if (CreateButton("Open Selected Door", ImVec2(175.0f, 25.0f)))
            NotifyGame({ 6, 1, Blockers::genesisDoorIndex });
        SAMELINE;
        if (CreateButton("Activate Selected Gen", ImVec2(175.0f, 25.0f)))
            NotifyGame({ 6, 3, Blockers::genesisGenIndex });
        SAMELINE;
        if (CreateButton("Trap Apothicon", ImVec2(150.0f, 25.0f)))
            NotifyGame({ 6, 4, 0 });
        ImGui::BeginGroup();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 300.0f / 2 - ImGui::CalcTextSize("Doors List").x / 2);
        ImGui::Text("Doors List");
        CreateListBox("##Genesis Doors", Blockers::genesisDoorItems, Blockers::genesisDoorIndex, ImVec2(300.0f, min(Blockers::genesisDoorItems.size() * 25.0f + 1, 401.0f)));
        ImGui::EndGroup();
        SAMELINE;
        ImGui::BeginGroup();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 250.0f / 2 - ImGui::CalcTextSize("Generators List").x / 2);
        ImGui::Text("Generators List");
        CreateListBox("##Genesis Generators", Blockers::genesisGenItems, Blockers::genesisGenIndex, ImVec2(250.0f, min(Blockers::genesisGenItems.size() * 25.0f + 1, 401.0f)));
        ImGui::EndGroup();
        break;
    }
    case hashstr("zm_prototype"):
    {
        if (CreateButton("Open Selected Door", ImVec2(175.0f, 25.0f)))
            NotifyGame({ 6, 1, Blockers::prototypeDoorIndex });
        ImGui::BeginGroup();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 250.0f / 2 - ImGui::CalcTextSize("Doors List").x / 2);
        ImGui::Text("Doors List");
        CreateListBox("##Prototype Doors", Blockers::prototypeDoorItems, Blockers::prototypeDoorIndex, ImVec2(250.0f, min(Blockers::prototypeDoorItems.size() * 25.0f + 1, 401.0f)));
        ImGui::EndGroup();
        break;
    }
    case hashstr("zm_asylum"):
    {
        if (CreateButton("Open Selected Door", ImVec2(175.0f, 25.0f)))
            NotifyGame({ 6, 1, Blockers::asylumDoorIndex });
        ImGui::BeginGroup();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 250.0f / 2 - ImGui::CalcTextSize("Doors List").x / 2);
        ImGui::Text("Doors List");
        CreateListBox("##Asylum Doors", Blockers::asylumDoorItems, Blockers::asylumDoorIndex, ImVec2(250.0f, min(Blockers::asylumDoorItems.size() * 25.0f + 1, 401.0f)));
        ImGui::EndGroup();
        break;
    }
    case hashstr("zm_sumpf"):
    {
        if (CreateButton("Open Selected Door", ImVec2(175.0f, 25.0f)))
            NotifyGame({ 6, 1, Blockers::sumpfDoorIndex });
        ImGui::BeginGroup();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 250.0f / 2 - ImGui::CalcTextSize("Doors List").x / 2);
        ImGui::Text("Doors List");
        CreateListBox("##Sumpf Doors", Blockers::sumpfDoorItems, Blockers::sumpfDoorIndex, ImVec2(250.0f, min(Blockers::sumpfDoorItems.size() * 25.0f + 1, 401.0f)));
        ImGui::EndGroup();
        break;
    }
    case hashstr("zm_theater"):
    {
        if (CreateButton("Open Selected Door", ImVec2(175.0f, 25.0f)))
            NotifyGame({ 6, 1, Blockers::theaterDoorIndex });
        SAMELINE;
        if (CreateButton("Link TP", ImVec2(125.0f, 25.0f)))
            NotifyGame({ 6, 4, 0 });
        ImGui::BeginGroup();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 250.0f / 2 - ImGui::CalcTextSize("Doors List").x / 2);
        ImGui::Text("Doors List");
        CreateListBox("##Theater Doors", Blockers::theaterDoorItems, Blockers::theaterDoorIndex, ImVec2(250.0f, min(Blockers::theaterDoorItems.size() * 25.0f + 1, 401.0f)));
        ImGui::EndGroup();
        break;
    }
    case hashstr("zm_cosmodrome"):
    {
        if (CreateButton("Open Selected Door", ImVec2(175.0f, 25.0f)))
            NotifyGame({ 6, 1, Blockers::cosmodromeDoorIndex });
        SAMELINE;
        if (CreateButton("Open PAP", ImVec2(125.0f, 25.0f)))
            NotifyGame({ 6, 4, 0 });
        ImGui::BeginGroup();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 250.0f / 2 - ImGui::CalcTextSize("Doors List").x / 2);
        ImGui::Text("Doors List");
        CreateListBox("##Cosmodrome Doors", Blockers::cosmodromeDoorItems, Blockers::cosmodromeDoorIndex, ImVec2(250.0f, min(Blockers::cosmodromeDoorItems.size() * 25.0f + 1, 401.0f)));
        ImGui::EndGroup();
        break;
    }
    case hashstr("zm_temple"):
    {
        if (CreateButton("Open Selected Door", ImVec2(175.0f, 25.0f)))
            NotifyGame({ 6, 1, Blockers::templeDoorIndex });
        SAMELINE;
        if (CreateButton("Open PAP", ImVec2(125.0f, 25.0f)))
            NotifyGame({ 6, 4, 0 });
        ImGui::BeginGroup();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 250.0f / 2 - ImGui::CalcTextSize("Doors List").x / 2);
        ImGui::Text("Doors List");
        CreateListBox("##STempleumpf Doors", Blockers::templeDoorItems, Blockers::templeDoorIndex, ImVec2(250.0f, min(Blockers::templeDoorItems.size() * 25.0f + 1, 401.0f)));
        ImGui::EndGroup();
        break;
    }
    case hashstr("zm_moon"):
    {
        if (CreateButton("Open Selected Door", ImVec2(175.0f, 25.0f)))
            NotifyGame({ 6, 1, Blockers::moonDoorIndex });
        SAMELINE;
        if (CreateButton("Open TP Cage", ImVec2(125.0f, 25.0f)))
            NotifyGame({ 6, 4, 0 });
        ImGui::BeginGroup();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 250.0f / 2 - ImGui::CalcTextSize("Doors List").x / 2);
        ImGui::Text("Doors List");
        CreateListBox("##Moon Doors", Blockers::moonDoorItems, Blockers::moonDoorIndex, ImVec2(250.0f, min(Blockers::moonDoorItems.size() * 25.0f + 1, 401.0f)));
        ImGui::EndGroup();
        break;
    }
    case hashstr("zm_tomb"):
    {
        if (CreateButton("Open Selected Door", ImVec2(175.0f, 25.0f)))
            NotifyGame({ 6, 1, Blockers::tombDoorIndex });
        SAMELINE;
        if (CreateButton("Activate Selected Gen", ImVec2(175.0f, 25.0f)))
            NotifyGame({ 6, 3, Blockers::tombGenIndex });
        ImGui::BeginGroup();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 250.0f / 2 - ImGui::CalcTextSize("Doors List").x / 2);
        ImGui::Text("Doors List");
        CreateListBox("##Tomb Doors", Blockers::tombDoorItems, Blockers::tombDoorIndex, ImVec2(250.0f, min(Blockers::tombDoorItems.size() * 25.0f + 1, 401.0f)));
        ImGui::EndGroup();
        SAMELINE;
        ImGui::BeginGroup();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 250.0f / 2 - ImGui::CalcTextSize("Doors List").x / 2);
        ImGui::Text("Doors List");
        CreateListBox("##Tomb Generators", Blockers::tombGenItems, Blockers::tombGenIndex, ImVec2(250.0f, min(Blockers::tombGenItems.size() * 25.0f + 1, 401.0f)));
        ImGui::EndGroup();
        break;
    }
    default:
        break;
    }
}

void MapOptionsPtr()
{
    ImGui::BeginGroup();
    CreateListBox("##Load Map List", MapOptions::mapList, MapOptions::mapListIndex, ImVec2(200.0f, 175.0f));
    SAMELINE;
    if (CreateButton("Load Map", ImVec2(100.0f, 25.0f)))
    {
        NotifyGame({ 7, 0, MapOptions::mapListIndex });
    }
    ImGui::EndGroup();
}

void ZombieCalculatorPtr()
{
    if (ImGui::BeginTabBar("Zombie Calc Tabs"))
    {
        if (ImGui::BeginTabItem("Basic"))
        {
            // Calc Player Input
            {
                ImGui::BeginChild("Calc Input", ImVec2(300.0f, 130.0f), true);
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Number Of Players").x) / 2 + 10);
                ImGui::Text("Number Of Players");
                ImGui::SetNextItemWidth(170.0f);
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcItemWidth()) / 2 + 10);
                if (ImGui::InputInt("##Number Of Players", &playerCount, 1, NULL, ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    if (playerCount < 1)
                        playerCount = 1;
                    else if (playerCount > 4)
                        playerCount = 4;
                    zombiesForRound = GetZombieCountForRound(roundNumber, playerCount);
                    hordesForRound = zombiesForRound / 24.0f;
                    zombiesUpToRound = GetZombiesUpToRound(roundNumber, playerCount);
                    zombieHealthForRound = GetZombieHealthForRound(roundNumber);
                    zombieSpawnRate = GetZombieSpawnRateForRound(roundNumber, playerCount);
                    zombieSpawnRateStr = ParseTimeFromMilli(zombieSpawnRate);
                    roundTime = CalcRoundTime(roundNumber, playerCount);
                    expectedRoundTime = CalcRoundTime(roundNumber, playerCount, corpseDelay[playerCount - 1][roundNumber - 1]);
                }
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Current Round").x) / 2 + 10);
                ImGui::Text("Current Round");
                ImGui::SetNextItemWidth(170.0f);
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcItemWidth()) / 2 + 10);
                if (ImGui::InputInt("##Current Round", &roundNumber, 1, NULL, ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    if (roundNumber < 1)
                        roundNumber = 1;
                    else if (roundNumber > 255)
                        roundNumber = 255;
                    zombiesForRound = GetZombieCountForRound(roundNumber, playerCount);
                    hordesForRound = zombiesForRound / 24.0f;
                    zombiesUpToRound = GetZombiesUpToRound(roundNumber, playerCount);
                    zombieHealthForRound = GetZombieHealthForRound(roundNumber);
                    zombieSpawnRate = GetZombieSpawnRateForRound(roundNumber, playerCount);
                    zombieSpawnRateStr = ParseTimeFromMilli(zombieSpawnRate);
                    roundTime = CalcRoundTime(roundNumber, playerCount);
                    expectedRoundTime = CalcRoundTime(roundNumber, playerCount, corpseDelay[playerCount - 1][roundNumber - 1]);
                }
                ImGui::EndChild();
            }

            // Calc Data 1
            {
                SAMELINE;
                ImGui::BeginChild("Calc Data 1", ImVec2(300.0f, 130.0f), true);
                std::string numZombiesText("Zombies On Round " + std::to_string(roundNumber) + ":");
                std::string numZombiesNum(std::to_string(zombiesForRound));
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(numZombiesText.c_str()).x) / 2 + 10);
                ImGui::Text(numZombiesText.c_str());
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(numZombiesNum.c_str()).x) / 2 + 10);
                ImGui::Text(numZombiesNum.c_str());
                std::string numHordesText("Hordes On Round " + std::to_string(roundNumber) + ":");
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(numHordesText.c_str()).x) / 2 + 10);
                ImGui::Text(numHordesText.c_str());
                char roundedHordes[16];
                snprintf(roundedHordes, sizeof(roundedHordes), "%.3f", hordesForRound);
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(roundedHordes).x) / 2 + 10);
                ImGui::Text(roundedHordes);
                ImGui::EndChild();
            }

            // Calc Data 2
            {
                SAMELINE;
                ImGui::BeginChild("Calc Data 2", ImVec2(300.0f, 130.0f), true);
                std::string numZombiesUpToText("Zombies Up To Round " + std::to_string(roundNumber) + ":");
                std::string numZombiesUpToNum(std::to_string(zombiesUpToRound));
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(numZombiesUpToText.c_str()).x) / 2 + 10);
                ImGui::Text(numZombiesUpToText.c_str());
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(numZombiesUpToNum.c_str()).x) / 2 + 10);
                ImGui::Text(numZombiesUpToNum.c_str());
                std::string numZombieHealthText("Zombie Health On Round " + std::to_string(roundNumber) + ":");
                std::string numZombieHealthNum(std::to_string(zombieHealthForRound));
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(numZombieHealthText.c_str()).x) / 2 + 10);
                ImGui::Text(numZombieHealthText.c_str());
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(numZombieHealthNum.c_str()).x) / 2 + 10);
                ImGui::Text(numZombieHealthNum.c_str());
                ImGui::EndChild();
            }
            DummySpace(150.0f, 0.0f);
            SAMELINE;
            // Calc Data 3
            {
                ImGui::BeginChild("Calc Data 3", ImVec2(300.0f, 130.0f), true);
                std::string spawnRateText("Spawn Rate On Round " + std::to_string(roundNumber) + ":");
                std::string spawnRateNum(zombieSpawnRateStr);
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(spawnRateText.c_str()).x) / 2 + 10);
                ImGui::Text(spawnRateText.c_str());
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(spawnRateNum.c_str()).x) / 2 + 10);
                ImGui::Text(spawnRateNum.c_str());
                std::string roundTimeText("Perfect Round Time For Round " + std::to_string(roundNumber) + ":");
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(roundTimeText.c_str()).x) / 2 + 10);
                ImGui::Text(roundTimeText.c_str());
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(roundTime.c_str()).x) / 2 + 10);
                ImGui::Text(roundTime.c_str());
                ImGui::EndChild();
            }

            // Calc Data 4
            {
                SAMELINE;
                ImGui::BeginChild("Calc Data 4", ImVec2(300.0f, 130.0f), true);
                std::string corpseDelayText("Corpse Delay On Round " + std::to_string(roundNumber) + ":");
                std::string corpseDelayNum(std::to_string(corpseDelay[playerCount - 1][roundNumber - 1]));
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(corpseDelayText.c_str()).x) / 2 + 10);
                ImGui::Text(corpseDelayText.c_str());
                HelpMarker("Expected corpse delay killing every zombie exactly 2.25 seconds after it spawns. Actual value may vary in game.");
                SAMELINE;
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(corpseDelayNum.c_str()).x) / 2 + 10);
                ImGui::Text(corpseDelayNum.c_str());
                std::string expectedRoundTimeText("Expected Round Time For Round " + std::to_string(roundNumber) + ":");
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(expectedRoundTimeText.c_str()).x) / 2 + 10);
                ImGui::Text(expectedRoundTimeText.c_str());
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(expectedRoundTime.c_str()).x) / 2 + 10);
                ImGui::Text(expectedRoundTime.c_str());
                ImGui::EndChild();
            }
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Advanced"))
        {
            // Calc Data 5
            {
                ImGui::BeginChild("Calc Data 5", ImVec2(300.0f, 400.0f), true);
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Number Of Players/Current Round").x) / 2 + 10);
                ImGui::Text("Number Of Players/Current Round");
                DummySpace(7.5f, 0.0f);
                SAMELINE;
                ImGui::SetNextItemWidth(120.0f);
                if (ImGui::InputInt("##Number Of Players - Special Enemies", &specialEnemyPlayerCount, 1, NULL, ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    if (specialEnemyPlayerCount < 1)
                        specialEnemyPlayerCount = 1;
                    else if (specialEnemyPlayerCount > 4)
                        specialEnemyPlayerCount = 4;
                    specialEnemiesRoundTime = CalcRoundTime(specialEnemyRound, specialEnemyPlayerCount, corpseDelay[specialEnemyPlayerCount - 1][specialEnemyRound - 1], true);
                    meatballDelay = "Meatball Spawn Delay: " + ParseTimeFromMilli(GetSpecialEnemySpawnRate(specialEnemyPlayerCount, "Meatballs"));
                    dogDelay = "Dog Spawn Delay: " + ParseTimeFromMilli(GetSpecialEnemySpawnRate(specialEnemyPlayerCount, "Dogs"));
                    spiderDelay = "Spider Spawn Delay: " + ParseTimeFromMilli(GetSpecialEnemySpawnRate(specialEnemyPlayerCount, "Spiders"));
                    manglerDelay = "Mangler Spawn Delay: " + ParseTimeFromMilli(GetSpecialEnemySpawnRate(specialEnemyPlayerCount, "Manglers"));
                    valkDelay = "Valk Spawn Delay: " + ParseTimeFromMilli(GetSpecialEnemySpawnRate(specialEnemyPlayerCount, "Valks"));
                    furyDelay = "Fury Spawn Delay: " + ParseTimeFromMilli(GetSpecialEnemySpawnRate(specialEnemyPlayerCount, "Furys"));
                    keeperDelay = "Keeper Spawn Delay: " + ParseTimeFromMilli(GetSpecialEnemySpawnRate(specialEnemyPlayerCount, "Keepers"));
                    if (meatballCount > specialZombiesForRound)
                    {
                        meatballCount = specialZombiesForRound;
                        specialEnemyCount_1 = meatballCount;
                    }
                    if (dogCount > specialZombiesForRound)
                    {
                        dogCount = specialZombiesForRound;
                        specialEnemyCount_1 = dogCount;
                    }
                    if (islandSpidersCount > specialZombiesForRound)
                    {
                        islandSpidersCount = specialZombiesForRound;
                        specialEnemyCount_1 = dogCount;
                    }
                    if (manglersCount + valksCount > specialZombiesForRound)
                    {
                        manglersCount = specialZombiesForRound - valksCount;
                        specialEnemyCount_1 = manglersCount;
                    }
                    if (valksCount + manglersCount > specialZombiesForRound)
                    {
                        valksCount = specialZombiesForRound - manglersCount;
                        specialEnemyCount_2 = valksCount;
                    }
                    if (lastEditedSpecialEnemy == "GenesisSpiders" && genesisSpidersCount + furysCount + keepersCount > specialZombiesForRound)
                    {
                        genesisSpidersCount = specialZombiesForRound - furysCount - keepersCount;
                        specialEnemyCount_1 = genesisSpidersCount;
                    }
                    if (lastEditedSpecialEnemy == "Furys" && genesisSpidersCount + furysCount + keepersCount > specialZombiesForRound)
                    {
                        furysCount = specialZombiesForRound - genesisSpidersCount - keepersCount;
                        specialEnemyCount_2 = furysCount;
                    }
                    if (lastEditedSpecialEnemy == "Keepers" && genesisSpidersCount + furysCount + keepersCount > specialZombiesForRound)
                    {
                        keepersCount = specialZombiesForRound - genesisSpidersCount - furysCount;
                        specialEnemyCount_3 = keepersCount;
                    }
                    specialEnemiesRoundTime = CalcRoundTime(specialEnemyRound, specialEnemyPlayerCount, corpseDelay[specialEnemyPlayerCount - 1][specialEnemyRound - 1], true);
                } SAMELINE;
                ImGui::SetNextItemWidth(120.0f);
                if (ImGui::InputInt("##Current Round - Special Enemies", &specialEnemyRound, 1, NULL, ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    if (specialEnemyRound < 1)
                        specialEnemyRound = 1;
                    else if (specialEnemyRound > 255)
                        specialEnemyRound = 255;
                    specialZombiesForRound = GetZombieCountForRound(specialEnemyRound, specialEnemyPlayerCount);
                    if (meatballCount > specialZombiesForRound)
                    {
                        meatballCount = specialZombiesForRound;
                        specialEnemyCount_1 = meatballCount;
                    }
                    if (dogCount > specialZombiesForRound)
                    {
                        dogCount = specialZombiesForRound;
                        specialEnemyCount_1 = dogCount;
                    }
                    if (islandSpidersCount > specialZombiesForRound)
                    {
                        islandSpidersCount = specialZombiesForRound;
                        specialEnemyCount_1 = dogCount;
                    }
                    if (manglersCount + valksCount > specialZombiesForRound)
                    {
                        manglersCount = specialZombiesForRound - valksCount;
                        specialEnemyCount_1 = manglersCount;
                    }
                    if (valksCount + manglersCount > specialZombiesForRound)
                    {
                        valksCount = specialZombiesForRound - manglersCount;
                        specialEnemyCount_2 = valksCount;
                    }
                    if (lastEditedSpecialEnemy == "GenesisSpiders" && genesisSpidersCount + furysCount + keepersCount > specialZombiesForRound)
                    {
                        genesisSpidersCount = specialZombiesForRound - furysCount - keepersCount;
                        specialEnemyCount_1 = genesisSpidersCount;
                    }
                    if (lastEditedSpecialEnemy == "Furys" && genesisSpidersCount + furysCount + keepersCount > specialZombiesForRound)
                    {
                        furysCount = specialZombiesForRound - genesisSpidersCount - keepersCount;
                        specialEnemyCount_2 = furysCount;
                    }
                    if (lastEditedSpecialEnemy == "Keepers" && genesisSpidersCount + furysCount + keepersCount > specialZombiesForRound)
                    {
                        keepersCount = specialZombiesForRound - genesisSpidersCount - furysCount;
                        specialEnemyCount_3 = keepersCount;
                    }
                    specialEnemiesRoundTime = CalcRoundTime(specialEnemyRound, specialEnemyPlayerCount, corpseDelay[specialEnemyPlayerCount - 1][specialEnemyRound - 1], true);
                }
                int prevMap = currentSpecialEnemyMap;
                int prevDogRoundCount = numDogRounds;
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcItemWidth()) / 2 - ImGui::CalcTextSize("Map").x / 2 + 10);
                if (ImGui::BeginCombo("Map", specialEnemiesMapCombo[currentSpecialEnemyMap].c_str(), ImGuiComboFlags_HeightRegular))
                {
                    for (int i = 0; i < specialEnemiesMapCombo.size(); i++)
                    {
                        const bool is_selected = currentSpecialEnemyMap == i;
                        if (ImGui::Selectable(specialEnemiesMapCombo[i].c_str(), is_selected))
                            currentSpecialEnemyMap = i;
                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }
                if (prevMap != currentSpecialEnemyMap)
                {
                    switch (hashstr(specialEnemiesMapCombo[currentSpecialEnemyMap].c_str()))
                    {
                    case hashstr("SOE"):
                        specialEnemyCount_1 = meatballCount;
                        specialEnemyCount_2 = 0;
                        specialEnemyCount_3 = 0;
                        specialEnemy_1 = "Meatballs";
                        break;
                    case hashstr("Giant + DE"):
                        specialEnemyCount_1 = dogCount;
                        specialEnemyCount_2 = 0;
                        specialEnemyCount_3 = 0;
                        specialEnemy_1 = "Dogs";
                        break;
                    case hashstr("ZNS"):
                        specialEnemyCount_1 = islandSpidersCount;
                        specialEnemyCount_2 = 0;
                        specialEnemyCount_3 = 0;
                        specialEnemy_1 = "Spiders";
                        break;
                    case hashstr("GK"):
                        specialEnemyCount_1 = manglersCount;
                        specialEnemyCount_2 = valksCount;
                        specialEnemyCount_3 = 0;
                        specialEnemy_1 = "Manglers";
                        specialEnemy_2 = "Valks";
                        break;
                    case hashstr("Rev"):
                        specialEnemyCount_1 = genesisSpidersCount;
                        specialEnemyCount_2 = furysCount;
                        specialEnemyCount_3 = keepersCount;
                        specialEnemy_1 = "Spiders";
                        specialEnemy_2 = "Furys";
                        specialEnemy_3 = "Keepers";
                        break;
                    }
                    specialEnemiesRoundTime = CalcRoundTime(specialEnemyRound, specialEnemyPlayerCount, corpseDelay[specialEnemyPlayerCount - 1][specialEnemyRound - 1], true);
                }
                switch (hashstr(specialEnemiesMapCombo[currentSpecialEnemyMap].c_str()))
                {
                case hashstr("SOE"):
                    ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(meatballDelay.c_str()).x) / 2 + 10);
                    ImGui::Text(meatballDelay.c_str());
                    HelpMarker("Bugs add no spawn delay, and are thus not included.");
                    SAMELINE;
                    ImGui::SetNextItemWidth(130.0f);
                    if (ImGui::InputInt("# of Meatballs", &meatballCount, 1, NULL, ImGuiInputTextFlags_EnterReturnsTrue))
                    {
                        if (meatballCount < 0)
                            meatballCount = 0;
                        if (meatballCount > specialZombiesForRound)
                            meatballCount = specialZombiesForRound;
                        specialEnemyCount_1 = meatballCount;
                        specialEnemiesRoundTime = CalcRoundTime(specialEnemyRound, specialEnemyPlayerCount, corpseDelay[specialEnemyPlayerCount - 1][specialEnemyRound - 1], true);
                        meatballDelay = "Meatball Spawn Delay: " + ParseTimeFromMilli(GetSpecialEnemySpawnRate(specialEnemyPlayerCount, "Meatballs"));
                        lastEditedSpecialEnemy = "Meatballs";
                    }
                    break;
                case hashstr("Giant + DE"):
                    ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(dogDelay.c_str()).x) / 2 + 10);
                    ImGui::Text(dogDelay.c_str());
                    ImGui::SetNextItemWidth(130.0f);
                    ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcItemWidth()) / 2 - ImGui::CalcTextSize("# of Dogs").x / 2 + 10);
                    if (ImGui::InputInt("# of Dogs", &dogCount, 1, NULL, ImGuiInputTextFlags_EnterReturnsTrue))
                    {
                        if (dogCount < 0)
                            dogCount = 0;
                        if (dogCount > specialZombiesForRound)
                            dogCount = specialZombiesForRound;
                        specialEnemyCount_1 = dogCount;
                        specialEnemiesRoundTime = CalcRoundTime(specialEnemyRound, specialEnemyPlayerCount, corpseDelay[specialEnemyPlayerCount - 1][specialEnemyRound - 1], true);
                        dogDelay = "Dogs Spawn Delay: " + ParseTimeFromMilli(GetSpecialEnemySpawnRate(specialEnemyPlayerCount, "Dogs"));
                        lastEditedSpecialEnemy = "Dogs";
                    }
                    ImGui::SetNextItemWidth(130.0f);
                    ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcItemWidth()) / 2 - ImGui::CalcTextSize("# of Dog Rounds").x / 2 + 10);
                    if (ImGui::BeginCombo("# of Dog Rounds", dogRoundCountCombo[numDogRounds - 1].c_str(), ImGuiComboFlags_HeightRegular))
                    {
                        for (int i = 1; i < dogRoundCountCombo.size() + 1; i++)
                        {
                            const bool is_selected = numDogRounds == i;
                            if (ImGui::Selectable(dogRoundCountCombo[i - 1].c_str(), is_selected))
                                numDogRounds = i;
                            if (is_selected)
                            {
                                ImGui::SetItemDefaultFocus();
                                specialEnemiesRoundTime = CalcRoundTime(specialEnemyRound, specialEnemyPlayerCount, corpseDelay[specialEnemyPlayerCount - 1][specialEnemyRound - 1], true);
                                dogDelay = "Dogs Spawn Delay: " + ParseTimeFromMilli(GetSpecialEnemySpawnRate(specialEnemyPlayerCount, "Dogs"));
                            }
                        }
                        ImGui::EndCombo();
                    }
                    if (prevDogRoundCount != numDogRounds)
                        specialEnemiesRoundTime = CalcRoundTime(specialEnemyRound, specialEnemyPlayerCount, corpseDelay[specialEnemyPlayerCount - 1][specialEnemyRound - 1], true);
                    break;
                case hashstr("ZNS"):
                    ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(spiderDelay.c_str()).x) / 2 + 10);
                    ImGui::Text(spiderDelay.c_str());
                    ImGui::SetNextItemWidth(130.0f);
                    ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcItemWidth()) / 2 - ImGui::CalcTextSize("# of Spiders").x / 2 + 10);
                    if (ImGui::InputInt("# of Spiders", &islandSpidersCount, 1, NULL, ImGuiInputTextFlags_EnterReturnsTrue))
                    {
                        if (islandSpidersCount < 0)
                            islandSpidersCount = 0;
                        if (islandSpidersCount > specialZombiesForRound)
                            islandSpidersCount = specialZombiesForRound;
                        specialEnemyCount_1 = islandSpidersCount;
                        specialEnemiesRoundTime = CalcRoundTime(specialEnemyRound, specialEnemyPlayerCount, corpseDelay[specialEnemyPlayerCount - 1][specialEnemyRound - 1], true);
                        spiderDelay = "Spider Spawn Delay: " + ParseTimeFromMilli(GetSpecialEnemySpawnRate(specialEnemyPlayerCount, "Spiders"));
                        lastEditedSpecialEnemy = "IslandSpiders";
                    }
                    break;
                case hashstr("GK"):
                    ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(manglerDelay.c_str()).x) / 2 + 10);
                    ImGui::Text(manglerDelay.c_str());
                    ImGui::SetNextItemWidth(130.0f);
                    ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcItemWidth()) / 2 - ImGui::CalcTextSize("# of Manglers").x / 2 + 10);
                    if (ImGui::InputInt("# of Manglers", &manglersCount, 1, NULL, ImGuiInputTextFlags_EnterReturnsTrue))
                    {
                        if (manglersCount < 0)
                            manglersCount = 0;
                        if (manglersCount + valksCount > specialZombiesForRound)
                            manglersCount = specialZombiesForRound - valksCount;
                        specialEnemyCount_1 = manglersCount;
                        specialEnemiesRoundTime = CalcRoundTime(specialEnemyRound, specialEnemyPlayerCount, corpseDelay[specialEnemyPlayerCount - 1][specialEnemyRound - 1], true);
                        manglerDelay = "Mangler Spawn Delay: " + ParseTimeFromMilli(GetSpecialEnemySpawnRate(specialEnemyPlayerCount, "Manglers"));
                        lastEditedSpecialEnemy = "Manglers";
                    }
                    ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(valkDelay.c_str()).x) / 2 + 10);
                    ImGui::Text(valkDelay.c_str());
                    ImGui::SetNextItemWidth(130.0f);
                    ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcItemWidth()) / 2 - ImGui::CalcTextSize("# of Valks").x / 2 + 10);
                    if (ImGui::InputInt("# of Valks", &valksCount, 1, NULL, ImGuiInputTextFlags_EnterReturnsTrue))
                    {
                        if (valksCount < 0)
                            valksCount = 0;
                        if (valksCount + manglersCount > specialZombiesForRound)
                            valksCount = specialZombiesForRound - manglersCount;
                        specialEnemyCount_2 = valksCount;
                        specialEnemiesRoundTime = CalcRoundTime(specialEnemyRound, specialEnemyPlayerCount, corpseDelay[specialEnemyPlayerCount - 1][specialEnemyRound - 1], true);
                        valkDelay = "Valk Spawn Delay: " + ParseTimeFromMilli(GetSpecialEnemySpawnRate(specialEnemyPlayerCount, "Valks"));
                        lastEditedSpecialEnemy = "Valks";
                    }
                    break;
                case hashstr("Rev"):
                    ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(spiderDelay.c_str()).x) / 2 + 10);
                    ImGui::Text(spiderDelay.c_str());
                    ImGui::SetNextItemWidth(130.0f);
                    ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcItemWidth()) / 2 - ImGui::CalcTextSize("# of Spiders").x / 2 + 10);
                    if (ImGui::InputInt("# of Spiders", &genesisSpidersCount, 1, NULL, ImGuiInputTextFlags_EnterReturnsTrue))
                    {
                        if (genesisSpidersCount < 0)
                            genesisSpidersCount = 0;
                        if (genesisSpidersCount + furysCount + keepersCount > specialZombiesForRound)
                            genesisSpidersCount = specialZombiesForRound - furysCount - keepersCount;
                        specialEnemyCount_1 = genesisSpidersCount;
                        specialEnemiesRoundTime = CalcRoundTime(specialEnemyRound, specialEnemyPlayerCount, corpseDelay[specialEnemyPlayerCount - 1][specialEnemyRound - 1], true);
                        spiderDelay = "Spider Spawn Delay: " + ParseTimeFromMilli(GetSpecialEnemySpawnRate(specialEnemyPlayerCount, "Spiders"));
                        lastEditedSpecialEnemy = "GenesisSpiders";
                    }
                    ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(furyDelay.c_str()).x) / 2 + 10);
                    ImGui::Text(furyDelay.c_str());
                    ImGui::SetNextItemWidth(130.0f);
                    ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcItemWidth()) / 2 - ImGui::CalcTextSize("# of Furys").x / 2 + 10);
                    if (ImGui::InputInt("# of Furys", &furysCount, 1, NULL, ImGuiInputTextFlags_EnterReturnsTrue))
                    {
                        if (furysCount < 0)
                            furysCount = 0;
                        if (genesisSpidersCount + furysCount + keepersCount > specialZombiesForRound)
                            furysCount = specialZombiesForRound - genesisSpidersCount - keepersCount;
                        specialEnemyCount_2 = furysCount;
                        specialEnemiesRoundTime = CalcRoundTime(specialEnemyRound, specialEnemyPlayerCount, corpseDelay[specialEnemyPlayerCount - 1][specialEnemyRound - 1], true);
                        furyDelay = "Fury Spawn Delay: " + ParseTimeFromMilli(GetSpecialEnemySpawnRate(specialEnemyPlayerCount, "Furys"));
                        lastEditedSpecialEnemy = "Furys";
                    }
                    ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(keeperDelay.c_str()).x) / 2 + 10);
                    ImGui::Text(keeperDelay.c_str());
                    ImGui::SetNextItemWidth(130.0f);
                    ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcItemWidth()) / 2 - ImGui::CalcTextSize("# of Keepers").x / 2 + 10);
                    if (ImGui::InputInt("# of Keepers", &keepersCount, 1, NULL, ImGuiInputTextFlags_EnterReturnsTrue))
                    {
                        if (keepersCount < 0)
                            keepersCount = 0;
                        if (genesisSpidersCount + furysCount + keepersCount > specialZombiesForRound)
                            keepersCount = specialZombiesForRound - genesisSpidersCount - furysCount;
                        specialEnemyCount_3 = keepersCount;
                        specialEnemiesRoundTime = CalcRoundTime(specialEnemyRound, specialEnemyPlayerCount, corpseDelay[specialEnemyPlayerCount - 1][specialEnemyRound - 1], true);
                        keeperDelay = "Keeper Spawn Delay: " + ParseTimeFromMilli(GetSpecialEnemySpawnRate(specialEnemyPlayerCount, "Keepers"));
                        lastEditedSpecialEnemy = "Keepers";
                    }
                    break;
                }
                std::string specialEnemiesRoundTimeText("Estimated Round Time:");
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(specialEnemiesRoundTimeText.c_str()).x) / 2 + 10);
                ImGui::Text(specialEnemiesRoundTimeText.c_str());
                HelpMarker("This doesn't account for reduced corpse delay by having more special enemies instead of normal zombies. Times will differ slightly from actual observed times, growing more inaccurate with higher rounds.");
                SAMELINE;
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(specialEnemiesRoundTime.c_str()).x) / 2 + 17.5f);
                ImGui::Text(specialEnemiesRoundTime.c_str());
                ImGui::EndChild();
            }
            // Calc Data 6
            {
                SAMELINE;
                ImGui::BeginChild("Calc Data 6", ImVec2(300.0f, 265.0f), true);
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Number Of Players/Current Round").x) / 2 + 10);
                ImGui::Text("Number Of Players/Current Round");
                HelpMarker("Moon doesn't increase spawn rate with player count. On top of that, spawn rate increases 1 round every time you travel to the Earth/Moon.");
                SAMELINE;
                ImGui::SetNextItemWidth(120.0f);
                if (ImGui::InputInt("##Number Of Players - Moon", &moonPlayerCount, 1, NULL, ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    if (moonPlayerCount < 1)
                        moonPlayerCount = 1;
                    else if (moonPlayerCount > 4)
                        moonPlayerCount = 4;
                    if (moonRoundSkip)
                        moonExpectedRoundTime = SpecialRoundTime(moonRound, moonPlayerCount, corpseDelay[moonPlayerCount - 1][4], false);
                    else
                        moonExpectedRoundTime = SpecialRoundTime(moonRound, moonPlayerCount, corpseDelay[moonPlayerCount - 1][moonRound - 1], false);
                } SAMELINE;
                ImGui::SetNextItemWidth(120.0f);
                if (ImGui::InputInt("##Current Round - Moon", &moonRound, 1, NULL, ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    if (moonRound < 1)
                        moonRound = 1;
                    else if (moonRound > 255)
                        moonRound = 255;
                    if (moonRoundSkip)
                        moonExpectedRoundTime = SpecialRoundTime(moonRound, moonPlayerCount, corpseDelay[moonPlayerCount - 1][4], false);
                    else
                        moonExpectedRoundTime = SpecialRoundTime(moonRound, moonPlayerCount, corpseDelay[moonPlayerCount - 1][moonRound - 1], false);
                }
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Number Of Earth Travels").x) / 2 + 10);
                ImGui::Text("Number Of Earth Travels");
                ImGui::SetNextItemWidth(120.0f);
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcItemWidth()) / 2 + 10);
                if (ImGui::InputInt("##Number of Earth travels", &moonEarthTravels, 1, NULL, ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    if (moonEarthTravels < 0)
                        moonEarthTravels = 0;
                    if (moonEarthTravels > 27)
                        moonEarthTravels = 27;
                    if (moonRoundSkip)
                        moonExpectedRoundTime = SpecialRoundTime(moonRound, moonPlayerCount, corpseDelay[moonPlayerCount - 1][4], false);
                    else
                        moonExpectedRoundTime = SpecialRoundTime(moonRound, moonPlayerCount, corpseDelay[moonPlayerCount - 1][moonRound - 1], false);
                }
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Number Of Round Skips").x) / 2 + 10);
                ImGui::Text("Number Of Round Skips");
                DummySpace(7.5f, 0.0f);
                SAMELINE;
                ImGui::SetNextItemWidth(120.0f);
                if (ImGui::InputInt("##Number of Round Skips", &moonRoundSkips, 1, NULL, ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    if (moonRoundSkips < 0)
                        moonRoundSkips = 0;
                    if (moonRoundSkips > moonEarthTravels)
                        moonRoundSkips = moonEarthTravels;
                    if (moonRoundSkip)
                        moonExpectedRoundTime = SpecialRoundTime(moonRound, moonPlayerCount, corpseDelay[moonPlayerCount - 1][4], false);
                    else
                        moonExpectedRoundTime = SpecialRoundTime(moonRound, moonPlayerCount, corpseDelay[moonPlayerCount - 1][moonRound - 1], false);
                } SAMELINE;
                if (ImGui::Checkbox("This Round?", &moonRoundSkip))
                {
                    if (moonRoundSkip)
                        moonExpectedRoundTime = SpecialRoundTime(moonRound, moonPlayerCount, corpseDelay[moonPlayerCount - 1][4], false);
                    else
                        moonExpectedRoundTime = SpecialRoundTime(moonRound, moonPlayerCount, corpseDelay[moonPlayerCount - 1][moonRound - 1], false);
                }
                std::string moonExpectedRoundTimeText("Expected Round Time For Round " + std::to_string(moonRound) + ":");
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(moonExpectedRoundTimeText.c_str()).x) / 2 + 10);
                ImGui::Text(moonExpectedRoundTimeText.c_str());
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(moonExpectedRoundTime.c_str()).x) / 2 + 10);
                ImGui::Text(moonExpectedRoundTime.c_str());
                ImGui::EndChild();
            }

            // Calc Data 7
            {
                SAMELINE;
                ImGui::BeginChild("Calc Data 7", ImVec2(300.0f, 265.0f), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Number Of Players/Current Round").x) / 2 + 10);
                ImGui::Text("Number Of Players/Current Round");
                HelpMarker("The GK Lockdown scales to the current round and player count, as well as increasing spawn rate each wave if not already maxed. The time presented will be when to nuke as long as you stay under 22 zombies at once. A new wave starts when the windows close.");
                SAMELINE;
                ImGui::SetNextItemWidth(120.0f);
                if (ImGui::InputInt("##Number Of Players - GK Lockdown", &gkLockdownPlayerCount, 1, NULL, ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    if (gkLockdownPlayerCount < 1)
                        gkLockdownPlayerCount = 1;
                    else if (gkLockdownPlayerCount > 4)
                        gkLockdownPlayerCount = 4;
                    CalcLockdownTime(gkLockdownRound, gkLockdownPlayerCount);
                } SAMELINE;
                ImGui::SetNextItemWidth(120.0f);
                if (ImGui::InputInt("##Current Round - GK Lockdown", &gkLockdownRound, 1, NULL, ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    if (gkLockdownRound < 1)
                        gkLockdownRound = 1;
                    else if (gkLockdownRound > 255)
                        gkLockdownRound = 255;
                    CalcLockdownTime(gkLockdownRound, gkLockdownPlayerCount);
                }
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Expected Time For Wave 1: ").x) / 2 + 10);
                ImGui::Text("Expected Time For Wave 1: ");
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(waveTime_1.c_str()).x) / 2 + 10);
                ImGui::Text(waveTime_1.c_str());
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Expected Time For Wave 2: ").x) / 2 + 10);
                ImGui::Text("Expected Time For Wave 2: ");
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(waveTime_2.c_str()).x) / 2 + 10);
                ImGui::Text(waveTime_2.c_str());
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Expected Time For Wave 3: ").x) / 2 + 10);
                ImGui::Text("Expected Time For Wave 3: ");
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(waveTime_3.c_str()).x) / 2 + 10);
                ImGui::Text(waveTime_3.c_str());
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Expected Time For Wave 4: ").x) / 2 + 10);
                ImGui::Text("Expected Time For Wave 4: ");
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(waveTime_4.c_str()).x) / 2 + 10);
                ImGui::Text(waveTime_4.c_str());
                ImGui::EndChild();
            }

            // Calc Data 8
            {
                ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 310.0f, ImGui::GetCursorPosY() - 135.0f));
                ImGui::BeginChild("Calc Data 8", ImVec2(300.0f, 130.0f), true);
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Number Of Players/Current Round").x) / 2 + 10);
                ImGui::Text("Number Of Players/Current Round");
                HelpMarker("SOE has a constant spawn rate for rounds 1-4 regardless of player count.");
                SAMELINE;
                ImGui::SetNextItemWidth(120.0f);
                if (ImGui::InputInt("##Number Of Players - SOE", &soePlayerCount, 1, NULL, ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    if (soePlayerCount < 1)
                        soePlayerCount = 1;
                    else if (soePlayerCount > 4)
                        soePlayerCount = 4;
                    soeExpectedRoundTime = SpecialRoundTime(soeRound, soePlayerCount, corpseDelay[soePlayerCount - 1][soeRound - 1], true);
                } SAMELINE;
                ImGui::SetNextItemWidth(120.0f);
                if (ImGui::InputInt("##Current Round - SOE", &soeRound, 1, NULL, ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    if (soeRound < 1)
                        soeRound = 1;
                    else if (soeRound > 4)
                        soeRound = 4;
                    soeExpectedRoundTime = SpecialRoundTime(soeRound, soePlayerCount, corpseDelay[soePlayerCount - 1][soeRound - 1], true);
                }
                std::string soeExpectedRoundTimeText("Expected Round Time For Round " + std::to_string(soeRound) + ":");
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(soeExpectedRoundTimeText.c_str()).x) / 2 + 10);
                ImGui::Text(soeExpectedRoundTimeText.c_str());
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(soeExpectedRoundTime.c_str()).x) / 2 + 10);
                ImGui::Text(soeExpectedRoundTime.c_str());
                ImGui::EndChild();
            }

            // Calc Data 9
            {
                SAMELINE;
                ImGui::BeginChild("Calc Data 9", ImVec2(300.0f, 130.0f), true);
                ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Number Of Zombies/Current Round").x) / 2 + 10);
                ImGui::Text("Number Of Zombies/Current Round");
                HelpMarker("If you know the amount of zombies left on a round, and you want to figure out how much longer the round is, you can input the data here to get that time. The main use is finding how long to wait after 1st flag on SOE before nuking. Does not account for corpse delay.");
                SAMELINE;
                ImGui::SetNextItemWidth(120.0f);
                if (ImGui::InputInt("##Number Of Zombies - Player Calc", &customZombiesLeft, 1, NULL, ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    if (customZombiesLeft < 0)
                        customZombiesLeft = 0;
                    else if (customZombiesLeft > GetZombieCountForRound(customZombiesLeftRound, customZombiesLeftPlayerCount))
                        customZombiesLeft = GetZombieCountForRound(customZombiesLeftRound, customZombiesLeftPlayerCount);
                    customCalcExpectedRoundTime = CustomRoundTime(customZombiesLeftRound, customZombiesLeftPlayerCount, customZombiesLeft);
                } SAMELINE;
                ImGui::SetNextItemWidth(120.0f);
                if (ImGui::InputInt("##Current Round - Player Calc", &customZombiesLeftRound, 1, NULL, ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    if (customZombiesLeftRound < 1)
                        customZombiesLeftRound = 1;
                    else if (customZombiesLeftRound > 255)
                        customZombiesLeftRound = 255;
                    customCalcExpectedRoundTime = CustomRoundTime(customZombiesLeftRound, customZombiesLeftPlayerCount, customZombiesLeft);
                }
                DummySpace(15.0f, 0.0f);
                SAMELINE;
                ImGui::Text("Number Of Players");
                DummySpace(15.0f, 0.0f);
                SAMELINE;
                ImGui::SetNextItemWidth(120.0f);
                if (ImGui::InputInt("##Number Of Players - Player Calc", &customZombiesLeftPlayerCount, 1, NULL, ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    if (customZombiesLeftPlayerCount < 0)
                        customZombiesLeftPlayerCount = 0;
                    else if (customZombiesLeftPlayerCount > 4)
                        customZombiesLeftPlayerCount = 4;
                    customCalcExpectedRoundTime = CustomRoundTime(customZombiesLeftRound, customZombiesLeftPlayerCount, customZombiesLeft);
                } SAMELINE;
                std::string calcTimeText("Time: " + customCalcExpectedRoundTime);
                ImGui::Text(calcTimeText.c_str());
                ImGui::EndChild();
            }
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}

void CodeGuidesPtr()
{
    if (ImGui::BeginTabBar("Code Guide Tabs"))
    {
        if (ImGui::BeginTabItem("SOE Code"))
        {
            ImGui::SetNextItemWidth(130.0f);
            if (ImGui::BeginCombo("Code 1", soeCodeCombo[codeIndex_1].c_str(), ImGuiComboFlags_HeightRegular))
            {
                for (int i = 0; i < soeCodeCombo.size(); i++)
                {
                    const bool is_selected = codeIndex_1 == i;
                    if (ImGui::Selectable(soeCodeCombo[i].c_str(), is_selected))
                        codeIndex_1 = i;
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            ImGui::SetNextItemWidth(130.0f);
            if (ImGui::BeginCombo("Code 2", soeCodeCombo[codeIndex_2].c_str(), ImGuiComboFlags_HeightRegular))
            {
                for (int i = 0; i < soeCodeCombo.size(); i++)
                {
                    const bool is_selected = codeIndex_2 == i;
                    if (ImGui::Selectable(soeCodeCombo[i].c_str(), is_selected))
                        codeIndex_2 = i;
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            ImGui::Image(codeImgList[codeIndex_1].imgTexture, ImVec2(490.0f, 490.0f));
            SAMELINE;
            ImGui::Image(codeImgList[codeIndex_2].imgTexture, ImVec2(490.0f, 490.0f));
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Ice Code"))
        {
            if (showSolution)
            {
                if (CreateButton(ICON_FA_ARROW_LEFT, ImVec2(50.0f, 25.0f)))
                    showSolution = false;
                ImGui::Image(ImageHelp::iceCodeImgList["solution"].imgTexture, ImVec2(min(1280.0f, ImGui::GetContentRegionAvail().x), min(720.0f, ImGui::GetContentRegionAvail().y - 30)));
            }
            else
            {
                ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 0, 0, 0)); ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(25, 100, 128, 100));
                bool skip = false;
                // Images group 1
                {
                    ImGui::BeginGroup();
                    if (gameChecked[0])
                    {
                        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(170, 0, 0, 255));
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(170, 0, 0, 255));
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(170, 0, 0, 255));
                    }
                    if (ImGui::ImageButton(randomIceCodePairs[0].symbolImage.imgTexture, ImVec2(170.0f, 152.0f)))
                    {
                        if (!gameChecked[0])
                            skip = true;
                        if (randomIceCodePairs[0].symbolImage.index == iceCodePairs[randomList[gameProgress]].symbolImage.index)
                            ProgressGame(true, 0);
                        else
                            ProgressGame(false, 0);
                    }
                    if (!skip && gameChecked[0])
                        ImGui::PopStyleColor(3);
                    skip = false;
                    SAMELINE;
                    if (gameChecked[1])
                    {
                        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(170, 0, 0, 255));
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(170, 0, 0, 255));
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(170, 0, 0, 255));
                    }
                    if (ImGui::ImageButton(randomIceCodePairs[1].symbolImage.imgTexture, ImVec2(170.0f, 152.0f)))
                    {
                        if (!gameChecked[1])
                            skip = true;
                        if (randomIceCodePairs[1].symbolImage.index == iceCodePairs[randomList[gameProgress]].symbolImage.index)
                            ProgressGame(true, 1);
                        else
                            ProgressGame(false, 1);
                    } SAMELINE;
                    if (!skip && gameChecked[1])
                        ImGui::PopStyleColor(3);
                    skip = false;
                    if (gameChecked[2])
                    {
                        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(170, 0, 0, 255));
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(170, 0, 0, 255));
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(170, 0, 0, 255));
                    }
                    if (ImGui::ImageButton(randomIceCodePairs[2].symbolImage.imgTexture, ImVec2(170.0f, 152.0f)))
                    {
                        if (!gameChecked[2])
                            skip = true;
                        if (randomIceCodePairs[2].symbolImage.index == iceCodePairs[randomList[gameProgress]].symbolImage.index)
                            ProgressGame(true, 2);
                        else
                            ProgressGame(false, 2);
                    } SAMELINE;
                    if (!skip && gameChecked[2])
                        ImGui::PopStyleColor(3);
                    skip = false;
                    if (gameChecked[3])
                    {
                        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(170, 0, 0, 255));
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(170, 0, 0, 255));
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(170, 0, 0, 255));
                    }
                    if (ImGui::ImageButton(randomIceCodePairs[3].symbolImage.imgTexture, ImVec2(170.0f, 152.0f)))
                    {
                        if (!gameChecked[3])
                            skip = true;
                        if (randomIceCodePairs[3].symbolImage.index == iceCodePairs[randomList[gameProgress]].symbolImage.index)
                            ProgressGame(true, 3);
                        else
                            ProgressGame(false, 3);
                    }
                    if (!skip && gameChecked[3])
                        ImGui::PopStyleColor(3);
                    skip = false;
                    ImGui::EndGroup();
                }
                SAMELINE;
                ImGui::BeginGroup();
                if (CreateButton("Show Solution Key", ImVec2(150.0f, 25.0f)))
                    showSolution = true;
                ImGui::Text(gameTime.c_str());
                ImGui::Text(accuracy.c_str());
                ImGui::EndGroup();
                // Images group 2
                {
                    ImGui::BeginGroup();
                    if (gameChecked[4])
                    {
                        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(170, 0, 0, 255));
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(170, 0, 0, 255));
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(170, 0, 0, 255));
                    }
                    if (ImGui::ImageButton(randomIceCodePairs[4].symbolImage.imgTexture, ImVec2(170.0f, 152.0f)))
                    {
                        if (!gameChecked[4])
                            skip = true;
                        if (randomIceCodePairs[4].symbolImage.index == iceCodePairs[randomList[gameProgress]].symbolImage.index)
                            ProgressGame(true, 4);
                        else
                            ProgressGame(false, 4);
                    } SAMELINE;
                    if (!skip && gameChecked[4])
                        ImGui::PopStyleColor(3);
                    skip = false;
                    if (gameChecked[5])
                    {
                        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(170, 0, 0, 255));
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(170, 0, 0, 255));
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(170, 0, 0, 255));
                    }
                    if (ImGui::ImageButton(randomIceCodePairs[5].symbolImage.imgTexture, ImVec2(170.0f, 152.0f)))
                    {
                        if (!gameChecked[5])
                            skip = true;
                        if (randomIceCodePairs[5].symbolImage.index == iceCodePairs[randomList[gameProgress]].symbolImage.index)
                            ProgressGame(true, 5);
                        else
                            ProgressGame(false, 5);
                    } SAMELINE;
                    if (!skip && gameChecked[5])
                        ImGui::PopStyleColor(3);
                    skip = false;
                    if (gameChecked[6])
                    {
                        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(170, 0, 0, 255));
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(170, 0, 0, 255));
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(170, 0, 0, 255));
                    }
                    if (ImGui::ImageButton(randomIceCodePairs[6].symbolImage.imgTexture, ImVec2(170.0f, 152.0f)))
                    {
                        if (!gameChecked[6])
                            skip = true;
                        if (randomIceCodePairs[6].symbolImage.index == iceCodePairs[randomList[gameProgress]].symbolImage.index)
                            ProgressGame(true, 6);
                        else
                            ProgressGame(false, 6);
                    } SAMELINE;
                    if (!skip && gameChecked[6])
                        ImGui::PopStyleColor(3);
                    skip = false;
                    if (gameChecked[7])
                    {
                        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(170, 0, 0, 255));
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(170, 0, 0, 255));
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(170, 0, 0, 255));
                    }
                    if (ImGui::ImageButton(randomIceCodePairs[7].symbolImage.imgTexture, ImVec2(170.0f, 152.0f)))
                    {
                        if (!gameChecked[7])
                            skip = true;
                        if (randomIceCodePairs[7].symbolImage.index == iceCodePairs[randomList[gameProgress]].symbolImage.index)
                            ProgressGame(true, 7);
                        else
                            ProgressGame(false, 7);
                    }
                    if (!skip && gameChecked[7])
                        ImGui::PopStyleColor(3);
                    skip = false;
                    ImGui::EndGroup();
                }
                SAMELINE;
                DummySpace(15.0f, 0.0f);
                SAMELINE;
                ImGui::Image(iceCodePairs[randomList[gameProgress]].digitImage.imgTexture, ImVec2(170.0f, 152.0f));
                // Images group 3
                {
                    ImGui::BeginGroup();
                    if (gameChecked[8])
                    {
                        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(170, 0, 0, 255));
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(170, 0, 0, 255));
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(170, 0, 0, 255));
                    }
                    if (ImGui::ImageButton(randomIceCodePairs[8].symbolImage.imgTexture, ImVec2(170.0f, 152.0f)))
                    {
                        if (!gameChecked[8])
                            skip = true;
                        if (randomIceCodePairs[8].symbolImage.index == iceCodePairs[randomList[gameProgress]].symbolImage.index)
                            ProgressGame(true, 8);
                        else
                            ProgressGame(false, 8);
                    } SAMELINE;
                    if (!skip && gameChecked[8])
                        ImGui::PopStyleColor(3);
                    skip = false;
                    if (gameChecked[9])
                    {
                        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(170, 0, 0, 255));
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(170, 0, 0, 255));
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(170, 0, 0, 255));
                    }
                    if (ImGui::ImageButton(randomIceCodePairs[9].symbolImage.imgTexture, ImVec2(170.0f, 152.0f)))
                    {
                        if (!gameChecked[9])
                            skip = true;
                        if (randomIceCodePairs[9].symbolImage.index == iceCodePairs[randomList[gameProgress]].symbolImage.index)
                            ProgressGame(true, 9);
                        else
                            ProgressGame(false, 9);
                    } SAMELINE;
                    if (!skip && gameChecked[9])
                        ImGui::PopStyleColor(3);
                    skip = false;
                    if (gameChecked[10])
                    {
                        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(170, 0, 0, 255));
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(170, 0, 0, 255));
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(170, 0, 0, 255));
                    }
                    if (ImGui::ImageButton(randomIceCodePairs[10].symbolImage.imgTexture, ImVec2(170.0f, 152.0f)))
                    {
                        if (!gameChecked[10])
                            skip = true;
                        if (randomIceCodePairs[10].symbolImage.index == iceCodePairs[randomList[gameProgress]].symbolImage.index)
                            ProgressGame(true, 10);
                        else
                            ProgressGame(false, 10);
                    } SAMELINE;
                    if (!skip && gameChecked[10])
                        ImGui::PopStyleColor(3);
                    skip = false;
                    if (gameChecked[11])
                    {
                        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(170, 0, 0, 255));
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(170, 0, 0, 255));
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(170, 0, 0, 255));
                    }
                    if (ImGui::ImageButton(randomIceCodePairs[11].symbolImage.imgTexture, ImVec2(170.0f, 152.0f)))
                    {
                        if (!gameChecked[11])
                            skip = true;
                        if (randomIceCodePairs[11].symbolImage.index == iceCodePairs[randomList[gameProgress]].symbolImage.index)
                            ProgressGame(true, 11);
                        else
                            ProgressGame(false, 11);
                    }
                    if (!skip && gameChecked[11])
                        ImGui::PopStyleColor(3);
                    ImGui::EndGroup();
                }
                ImGui::PopStyleColor(2);

                // Timer operations
                if (gameStarted)
                {
                    std::time_t finalTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - startTime;
                    gameTime = "Time: " + ParseTimeFromMilli((int)finalTime);
                }
            }
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}

void GKValveSolverPtr()
{
    if (showEvaluation)
    {
        if (CreateButton(ICON_FA_ARROW_LEFT, ImVec2(50.0f, 25.0f)))
            showEvaluation = false;
        
        DummySpace(15.0f, 0.0f);
        SAMELINE;
        ImGui::BeginGroup();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 450 - ImGui::CalcTextSize("Possible Solutions").x / 2);
        ImGui::Text("Possible Solutions");

        if (ImGui::BeginTable("PossibleValves", 6, ImGuiTableFlags_NoBordersInBody, ImVec2(910.0f, 50.0f)))
        {
            ImGui::TableNextRow();
            int used = 1;
            ImGui::TableSetColumnIndex(0);
            ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, IM_COL32(128.0f, 128.0f, 128.0f, 128.0f));
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x / 2 - ImGui::CalcTextSize("Green, P").x / 2);
            ImGui::Text("Green, P");
            for (int column = 0; column < 6; column++)
            {
                if (valveLocations[column] == passwordLocation)
                    continue;
                ImGui::TableSetColumnIndex(used);
                ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, IM_COL32(128.0f, 128.0f, 128.0f, 128.0f));
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x / 2 - ImGui::CalcTextSize(valveLocations[column].c_str()).x / 2);
                ImGui::Text(valveLocations[column].c_str());
                used++;
            }
            for (int row = 0; row < 6; row++)
            {
                if (possibleValves_1.find(valveLocations[row]) == possibleValves_1.end())
                    continue;
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, IM_COL32(128.0f, 128.0f, 128.0f, 128.0f));
                std::string key = valveLocationsAbbr[valveLocations[row]] + ", " + valveLocationsAbbr[passwordLocation] + " - 1";
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x / 2 - ImGui::CalcTextSize(key.c_str()).x / 2);
                ImGui::Text(key.c_str());
                used = 1;
                for (int column = 0; column < 6; column++)
                {
                    if (valveLocations[column] == passwordLocation)
                        continue;
                    ImVec4 color;
                    ImGui::TableSetColumnIndex(used);
                    if (possibleValves_1[valveLocations[row]][column] == "P")
                        color = ImVec4(255.0f, 105.0f, 180.0f, 255.0f);
                    else if (valveDirections[column][std::stoi(possibleValves_1[valveLocations[row]][column]) - 1])
                        color = ImVec4(0.0f, 128.0f, 0.0f, 255.0f);
                    else
                        color = ImVec4(140.0f, 0.0f, 0.0f, 255.0f);
                    FakeButton(possibleValves_1[valveLocations[row]][column].c_str(), ImVec2(145.0f, 25.0f), color);
                    used++;
                }
                if (possibleValves_2.find(valveLocations[row]) == possibleValves_2.end())
                    continue;
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, IM_COL32(128.0f, 128.0f, 128.0f, 128.0f));
                key = valveLocationsAbbr[valveLocations[row]] + ", " + valveLocationsAbbr[passwordLocation] + " - 2";
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x / 2 - ImGui::CalcTextSize(key.c_str()).x / 2);
                ImGui::Text(key.c_str());
                used = 1;
                for (int column = 0; column < 6; column++)
                {
                    if (valveLocations[column] == passwordLocation)
                        continue;
                    ImVec4 color;
                    ImGui::TableSetColumnIndex(used);
                    if (possibleValves_2[valveLocations[row]][column] == "P")
                        color = ImVec4(255.0f, 105.0f, 180.0f, 255.0f);
                    else if (valveDirections[column][std::stoi(possibleValves_2[valveLocations[row]][column]) - 1])
                        color = ImVec4(0.0f, 128.0f, 0.0f, 255.0f);
                    else
                        color = ImVec4(140.0f, 0.0f, 0.0f, 255.0f);
                    FakeButton(possibleValves_2[valveLocations[row]][column].c_str(), ImVec2(145.0f, 25.0f), color);
                    used++;
                }
            }
            ImGui::EndTable();
        }
        ImGui::EndGroup();
    }
    else
    {
        DummySpace(30.0f, 0.0f);
        SAMELINE;
        for (int i = 0; i < 6; i++)
        {
            ImGui::BeginGroup();
            ImGui::AlignTextToFramePadding();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (60 - ImGui::CalcTextSize(valveLocations[i].c_str()).x / 2));
            ImGui::Text(valveLocations[i].c_str());
            SAMELINE;
            std::string checkedGreen = "Not Green##";
            checkedGreen += valveLocations[i];
            bool disableCheck = false;
            if (greenLocation == valveLocations[i] || passwordLocation == valveLocations[i])
            {
                ImGui::BeginDisabled();
                disableCheck = true;
                checkedGreenArray[i] = false;
            }
            if (ImGui::Checkbox(checkedGreen.c_str(), &checkedGreenArray[i]))
            {
                valveGreen[i] = false;
                CalcValveProbabilities();
                CalcRemainingGreen();
            }
            if (disableCheck && (greenLocation == valveLocations[i] || passwordLocation == valveLocations[i]))
            {
                ImGui::EndDisabled();
            }
            if (checkedGreenArray[i])
            {
                ImGui::BeginDisabled();
            }
            ImVec4 color = { 170.0f, 0.0f, 0.0f, 255.0f };
            if (valveGreen[i])
                color = { 0.0f, 128.0f, 0.0f, 255.0f };
            std::string green = "Green##";
            green += valveLocations[i];
            if (CreateButton(green, ImVec2(120.0f, 25.0f), NULL, NULL, color))
            {
                if (valveGreen[i])
                {
                    if (!noGreenChoice)
                    {
                        valveGreen = { false, false, false, false, false, false };
                        greenChosen = false;
                        greenLocation = "";
                        valveSolutionsText_1 = { "?", "?", "?", "?", "?", "?" };
                        valveSolutionsText_2 = { "?", "?", "?", "?", "?", "?" };
                        CalcValveProbabilities();
                    }
                }
                else
                {
                    valveGreen = { false, false, false, false, false, false };
                    valveGreen[i] = true;
                    greenLocation = valveLocations[i];
                    if (valvePassword[i])
                    {
                        valvePassword[i] = false;
                        passwordChosen = false;
                        valveSolutionsText_1 = { "?", "?", "?", "?", "?", "?" };
                        valveSolutionsText_2 = { "?", "?", "?", "?", "?", "?" };
                    }
                    if (passwordChosen)
                    {
                        valveSolutionsText_1 = valveSolutions_1[passwordLocation][greenLocation];
                        valveSolutionsText_2 = valveSolutions_2[passwordLocation][greenLocation];
                    }
                    greenChosen = true;
                    CalcValveProbabilities();
                }
            } SAMELINE;
            if (checkedGreenArray[i])
            {
                ImGui::EndDisabled();
            }
            if (valvePassword[i])
                color = { 0.0f, 128.0f, 0.0f, 255.0f };
            else
                color = { 170.0f, 0.0f, 0.0f, 255.0f };
            std::string password = "Password##";
            password += valveLocations[i];
            if (CreateButton(password, ImVec2(120.0f, 25.0f), NULL, NULL, color))
            {
                if (valvePassword[i])
                {
                    valvePassword = { false, false, false, false, false, false };
                    passwordChosen = false;
                    passwordLocation = "";
                    valveSolutionsText_1 = { "?", "?", "?", "?", "?", "?" };
                    valveSolutionsText_2 = { "?", "?", "?", "?", "?", "?" };
                    CalcExcludedValves();
                    CalcValveProbabilities();
                }
                else
                {
                    valvePassword = { false, false, false, false, false, false };
                    valvePassword[i] = true;
                    passwordLocation = valveLocations[i];
                    if (valveGreen[i])
                    {
                        valveGreen[i] = false;
                        greenChosen = false;
                        valveSolutionsText_1 = { "?", "?", "?", "?", "?", "?" };
                        valveSolutionsText_2 = { "?", "?", "?", "?", "?", "?" };
                    }
                    if (greenChosen)
                    {
                        valveSolutionsText_1 = valveSolutions_1[passwordLocation][greenLocation];
                        valveSolutionsText_2 = valveSolutions_2[passwordLocation][greenLocation];
                    }
                    passwordChosen = true;
                    CalcExcludedValves();
                    CalcValveProbabilities();
                }
            }
            ImGui::Image(valveSolverImgList[0].imgTexture, ImVec2(250.0f, 78.0f));
            if (valvePassword[i])
            {
                ImGui::BeginDisabled();
                valveDirections[i] = { false, false, false };
            }
            if (valveDirections[i][0])
                color = { 0.0f, 128.0f, 0.0f, 255.0f };
            else
                color = { 170.0f, 0.0f, 0.0f, 255.0f };
            std::string num_1 = "1##";
            num_1 += valveLocations[i];
            float startPos_1 = ImGui::GetCursorPosX();
            if (CreateButton(num_1, ImVec2(78.0f, 25.0f), NULL, NULL, color))
            {
                if (valveDirections[i][0])
                    valveDirections[i] = { false, false, false };
                else
                    valveDirections[i] = { true, false, false };
                CalcExcludedValves();
                CalcValveProbabilities();
            } SAMELINE;
            if (valveDirections[i][1])
                color = { 0.0f, 128.0f, 0.0f, 255.0f };
            else
                color = { 170.0f, 0.0f, 0.0f, 255.0f };
            std::string num_2 = "2##";
            num_2 += valveLocations[i];
            SAMELINE;
            float startPos_2 = ImGui::GetCursorPosX();
            if (CreateButton(num_2, ImVec2(78.0f, 25.0f), NULL, NULL, color))
            {
                if (valveDirections[i][1])
                    valveDirections[i] = { false, false, false };
                else
                    valveDirections[i] = { false, true, false };
                CalcExcludedValves();
                CalcValveProbabilities();
            } SAMELINE;
            if (valveDirections[i][2])
                color = { 0.0f, 128.0f, 0.0f, 255.0f };
            else
                color = { 170.0f, 0.0f, 0.0f, 255.0f };
            std::string num_3 = "3##";
            num_3 += valveLocations[i];
            float startPos_3 = ImGui::GetCursorPosX();
            if (CreateButton(num_3, ImVec2(78.0f, 25.0f), NULL, NULL, color))
            {
                if (valveDirections[i][2])
                    valveDirections[i] = { false, false, false };
                else
                    valveDirections[i] = { false, false, true };
                CalcExcludedValves();
                CalcValveProbabilities();
            }
            if (valvePassword[i])
            {
                ImGui::EndDisabled();
            }

            // lmao this is scuffed af
            {
                int addPos = 10;
                if (valvePassword[i])
                    addPos = 0;
                ImGui::SetCursorPosX(startPos_1 + 39 - ImGui::CalcTextSize(valveDirectionOdds_1[i][0].c_str()).x / 2 + addPos);
                ImGui::Text(valveDirectionOdds_1[i][0].c_str());
                SAMELINE;
                addPos = 10;
                if (valvePassword[i])
                    addPos = 0;
                ImGui::SetCursorPosX(startPos_2 + 39 - ImGui::CalcTextSize(valveDirectionOdds_1[i][1].c_str()).x / 2 + addPos);
                ImGui::Text(valveDirectionOdds_1[i][1].c_str());
                SAMELINE;
                addPos = 10;
                if (valvePassword[i])
                    addPos = 0;
                ImGui::SetCursorPosX(startPos_3 + 39 - ImGui::CalcTextSize(valveDirectionOdds_1[i][2].c_str()).x / 2 + addPos);
                ImGui::Text(valveDirectionOdds_1[i][2].c_str());

                addPos = 10;
                if (valvePassword[i])
                    addPos = 0;
                ImGui::SetCursorPosX(startPos_1 + 39 - ImGui::CalcTextSize(valveDirectionOdds_2[i][0].c_str()).x / 2 + addPos);
                ImGui::Text(valveDirectionOdds_2[i][0].c_str());
                SAMELINE;
                addPos = 10;
                if (valvePassword[i])
                    addPos = 0;
                ImGui::SetCursorPosX(startPos_2 + 39 - ImGui::CalcTextSize(valveDirectionOdds_2[i][1].c_str()).x / 2 + addPos);
                ImGui::Text(valveDirectionOdds_2[i][1].c_str());
                SAMELINE;
                addPos = 10;
                if (valvePassword[i])
                    addPos = 0;
                ImGui::SetCursorPosX(startPos_3 + 39 - ImGui::CalcTextSize(valveDirectionOdds_2[i][2].c_str()).x / 2 + addPos);
                ImGui::Text(valveDirectionOdds_2[i][2].c_str());
            }

            ImGui::EndGroup();
            SAMELINE;
            DummySpace(30.0f, 0.0f);
            if (i != 2 && i != 5)
                SAMELINE;
            else if (i == 2)
            {
                DummySpace(0.0f, 30.0f);
                DummySpace(30.0f, 0.0f);
                SAMELINE;
            }
        }
        DummySpace(15.0f, 0.0f);
        SAMELINE;
        ImGui::BeginGroup();
        DummySpace(0.0f, 15.0f);
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 450 - ImGui::CalcTextSize("Solutions").x / 2);
        ImGui::Text("Solutions");
        ImGui::SameLine();
        if (!passwordChosen || greenChosen)
        {
            ImGui::BeginDisabled();
        }
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 2.5f);
        if (CreateButton("Evaluate Valves", ImVec2(150.0f, 25.0f)))
            showEvaluation = true;
        if (!passwordChosen || greenChosen)
        {
            ImGui::EndDisabled();
        }
        ImGui::SameLine();
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 2.5f);
        if (CreateButton("Reset Valves", ImVec2(150.0f, 25.0f)))
        {
            greenChosen = false;
            passwordChosen = false;
            noGreenChoice = false;
            valveComboSet = false;
            showEvaluation = false;
            greenLocation = "";
            passwordLocation = "";
            valveGreen = { false, false, false, false, false, false };
            valvePassword = { false, false, false, false, false, false };
            valveDirections = { { false, false, false }, { false, false, false }, { false, false, false }, { false, false, false }, { false, false, false }, { false, false, false } };
            CalcValveProbabilities();
            CalcRemainingGreen();
            CalcExcludedValves();
        }
        if (ImGui::BeginTable("ValveSolutions", 6, ImGuiTableFlags_Borders, ImVec2(900.0f, 50.0f)))
        {
            for (int row = 0; row < 3; row++)
            {
                ImGui::TableNextRow();
                for (int column = 0; column < 6; column++)
                {
                    ImGui::TableSetColumnIndex(column);
                    if (!row)
                    {
                        ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, IM_COL32(128.0f, 128.0f, 128.0f, 128.0f));
                        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x / 2 - ImGui::CalcTextSize(valveLocations[column].c_str()).x / 2);
                        ImGui::Text(valveLocations[column].c_str());
                        continue;
                    }
                    if (!greenChosen || !passwordChosen)
                        ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, IM_COL32(128.0f, 128.0f, 128.0f, 90.0f));
                    else
                    {
                        if (row == 1)
                        {
                            if (valveSolutionsText_1[column] == "P")
                                ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, IM_COL32(255.0f, 105.0f, 180.0f, 255.0f));
                            else if (valveDirections[column][std::stoi(valveSolutionsText_1[column]) - 1])
                                ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, IM_COL32(0.0f, 128.0f, 0.0f, 255.0f));
                            else
                                ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, IM_COL32(140.0f, 0.0f, 0.0f, 255.0f));
                        }
                        else
                        {
                            if (valveSolutionsText_2[column] == "P")
                                ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, IM_COL32(255.0f, 105.0f, 180.0f, 255.0f));
                            else if (valveDirections[column][std::stoi(valveSolutionsText_2[column]) - 1])
                                ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, IM_COL32(0.0f, 128.0f, 0.0f, 255.0f));
                            else
                                ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, IM_COL32(140.0f, 0.0f, 0.0f, 255.0f));
                        }
                    }
                    ImGui::TableSetColumnIndex(column);
                    if (row == 1)
                    {
                        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x / 2 - ImGui::CalcTextSize(valveSolutionsText_1[column].c_str()).x / 2);
                        ImGui::Text(valveSolutionsText_1[column].c_str());
                    }
                    else
                    {
                        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x / 2 - ImGui::CalcTextSize(valveSolutionsText_2[column].c_str()).x / 2);
                        ImGui::Text(valveSolutionsText_2[column].c_str());
                    }
                }
            }
            ImGui::EndTable();
        }
        ImGui::EndGroup();
    }
}

void PresetSelectionFunc(int input)
{
    BGB::gumSelectIndex = input;
    BGB::showGumSelection = true;
}

void SwapSelectionFunc(int input)
{
    SwapGumSelection(input, BGB::gumSelectIndex);
    BGB::showGumSelection = false;
    if (BGB::writeGums && appStatus == "Status: Active")
        BGB::WritePresetToGame(BGB::gumPresets[BGB::currentPreset], bo3Directory + "\\Practice Tool\\Settings\\Active Gum Preset.txt");
    else
        BGB::WritePresetToGame(BGB::inactivePreset, bo3Directory + "\\Practice Tool\\Settings\\Active Gum Preset.txt");
}