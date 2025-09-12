using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CviConverter.DTO
{
    public class RTImage : ElementPanelBase
    {
        public RTImageWidget widget { get; set; } = new RTImageWidget();
    }
    public class RTImageWidget : BaseWidget
    {
        [JsonProperty]
        private string type = "Analogs.RTImage";
        public RTImageWidgetOptions options { get; set; } = new RTImageWidgetOptions();
        public RtData rtdata { get; set; } = new RtData();
    }
    public class RTImageWidgetOptions : BaseWidgetOptions
    {
        public List<RTImageWidgetOptionsValue> values = new List<RTImageWidgetOptionsValue>() 
        { 
            new RTImageWidgetOptionsValue(){ value = 0},
            new RTImageWidgetOptionsValue(){ value = 1}
        };
    }
    public class RTImageWidgetOptionsValue
    {
        public int value { get; set; }
        public string image { get; set; }
    }
}
