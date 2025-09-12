using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
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
        //  public DbSet<SysTblLst> {get;set;}

        // Standard funcs

        protected override void OnConfiguring(DbContextOptionsBuilder optionsBuilder)
        {
           // optionsBuilder.UseLoggerFactory(_loggerFactory);

            base.OnConfiguring(optionsBuilder);
        }

        protected override void OnModelCreating(ModelBuilder modelBuilder)
        {
            switch (Database.ProviderName)
            {/*
                case "Oracle.EntityFrameworkCore":
                    modelBuilder.UseOracle();
                    break;

                case "PostgreSQL":
                    modelBuilder.UsePgsql();
                    break;*/
            }
        }
    }
}
