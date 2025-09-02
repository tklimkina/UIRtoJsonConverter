using CviConverter.dto;
using System;
using System.Text;
using System.Text.Encodings.Web;
using System.Text.Unicode;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

namespace CviConverter
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.ToList().Count == 0)
                return;

            string path = "";
            foreach (var arg in args)
            {
                path += arg;
                if (!arg.Contains('.'))
                    path += ' ';
                else
                    break;
            }

            char[] filename = path.ToCharArray(); 
            var panel = LibWrapper.LoadPanelW(0, filename, 1);

            if (panel < 0)
            {
                Console.WriteLine($"Не удалось открыть {0}", args[0]);
                return;
            }

            LibWrapper.InstallPanelCallbackW(panel);

            LibWrapper.DisplayPanelW(panel);

            string panelname = path.Replace(".uir", "");

            var CVIpanel = CreateDTO(panel, panelname);

            SaveJson(CVIpanel, panelname);

            Console.WriteLine("Cхема успешно конвертирована в {0}.json", panelname);
        }

        static MainPanel CreateDTO(int panel, string panelname)
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

            int attr_label_on_x;
            int attr_label_on_y;
            int attr_label_on_length;
           // char attr_label_on_text[2048];
            int label_on_color;
            int label_off_x;
            int label_off_y;
            int label_off_length;
           // char attr_label_off_text[2048];
            int label_off_color;

            int attr_frame_color;

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
                    switch(ctrl_style)
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
                            lab.widget.options.text = dflt_value.ToString();

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
                                //LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_LABEL_HEIGHT, &h);
                               // LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_LABEL_WIDTH, &w);
                                // LibWrapper.GetCtrlAttributeW(panel, nextControl, ATTR_LABEL_SIZE_TO_TEXT, &attr_size_to_text);
                                num.widget.options.title.text = label.ToString();
                                num.widget.options.title.color = '#' + label_color.ToString("X");
                                num.widget.options.wrapper.backgroundColor = '#' + bg_color.ToString("X");
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

                            led.widget.options.visualType = "image";

                            // Values
                            led.widget.options.valueTrue.imageName = GetImageName(on_color.ToString("x"), ctrl_style);
                            led.widget.options.valueFalse.imageName = GetImageName(off_color.ToString("x"), ctrl_style);


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

                            break;
                    }

                    // itteration
                    LibWrapper.GetCtrlAttributeW(panel, nextControl, (int)Consts.ATTR_NEXT_CTRL, &nextControl);
                }
            }

            return PanelDTO;
        }

        static void SaveJson(MainPanel dto, string panelname)
        {
            var filename = panelname + ".json";

           /* var mainSettings = new JsonSerializerSettings
            {
                Formatting = Formatting.Indented
            };
            var jObject = JObject.FromObject(dto, JsonSerializer.Create(mainSettings));

            var vstyletockens = jObject.SelectTokens("$..vstyle").ToList();


            foreach (var vstyleToken in vstyletockens)
            {
                var compactVstyle = JsonConvert.SerializeObject(vstyleToken, Formatting.None);
                // Заменяем родительское свойство
                var parentProperty = vstyleToken.Parent as JProperty;
                if (parentProperty != null)
                {
                    parentProperty.Value = JToken.Parse(compactVstyle);
                }
            }


            string json = jObject.ToString();*/



           string json = JsonConvert.SerializeObject(dto, Formatting.Indented);
            File.WriteAllText(filename, json);

        }

        static string GetImageName(string color, int fnum)
        {
            string res = "";
            
            if(fnum % 2 == 0)                
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