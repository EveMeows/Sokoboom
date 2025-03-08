using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using MonoGayme.Core.Controllers;
using MonoGayme.Core.States;
using MonoGayme.Core.UI;
using Sokoboom.Entities;
using Sokoboom.Input;
using Sokoboom.Map;

namespace Sokoboom.States;

public class Playing(Sokoban window, MapData map) : State
{
    #region Fields
    private readonly EntityController entities = new EntityController();
    private readonly UIController ui = new UIController(false);

    private TileMap activeMap = null!;

    private Player player = null!;
    private Box box = null!;
    private Goal goal = null!;

    private SpriteFont font = null!;

    private record History(Vector2 PlayerPosition, Vector2 BoxPosition);
    private List<History> history = [];

    private float baseX;

    private readonly float Delay = 100;
    private bool isUndoing = false;
    private float time = 0;

    private int undos = 0;
    #endregion

    private void Undo()
    { 
        History? history = this.history.LastOrDefault();
        if (history is not null)
        {
            this.player.Position = history.PlayerPosition;
            this.box.Position = history.BoxPosition;

            if (history != this.history[0])
            {
                this.history.Remove(history);
                this.undos++;
            }
        }
    }

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
                    this.player.Moves--;
                    return;
                
                case Direction.Right:
                    boxGrid.X += 1;
                    if (this.activeMap.IDAtPosition(boxGrid) != 1)
                    {
                        this.box.Position = boxGrid * window.CellSize;
                        break;
                    }

                    this.player.Position.X -= window.CellSize;
                    this.player.Moves--;
                    return;

                case Direction.Up:
                    boxGrid.Y -= 1;
                    if (this.activeMap.IDAtPosition(boxGrid) != 1)
                    {
                        this.box.Position = boxGrid * window.CellSize;
                        break;
                    }

                    this.player.Position.Y += window.CellSize;
                    this.player.Moves--;
                    return;
               
                case Direction.Down:
                    boxGrid.Y += 1;
                    if (this.activeMap.IDAtPosition(boxGrid) != 1)
                    { 
                        this.box.Position = boxGrid * window.CellSize;
                        break;
                    }

                    this.player.Position.Y -= window.CellSize;
                    this.player.Moves--;
                    return;
            }

            // TODO: Checks.
        }

        this.history.Add(new History(args.Position, this.box.Position));
    }

    private void CreateMap(TileMap @new)
    {
        this.activeMap = @new;

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

        // Initial positions.
        this.history.Add(new History(this.player.Position, this.box.Position));
    }

    private void CreateUI()
    { 
        this.ui.Add(
            new TextButton(
                this.font,
                "undo",
                new Vector2(this.baseX, window.GameSize.Y - 12),
                Color.White
            )
            { 
                OnClick = (self) => {
                    this.Undo();
                },

                OnMouseEnter = (self) => {
                    self.Colour = Color.Gold;
                },

                OnMouseExit = (self) => {
                    self.Colour = Color.White;
                }
            }
        );

        this.ui.Add(
            new TextButton(
                this.font,
                "retry",
                new Vector2(this.baseX + 43, window.GameSize.Y - 12),
                Color.White
            )
            { 
                OnClick = (self) => {
                    return;
                },

                OnMouseEnter = (self) => {
                    self.Colour = Color.Gold;
                },

                OnMouseExit = (self) => {
                    self.Colour = Color.White;
                }
            }
        );

        float uiWidth = window.GameSize.X - this.baseX;

        string title = $"-{map.Name}-";
        Vector2 size = this.font.MeasureString(title);
        this.ui.Add(
            new Label(title, Color.White, this.font, new Vector2((int)(this.baseX + (size.X - (uiWidth / 2))), 2))    
        );
    }

    public override void LoadContent()
    {
        this.font = window.Content.Load<SpriteFont>("Fonts/PicoEight");
        this.baseX = window.MapSize.X * window.CellSize + 2;

        this.CreateMap(new TileMap(map.Data, window));
        this.CreateUI();
    }

    public override void Update(GameTime time)
    {
        float delta = (float)time.ElapsedGameTime.TotalMilliseconds;

        this.entities.Update(window.GraphicsDevice, time);
        this.ui.Update(window.Renderer.GetVirtualMousePosition());

        if (window.Keybinds.Undo.IsDown())
        {
            this.isUndoing = true;
        }
        else
        {
            this.time = 0;
            this.isUndoing = false;
        }

        if (this.isUndoing)
        {
            this.time += delta;
            if (this.time > this.Delay)
            {
                this.time = 0;
                this.Undo();
            }
        }
    }

    public override void Draw(GameTime time, SpriteBatch batch)
    {
        batch.Begin();
            this.entities.Draw(batch, time);
            this.activeMap.Draw(batch);

            batch.DrawString(this.font, $"{this.player.Moves} moves", new Vector2(this.baseX, 15), Color.White);
            batch.DrawString(this.font, $"{this.undos} undos", new Vector2(this.baseX, 26), Color.White);

            this.ui.Draw(batch);
        batch.End();
    }
}
