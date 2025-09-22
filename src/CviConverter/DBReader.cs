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

            var panel = dbContext.VpPanels
                                 .Where(v => v.UirName.Contains(dto.name))
                                 .AsNoTracking()
                                 .FirstOrDefault();

            if(panel == null)
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
                Log.Error("Отсутствует описание схемы '{0}' в таблице 'VP_CTRL'. Id схемы: {1}", dto.name, panel.Id);
                return new List<MainPanel>() { dto };
            }

            for (int i = 0; i < dto.layout.frames.Count; i++)
            {
                    ctrl = dbContext.VpCtrls
                                 .Where(v => v.PanelId == panel.Id)
                                 .Where(v => v.ConstName == dto.layout.frames[i].id)
                                 .AsNoTracking()
                                 .FirstOrDefault();

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
                            Log.Error("Отсутствует описание параметра для контрольного элемента '{0}'", ctrl.Id);
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
                            Log.Error("Отсутствует описание параметраов для контрольного элемента '{0}'", ctrl.Id);
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
                            Log.Error("Отсутствует описание параметраов для контрольного элемента '{0}'", ctrl.Id);
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
            widget.rtdata.paramId = param.ParamId;
            widget.rtdata.tableId = param.TableId;
            widget.rtdata.gtopt = gtopt?.DefineAlias;
            return widget;
        }
        internal static TeleSignalWidget FillRtDataSig(dynamic widget, VpParam param, SysGtopt gtopt)
        {
            widget.rtdata.paramId = param.ParamId;
            widget.rtdata.tableId = param.TableId;
            widget.rtdata.gtopt = gtopt?.DefineAlias;
            return widget;
        }
        internal static SingleValueWidget FillRtDataVal(dynamic widget, VpParam param, SysGtopt gtopt)
        {
            widget.options.rtdata.paramId = param.ParamId;
            widget.options.rtdata.tableId = param.TableId;
            widget.options.rtdata.gtopt = gtopt?.DefineAlias;
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
