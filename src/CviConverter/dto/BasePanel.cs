using Newtonsoft.Json;

namespace CviConverter.DTO
{
    public abstract class BasePanel : IPanel
    {
     //   [JsonProperty(Order = 0, DefaultValueHandling = DefaultValueHandling.Ignore)]
        public int width { get; set; }

      //  [JsonProperty(Order = 1, DefaultValueHandling = DefaultValueHandling.Ignore)]
        public int height { get; set; }
    }
}
