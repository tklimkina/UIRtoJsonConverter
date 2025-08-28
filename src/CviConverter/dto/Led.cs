using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CviConverter.dto
{
    public class Led : ElementPanelBase
    {
       /* [JsonProperty(Order = 6)]
        public string id { get; set; }*/

        [JsonProperty(Order = 5)]
        public string style { get; set; }

        [JsonProperty(Order = 11)]
        public Dictionary<int, VStyle[]> vstyle { get; set; } = new Dictionary<int, VStyle[]>();

        [JsonProperty(Order = 12)]
        public string cls { get; set; }
    }
    public class VStyle
    {
        public string backgroung { get; set; }
    }
}
