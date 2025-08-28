using Newtonsoft.Json;

namespace CviConverter.dto
{
    public abstract class BasePanel : IPanel
    {
        [JsonProperty(Order = 0)]
        public int width { get; set; }

        [JsonProperty(Order = 1)]
        public int height { get; set; }
    }
}
