using Microsoft.EntityFrameworkCore;
//using Microsoft.EntityFrameworkCore.r
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataBase
{
    public static class PostgreModelBuilderExtension
    {
        public static ModelBuilder UsePostgre(this ModelBuilder modelBuilder)
        {
            foreach (var entity in modelBuilder.Model.GetEntityTypes())
            {
                entity.SetTableName(entity.GetTableName().ToLower());

                foreach (var property in entity.GetProperties())
                {
                    property.SetColumnName(property.GetColumnName().ToLower());
                }
            }

            return modelBuilder;
        }
    }
}
