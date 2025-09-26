using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CviConverter.DTO
{
    public class SingleValue : ElementPanelBase
    {

    }
    public class SingleValueWidget : BaseWidget
    {
        public SingleValueWidgetOptions options { get; set; } = new SingleValueWidgetOptions();
    }
    public class SingleValueWidgetOptions : BaseWidgetOptions
    {
        [JsonProperty(DefaultValueHandling = DefaultValueHandling.Ignore)]
        public Wrapper wrapper { get; set; } = new Wrapper();

        [JsonProperty(DefaultValueHandling = DefaultValueHandling.Ignore)]
        public Unit title { get; set; } = new Unit();

        [JsonProperty(DefaultValueHandling = DefaultValueHandling.Ignore)]
        public SingleValueWidgetOptionsValue value { get; set; } = new SingleValueWidgetOptionsValue();

        [JsonProperty(DefaultValueHandling = DefaultValueHandling.Ignore)]
        public Unit unit { get; set; } = new Unit();
        public RtData rtdata { get; set; } = new RtData();
    }
    public class Wrapper
    {
        public string backgroundColor { get; set; } = "white";
    }
    public class Unit : WidgetTextBase
    {

        [JsonProperty(DefaultValueHandling = DefaultValueHandling.Ignore)]
        public string verticalAlign { get; set; }

        [JsonProperty(DefaultValueHandling = DefaultValueHandling.Ignore)]
        public string horizontalAlign { get; set; }
    }
    public class SingleValueWidgetOptionsValue
    {
        public int digitsAfterDot { get; set; }
       // [JsonProperty(DefaultValueHandling = DefaultValueHandling.Ignore)]
        public string color { get; set; } = "#000000";

        [JsonProperty(DefaultValueHandling = DefaultValueHandling.Ignore)]
        public int fontSize { get; set; } = 16;

        [JsonProperty(DefaultValueHandling = DefaultValueHandling.Ignore)]
        public int fontWeight { get; set; } = 400;

        [JsonProperty(DefaultValueHandling = DefaultValueHandling.Ignore)]
        public string fontStyle { get; set; }

        [JsonProperty(DefaultValueHandling = DefaultValueHandling.Ignore)]
        public string verticalAlign { get; set; }

        public string horizontalAlign { get; set; } = "right";
        public int blockHeight { get; set; } = 60;
        public int blockWidht { get; set; } = 60;
        public int marginRight { get; set; } = 5;
    }
}
