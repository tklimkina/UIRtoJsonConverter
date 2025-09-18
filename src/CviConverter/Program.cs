using CviConverter.DTO;
using System;
using System.Text;
using System.Text.Encodings.Web;
using System.Text.Unicode;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Hosting;
using Microsoft.EntityFrameworkCore;
using DataBase;


namespace CviConverter
{
    class Program
    {
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

            var CVIpanel = UirReader.BuildDTO(path, panelname);
            // widget null
            if (CVIpanel == null)
                return;

            /// build bd
            var opts = GetOptions();

            var host = Host.CreateDefaultBuilder(args)
            .ConfigureServices(services =>
            {
                // Register services
                services.AddDBEFCore(opts);
            })
            .Build();

             var panelList = DBReader.ReadSettingsFromDB(host.Services, CVIpanel);

            SaveJson(panelList, panelname);

            Console.WriteLine("Cхема успешно конвертирована в {0}.json", panelname);
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