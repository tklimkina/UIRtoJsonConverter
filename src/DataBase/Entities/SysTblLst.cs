using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using System.Text;

namespace Ema.Rsdu.Database.EFCore.Entities
{
    /// <summary>
    /// Список таблиц комплекса РСДУ2
    /// </summary>
    [Table("SYS_TBLLST")]
    public class SysTblLst
    {
        /// <summary>
        /// Идентификатор
        /// </summary>
        [Key]
        [Column("ID")]
        public int Id { get; set; }

        /// <summary>
        /// Идентификатор подсистемы
        /// </summary>
        [Column("ID_NODE")]
        public int? NodeId { get; set; }

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
        /// Уникальный строковый идентификатор
        /// </summary>
        [Column("DEFINE_ALIAS")]
        public string DefineAlias { get; set; }

        /// <summary>
        /// Название таблицы
        /// </summary>
        [Column("TABLE_NAME")]
        public string TableName { get; set; }

         /// <summary>
        /// Идентификатор типа
        /// </summary>
        [Column("ID_TYPE")]
        public int TypeId { get; set; }

        /// <summary>
        /// Тип объекта
        /// </summary>
     //   public SysOTyp Type { get; set; }

        /// <summary>
        /// Идентификатор файла системы озвучивания
        /// </summary>
        [Column("ID_FILEWAV")]
        public int? AudioFileId { get; set; }

        [Column("LAST_UPDATE")]
        public int? LastUpdate { get; set; }
    }
}