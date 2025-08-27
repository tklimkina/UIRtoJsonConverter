using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Newtonsoft.Json;

namespace CviConverter.dto
{
    public class MainPanel : BasePanel
    {
        [JsonProperty(Order = 2)]
        public string title { get; set; }

        [JsonProperty(Order = 5)]
        public Layout layout { get; set; } = new Layout();

        [JsonProperty(Order = 6)]
        public string bodystyle { get; set; }

        [JsonProperty(Order = 7)]
        public List<IPanel> items { get; set; } = new List<IPanel>();
    }

    public class Layout
    {
       public string type { get; set; } = "absolute";
    }
}
