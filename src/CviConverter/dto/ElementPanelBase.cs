using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CviConverter.dto
{
    public class ElementPanelBase : BasePanel
    {
        [JsonProperty(Order = -5)]
        public string xtype { get; set; }

        [JsonProperty(Order = -4)]
        public int x { get; set; }

        [JsonProperty(Order = -3)]
        public int y { get; set; }
        [JsonProperty(Order = 10)]
        public string id { get; set; }
    }
}
