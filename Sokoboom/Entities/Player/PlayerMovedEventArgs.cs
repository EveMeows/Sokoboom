using Microsoft.Xna.Framework;
using Sokoboom.Input;

namespace Sokoboom.Entities.Player;

public class PlayerMovedEventArgs(Vector2 pos, Direction dir)
{
    public Vector2 Position { get; } = pos;
    public Direction Direction { get; } = dir;
}
