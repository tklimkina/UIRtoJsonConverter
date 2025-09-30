using CviConverter.DTO;
using DataBase;
using DataBase.Entities;
using Ema.Rsdu.Database.EFCore.Entities;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.DependencyInjection;
using Serilog;

namespace CviConverter
{
    public static class DBReader
    {
        public static List<MainPanel> ReadSettingsFromDB(IServiceProvider services, MainPanel dto)
        {
            using var scope = services.CreateScope();
            using var dbContext = scope.ServiceProvider.GetRequiredService<RsduDbContext>();

            var panel = new VpPanel();

            try
            {
                panel = dbContext.VpPanels
                                 .Where(v => v.UirName.Contains('\\' + dto.name + ".uir") || v.UirName == dto.name + ".uir")
                                 .AsNoTracking()
                                 .FirstOrDefault();
            }
            catch
            {
                Console.WriteLine("Не удалось подключиться к БД. Настройки панели не будут считаны.");
                return new List<MainPanel>() { dto };
            }

            if (panel == null)
            {
                Log.Error("Отсутствует описание схемы '{0}' в таблице 'VP_PANEL'", dto.name);
                return new List<MainPanel> (){ dto };
            }

            var ctrl = dbContext.VpCtrls
                                 .Where(v => v.PanelId == panel.Id)
                                 .AsNoTracking()
                                 .FirstOrDefault();

            if (ctrl == null)
            {
                Log.Error("Отсутствует описание схемы '{0}' в таблице 'VP_CTRL'.Id панели: {1}", dto.name, panel.Id);
                return new List<MainPanel>() { dto };
            }

            for (int i = 0; i < dto.layout.frames.Count; i++)
            {
                    ctrl = dbContext.VpCtrls
                                 .Where(v => v.PanelId == panel.Id)
                                 .Where(v => v.ConstName == dto.layout.frames[i].id)
                                 .AsNoTracking()
                                 .FirstOrDefault();

                if(ctrl == null)
                {
                    Log.Error("Отсутствует запись в таблице 'VP_CTRL' для элемента '{0}'. Id панели: {1}", dto.layout.frames[i].id, panel.Id);
                    continue;
                }

                switch (dto.layout.frames[i].widget.type)
                {
                    case "Analogs.RTImage":
                    case "Analogs.SingleValue":
                    case "Analogs.Telesignal":
                        var param = dbContext.VpParams
                                             .Where(v => v.CtrlId == ctrl.Id)
                                             .AsNoTracking()
                                             .FirstOrDefault();
                        if (param == null)
                        {
                            Log.Error("Отсутствует описание параметра для контрольного элемента {0}. Id панели: {1}", ctrl.Id, panel.Id);
                            continue;
                        }
                        var gtopt = dbContext.SysGtopts
                                             .Where( s => s.Id == param.GtopId)
                                             .AsNoTracking()
                                             .FirstOrDefault();
                        dto.layout.frames[i].widget = FillRtData(dto.layout.frames[i].widget, param, gtopt);
                        break;
                    case "Charts.ChartRTDTracking":
                        var prms = dbContext.VpParams
                                             .Where(v => v.CtrlId == ctrl.Id)
                                             .AsNoTracking()
                                             .ToList();

                        if (prms.Count == 0)
                        {
                            Log.Error("Отсутствует описание параметров для контрольного элемента {0} в таблице 'VP_PARAMS'. Id панели: {1}", ctrl.Id, panel.Id);
                            continue;
                        }

                        dto.layout.frames[i] = FillRtDataCh(dto.layout.frames[i], prms, dbContext);
                        break;

                    case "Analogs.TableGtp":
                        var prmst = dbContext.VpParams
                                             .Where(v => v.CtrlId == ctrl.Id)
                                             .AsNoTracking()
                                             .ToList();

                        if (prmst.Count == 0)
                        {
                            Log.Error("Отсутствует описание параметров для контрольного элемента {0}. Id панели: {1}", ctrl.Id, panel.Id);
                            continue;
                        }

                        dto.layout.frames[i].widget = FillRtDataT(dto.layout.frames[i].widget, prmst, dbContext);
                        break;
                }

            }

            //Several Tabs
            if (ctrl.TabPageName != null)
                return DistributeToTabs(dbContext, dto, panel.Id);

            // No tabs
            return new List<MainPanel>() { dto };
        }

