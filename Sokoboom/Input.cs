using Microsoft.Xna.Framework.Input;
using MonoGayme.Core.Input;
using System.Numerics;

namespace Sokoboom;

public class Input
{
    public VirtualButton Left = new VirtualButton();
    public VirtualButton Right = new VirtualButton();
    public VirtualButton Up = new VirtualButton();
    public VirtualButton Down = new VirtualButton();

    public Input()
    {
        this.Left.AddKeyboard(Keys.A, Keys.Left);
        this.Right.AddKeyboard(Keys.D, Keys.Right);

        this.Up.AddKeyboard(Keys.W, Keys.Up);
        this.Down.AddKeyboard(Keys.S, Keys.Down);
    }
}
