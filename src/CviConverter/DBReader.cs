using CviConverter.DTO;
using DataBase;
using DataBase.Entities;
using Ema.Rsdu.Database.EFCore.Entities;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.DependencyInjection;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CviConverter
{
    public static class DBReader
    {
        public static List<MainPanel> ReadSettingsFromDB(IServiceProvider services, MainPanel dto)
        {
            using var scope = services.CreateScope();
            using var dbContext = scope.ServiceProvider.GetRequiredService<RsduDbContext>();

            /*var panel = dbContext.VpPanels
                                 .Where(v => v.UirName.Contains(dto.name))
                                 .AsNoTracking()
                                 .FirstOrDefault();*/
            var panel = dbContext.VpPanels ////////////////////////////////////////// for debug
                                 .Where(v => v.UirName.Contains("Ключ ТУ"))
                                 .AsNoTracking()
                                 .FirstOrDefault();

            ////////////////////// Make a log record if null////////////

            var ctrl = dbContext.VpCtrls
                                 .Where(v => v.PanelId == panel.Id)
                                 .AsNoTracking()
                                 .FirstOrDefault();

            ////////////////////// Make a log record if null////////////

            for(int i = 0; i < dto.layout.frames.Count; i++)
            {
                    var ctrls = dbContext.VpCtrls
                                 .Where(v => v.PanelId == panel.Id)
                                 .Where(v => v.ConstName == dto.layout.frames[i].id)
                                 .AsNoTracking()
                                 .ToList();

                switch (dto.layout.frames[i].widget.type)
                {
                    case "Analogs.RTImage":
                    case "Analogs.SingleValue":
                    case "Analogs.Telesignal":
                        var param = dbContext.VpParams
                                             .Where(v => v.CtrlId == ctrls[0].Id)
                                             .AsNoTracking()
                                             .FirstOrDefault();
                        var gtopt = dbContext.SysGtopts
                                             .Where( s => s.Id == param.GtopId)
                                             .AsNoTracking()
                                             .FirstOrDefault();
                        dto.layout.frames[i].widget = FillRtData(dto.layout.frames[i].widget, param, gtopt);
                        break;
                }

            }

            //Several Tabs
            if (ctrl.TabPageName != null)
                return DistributeToTabs(dbContext, dto);

            // No tabs

            return new List<MainPanel>() { dto };
        }

       /* private static IWidget FillRtData(IWidget widget, VpParam? param, SysGtopt? gtopt)
        {
            var typ = typeof(Widget.GetType());
            return FillRtData( ()widget,  param, gtopt);
        }*/

        internal static List<MainPanel> DistributeToTabs(RsduDbContext dbContext, MainPanel dto)
        {
            return new List<MainPanel>();
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
            widget.rtdata.gtopt = gtopt.DefineAlias;
            return widget;
        }
        internal static TeleSignalWidget FillRtDataSig(dynamic widget, VpParam param, SysGtopt gtopt)
        {
            widget.rtdata.paramId = param.ParamId;
            widget.rtdata.tableId = param.TableId;
            widget.rtdata.gtopt = gtopt.DefineAlias;
            return widget;
        }
        internal static SingleValueWidget FillRtDataVal(dynamic widget, VpParam param, SysGtopt gtopt)
        {
            widget.options.rtdata.paramId = param.ParamId;
            widget.options.rtdata.tableId = param.TableId;
            widget.options.rtdata.gtopt = gtopt.DefineAlias;
            return widget;
        }

    }
}
