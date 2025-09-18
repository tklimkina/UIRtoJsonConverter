using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Formats.Asn1;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CviConverter.DTO
{
    public class ChartRTDTracking : ElementPanelBase
    {
        public List<RtData> rtdata { get; set; } = new List<RtData>();
        public List<RtData> archives { get; set; } = new List<RtData>();
    }
    public class ChartRTDTrackingWidget : BaseWidget
    {
        public ChartRTDTrackingWidgetOptions options { get; set; } = new ChartRTDTrackingWidgetOptions();
    }
    public class ChartRTDTrackingWidgetOptions : BaseWidgetOptions
    {
        public string text { get; set; }
        public string interval { get; set; }
        public int duration { get; set; }
        public int digitsAfterDot { get; set; }
        public int minDifference { get; set; }
        public int maxDifference { get; set; }
        public string render { get; set; }
        public int RTDRenderDelay { get; set; }
        public int RTDLimit { get; set; }
        public SeriesOptions seriesOptions { get; set; } = new SeriesOptions();
    }
    public class SeriesOptions
    {
        public string lineColor { get; set; }
        public int lineWidth { get; set; } = 4;
    }
}
