using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CviConverter.dto
{
    public class Numeric : ElementPanelBase
    {
        [JsonProperty(Order = 5)]
        public bool readOnly { get; set; } = true;

        [JsonProperty(Order = 6)]
        public string style { get; set; }

        [JsonProperty(Order = 7)]
        public int decimalPrecision { get; set; }

        [JsonProperty(Order = 11)]
        public bool hideLabel { get; set; } = true;

        [JsonProperty(Order = 12)]
        public bool disabled { get; set; } = true;

        [JsonProperty(Order = 13)]
        public FieldStyle fieldStyle { get; set; } = new FieldStyle();
    }
    public class FieldStyle
    {
        [JsonProperty(DefaultValueHandling = DefaultValueHandling.Ignore)]
        public string fontSize { get; set; }

        [JsonProperty(DefaultValueHandling = DefaultValueHandling.Ignore)]
        public string fontWeight { get; set; }

        [JsonProperty(DefaultValueHandling = DefaultValueHandling.Ignore)]
        public string color { get; set; }

        /// Originally, "text-align" but C# has a problem with char '-'
        [JsonProperty("text-align")]
        public string textalign { get; set; }

    }
}
