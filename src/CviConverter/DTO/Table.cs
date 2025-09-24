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
        public int headerFontSize { get; set; } = 16;
        public int bodyFontSize { get; set; } = 16;
        public List<Column> columns { get; set; } = new List<Column>();
        public List<Row> rows { get; set; } = new List<Row>();
    }
    public class Column
    {
        public int width { get; set; }

        [JsonProperty(DefaultValueHandling = DefaultValueHandling.Ignore)]
        public Header header { get; set; } = new Header();

        [JsonProperty(DefaultValueHandling = DefaultValueHandling.Ignore)]
        public Body body { get; set; }
    }
    public class Header
    {
        public string label { get; set; }
        public int fontSize { get; set; } = 16;
        public int fontWeight { get; set; } = 400;
    }
    public class Body
    {
        public int countNumbersAfterDot { get; set; }
        public int fontSize { get; set; } = 16;
        public int fontWeight { get; set; } = 400;
    }
    public class ColumnData
    {
        [JsonProperty(DefaultValueHandling = DefaultValueHandling.Ignore)]
        public string label { get; set; }

        [JsonProperty(DefaultValueHandling = DefaultValueHandling.Ignore)]
        public string tag { get; set; }
    }
    public class Row
    {
        public List<ColumnData> columnsData { get; set; } = new List<ColumnData>();
    }
}
