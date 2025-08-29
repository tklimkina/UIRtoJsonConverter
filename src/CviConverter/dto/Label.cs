using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json.Serialization;
using System.Threading.Tasks;

namespace CviConverter.dto
{
    public class Label : ElementPanelBase
    {
        [JsonProperty(Order = 5)]
        public LabelWidget widget { get; set; } = new LabelWidget();
    }
    public class LabelWidget : BaseWidget
    {
        public string type { get; set; } = "Labels.SimpleText";
        public LabelWidgetOptions options { get; set; } = new LabelWidgetOptions(); 
    }
    public class LabelWidgetOptions : BaseWidgetOptions
    {
        public string text { get; set; }

        [JsonProperty(DefaultValueHandling = DefaultValueHandling.Ignore)]
        public string color { get; set; }

        [JsonProperty(DefaultValueHandling = DefaultValueHandling.Ignore)]
        public int fontSize { get; set; }

        [JsonProperty(DefaultValueHandling = DefaultValueHandling.Ignore)]
        public int fontWeight { get; set; }

        [JsonProperty(DefaultValueHandling = DefaultValueHandling.Ignore)]
        public string fontStyle { get; set; }

        [JsonProperty(DefaultValueHandling = DefaultValueHandling.Ignore)]
        public string textAlign { get; set; }
    }
}
