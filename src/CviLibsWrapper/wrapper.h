#pragma once

// явное подключение библиотеки
#pragma comment(lib, "..\\cvi85\\extlib\\cvirt.lib")

extern "C" 
{
    // types
typedef  int(__cdecl* PanelCallbackPtr)(int panel, int event, void* callbackData, int eventData1, int eventData2);

    // funcs
__declspec(dllexport) int __stdcall LoadPanel(int parentPanel, const char* fileName, int panelResourceId);
__declspec(dllexport) int __stdcall DisplayPanel(int panel);
__declspec(dllexport) int __stdcall DiscardPanel(int panel);
__declspec(dllexport) int __stdcall QuitUserInterface(int returnCode);
__declspec(dllexport) int __stdcall InstallPanelCallback(int panel, PanelCallbackPtr eventFunction, void* callbackData);
__declspec(dllexport) int __stdcall GetPanelAttribute(int panel, int attribute, void* value);
__declspec(dllexport) int __stdcall GetCtrlAttribute(int panel, int control, int attribute, void* value);

}