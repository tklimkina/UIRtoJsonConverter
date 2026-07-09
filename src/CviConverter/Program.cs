using CviConverter.DTO;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using Microsoft.Extensions.Hosting;
using DataBase;
using Serilog;
using System.IO;



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

            // to cut all rubbish
            path += '\0';

            string panelname = path.Replace(".uir\0", "");
            if(panelname.Contains('\\'))
            {
                var pathparts = panelname.Split('\\');
                panelname = pathparts[pathparts.Length - 1];
            }

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
            string outputdir = "jsons";
            if (!Directory.Exists(outputdir))
            {
                Directory.CreateDirectory(outputdir);
            }

            foreach (var dto in dtos)
            {
                var filename = outputdir + "/" + dto.name + ".json";
                string json = JsonConvert.SerializeObject(dto, Formatting.Indented);
                File.WriteAllText(filename, json);
            }
        }



        static DbConnectionOptions GetOptions()
        {
            var filename = "db.json";
            string json = File.ReadAllText(filename);
            var jObject = JObject.Parse(json);
            var ConnectionOpts = jObject["DatabaseSettings"].ToObject<DbConnectionOptions>();
            return ConnectionOpts;
        }
    }
}