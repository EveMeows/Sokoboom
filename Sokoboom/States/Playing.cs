using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using MonoGayme.Core.Controllers;
using MonoGayme.Core.States;
using Sokoboom.Entities;
using Sokoboom.Input;
using Sokoboom.Map;

namespace Sokoboom.States;

public class Playing(Sokoban window) : State
{
    private EntityController entities = new EntityController();

    private TileMap activeMap = null!;

    private Player player = null!;
    private Box box = null!;
    private Goal goal = null!;

    private void OnPlayerMoved(object? sender, PlayerMovedEventArgs args)
    {
        Vector2 playerGrid = args.Position / window.CellSize;
        Vector2 boxGrid = this.box.Position / window.CellSize;

        if (playerGrid == boxGrid)
        {
            switch (args.Direction)
            {
                case Direction.Left:
                    boxGrid.X -= 1;
                    if (this.activeMap.IDAtPosition(boxGrid) != 1)
                    {
                        this.box.Position = boxGrid * window.CellSize;
                        break;
                    }

                    this.player.Position.X += window.CellSize;
                    break;
                
                case Direction.Right:
                    boxGrid.X += 1;
                    if (this.activeMap.IDAtPosition(boxGrid) != 1)
                    {
                        this.box.Position = boxGrid * window.CellSize;
                        break;
                    }

                    this.player.Position.X -= window.CellSize;
                    break;

                case Direction.Up:
                    boxGrid.Y -= 1;
                    if (this.activeMap.IDAtPosition(boxGrid) != 1)
                    {
                        this.box.Position = boxGrid * window.CellSize;
                        break;
                    }

                    this.player.Position.Y += window.CellSize;
                    break;
               
                case Direction.Down:
                    boxGrid.Y += 1;
                    if (this.activeMap.IDAtPosition(boxGrid) != 1)
                    { 
                        this.box.Position = boxGrid * window.CellSize;
                        break;
                    }

                    this.player.Position.Y -= window.CellSize;
                    break;
            }
        }
    }

    private void SwitchMap(TileMap next)
    {
        this.activeMap = next;

        this.entities.QueueRemoveAll();

        for (int x = 0; x < this.activeMap.TileData.GetLength(0); x++)
        {
            for (int y = 0; y < this.activeMap.TileData.GetLength(1); y++)
            {
                Vector2 mapPos = new Vector2(y * window.CellSize, x * window.CellSize);
                int id = this.activeMap.TileData[x, y];

                switch (id)
                {
                    // Player
                    case 5:
                        Player player = new Player(window, this.activeMap) {
                            Position = mapPos,
                        };

                        player.OnMoved += this.OnPlayerMoved;

                        this.entities.Add(player);
                        this.player = player;

                        break;

                    // Box
                    case 2:
                        Box box = new Box(window) {
                            Position = mapPos
                        };

                        this.entities.Add(box);
                        this.box = box;

                        break;

                    // Goal
                    case 3:
                        Goal goal = new Goal(window) {
                            Position = mapPos
                        };

                        this.entities.Add(goal);
                        this.goal = goal;

                        break;

                    default:
                        continue;
                }
            }
        }
    }

    public override void LoadContent()
    {
        int[,] data = window.Content.Load<int[,]>("Maps/Intro");
        this.SwitchMap(new TileMap(data, window));
    }

    public override void Update(GameTime time)
    {
        this.entities.Update(window.GraphicsDevice, time);
    }

    public override void Draw(GameTime time, SpriteBatch batch)
    {
        batch.Begin();
            this.entities.Draw(batch, time);
            this.activeMap.Draw(batch);
        batch.End();
    }
}
