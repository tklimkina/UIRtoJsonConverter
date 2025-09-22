using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CviConverter.DTO
{
    public class Table : ElementPanelBase
    {
    }

    public class TableWidget : BaseWidget
    {
        public List<RtData> rtdata { get; set; } = new List<RtData>();
    }
    public class TableWidgetOptions : BaseWidgetOptions
    {
        public TableContent table { get; set; } = new TableContent();
    }
    public class TableContent
    {
        public int headerHeight { get; set; }
        public int headerFontSize { get; set; }
        public int bodyFontSize { get; set; }
        public List<Column> columns { get; set; } = new List<Column>();
        public List<ColumnData> columnsData { get; set; } = new List<ColumnData>();
    }
    public class Column
    {
        public int width { get; set; }

        [JsonProperty(DefaultValueHandling = DefaultValueHandling.Ignore)]
        public Header header { get; set; }

        [JsonProperty(DefaultValueHandling = DefaultValueHandling.Ignore)]
        public Body body { get; set; }
    }
    public class Header
    {
        public string label { get; set; }
        public int fontSize { get; set; }
        public int fontWeight { get; set; }
    }
    public class Body
    {
        public int countNumbersAfterDot { get; set; }
        public int fontSize { get; set; }
        public int fontWeight { get; set; }
    }
    public class ColumnData
    {
        [JsonProperty(DefaultValueHandling = DefaultValueHandling.Ignore)]
        public string label { get; set; }

        [JsonProperty(DefaultValueHandling = DefaultValueHandling.Ignore)]
        public string tag { get; set; }
    }
}
