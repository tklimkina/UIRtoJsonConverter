using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations.Schema;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataBase.Entities
{
    [Table("VP_GROUP")]
    public class VpGroup
    {
        [Key]
        [Column("ID")]
        public int Id { get; set; }

        [Column("ID_PARENT")]
        public int ParentId { get; set; }

        [Column("ID_TYPE")]
        public int TypeId { get; set; }

        [Column("NAME")]
        public string Name { get; set; }

        [Column("ALIAS")]
        public string Alias { get; set; }
    }
}
