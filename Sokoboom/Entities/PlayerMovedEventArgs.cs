using Microsoft.Xna.Framework;
using Sokoboom.Input;

namespace Sokoboom.Entities;

public class PlayerMovedEventArgs(Vector2 pos, Direction dir)
{
    public Vector2 Position { get; } = pos;
    public Direction Direction { get; } = dir;
}
