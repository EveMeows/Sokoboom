using Microsoft.Xna.Framework.Input;
using MonoGayme.Core.Input;
using System.Numerics;

namespace Sokoboom.Input;

public class Keybinds
{
    public VirtualButton Left = new VirtualButton();
    public VirtualButton Right = new VirtualButton();
    public VirtualButton Up = new VirtualButton();
    public VirtualButton Down = new VirtualButton();

    public VirtualButton Undo = new VirtualButton();

    public VirtualButton Pause = new VirtualButton();

    public Keybinds()
    {
        this.Left.AddKeyboard(Keys.A, Keys.Left);
        this.Right.AddKeyboard(Keys.D, Keys.Right);

        this.Up.AddKeyboard(Keys.W, Keys.Up);
        this.Down.AddKeyboard(Keys.S, Keys.Down);

        this.Undo.AddKeyboard(Keys.U);

        this.Pause.AddKeyboard(Keys.P, Keys.Escape);
    }
}
