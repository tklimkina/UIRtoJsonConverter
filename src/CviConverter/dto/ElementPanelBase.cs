using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CviConverter.DTO
{
    public class ElementPanelBase : IPanel
    {
        public int width { get; set; }

        public int height { get; set; }

        [JsonProperty(Order = -10)]
        public string id { get; set; }

        [JsonProperty(Order = -9)]
        public int x { get; set; }

        [JsonProperty(Order = -8)]
        public int y { get; set; }

        [JsonProperty(Order = 5)]
        public  IWidget widget { get; set; }
    }
}
