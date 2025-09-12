using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json.Serialization;
using System.Threading.Tasks;

namespace DataBase
{
    //[JsonProperty("")]
    public class DbConnectionOptions
    {
        [JsonProperty("Provider")]
        public string Provider { get; set; }
        public string ConnectionString { get; set; }
    }
}
