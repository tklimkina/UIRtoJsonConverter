using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json.Serialization;
using System.Threading.Tasks;

namespace CviConverter.dto
{
    public class Label : BasePanel
    {
        public string xtype { get; set; } = "label";
        public int x { get; set; }
        public int y { get; set; }
        public string style { get; set; }
        public string id { get; set; }
        public string text { get; set; }
    }
    public class LabelStyle
    {
        public int label_bold { get; set; }
        public int label_color { get; set; }
        public long bg_color { get; set; }
        public int zplane_position { get; set; }
        public int text_justify { get; set; }
        public int text_point_size { get; set; }

        public string ToString()
        {
            string strlstyle = "padding: 2px 0 2px 4px; ";

            strlstyle += "z-index: " + zplane_position.ToString();

            if (text_point_size != 16)
                strlstyle += "; font-size: " + Convert.ToInt32(5.2).ToString() + "pt";

            if (label_bold == 1)
                strlstyle += "; font-weight: bold";

            if (label_color != 0)
                strlstyle += "; color: #" + label_color.ToString("x");

            if (bg_color != (int)Consts.VAL_TRANSPARENT)
                strlstyle += "; background-color: #" + bg_color.ToString("x");

            if(text_justify != 0)
            {
                strlstyle += ";  text-align:";

                if (text_justify == (int)Consts.VAL_RIGHT_JUSTIFIED)
                    strlstyle += " right";

                if (text_justify == (int)Consts.VAL_CENTER_JUSTIFIED)
                    strlstyle += " center";
            }

            return strlstyle;
        }
    }
}
