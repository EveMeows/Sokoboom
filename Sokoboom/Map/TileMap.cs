using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using System.Xml;
using System.Xml.Schema;
using System.Xml.Serialization;

namespace Sokoboom.Map;

public class TileMap
{
    private Sokoban window;

    private int[,] data = {};

    private Dictionary<int, Texture2D> textures = new Dictionary<int, Texture2D>();

    public TileMap(int[,] data, Sokoban window)
    {
        this.data = data;
        this.window = window;
        this.textures.Add(1, this.window.Content.Load<Texture2D>("Entities/Wall"));
    }

    public int IDAtPosition(int x, int y) => this.data[x, y];

    public int IDAtPosition(Vector2 pos) => this.data[(int)pos.X, (int)pos.Y];

    public void Draw(SpriteBatch batch)
    {
        for (int x = 0; x < this.data.GetLength(0); x++)
        {
            for (int y = 0; y < this.data.GetLength(1); y++)
            {
                if (this.textures.TryGetValue(this.data[x, y], out Texture2D? texture))
                {
                    batch.Draw(texture, new Vector2(x, y) * this.window.CellSize, Color.White);
                }
            }
        }
    }

    public int[,] TileData => this.data;
}
