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
    public class RsduDbContext : DbContext
    {
        //Transfering options
        public RsduDbContext(DbContextOptions<RsduDbContext> options)
            : base(options)
        {

        }
        // DbSets
          public DbSet<VpPanel> VpPanels { get;set;}
          public DbSet<VpGroup> VpGroups { get;set;}
          public DbSet<VpCtrlFeature> VpCtrlFeatures { get;set;}
          public DbSet<VpParamsFeature> VpParamsFeatures { get;set;}
          public DbSet<VpCtrl> VpCtrls { get;set;}
          public DbSet<VpTctrl> VpTctrls { get;set;}
          public DbSet<VpParam> VpParams { get;set;}
          public DbSet<VpSql> VpSqls { get;set;}
          public DbSet<SysGtopt> SysGtopts { get; set; }

// Standard funcs

protected override void OnModelCreating(ModelBuilder modelBuilder)
        {
            switch (Database.ProviderName)
            {
               /* case "Oracle.EntityFrameworkCore":
                    modelBuilder.UseOracle();
                    break;*/

                case "Npgsql.EntityFrameworkCore.PostgreSQL":
                    modelBuilder.UsePostgre();
                    break;
            }
        }
    }
}
