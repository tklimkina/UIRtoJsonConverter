using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json.Serialization;
using System.Threading.Tasks;

namespace CviConverter.DTO
{
    public class Label : ElementPanelBase
    {
       
    }
    public class LabelWidget : BaseWidget
    {
        public LabelWidgetOptions options { get; set; } = new LabelWidgetOptions(); 
    }
    public class LabelWidgetOptions : WidgetTextBase
    {
        [JsonProperty(DefaultValueHandling = DefaultValueHandling.Ignore)]
        public string textAlign { get; set; }
    }
}
