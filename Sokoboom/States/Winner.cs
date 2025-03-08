using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using MonoGayme.Core.States;

namespace Sokoboom.States;

public class Winner(Sokoban window) : State
{
    public override void LoadContent()
    {
    }

    public override void Update(GameTime time)
    {
    }

    public override void Draw(GameTime time, SpriteBatch batch)
    {
        window.GraphicsDevice.Clear(Color.SkyBlue);
    }
}
