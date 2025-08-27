using Newtonsoft.Json;

namespace CviConverter.dto
{
    public abstract class BasePanel : IPanel
    {
       // [JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
        [JsonProperty(Order = 3)]
        public int width { get; set; }

       // [JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
        [JsonProperty(Order = 4)]
        public int height { get; set; }
        [JsonProperty(Order = 1)]
        public string id { get; set; }
    }
}
