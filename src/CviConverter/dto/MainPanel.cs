using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Newtonsoft.Json;

namespace CviConverter.dto
{
    public class MainPanel
    {
        public string name { get; set; }
        public string description { get; set; }
        public Window window { get; set; } = new Window();
        public Layout layout { get; set; } = new Layout();
    }

    public class Window : BasePanel
    {
        public string backgroundColor { get; set; } = "#EBEBEB";
    }

    public class Layout
    {
      // public string type { get; set; } = "absolute";
       public Grid grid { get; set;} = new Grid();
       public List<IPanel> frames { get; set; } = new List<IPanel>();
    }

    public class Grid
    {
        public int cols { get; set; } = 1920;
        public int rows { get; set; } = 1080;
        public List<int> margin { get; set; } = new List<int>() {0, 0};
    }
}
