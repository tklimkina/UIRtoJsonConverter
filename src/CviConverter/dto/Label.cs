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
        [JsonProperty(Order = 5)]
        public LabelWidget widget { get; set; } = new LabelWidget();
    }
    public class LabelWidget : BaseWidget
    {
        [JsonProperty]
        private readonly string type = "Labels.SimpleText";
        public LabelWidgetOptions options { get; set; } = new LabelWidgetOptions(); 
    }
    public class LabelWidgetOptions : WidgetTextBase
    {
        [JsonProperty(DefaultValueHandling = DefaultValueHandling.Ignore)]
        public string textAlign { get; set; }
    }
}
