using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CviConverter.dto
{
    public class MainPanel : BasePanel
    {
        public string title { get; set; }
        public Layout layout { get; set; } = new Layout();
        public string bodystyle { get; set; }
        public List<IPanel> items { get; set; } = new List<IPanel>();
    }

    public class Layout
    {
       public string type { get; set; } = "absolute";
    }
}
