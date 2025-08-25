using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CviConverter.dto
{
    public abstract class BasePanel : IPanel
    {
        public double width { get; set; }
        public double height { get; set; }
        public string id { get; set; }
    }
}
