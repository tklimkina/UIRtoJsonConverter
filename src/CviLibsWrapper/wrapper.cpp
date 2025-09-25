#include "..\cvi85\include\userint.h" // заголовочный файл от .lib
#include "pch.h"
#include "wrapper.h"
#include <iostream>

void ClosePanel(int panel)
{
    if (panel >= 0)
    {
        DiscardPanel(panel);
        panel = -1;
    }
}
int __cdecl GraphPanelCallback(int panel, int evnt, void* callbackData, int eventData1, int eventData2)
{
    switch (evnt)
    {
    case 11:
        ClosePanel(panel);
        QuitUserInterface(0);
        break;
    }
    return 0;
}

extern "C"
{
    __declspec(dllexport) int __stdcall LoadPanelW(int parentPanel, char fileName[], int panelResourceId)
    {
        // Вызов функции из .lib
        return LoadPanel(parentPanel, fileName, panelResourceId);
    }

    __declspec(dllexport) int __stdcall InstallPanelCallbackW(int panel)
    {

        return InstallPanelCallback(panel, GraphPanelCallback, 0);
    }

     __declspec(dllexport) int __stdcall DisplayPanelW(int panel)
     {
         return DisplayPanel(panel);
     }

     __declspec(dllexport) int __stdcall GetPanelAttributeW(int panel, int attribute, void* value)
     {
         return GetPanelAttribute(panel, attribute, value);
     }
     __declspec(dllexport) int __stdcall GetStrPanelAttributeW(int panel, int attribute, char* buf)
     {
       //  setlocale(LC_ALL, "Russian");
         return GetPanelAttribute(panel, attribute, buf);
     }
     __declspec(dllexport) int __stdcall GetCtrlAttributeW(int panel, int control, int attribute, void* value)
     {
         return GetCtrlAttribute(panel, control, attribute, value);
     }
     __declspec(dllexport) int __stdcall GetCtrlStrAttributeW(int panel, int control, int attribute, char* buf)
     {
         return GetCtrlAttribute(panel, control, attribute, buf);
     }
     __declspec(dllexport) int __stdcall GetTraceAttributeW(int panel, int control, int traceNum, int attribute, void* value)
     {
         return  GetTraceAttribute(panel, control, traceNum, attribute, value);
     }
     __declspec(dllexport) int __stdcall GetNumTableColumnsW(int panel, int control, void* value)
     {
         return GetNumTableColumns( panel, control,value);
     }
     __declspec(dllexport) int __stdcall GetNumTableRowsW(int panel, int control, void* value)
     {
         return GetNumTableRows(panel, control, value);
     }
     __declspec(dllexport) int __stdcall GetTableStrCellValW(int panel, int control, Point cell, char* value)
     {
         return GetTableCellVal(panel, control, cell, value);
     }
     __declspec(dllexport) int __stdcall GetTableCellValW(int panel, int control, Point cell, void* value)
     {
         return GetTableCellVal(panel, control, cell, value);
     }
     __declspec(dllexport) int __stdcall GetTableColumnAttributeW(int panelHandle, int controlID, int columnIndex, int columnAttribute, void* attributeValue)
     {
         return GetTableColumnAttribute(panelHandle, controlID, columnIndex, columnAttribute, attributeValue);
     }
     __declspec(dllexport) int __stdcall GetCtrlValW(int panelHandle, int controlID, void* value)
     {
         return GetCtrlValW(panelHandle, controlID, value);
     }
}


