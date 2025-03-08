using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using MonoGayme.Core.Controllers;
using MonoGayme.Core.States;
using MonoGayme.Core.UI;
using Sokoboom.Entities.Player;
using Sokoboom.Entities.Static;
using Sokoboom.Input;
using Sokoboom.Map;

namespace Sokoboom.States;

public class Playing(Sokoban window, MapData map) : State
{
    #region Fields
    private readonly EntityController entities = new EntityController();

    private readonly UIController ui = new UIController(false);
    private readonly UIController pause = new UIController(false);

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

    private bool paused = false;
    private Rectangle screenRect;
    private Texture2D pixel;
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

            Vector2 goalGrid = this.goal.Position / window.CellSize;
            if (boxGrid == goalGrid)
            {
                // End the game.
                if (window.ActiveMap == window.Data.Count - 1)
                {
                    window.Context.SwitchState(new Winner(window));
                    return;
                }

                // Continue to the next map
                window.ActiveMap++;
                window.Context.SwitchState(new Playing(window, window.Data[window.ActiveMap]));
            }
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
                    window.Context.SwitchState(new Playing(window, window.Data[window.ActiveMap]));    
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
                "pause",
                new Vector2(this.baseX + 19, window.GameSize.Y - 22),
                Color.White
            )
            { 
                OnClick = (self) => {
                    this.paused = true;    
                },

                OnMouseEnter = (self) => {
                    self.Colour = Color.Gold;
                },

                OnMouseExit = (self) => {
                    self.Colour = Color.White;
                }
            }
        );


        // Title
        float uiWidth = window.GameSize.X - this.baseX;

        string title = $"-{map.Name}-";
        Vector2 titleDim = this.font.MeasureString(title);
        Vector2 size = this.font.MeasureString(title);
        this.ui.Add(
            new Label(title, Color.White, this.font, new Vector2((int)(this.baseX + ((uiWidth - titleDim.X) / 2)), 2))    
        );
    }

    private void CreatePause()
    {
        this.screenRect = new Rectangle(0, 0, (int)window.GameSize.X, (int)window.GameSize.Y);

        this.pixel = new Texture2D(window.GraphicsDevice, 1, 1);
        this.pixel.SetData([Color.Black]);

        string pause = "PAUSED";
        Vector2 pauseDim = this.font.MeasureString(pause);
        this.pause.Add(
            new Label(
                pause,
                Color.White,
                this.font,
                new Vector2((int)((window.GameSize.X - pauseDim.X) / 2), 2)
            )
        );

        string menu = "main menu";
        Vector2 menuDim = this.font.MeasureString(menu);
        this.pause.Add(
            new TextButton(
                this.font,
                menu,
                new Vector2((int)(window.GameSize.X - menuDim.X) / 2, 23),
                Color.White
            )
            { 
                OnClick = (self) => {
                    window.Context.SwitchState(new MainMenu(window));    
                },

                OnMouseEnter = (self) => {
                    self.Colour = Color.Gold;
                },

                OnMouseExit = (self) => {
                    self.Colour = Color.White;
                }

            }
        );

        string quit = "quit game";
        Vector2 quitDim = this.font.MeasureString(quit);
        this.pause.Add(
            new TextButton(
                this.font,
                quit,
                new Vector2((int)(window.GameSize.X - quitDim.X) / 2, 35),
                Color.White
            )
            { 
                OnClick = (self) => {
                    window.Exit();    
                },

                OnMouseEnter = (self) => {
                    self.Colour = Color.Gold;
                },

                OnMouseExit = (self) => {
                    self.Colour = Color.White;
                }

            }
        );

        string resume = "resume";
        Vector2 resumeDim = this.font.MeasureString(resume);
        this.pause.Add(
            new TextButton(
                this.font,
                resume,
                new Vector2((int)(window.GameSize.X - resumeDim.X) / 2, 47),
                Color.White
            )
            { 
                OnClick = (self) => {
                    this.paused = false; 
                },

                OnMouseEnter = (self) => {
                    self.Colour = Color.Gold;
                },

                OnMouseExit = (self) => {
                    self.Colour = Color.White;
                }

            }
        );

    }

    public override void LoadContent()
    {
        this.font = window.Content.Load<SpriteFont>("Fonts/PicoEight");
        this.baseX = window.MapSize.X * window.CellSize + 2;

        this.CreateMap(new TileMap(map.Data, window));

        this.CreateUI();
        this.CreatePause();
    }

    public override void Update(GameTime time)
    {
        float delta = (float)time.ElapsedGameTime.TotalMilliseconds;

        if (window.Keybinds.Pause.IsPressed())
        {
            this.paused = !this.paused;
        }

        if (this.paused)
        {
            this.pause.Update(window.Renderer.GetVirtualMousePosition());
            return;
        }

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
        window.GraphicsDevice.Clear(Color.SkyBlue);

        batch.Begin();
        { 
            this.entities.Draw(batch, time);
            this.activeMap.Draw(batch);

            batch.DrawString(this.font, $"{this.player.Moves} moves", new Vector2(this.baseX, 15), Color.White);
            batch.DrawString(this.font, $"{this.undos} undos", new Vector2(this.baseX, 26), Color.White);

            this.ui.Draw(batch);

            if (this.paused)
            {
                batch.Draw(this.pixel, this.screenRect, Color.White * 0.8f);
                this.pause.Draw(batch);
            }
        }
        batch.End();
    }
}
