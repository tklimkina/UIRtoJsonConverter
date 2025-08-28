using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CviConverter.dto
{
    public class BaseWidget : IWidget
    {
       public string type { get; set; }
       public IWidgetOptions options { get; set; }
    }
    public class BaseWidgetOptions : IWidgetOptions
    {

    }
}
