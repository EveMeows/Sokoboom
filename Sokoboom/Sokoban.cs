using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using MonoGayme.Core.States;
using MonoGayme.Core.Utilities;
using MonoGayme.Extensions;

namespace Sokoboom;

public class Sokoban : Game
{
    private GraphicsDeviceManager graphics;
    private SpriteBatch spriteBatch = null!;

    public Renderer Renderer { get; private set; } = null!;
    public StateContext Context { get; private set; }

    public Vector2 GameSize { get; private set; }

    public readonly int CellSize = 8;
    public readonly Vector2 MapSize = new Vector2(8, 8);
        
    public Sokoban()
    {
        this.graphics = new GraphicsDeviceManager(this);
        this.Content.RootDirectory = "Content";

        this.IsMouseVisible = true;
        this.Window.AllowUserResizing = true;

        this.Context = new StateContext();

        this.GameSize = new Vector2(
            this.CellSize * this.MapSize.X, 
            this.CellSize * this.MapSize.Y
        );

        this.graphics.SetWindowSize(this.GameSize * 7);
    }

    protected override void LoadContent()
    {
        this.Renderer = new Renderer(
            this.GameSize,
            this.GraphicsDevice
        );

        this.spriteBatch = new SpriteBatch(this.GraphicsDevice);
    }

    protected override void Update(GameTime gameTime)
    {
        this.Context.Update(gameTime);

        base.Update(gameTime);
    }

    protected override void Draw(GameTime gameTime)
    {
        // Outside renderer bars.
        this.GraphicsDevice.Clear(Color.Black);
        
        this.Renderer.Attach();
        
        this.GraphicsDevice.Clear(Color.SkyBlue);
        this.spriteBatch.Begin();
            this.Context.Draw(gameTime, this.spriteBatch);
        this.spriteBatch.End();

        this.Renderer.DetachAndDraw(this.spriteBatch);
        base.Draw(gameTime);
    }
}
