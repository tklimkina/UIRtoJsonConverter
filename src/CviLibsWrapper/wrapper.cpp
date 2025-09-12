#include "..\cvi85\include\userint.h" // заголовочный файл от .lib
//#include <atlconv.h>
#include "pch.h"
#include "wrapper.h"
//#include <string>
#include <iostream>
/*#include <fstream>
#include <iomanip>
#include <iterator>
#include <sstream>*/
//#include <altbase.h>
//#pragma comment(lib, "..\cvi85\extlib\cvirt.lib")

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
}


