# Brick-Breaker-Game
Brick Breaker Game.
This code implements a "Break Breaker" game that includes features like a ball, bricks, a paddle, and power-ups. Here's a brief breakdown of the major components:

### Key Classes and Their Functions:
1. **Ball**: 
   - Stores the ball's position (`x`, `y`) and its color.
   - `draw()` method to visually render the ball.

2. **Brick**: 
   - Stores the brick's position (`x`, `y`), its color, and hit points (`hitPoints`).
   - `draw()` method to render each brick.
   - When hit by the ball, the brick decreases in hit points and may be destroyed.

3. **Paddle**: 
   - Represents the paddle's position, width, and color.
   - `draw()` method renders the paddle, which can be controlled by the player.

4. **PowerUp**: 
   - These are special bonuses or changes like extra life, bigger paddle, or faster ball.
   - Different types are represented as different shapes (circle, square, triangle).
   - `draw()` method renders the power-ups on the screen.

5. **TopPaddle**: 
   - A subclass of `Paddle` that acts as a secondary paddle, located at the top of the screen for additional gameplay complexity in later levels.

### **Game Class**:
   - **Ball** and **Paddle**: The player controls the paddle, and the ball moves on the screen. The ball bounces off the paddle and walls.
   - **Bricks**: The game consists of bricks arranged in a grid. In higher levels, more complex brick layouts are used, such as a diamond shape in level 3.
   - **Power-ups**: When bricks are hit, power-ups may appear and provide bonuses to the player if collected.
   - **Collision Detection**: The ball bounces off bricks and the paddle, and the game detects collisions with both.
   - **Lives and Score**: Players have 3 lives, and the score increases as bricks are destroyed. If the ball falls below the paddle, a life is lost, and the ball is reset.

### Game Functionality:
- **Levels**: Three levels with increasing difficulty. Bricks are arranged in different patterns (grid, diagonal, diamond).
- **Power-Ups**: Power-ups such as extra life, bigger paddle, and faster ball are triggered when bricks are destroyed.
- **Paddle Movement**: The paddle can be moved using the mouse or keyboard. 
- **Ball Movement**: The ball starts on the paddle and moves after being launched, bouncing off walls, bricks, and paddles.

### Other Features:
- **GameDisplay**: Handles the drawing of all game objects, including the bricks, ball, paddle, power-ups, and the top paddle.
- **Keyboard/Mouse Controls**: Arrow keys to move the top paddle, and the mouse to move the bottom paddle.
- **Game Over**: When lives reach zero, the game displays a "Game Over" message and stops updating.
- **Save Score**: Saves the player's score in a text file.
