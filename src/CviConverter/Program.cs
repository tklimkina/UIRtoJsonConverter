using CviConverter.dto;
using System.Globalization;
using System.IO;
using System.IO.Enumeration;
using System.Runtime.InteropServices;
using System.Text;
//using "..\cvi85\include\userint.h"

namespace CviConverter
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.ToList().Count == 0)
                return;

            char[] filename = args[0].ToCharArray(); 
            var panel = LibWrapper.LoadPanelW(0, filename, 1);

            if (panel < 0)
            {
                Console.WriteLine($"Не удалось открыть {0}", args[0]);
                return;
            }

            var c = LibWrapper.InstallPanelCallbackW(panel);

            var v = LibWrapper.DisplayPanelW(panel);

            CreateDTO(panel);

            Console.WriteLine(args[0]);
        }

        public static void CreateDTO(int panel)
        {
            var PanelDTO = new MainPanel();

            int nextControl;
            int h, w, bgcolour;
            var label = new StringBuilder(200);
            unsafe
            {
                LibWrapper.GetPanelAttributeW(panel, (int)Consts.ATTR_PANEL_FIRST_CTRL, &nextControl);
                LibWrapper.GetPanelAttributeW(panel, (int)Consts.ATTR_HEIGHT, &h);
                LibWrapper.GetPanelAttributeW(panel, (int)Consts.ATTR_WIDTH, &w);
                LibWrapper.GetPanelAttributeW(panel, (int)Consts.ATTR_BACKCOLOR, &bgcolour);
                LibWrapper.GetStrPanelAttributeW(panel, (int)Consts.ATTR_TITLE, label);

                PanelDTO.width = w;
                PanelDTO.height = h;
                PanelDTO.title = label.ToString();
                PanelDTO.bodystyle = "background-color:#" + bgcolour.ToString("x");

                while (nextControl != 0)
                {
                    int constant_name, ctrl_style, dflt_value;
                    int label_text, label_visible, label_top, label_left;
                    int x, y, zplane_position, ctrl_tab_position;

                    var cnstptr = LibWrapper.GetCtrlStrAttributeW(panel, nextControl, (int)Consts.ATTR_CONSTANT_NAME, label);
                    LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_CTRL_STYLE, &ctrl_style);
                    LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_DFLT_VALUE, &dflt_value);
                    LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_LABEL_TEXT, &label_text);
                    LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_LABEL_VISIBLE, &label_visible);
                    LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_LABEL_TOP, &label_top);
                    LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_LABEL_LEFT, &label_left);
                    LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_LEFT, &x);
                    LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_TOP, &y);
                    LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_HEIGHT, &h);
                    LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_WIDTH, &w);
                    LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_ZPLANE_POSITION, &zplane_position);
                    LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_CTRL_TAB_POSITION, &ctrl_tab_position);


                    // itteration
                    LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_NEXT_CTRL, &nextControl);
                }
            }
        }
    }
}