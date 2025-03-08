using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using MonoGayme.Core.Input;
using MonoGayme.Core.States;
using MonoGayme.Core.Utilities;
using MonoGayme.Extensions;
using Sokoboom.Input;
using Sokoboom.States;

namespace Sokoboom;

public class Sokoban : Game
{
    private GraphicsDeviceManager graphics;
    private SpriteBatch spriteBatch = null!;

    public Renderer Renderer { get; private set; } = null!;
    public StateContext Context { get; private set; }

    public Keybinds Keybinds { get; private set; }

    public Vector2 GameSize { get; private set; }

    public readonly int CellSize = 8;
    public readonly Vector2 MapSize = new Vector2(9, 9);

    public readonly float ExtraWidth = 85;

    public int ActiveMap { get; set; } = 0;
    public IReadOnlyList<MapData> Data { get; private set; }

    public Sokoban()
    {
        this.graphics = new GraphicsDeviceManager(this);
        this.Content.RootDirectory = "Content";

        this.IsMouseVisible = true;
        this.Window.AllowUserResizing = true;

        this.Context = new StateContext();

        this.GameSize = new Vector2(
            this.CellSize * this.MapSize.X + this.ExtraWidth,
            this.CellSize * this.MapSize.Y
        );

        this.graphics.SetWindowSize(this.GameSize * 7);

        this.Keybinds = new Keybinds();
    }

    protected override void LoadContent()
    {
        this.Data = [
            new MapData(this.Content.Load<int[,]>("Maps/Intro"), "intro")
        ];

        this.Renderer = new Renderer(
            this.GameSize,
            this.GraphicsDevice
        );

        this.Context.SwitchState(new Playing(this, this.Data[this.ActiveMap]));

        this.spriteBatch = new SpriteBatch(this.GraphicsDevice);
    }

    protected override void Update(GameTime gameTime)
    {
        // This must be ran every frame.
        InputHelper.GetState();

        this.Context.Update(gameTime);

        base.Update(gameTime);
    }

    protected override void Draw(GameTime gameTime)
    {
        // Outside renderer bars.
        this.GraphicsDevice.Clear(Color.Black);
        
        this.Renderer.Attach();

        this.GraphicsDevice.Clear(Color.SkyBlue);
        this.Context.Draw(gameTime, this.spriteBatch);

        this.Renderer.DetachAndDraw(this.spriteBatch);
        base.Draw(gameTime);
    }
}
