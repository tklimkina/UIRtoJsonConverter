using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CviConverter.DTO
{
    public class BaseWidget : IWidget
    {
        // [JsonProperty]
        public virtual string type { get; set; }
        public IWidgetOptions options { get; set; }
    }
    public class BaseWidgetOptions : IWidgetOptions
    {

    }
    public class RtData
    {
        public string tag { get; set; }
        public int tableId { get; set; }
        public int paramId { get; set; }
        public string gtopt { get; set; }
    }
    public class  WidgetTextBase : IWidgetOptions
    {
        public string text { get; set; }

        [JsonProperty(DefaultValueHandling = DefaultValueHandling.Ignore)]
        public string color { get; set; }

        [JsonProperty(DefaultValueHandling = DefaultValueHandling.Ignore)]
        public int fontSize { get; set; } = 16;

        [JsonProperty(DefaultValueHandling = DefaultValueHandling.Ignore)]
        public int fontWeight { get; set; } = 400;

        [JsonProperty(DefaultValueHandling = DefaultValueHandling.Ignore)]
        public string fontStyle { get; set; } = "normal";
    }
}
