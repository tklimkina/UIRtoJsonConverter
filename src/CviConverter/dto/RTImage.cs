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

    }
    public class RTImageWidget : BaseWidget
    {
        public RTImageWidgetOptions options { get; set; } = new RTImageWidgetOptions();
        public List<RtData> rtdata { get; set; } = new List<RtData>();
    }
    public class RTImageWidgetOptions : BaseWidgetOptions
    {
        public List<RTImageWidgetOptionsValue> values = new List<RTImageWidgetOptionsValue>() 
        { 
            new RTImageWidgetOptionsValue(){ value = 0, image = "http://10.51.128.42:8080/rsdu/img/otkl.png" },
            new RTImageWidgetOptionsValue(){ value = 1, image = "http://10.51.128.42:8080/rsdu/img/vkl.png"}
        };
    }
    public class RTImageWidgetOptionsValue
    {
        public int value { get; set; }
        public string image { get; set; }
    }
}
