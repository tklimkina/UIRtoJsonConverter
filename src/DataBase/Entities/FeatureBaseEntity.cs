using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataBase.Entities
{
    public class FeatureBaseEntity
    {
        [Key]
        [Column("MASK")]
        public int Mask { get; set; }

        [Column("NAME")]
        public string Name { get; set; }

        [Column("DEFINE_ALIAS")]
        public string DefineAlias { get; set; }
    }
}
