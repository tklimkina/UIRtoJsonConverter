using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations.Schema;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataBase.Entities
{
    [Table("VP_SQL")]
    public class VpSql
    {
        [Key]
        [Column("ID")]
        public int Id { get; set; }

        [Column("INTERVAL")]
        public int Interval { get; set; }

        [Column("QUERY")]
        public string Query { get; set; }
    }
}
