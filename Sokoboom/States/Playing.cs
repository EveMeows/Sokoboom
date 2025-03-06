using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using MonoGayme.Core.Controllers;
using MonoGayme.Core.States;
using Sokoboom.Entities;
using Sokoboom.Map;

namespace Sokoboom.States;

public class Playing(Sokoban window) : State
{
    private EntityController entities = new EntityController();

    private TileMap activeMap = null!;

    public override void LoadContent()
    {
        this.entities.Add(new Player(window));

        int[,] data = window.Content.Load<int[,]>("Maps/Intro");
        this.activeMap = new TileMap(data, window);
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
