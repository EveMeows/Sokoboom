using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using MonoGayme.Core.Components.Sprites;
using MonoGayme.Core.Entities;

namespace Sokoboom.Entities;

public class Player(Sokoban window) : Entity
{
    #region Constants
    #endregion

    public override void LoadContent()
    {
        this.Components.Add(
            new Sprite(window.Content.Load<Texture2D>("Entities/Player"))
        );

        this.Position = new Vector2(
            window.CellSize * 4,
            window.CellSize * 4
        );
    }

    public override void Update(GameTime time)
    {
        // Grid movement
        // Left and Right
        if (window.Input.Left.IsPressed())
            this.Position.X -= window.CellSize;
        else if (window.Input.Right.IsPressed())
            this.Position.X += window.CellSize;

        // Up and Down
        if (window.Input.Up.IsPressed())
            this.Position.Y -= window.CellSize;
        else if (window.Input.Down.IsPressed())
            this.Position.Y += window.CellSize;
    }

    public override void Draw(SpriteBatch batch, GameTime time)
    {
    }
}
