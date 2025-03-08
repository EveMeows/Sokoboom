using Microsoft.Xna.Framework.Content.Pipeline;
using System.IO;

namespace Sokoboom.Pipelines;

[ContentImporter(".csv", CacheImportedData = false, DisplayName = "CSV File Importer.", DefaultProcessor = "PassThroughProcessor")]
public class TileMapImporter : ContentImporter<int[,]>
{
    public override int[,] Import(string filename, ContentImporterContext context)
    {
        string[] content = File.ReadAllLines(filename);
        int rows = content.Length;
        int cols = content[0].Split(',').Length;


        int[,] data = new int[rows, cols];
        for (int i = 0; i < rows; i++)
        {
            string[] values = content[i].Split(',');
            for (int j = 0; j < cols; j++)
            {
                if (int.TryParse(values[j], out int value))
                {
                    data[i, j] = value;
                    continue;
                }

                throw new FileLoadException("Bad data.");
            }
        }

        return data;
    }
}
