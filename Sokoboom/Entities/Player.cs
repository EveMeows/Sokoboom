using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using MonoGayme.Core.Components.Sprites;
using MonoGayme.Core.Entities;
using Sokoboom.Input;
using Sokoboom.Map;

namespace Sokoboom.Entities;

public class Player(Sokoban window, TileMap map) : Entity
{
    public EventHandler<PlayerMovedEventArgs>? OnMoved;

    public int Moves = 0;

    public override void LoadContent()
    {
        this.Components.Add(
            new Sprite(window.Content.Load<Texture2D>("Entities/Player"))
        );
    }

    public override void Update(GameTime time)
    {
        // Grid movement
        // Left and Right
        int gridX = (int)Math.Floor(this.Position.X / window.CellSize);
        int gridY = (int)Math.Floor(this.Position.Y / window.CellSize);

        if (window.Keybinds.Left.IsPressed())
        {
            if (map.IDAtPosition(gridX - 1, gridY) != 1)
            { 
                this.Position.X -= window.CellSize;
                this.OnMoved?.Invoke(this, new PlayerMovedEventArgs(this.Position, Direction.Left));

                this.Moves++;
            }
        }
        else if (window.Keybinds.Right.IsPressed())
        {
            if (map.IDAtPosition(gridX + 1, gridY) != 1)
            {
                this.Position.X += window.CellSize;
                this.OnMoved?.Invoke(this, new PlayerMovedEventArgs(this.Position, Direction.Right));
            
                this.Moves++; 
            }
        }

        // Up and Down
        if (window.Keybinds.Up.IsPressed())
        {
            if (map.IDAtPosition(gridX, gridY - 1) != 1)
            {
                this.Position.Y -= window.CellSize;
                this.OnMoved?.Invoke(this, new PlayerMovedEventArgs(this.Position, Direction.Up));
            
                this.Moves++;
            }
        }
        else if (window.Keybinds.Down.IsPressed())
        {
            if (map.IDAtPosition(gridX, gridY + 1) != 1)
            {
                this.Position.Y += window.CellSize;
                this.OnMoved?.Invoke(this, new PlayerMovedEventArgs(this.Position, Direction.Down));
            
                this.Moves++;
            }
        }
    }

    public override void Draw(SpriteBatch batch, GameTime time) {}
}
