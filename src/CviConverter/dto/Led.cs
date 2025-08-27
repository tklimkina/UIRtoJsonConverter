using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CviConverter.dto
{
    public class Led : BasePanel
    {
        [JsonProperty(Order = -5)]
        public string xtype { get; set; } = "fieldset";

        [JsonProperty(Order = -4)]
        public int x { get; set; }

        [JsonProperty(Order = -3)]
        public int y { get; set; }

        [JsonProperty(Order = 6)]
        public string id { get; set; }

        [JsonProperty(Order = 5)]
        public string style { get; set; }

        [JsonProperty(Order = 7)]
        public Dictionary<int, VStyle[]> vstyle { get; set; } = new Dictionary<int, VStyle[]>();

        [JsonProperty(Order = 8)]
        public string cls { get; set; }
    }
    public class VStyle
    {
        public string backgroung { get; set; }
    }
}
