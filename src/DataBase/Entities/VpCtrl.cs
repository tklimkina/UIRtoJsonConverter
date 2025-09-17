using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataBase.Entities
{
    [Table("VP_CTRL")]
    public class VpCtrl
    {
        [Key]
        [Column("ID")]
        public int Id { get; set; }

        [Column("ID_PANEL")]
        public int PanelId { get; set; }

        [Column("CONST_NAME")]
        public string ConstName { get; set; }

        [Column("ID_TCTRL")]
        public int TctrlId { get; set; }

        [Column("FEATURE")]
        public int Feature { get; set; }

        [Column("TAB_PAGE_NAME")]
        public string? TabPageName { get; set; }
    }
}
