using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using MonoGayme.Core.Components.Sprites;
using MonoGayme.Core.Entities;

namespace Sokoboom.Entities;

class Goal(Sokoban window) : Entity(-1)
{
    public override void LoadContent()
        => this.Components.Add(new Sprite(window.Content.Load<Texture2D>("Entities/Goal")));

    public override void Update(GameTime time) {}
    public override void Draw(SpriteBatch batch, GameTime time) {}
}
