#pragma once

// явное подключение библиотеки
#pragma comment(lib, "..\\cvi85\\extlib\\cvirt.lib")

extern "C" 
{
    // types
typedef  int(__cdecl* PanelCallbackPtr)(int panel, int event, void* callbackData, int eventData1, int eventData2);
typedef struct
{
    int X;
    int Y;
} Point;

    // funcs
__declspec(dllexport) int __stdcall LoadPanel(int parentPanel, const char* fileName, int panelResourceId);
__declspec(dllexport) int __stdcall DisplayPanel(int panel);
__declspec(dllexport) int __stdcall DiscardPanel(int panel);
__declspec(dllexport) int __stdcall QuitUserInterface(int returnCode);
__declspec(dllexport) int __stdcall InstallPanelCallback(int panel, PanelCallbackPtr eventFunction, void* callbackData);
__declspec(dllexport) int __stdcall GetPanelAttribute(int panel, int attribute, void* value);
__declspec(dllexport) int __stdcall GetCtrlAttribute(int panel, int control, int attribute, void* value);
__declspec(dllexport) int __stdcall GetTraceAttribute(int panel, int control, int traceNum, int attribute, void* value);
__declspec(dllexport) int __stdcall GetNumTableColumns(int panel, int control, void* value);
__declspec(dllexport) int __stdcall GetNumTableRows(int panel, int control, void* value);
__declspec(dllexport) int __stdcall GetTableCellVal(int panel, int control, Point cell, void* value);
__declspec(dllexport) int __stdcall GetTableColumnAttribute(int panelHandle, int controlID, int columnIndex, int columnAttribute, void* attributeValue);
__declspec(dllexport) int __stdcall GetCtrlVal(int panelHandle, int controlID, void* value);
//__declspec(dllexport) int __stdcall
}