        internal static List<MainPanel> DistributeToTabs(RsduDbContext dbContext, MainPanel dto, int pId)
        {
            var res = new List<MainPanel>();
            var tabs = new Dictionary<string?, List<IPanel>>();
            // Distributing frames
            foreach (var f in dto.layout.frames)
            {
                var tabpage = dbContext.VpCtrls
                                       .Where(v => v.PanelId == pId)
                                       .Where(v => v.ConstName == f.id)
                                       .AsNoTracking()
                                       .FirstOrDefault();
                
                if(tabpage == null)
                {
                    Log.Error("Отсутствует описаниев таблице 'VP_CTRL' для элемента {0}", f.id);
                }

                if(!tabs.Keys.Contains(tabpage?.TabPageName))
                {
                    tabs.Add(tabpage?.TabPageName, new List<IPanel>() { f });
                }
                else
                {
                    tabs[tabpage?.TabPageName].Add(f);
                }
            }

            foreach(var k in tabs.Keys)
            {
                // Making tabs
                var tabpanel = new MainPanel()
                {
                    name = dto.name + '_' + k,
                    description = dto.description + '_' + k,
                    window = dto.window
                };

                //Adding frames
                foreach (var v in tabs[k])
                {
                    tabpanel.layout.frames.Add(v);
                }
                res.Add(tabpanel);
            }

            return res;
        }
        internal static IWidget FillRtData(dynamic widget, VpParam param, SysGtopt gtopt)
        {
            return widget switch
            {
                RTImageWidget img => FillRtDataIm(img, param, gtopt),
                TeleSignalWidget sig => FillRtDataSig(sig, param, gtopt),
                SingleValueWidget val => FillRtDataVal(val, param, gtopt),
                _ => throw new ArgumentException($"Unsupported widget type: {widget?.GetType()}")
            };
        }
        internal static RTImageWidget FillRtDataIm(dynamic widget, VpParam param, SysGtopt gtopt)
        {
            var rtd = new RtData();
            rtd.paramId = param.ParamId;
            rtd.tableId = param.TableId;
            rtd.gtopt = gtopt?.DefineAlias;
            widget.rtdata.Add(rtd);
            return widget;
        }
        internal static TeleSignalWidget FillRtDataSig(dynamic widget, VpParam param, SysGtopt gtopt)
        {
            var rtd = new RtData();
            rtd.paramId = param.ParamId;
            rtd.tableId = param.TableId;
            rtd.gtopt = gtopt?.DefineAlias;
            widget.rtdata.Add(rtd);
            return widget;
        }
        internal static SingleValueWidget FillRtDataVal(dynamic widget, VpParam param, SysGtopt gtopt)
        {
            var rtd = new RtData();
            rtd.paramId = param.ParamId;
            rtd.tableId = param.TableId;
            rtd.gtopt = gtopt?.DefineAlias;
            widget.options.rtdata.Add(rtd);
            return widget;
        }
        internal static ChartRTDTracking FillRtDataCh (dynamic chart, List<VpParam> prms, RsduDbContext dbContext)
        {
            foreach ( var p in prms)
            {
                var gtopt = dbContext.SysGtopts
                                     .Where(g => g.Id == p.GtopId)
                                     .AsNoTracking()
                                     .FirstOrDefault();

                chart.rtdata.Add(new RtData() {
                    tag = "p",
                    tableId = p.TableId,
                    paramId = p.ParamId,
                    gtopt = gtopt?.DefineAlias
                });
                chart.archives.Add(new RtData()
                {
                    tag = "p",
                    tableId = p.TableId,
                    paramId = p.ParamId,
                    gtopt = "GLT_ANALOG_OPT_MEAN1"
                });
            }
            return chart;
        }
        internal static TableWidget FillRtDataT (dynamic tab, List<VpParam> prms, RsduDbContext dbContext)
        {
            foreach (var p in prms)
            {
                var gtopt = dbContext.SysGtopts
                                     .Where(g => g.Id == p.GtopId)
                                     .AsNoTracking()
                                     .FirstOrDefault();

                tab.rtdata.Add(new RtData()
                {
                    //tag = "p",
                    tableId = p.TableId,
                    paramId = p.ParamId,
                    gtopt = gtopt?.DefineAlias
                });
            }
            return tab;
        }

    }
}
