using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using MonoGayme.Core.Controllers;
using MonoGayme.Core.States;
using Sokoboom.Entities;

namespace Sokoboom.States;

public class Playing(Sokoban window) : State
{
    private EntityController entities = new EntityController();

    public override void LoadContent()
    {
        this.entities.Add(new Player(window));
    }

    public override void Update(GameTime time)
    {
        this.entities.Update(window.GraphicsDevice, time);
    }

    public override void Draw(GameTime time, SpriteBatch batch)
    {
        batch.Begin();
            this.entities.Draw(batch, time);
        batch.End();
    }
}
