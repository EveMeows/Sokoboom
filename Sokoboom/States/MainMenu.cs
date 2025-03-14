using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using MonoGayme.Core.Controllers;
using MonoGayme.Core.States;
using MonoGayme.Core.UI;
using MonoGayme.Extensions;
using Sokoboom.Entities.Static;

namespace Sokoboom.States;

public class MainMenu(Sokoban window) : State
{
    #region Fields
    private readonly UIController ui = new UIController(false);
    private SpriteFont font = null!;

    private readonly EntityController boxes = new EntityController();
    private readonly int BoxCount = 25;
    #endregion

    private void BuildUI()
    { 
        string title = "Sokoboom";
        Vector2 titleDim = this.font.MeasureString(title);
        this.ui.Add(
            new Label(title, Color.White, this.font, new Vector2((int)((window.GameSize.X - titleDim.X) / 2), 2))
        );

        string play = "play";
        Vector2 playDim = this.font.MeasureString(play);
        this.ui.Add(
            new TextButton(
                this.font,
                play,
                new Vector2((int)((window.GameSize.X - playDim.X) / 2), 25),
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

        string options = "options";
        Vector2 optionsDim = this.font.MeasureString(options);
        this.ui.Add(
            new TextButton(
                this.font,
                options,
                new Vector2((int)((window.GameSize.X - optionsDim.X) / 2), 37),
                Color.White
            )
            { 
                OnClick = (self) => {
                },

                OnMouseEnter = (self) => {
                    self.Colour = Color.Gold;
                },

                OnMouseExit = (self) => {
                    self.Colour = Color.White;
                }
            }
        );

        string quit = "quit";
        Vector2 quitDim = this.font.MeasureString(quit);
        this.ui.Add(
            new TextButton(
                this.font,
                quit,
                new Vector2((int)((window.GameSize.X - quitDim.X) / 2), 49),
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

        this.ui.Add(
            new Label(window.Version, Color.White, this.font, new Vector2(1, window.GameSize.Y - 12))
        );
    }

    private void OnBoxUpdated(object? sender, EntityUpdateEventArgs args)
    {
        if (args.Entity is Box box)
        {
            float delta = (float)args.GameTime.ElapsedGameTime.TotalSeconds;
            box.Position.Y += box.Speed * delta;

            if (box.Position.Y > window.GameSize.X + 10)
            { 
                box.Position = new Vector2(Random.Shared.NextSingle(8, window.GameSize.X - 8), Random.Shared.NextSingle(-10, -30));
            }
        }
    }

    public override void LoadContent()
    {
        this.font = window.Content.Load<SpriteFont>("Fonts/PicoEight");
        this.BuildUI();
    
        for (int i = 0; i < this.BoxCount; i++)
        {
            Box box = new Box(window);
            box.Position = new Vector2(Random.Shared.NextSingle(8, window.GameSize.X - 8), Random.Shared.NextSingle(-10, -70));
            box.Speed = Random.Shared.NextSingle(50, 100);

            this.boxes.Add(box);
        }

        this.boxes.OnEntityUpdate += this.OnBoxUpdated;
    }

    public override void Update(GameTime time)
    {
        this.ui.Update(window.Renderer.GetVirtualMousePosition());
        this.boxes.Update(window.GraphicsDevice, time);
    }

    public override void Draw(GameTime time, SpriteBatch batch)
    {
        window.GraphicsDevice.Clear(Color.SkyBlue);

        batch.Begin();
            this.boxes.Draw(batch, time);
            this.ui.Draw(batch);
        batch.End();
    }
}
