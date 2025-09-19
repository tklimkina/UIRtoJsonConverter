using CviConverter.DTO;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using Microsoft.Extensions.Hosting;
using DataBase;
using Serilog;



namespace CviConverter
{
    class Program
    {
        static IHost _host;
        static void Main(string[] args)
        {
            if (args.ToList().Count == 0)
                return;


            string path = "";
            foreach (var arg in args)
            {
                path += arg;
                if (!arg.Contains('.'))
                    path += ' ';
                else
                    break;
            }

            string panelname = path.Replace(".uir", "");

            ConfigureServices();

            var CVIpanel = UirReader.BuildDTO(path, panelname);
            // widget null
            if (CVIpanel == null)
                return;

            
             var panelList = DBReader.ReadSettingsFromDB(_host.Services, CVIpanel);

            SaveJson(panelList, panelname);

            Console.WriteLine("Cхема успешно конвертирована в {0}.json", panelname);
        }

        static void ConfigureServices()
        {
            /// build bd
            var opts = GetOptions();

            _host = Host.CreateDefaultBuilder()
            .ConfigureServices(services =>
            {
                // Register services
                services.AddDBEFCore(opts);
                services.AddSerilog((context, configuration) => configuration
                                                                     .WriteTo.File("logs/CviConverterLog.txt")
                                                                     .MinimumLevel.Error()
                                                                     );
            })
            .Build();

        }

        static void SaveJson(List<MainPanel> dtos, string panelname)
        {
            foreach(var dto in dtos)
            {
                var filename = dto.name + ".json";
                string json = JsonConvert.SerializeObject(dto, Formatting.Indented);
                File.WriteAllText(filename, json);
            }
        }



        static DbConnectionOptions GetOptions()
        {
            var filename = "db.json";
            string json = File.ReadAllText(filename);
            var jObject = JObject.Parse(json);
            var ConnectionOpts = jObject["RsduDatabase"].ToObject<DbConnectionOptions>();
            return ConnectionOpts;
        }
    }
}