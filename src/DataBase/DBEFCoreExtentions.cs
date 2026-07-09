using Microsoft.Extensions.DependencyInjection;
using Microsoft.EntityFrameworkCore;
namespace DataBase
{
    public static class DBEFCoreExtentions
    {
        public static IServiceCollection AddDBEFCore(this IServiceCollection services, DbConnectionOptions options)
        {
            switch (options.Provider)
            {
                case "Oracle":
                    services.AddDbContext<DbContext>(opt =>
                            opt.UseOracle(options.ConnectionString,
                                o => o.UseOracleSQLCompatibility("11")));
                    break;
                case "PostgreSQL":
                    services.AddDbContext<DbContext>(opt =>
                            opt.UseNpgsql(options.ConnectionString,
                                o => o.SetPostgresVersion(11, 5)));
                    break;
            }
            return services;
        }
    }
}
