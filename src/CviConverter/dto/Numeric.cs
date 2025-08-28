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
