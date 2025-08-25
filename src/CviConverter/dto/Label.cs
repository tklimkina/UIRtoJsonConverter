using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CviConverter.dto
{
    public class Label : BasePanel
    {
        public string xtype { get; set; }
        public double x { get; set; }
        public double y { get; set; }
        public string style { get; set; }
        public string text { get; set; }
    }
}
