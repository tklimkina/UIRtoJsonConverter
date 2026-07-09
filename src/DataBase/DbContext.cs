using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using DataBase.Entities;
using Ema.Rsdu.Database.EFCore.Entities;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.Logging;
using Microsoft.Extensions.Options;

namespace DataBase
{
    public class DbContext : Microsoft.EntityFrameworkCore.DbContext
    {
        //Transfering options
        public DbContext(DbContextOptions<DbContext> options)
            : base(options)
        {

        }
        // DbSets
          public DbSet<VpPanel> VpPanels { get;set;}
          public DbSet<VpCtrl> VpCtrls { get;set;}
          public DbSet<VpParam> VpParams { get;set;}
          public DbSet<SysGtopt> SysGtopts { get; set; }

// Standard funcs

protected override void OnModelCreating(ModelBuilder modelBuilder)
        {
            switch (Database.ProviderName)
            {
                case "Oracle.EntityFrameworkCore":
                    modelBuilder.UseOracle();
                    break;

                case "Npgsql.EntityFrameworkCore.PostgreSQL":
                    modelBuilder.UsePostgre();
                    break;
            }
        }
    }
}
