using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CviConverter.DTO
{
    public interface IPanel
    {
        public string id { get; set; }
        public IWidget widget { get; set; }
    }
}
