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
    public class SysGtOpt
    {
        /// <summary>
        /// Идентификатор
        /// </summary>
        [Key]
        [Column("ID")]
        public int Id { get; set; }

        /// <summary>
        /// Наименование
        /// </summary>
        [Column("NAME")]
        public string Name { get; set; }

        /// <summary>
        /// Краткое наименование
        /// </summary>
        [Column("ALIAS")]
        public string Alias { get; set; }

        /// <summary>
        /// Уникальный строковый идентификатор.
        /// </summary>
        [Column("DEFINE_ALIAS")]
        public string DefineAlias { get; set; }

        /// <summary>
        /// Идентификатор глобального типа
        /// </summary>
        [Column("ID_GTYPE")]
        public int? GTypeId { get; set; }

      //  public SysGTyp GType { get; set; }

        /// <summary>
        /// Интервал получения значении в сек
        /// </summary>
        [Column("INTERVAL")]
        public int? Interval { get; set; }

        /// <summary>
        /// Идентификатор агрегирующего типа данных
        /// </summary>
        [Column("ID_ATYPE")]
        public int? ATypeId { get; set; }

      //  public SysATyp AType { get; set; }
    }
}