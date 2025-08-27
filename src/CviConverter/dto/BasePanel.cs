using Newtonsoft.Json;

namespace CviConverter.dto
{
    public abstract class BasePanel : IPanel
    {
        [JsonProperty(Order = 4, DefaultValueHandling = DefaultValueHandling.Ignore)]
        public int width { get; set; }

        [JsonProperty(Order = 3, DefaultValueHandling = DefaultValueHandling.Ignore)]
        public int height { get; set; }
        [JsonProperty(Order = 1)]
        public string id { get; set; }
    }
}
