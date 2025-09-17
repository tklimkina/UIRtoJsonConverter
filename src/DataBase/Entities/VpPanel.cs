using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using System.Text;

namespace Ema.Rsdu.Database.EFCore.Entities
{
    /// <summary>
    /// Объект модели раздела "Структура объекта управления"
    /// </summary>
    [Table("VP_PANEL")]
    public class VpPanel
    {
        [Key]
        [Column("ID")]
        public int Id { get; set; }

        [Column("ID_NODE")]
        public int? NodeId { get; set; }

        [Column("ID_TYPE")]
        public int TypeId { get; set; }

        [Column("NAME")]
        public string Name { get; set; }

        [Column("ALIAS")]
        public string Alias { get; set; }

        [Column("NAME_UIR")]
        public string UirName { get; set; }

        [Column("ID_GTOPT")]
        public int? GtoptId { get; set; }

        [Column("AUTOSCALE")]
        public int? Autoscale { get; set; }

        [Column("LAST_UPDATE")]
        public int? LastUpdate { get; set; }

        [Column("LAST_ACCESS")]
        public int? LastAccess { get; set; }

        [Column("ID_FILEWAV")]
        public int? AudioFileId { get; set; }
    }
}
