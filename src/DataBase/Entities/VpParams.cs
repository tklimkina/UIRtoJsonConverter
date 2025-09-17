using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataBase.Entities
{
    [Table("VP_PARAMS")]
    public class VpParam
    {
        [Key]
        [Column("ID")]
        public int Id { get; set; }

        [Column("ID_CTRL")]
        public int CtrlId { get; set; }

        [Column("ID_TABLE")]
        public int TableId { get; set; }

        [Column("ID_PARAM")]
        public int ParamId { get; set; }

        [Column("ID_GTOPT")]
        public int GtopId { get; set; }

        [Column("ID_SQL")]
        public int? SqlId { get; set; }

        [Column("POS_X")]
        public int PosX { get; set; }

        [Column("POS_Y")]
        public int PosY { get; set; }

        [Column("FEATURE")]
        public int Feature { get; set; }
    }
}
