using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CviConverter.dto
{
    public class Led : BasePanel
    {
        public string xtype { get; set; } = "fieldset";
        public double x { get; set; }
        public double y { get; set; }
        public string style { get; set; }
        public List<VStyle> vstyle { get; set; } = new List<VStyle>();
        public string cls { get; set; }
    }
    public class VStyle
    {
        public string backgroung { get; set; }
    }
}
