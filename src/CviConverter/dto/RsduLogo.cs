using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CviConverter.dto
{
    public class RsduLogo : ElementPanelBase
    {
        public RsduLogoWidget widget { get; set; } = new RsduLogoWidget();
    }
    public class RsduLogoWidget : BaseWidget
    {
        [JsonProperty]
        private string type = "Labels.Rsdu5Logo";
        public RsduLogoWidgetOptions options { get; set; } = new RsduLogoWidgetOptions();
    }
    public class RsduLogoWidgetOptions : BaseWidgetOptions
    {

    }
}
