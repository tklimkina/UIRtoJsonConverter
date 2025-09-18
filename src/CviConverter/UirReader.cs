using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CviConverter.DTO;

namespace CviConverter
{
    public static class UirReader
    {
        public static MainPanel BuildDTO(string path, string panelname)
        {

            char[] filename = path.ToCharArray();
            var panel = LibWrapper.LoadPanelW(0, filename, 1);

            if (panel < 0)
            {
                Console.WriteLine($"Не удалось открыть {0}", path);
                return null;
            }

            LibWrapper.InstallPanelCallbackW(panel);

            LibWrapper.DisplayPanelW(panel);

            return CreateDTO(panel, panelname);
        }

        internal static MainPanel CreateDTO(int panel, string panelname)
        {
            var PanelDTO = new MainPanel();

            int nextControl;
            int h, w, bgcolour;
            int x, y;
            var label = new StringBuilder(2048);
            var dflt_value = new StringBuilder(2048);
            var constant_name = new StringBuilder(2048);

            int label_top;
            int label_left;
            int label_visible;
            int ctrl_style;
            int label_bold;
            int label_color;
            int bg_color;
            int text_justify;
            int size_to_text;
            int precision;
            int text_point_size;
            int zplane_position;
            int ctrl_tab_position;

            int label_on_x;
            int label_on_y;
            int label_on_length;
            int label_on_color;
            int label_off_x;
            int label_off_y;
            int label_off_length;
            int label_off_color;

            int frame_color;

            int label_x;
            int label_y;
            int label_h;
            int label_w;

            int max_value = 0;
            int min_value = 0;

            unsafe
            {
                LibWrapper.GetPanelAttributeW(panel, (int)Consts.ATTR_PANEL_FIRST_CTRL, &nextControl);
                LibWrapper.GetPanelAttributeW(panel, (int)Consts.ATTR_HEIGHT, &h);
                LibWrapper.GetPanelAttributeW(panel, (int)Consts.ATTR_WIDTH, &w);
                LibWrapper.GetPanelAttributeW(panel, (int)Consts.ATTR_BACKCOLOR, &bgcolour);
                LibWrapper.GetStrPanelAttributeW(panel, (int)Consts.ATTR_TITLE, label);

                PanelDTO.window.width = 1920;
                PanelDTO.window.height = 1080;
                PanelDTO.name = panelname;
                PanelDTO.description = label.ToString();

                while (nextControl != 0)
                {
                    LibWrapper.GetCtrlStrAttributeW(panel, nextControl, (int)Consts.ATTR_CONSTANT_NAME, constant_name);
                    LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_CTRL_STYLE, &ctrl_style);
                    LibWrapper.GetCtrlStrAttributeW(panel, nextControl, (int)Consts.ATTR_DFLT_VALUE, dflt_value);
                    LibWrapper.GetCtrlStrAttributeW(panel, nextControl, (int)Consts.ATTR_LABEL_TEXT, label);
                    LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_LABEL_VISIBLE, &label_visible);
                    LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_LABEL_TOP, &label_top);
                    LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_LABEL_LEFT, &label_left);
                    LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_LEFT, &x);
                    LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_TOP, &y);
                    LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_HEIGHT, &h);
                    LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_WIDTH, &w);
                    LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_ZPLANE_POSITION, &zplane_position);
                    LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_CTRL_TAB_POSITION, &ctrl_tab_position);

                    // inversion for js coordination system
                    zplane_position = (int)Consts.ZPLANE_MAX_VALUE - 1 - zplane_position;

                    // filally creating all the elements from the scheme
                    switch (ctrl_style)
                    {
                        case (int)Consts.CTRL_TEXT_MSG:
                        case (int)Consts.CTRL_TEXT_BOX:
                        case (int)Consts.CTRL_TEXT_BOX_LS:
                            LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_TEXT_BOLD, &label_bold);
                            LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_TEXT_COLOR, &label_color);
                            LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_TEXT_BGCOLOR, &bg_color);
                            LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_TEXT_JUSTIFY, &text_justify);
                            LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_SIZE_TO_TEXT, &size_to_text);
                            LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_TEXT_POINT_SIZE, &text_point_size);

                            var lab = new Label()
                            {
                                x = x,
                                y = y,
                                height = h,
                                width = w,
                                id = constant_name.ToString()
                            };

                            var lwid = new LabelWidget();
                            lwid.options.text = dflt_value.ToString();
                            lwid.type = "Labels.SimpleText";

                            lab.widget = lwid;

                            PanelDTO.layout.frames.Add(lab);
                            break;

                        case (int)Consts.CTRL_NUMERIC:
                        case (int)Consts.CTRL_NUMERIC_LS:
                        case (int)Consts.CTRL_STRING:
                        case (int)Consts.CTRL_STRING_LS:
                            LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_PRECISION, &precision);
                            LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_TEXT_BOLD, &label_bold);
                            LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_TEXT_COLOR, &label_color);
                            LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_TEXT_POINT_SIZE, &text_point_size);
                            LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_TEXT_JUSTIFY, &text_justify);

                            var num = new SingleValue()
                            {
                                id = constant_name.ToString(),
                                x = x,
                                y = y,
                                width = w,
                                height = h
                            };

                            if (label_visible == 1 && label.ToString().Length != 0)
                            {
                                LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_LABEL_BOLD, &label_bold);
                                LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_LABEL_COLOR, &label_color);
                                LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_LABEL_BGCOLOR, &bg_color);

                                var nwid = new SingleValueWidget();

                                nwid.options.title.text = label.ToString();
                                nwid.options.title.color = '#' + label_color.ToString("X");
                                nwid.options.wrapper.backgroundColor = '#' + bg_color.ToString("X");
                                nwid.type = "Analogs.SingleValue";

                                num.widget = nwid;
                            }

                            PanelDTO.layout.frames.Add(num);
                            break;

                        case (int)Consts.CTRL_PICTURE_RING:
                        case (int)Consts.CTRL_PICTURE_RING_LS:

                            var im = new RTImage()
                            {
                                id = constant_name.ToString(),
                                x = x,
                                y = y,
                                width = w,
                                height = h
                            };

                            im.widget = new RTImageWidget() { type = "Analogs.RTImage" };

                            break;

                        case (int)Consts.CTRL_ROUND_LIGHT:
                        case (int)Consts.CTRL_ROUND_LED:
                        case (int)Consts.CTRL_ROUND_LED_LS:
                        case (int)Consts.CTRL_SQUARE_LIGHT:
                        case (int)Consts.CTRL_SQUARE_LED:
                        case (int)Consts.CTRL_SQUARE_LED_LS:
                            int on_color, off_color;
                            LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_ON_COLOR, &on_color);
                            LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_OFF_COLOR, &off_color);
                            var led = new TeleSignal()
                            {
                                x = x,
                                y = y,
                                width = w,
                                height = h,
                                id = constant_name.ToString()
                            };

                            var wid = new TeleSignalWidget();

                            wid.options.visualType = "image";
                            wid.type = "Analogs.Telesignal";

                            // Values
                            wid.options.valueTrue.imageName = GetImageName(on_color.ToString("x"), ctrl_style);
                            wid.options.valueFalse.imageName = GetImageName(off_color.ToString("x"), ctrl_style);
                            led.widget = wid;

                            PanelDTO.layout.frames.Add(led);
                            break;

                        case (int)Consts.CTRL_SQUARE_COMMAND_BUTTON:
                        case (int)Consts.CTRL_OBLONG_COMMAND_BUTTON:
                        case (int)Consts.CTRL_ROUND_COMMAND_BUTTON:
                        case (int)Consts.CTRL_ROUNDED_COMMAND_BUTTON:
                        case (int)Consts.CTRL_PICTURE_COMMAND_BUTTON:
                        case (int)Consts.CTRL_SQUARE_COMMAND_BUTTON_LS:
                        case (int)Consts.CTRL_PICTURE_COMMAND_BUTTON_LS:

                            break;

                        case (int)Consts.CTRL_PICTURE:
                        case (int)Consts.CTRL_PICTURE_LS:

                            var logo = new RsduLogo()
                            {
                                x = x,
                                y = y,
                                width = w,
                                height = h,
                                id = constant_name.ToString()
                            };

                            logo.widget = new RsduLogoWidget() { type = "Labels.Rsdu5Logo" };

                            break;

                        case (int)Consts.CTRL_RAISED_FRAME:
                        case (int)Consts.CTRL_RECESSED_FRAME:
                        case (int)Consts.CTRL_FLAT_FRAME:
                        case (int)Consts.CTRL_RAISED_ROUND_FRAME:
                        case (int)Consts.CTRL_RECESSED_ROUND_FRAME:
                        case (int)Consts.CTRL_FLAT_ROUND_FRAME:
                        case (int)Consts.CTRL_RECESSED_NARROW_FRAME:

                            break;

                        case (int)Consts.CTRL_RAISED_BOX:
                        case (int)Consts.CTRL_RECESSED_BOX:
                        case (int)Consts.CTRL_FLAT_BOX:
                        case (int)Consts.CTRL_RAISED_CIRCLE:
                        case (int)Consts.CTRL_RECESSED_CIRCLE:
                        case (int)Consts.CTRL_FLAT_CIRCLE:
                        case (int)Consts.CTRL_RAISED_ROUNDED_BOX:
                        case (int)Consts.CTRL_RECESSED_ROUNDED_BOX:
                        case (int)Consts.CTRL_FLAT_ROUNDED_BOX:
                        case (int)Consts.CTRL_RECESSED_BOX_LS:
                        case (int)Consts.CTRL_SMOOTH_HORIZONTAL_BOX_LS:
                        case (int)Consts.CTRL_RAISED_BOX_LS:
                        case (int)Consts.CTRL_SMOOTH_VERTICAL_BOX_LS:
                        case (int)Consts.CTRL_HORIZONTAL_SPLITTER_LS:
                        case (int)Consts.CTRL_VERTICAL_SPLITTER_LS:

                            break;

                        case (int)Consts.CTRL_HSWITCH:
                        case (int)Consts.CTRL_GROOVED_HSWITCH:
                        case (int)Consts.CTRL_TOGGLE_HSWITCH:
                        case (int)Consts.CTRL_TOGGLE_HSWITCH_LS:

                            break;

                        case (int)Consts.CTRL_VSWITCH:
                        case (int)Consts.CTRL_GROOVED_VSWITCH:
                        case (int)Consts.CTRL_TOGGLE_VSWITCH:
                        case (int)Consts.CTRL_TOGGLE_VSWITCH_LS:

                            break;

                        case (int)Consts.CTRL_NUMERIC_GAUGE_LS:

                            break;

                        case (int)Consts.CTRL_STRIP_CHART:
                        case (int)Consts.CTRL_STRIP_CHART_LS:
                            int points_per_screen, num_traces, grid_color;
                            double yaxis_min, yaxis_max;
                            int value;

                            LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_PLOT_BGCOLOR, &bg_color);
                            LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_XLABEL_COLOR, &label_color);
                            LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_POINTS_PER_SCREEN, &points_per_screen);
                            LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_NUM_TRACES, &num_traces);
                            LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_GRID_COLOR, &grid_color);
                            // GetAxisScalingMode(panel, nextControl, VAL_LEFT_YAXIS, NULL, &yaxis_min, &yaxis_max);

                            var chart = new ChartRTDTracking()
                            {
                                id = constant_name.ToString(),
                                x = x,
                                y = y,
                                width = w,
                                height = h
                            };

                            var cwid = new ChartRTDTrackingWidget();

                            cwid.options.text = dflt_value.ToString();
                            cwid.type = "Charts.ChartRTDTracking";

                            chart.widget = cwid;

                            for (int n = 1; n < num_traces; n++)
                            {
                                LibWrapper.GetTraceAttributeW(panel, nextControl, n, (int)Consts.ATTR_TRACE_COLOR, &value);
                            }

                            break;
                    }

                    // itteration
                    LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_NEXT_CTRL, &nextControl);
                }
            }

            return PanelDTO;
        }

        internal static string GetImageName(string color, int fnum)
        {
            string res = "";

            if (fnum % 2 == 0)
                res = "lamp_circle_";
            else
                res = "lamp_square_";

            // pick colour
            int R = Convert.ToInt32("0x" + color.Substring(0, 2), 16);
            int G = Convert.ToInt32("0x" + color.Substring(2, 2), 16);
            int B = Convert.ToInt32("0x" + color.Substring(4, 2), 16);

            // red
            if (R > G + B)
                res += "red";
            // white 
            else if (R > 240 && B > 240 && G > 240)
                res += "green";
            // black
            else if (R < 20 && B < 20 && G < 20)
                res += "green";
            // green
            else if (G > R + B)
                res += "green";


            return res;

        }
    }
}
