using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CviConverter.dto
{
    public interface IPanel
    {
        double width { get; set; }
        double height { get; set; }
        string id { get; set; }
    }
}
