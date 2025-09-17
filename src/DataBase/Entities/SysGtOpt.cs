using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using System.Text;

namespace Ema.Rsdu.Database.EFCore.Entities
{
    /// <summary>
    /// Опция глобального типа
    /// </summary>
    [Table("SYS_GTOPT")]
    public class SysGtopt
    {
        [Key]
        [Column("ID")]
        public int Id { get; set; }

        [Column("NAME")]
        public string Name { get; set; }

        [Column("ALIAS")]
        public string Alias { get; set; }

        [Column("DEFINE_ALIAS")]
        public string DefineAlias { get; set; }

        [Column("ID_GTYPE")]
        public int? GTypeId { get; set; }

        [Column("INTERVAL")]
        public int? Interval { get; set; }

        [Column("ID_ATYPE")]
        public int? ATypeId { get; set; }
    }
}