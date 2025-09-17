using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CviConverter.DTO
{
    public class RsduLogo : ElementPanelBase
    {

    }
    public class RsduLogoWidget : BaseWidget
    {
        public RsduLogoWidgetOptions options { get; set; } = new RsduLogoWidgetOptions();
    }
    public class RsduLogoWidgetOptions : BaseWidgetOptions
    {

    }
}
