using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CviConverter.dto
{
    public class TeleSignal : ElementPanelBase
    {
        [JsonProperty(Order = 5)]
        public TeleSignalWidget widget { get; set; } = new TeleSignalWidget();
    }
    public class TeleSignalWidget : BaseWidget
    {
        public string type { get; set; } = "Analogs.Telesignal";
        public TeleSignalWidgetOptions options { get; set; } = new TeleSignalWidgetOptions();
        public RData rdata { get; set; } = new RData();
    }

    public class TeleSignalWidgetOptions : BaseWidgetOptions
    {
        public string visualType { get; set; }

        [JsonProperty(DefaultValueHandling = DefaultValueHandling.Ignore)]
        public int padding { get; set; }

        public Value valueFalse { get; set; } = new Value();
        public Value valueTrue { get; set; } = new Value();
    }

    public class Value
    {
        [JsonProperty(DefaultValueHandling = DefaultValueHandling.Ignore)]
        public string imageName { get; set; }

        [JsonProperty(DefaultValueHandling = DefaultValueHandling.Ignore)]
        public string text { get; set; }

        [JsonProperty(DefaultValueHandling = DefaultValueHandling.Ignore)]
        public string color { get; set; }

        [JsonProperty(DefaultValueHandling = DefaultValueHandling.Ignore)]
        public string backgroundColor { get; set; }

        [JsonProperty(DefaultValueHandling = DefaultValueHandling.Ignore)]
        public int fontSize { get; set; }

        [JsonProperty(DefaultValueHandling = DefaultValueHandling.Ignore)]
        public int fontWeight { get; set; }

        [JsonProperty(DefaultValueHandling = DefaultValueHandling.Ignore)]
        public string fontStyle { get; set; }

        [JsonProperty(DefaultValueHandling = DefaultValueHandling.Ignore)]
        public string textAlign { get; set; }

        [JsonProperty(DefaultValueHandling = DefaultValueHandling.Ignore)]
        public string verticalAlign { get; set; }

        [JsonProperty(DefaultValueHandling = DefaultValueHandling.Ignore)]
        public string horizontalAlign { get; set; }
    }

    public class RData
    {
        public string tag { get; set; }
        public int tableId { get; set; }
        public int paramId { get; set; }
        public string gtopt { get; set; }
    }
}
