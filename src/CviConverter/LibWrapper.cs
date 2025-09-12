using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace CviConverter
{

    public static class LibWrapper
    {
        [DllImport("CviLibsWrapper.dll")]
        public static extern int LoadPanelW(int parentPanel,  char[] fileName, int panelResourceId);

        [DllImport("CviLibsWrapper.dll")]
        public static extern int DisplayPanelW(int panel);

        [DllImport("CviLibsWrapper.dll")]
        public static extern int InstallPanelCallbackW(int panel);

        [DllImport("CviLibsWrapper.dll")]
        public static unsafe extern int GetPanelAttributeW(int panel, int attribute, void* value);

        [DllImport("CviLibsWrapper.dll")]
        public static extern int GetStrPanelAttributeW(int panel, int attribute, StringBuilder buf);

        [DllImport("CviLibsWrapper.dll")]
        public static unsafe extern int GetCtrlAttributeW(int panel, int control, int attribute, void* value);

        [DllImport("CviLibsWrapper.dll")]
        public static unsafe extern int GetCtrlStrAttributeW(int panel, int control, int attribute, StringBuilder buf);

        [DllImport("CviLibsWrapper.dll")]
        public static unsafe extern int GetTraceAttributeW(int panel, int control, int traceNum, int attribute, void* value);
    }
}
