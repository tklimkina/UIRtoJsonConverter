using CviConverter.dto;
using System;
using System.Text;
using System.Text.Encodings.Web;
using System.Text.Unicode;
using Newtonsoft.Json;

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

            LibWrapper.InstallPanelCallbackW(panel);

            LibWrapper.DisplayPanelW(panel);

            string panelname = args[0].Replace(".uir", "");

            var CVIpanel = CreateDTO(panel, panelname);

            SaveJson(CVIpanel, panelname);

            Console.WriteLine(args[0]);
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

                PanelDTO.width = w;
                PanelDTO.height = h;
                PanelDTO.title = label.ToString();
                PanelDTO.bodystyle = "background-color: #" + bgcolour.ToString("x");
                PanelDTO.id = panelname;

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

                    // filally crating all the elements from the scheme
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
                            if (size_to_text == 1)              // 1 is true. The old library uses int as bool
                            {
                                h = 0;
                                w = 0;
                            }

                            var ls = new LabelStyle()
                            {
                                label_bold = label_bold,
                                label_color = label_color,
                                bg_color = bg_color,
                                zplane_position = zplane_position,
                                text_justify = text_justify,
                                text_point_size = text_point_size
                            };

                            var lab = new Label()
                            { 
                                x = x,
                                y = y,
                                height = h,
                                width = w,
                                id = constant_name.ToString(),
                                text = dflt_value.ToString(),
                                style = ls.ToString()
                            };
                            PanelDTO.items.Add(lab);
                            break;

                        case (int)Consts.CTRL_NUMERIC:
                        case (int)Consts.CTRL_NUMERIC_LS:
                        case (int)Consts.CTRL_STRING:
                        case (int)Consts.CTRL_STRING_LS:

                            break;

                        case (int)Consts.CTRL_PICTURE_RING:
                        case (int)Consts.CTRL_PICTURE_RING_LS:

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
                            var led = new Led()
                            {
                                x = x,
                                y = y,
                                width = w,
                                height = h,
                                id = constant_name.ToString(),
                                style = "z-index: " + zplane_position.ToString()
                            };

                            led.vstyle.Add(new VStyle() { backgroung = on_color.ToString("x") });
                            led.vstyle.Add(new VStyle() { backgroung = off_color.ToString("x") });

                            if (ctrl_style % 2 == 0)
                                led.cls = "wasutp_led_state_default";
                            else
                                led.cls = "wasutp_led_sq_state_default";
                            PanelDTO.items.Add(led);
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

            string json = JsonConvert.SerializeObject(dto, Formatting.Indented);
            File.WriteAllText(filename, json);

            /*  var options = new JsonSerializerOptions
              {
                  Encoder = JavaScriptEncoder.Create(UnicodeRanges.BasicLatin, UnicodeRanges.Cyrillic),
                  WriteIndented = true
              };

              using (FileStream fs = new FileStream(filename, FileMode.OpenOrCreate))
              {
                  JsonSerializer.Serialize<MainPanel>(fs, dto, options);
                  Console.WriteLine("Data has been saved to file");
              }*/
        }
    }
}