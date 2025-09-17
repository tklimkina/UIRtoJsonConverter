using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CviConverter.DTO
{
    public class TeleSignal : ElementPanelBase
    {

    }
    public class TeleSignalWidget : BaseWidget
    {
        public TeleSignalWidgetOptions options { get; set; } = new TeleSignalWidgetOptions();
        public RtData rtdata { get; set; } = new RtData();
    }

    public class TeleSignalWidgetOptions : BaseWidgetOptions
    {
        public string visualType { get; set; }

        [JsonProperty(DefaultValueHandling = DefaultValueHandling.Ignore)]
        public int padding { get; set; }

        public TeleSignalWidgetOptionsValue valueFalse { get; set; } = new TeleSignalWidgetOptionsValue();
        public TeleSignalWidgetOptionsValue valueTrue { get; set; } = new TeleSignalWidgetOptionsValue();
    }

    public class TeleSignalWidgetOptionsValue
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

}
