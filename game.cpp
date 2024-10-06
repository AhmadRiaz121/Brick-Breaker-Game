//============================================================================
// Name        : Muhammad Ahmed Riaz
// Author      : Sibt ul Hussain
// Version     :
// Copyright   : (c) Reserved
// Description : Break Breaker Game
//============================================================================
#ifndef CENTIPEDE_CPP_
#define CENTIPEDE_CPP_

#include "util.h"
#include <iostream>
#include <string>
#include <cmath> // for basic math functions such as cos, sin, sqrt
#include <fstream>
using namespace std;

class Ball
{
public:
    int x, y;
    ColorNames color; // Store the color of the ball
    Ball() {}
    Ball(int x1, int y1, ColorNames c)
    {
        x = x1;
        y = y1;
        color = c; // Assign the color to the ball
    }
    void draw()
    {
        DrawCircle(x, y, 10, colors[color]);
    }
};

class Brick
{
public:
    int x, y;
    ColorNames color; // Store the color of the brick
    int hitPoints;
    Brick() {}
    Brick(int x1, int y1, ColorNames c, int hp)
    {
        x = x1;
        y = y1;
        color = c; // Assign the color to the brick
        hitPoints = hp;
    }
    void draw()
    {
        DrawRoundRect(x, y, 50, 20, colors[color], 10);
    }
};

class Paddle
{
public:
    int x, y;
    int width;        // Store the width of the paddle
    ColorNames color; // Store the color of the paddle
    Paddle() {}
    Paddle(int x1, int y1, int w, ColorNames c)
    {
        x = x1;
        y = y1;
        width = w;
        color = c; // Assign the color to the paddle
    }
    void draw()
    {
        DrawRoundRect(x, y, width, 20, colors[color], 10);
    }
};

class PowerUp
{
public:
    int x, y;
    int type;  // 0: extra life, 1: bigger paddle, 2: faster ball
    int shape; // 0: circle, 1: square, 2: triangle, etc.
    int timer; // Timer to reset the power-up ability
    PowerUp() {}
    PowerUp(int x1, int y1, int t, int s)
    {
        x = x1;
        y = y1;
        type = t;
        shape = s;
        timer = 0; // Initialize the timer
    }
    void draw()
    {
        switch (shape)
        {
        case 0: // Circle
            DrawCircle(x, y, 20, colors[YELLOW]);
            break;
        case 1: // Square
            DrawSquare(x, y, 20, colors[BLUE]);
            break;
        case 2: // Triangle
            DrawTriangle(x, y, x + 20, y, x + 20, y + 20, colors[GREEN]);
            break;
        default:
            DrawCircle(x, y, 20, colors[YELLOW]); // Default to circle
            break;
        }
    }
};

class TopPaddle : public Paddle
{
public:
    TopPaddle() {}
    TopPaddle(int x1, int y1, int w, ColorNames c) : Paddle(x1, y1, w, c) {}
};
class Game
{
public:
    const int NUM_ROWS = 5;      // Number of rows of bricks
    const int NUM_COLS = 10;     // Number of columns of bricks
    const int BRICK_WIDTH = 80;  // Width of each brick
    const int BRICK_HEIGHT = 30; // Height of each brick
    const int BRICK_GAP = 5;     // Gap between bricks
    Paddle paddle;
    Ball ball;
    bool ballLaunched = false;
    Brick *bricks[5][10];  // Array to track the existence of bricks
    PowerUp *powerUps[10]; // Array to track the existence of power-ups
    TopPaddle topPaddle;
    int dx;
    int dy;
    int score;
    int lives;
    int level;
    Game()
    {
        ball = Ball(420, 420, RED);
        paddle = Paddle(400, 100, 100, RED);
        dx = 4;
        dy = 4;
        score = 0;
        lives = 3;
        level = 1;
        InitializeBricks();
        InitializePowerUps();
    }
    // Recursive Function to create the diamond shape for level 3
    void FillDiamond(int i, int j, int numRows, int numCols, ColorNames colorsArray[], int level)
    {
        if (i >= numRows || j >= numCols || i < 0 || j < 0)
        {
            return;
        }

        int centerX = numCols / 2;
        int centerY = numRows / 2;
        int deltaX = i - centerY;
        int deltaY = j - centerX;
        int distance = deltaX + deltaY;
        if (deltaX < 0)
        {
            deltaX = -deltaX;
        }
        if (deltaY < 0)
        {
            deltaY = -deltaY;
        }
        distance = deltaX + deltaY;

        if (distance <= level)
        {
            bricks[i][j] = new Brick(j * (BRICK_WIDTH + BRICK_GAP), 600 - (i * (BRICK_HEIGHT + BRICK_GAP)), colorsArray[(i + j) % 4], ((i + j) % 4) + 1);
        }
        else
        {
            bricks[i][j] = nullptr;
        }

        // Move to the next cell
        if (j + 1 < numCols)
        {
            FillDiamond(i, j + 1, numRows, numCols, colorsArray, level);
        }
        else if (i + 1 < numRows)
        {
            FillDiamond(i + 1, 0, numRows, numCols, colorsArray, level);
        }
    }

    void InitializeBricks()
    {
        ColorNames colorsArray[] = {GREEN, BLUE, PINK, RED};
        switch (level)
        {
        case 1:
            // Initialize all bricks as existing
            for (int i = 0; i < NUM_ROWS; i++)
            {
                for (int j = 0; j < NUM_COLS; j++)
                {
                    bricks[i][j] = new Brick(j * (BRICK_WIDTH + BRICK_GAP), 600 - (i * (BRICK_HEIGHT + BRICK_GAP)), colorsArray[(i + j) % 4], ((i + j) % 4) + 1);
                }
            }
            break;
        case 2:
            for (int i = 0; i < NUM_ROWS; i++)
            {
                for (int j = 0; j < NUM_COLS; j++)
                {
                    if (i == j || j == NUM_COLS - 1 - i)
                        bricks[i][j] = new Brick(j * (BRICK_WIDTH + BRICK_GAP), 600 - (i * (BRICK_HEIGHT + BRICK_GAP)), colorsArray[(i + j) % 4], ((i + j) % 4) + 1);
                }
            }
            break;
        case 3:
            topPaddle = TopPaddle(400, 700, 100, RED);
            // Fill the diamond shape
            FillDiamond(0, 0, NUM_ROWS, NUM_COLS, colorsArray, NUM_ROWS / 2);
            break;
        }
    }

    void InitializePowerUps()
    {
        for (int i = 0; i < 10; i++)
        {
            powerUps[i] = nullptr;
        }
    }

    void DrawBricks()
    {
        ColorNames colorsArray[] = {GREEN, BLUE, PINK, RED}; // Array of colors
        int colorIndex = 0;                                  // Index to iterate over colorsArray

        for (int i = 0; i < NUM_ROWS; i++)
        {
            for (int j = 0; j < NUM_COLS; j++)
            {
                if (bricks[i][j])
                {                         // Only draw bricks that exist
                    bricks[i][j]->draw(); // Draw the brick using its draw method
                }
            }
        }
    }

    void DrawPowerUps()
    {
        for (int i = 0; i < 10; i++)
        {
            if (powerUps[i])
            {
                powerUps[i]->draw();
            }
        }
    }

    void hit()
    {
        for (int i = 0; i < NUM_ROWS; i++)
        {
            for (int j = 0; j < NUM_COLS; j++)
            {
                if (bricks[i][j])
                {
                    int brickX = j * (BRICK_WIDTH + BRICK_GAP);
                    int brickY = 600 - (i * (BRICK_HEIGHT + BRICK_GAP));

                    if (ball.x + 10 >= brickX && ball.x - 10 <= brickX + BRICK_WIDTH && ball.y + 10 >= brickY && ball.y - 10 <= brickY + BRICK_HEIGHT)
                    {
                        dy = -(dy);
                        bricks[i][j]->hitPoints--;        // Decrement hitPoints of the brick
                        ball.color = bricks[i][j]->color; // Change ball color to brick color
                        if (bricks[i][j]->hitPoints == 0)
                        {
                            delete bricks[i][j]; // Delete the brick object
                            bricks[i][j] = nullptr;
                            score += 5;
                            // Create a power-up at the brick's position
                            int type = rand() % 3; // Randomly select a power-up type
                            int shape = rand() % 3;
                            powerUps[rand() % 10] = new PowerUp(brickX + 25, brickY + 15, type, shape);
                        }
                        bool allBricksDestroyed = true;
                        for (int i = 0; i < NUM_ROWS; i++)
                        {
                            for (int j = 0; j < NUM_COLS; j++)
                            {
                                if (bricks[i][j] != nullptr)
                                {
                                    allBricksDestroyed = false;
                                    break;
                                }
                            }
                        }
                        if (allBricksDestroyed)
                        {
                            level++;
                            InitializeBricks();
                            ball.x = 420;
                            ball.y = 420;
                            paddle.width = 100;
                            dx = 4;
                            dy = 4;
                            ballLaunched = false;
                            lives = 3;
                        }
                        return;
                    }
                }
            }
        }
    }

    void DrawPaddle()
    {
        paddle.draw();
    }

    void DrawBall()
    {
        ball.draw();
    }

    void UpdatePowerUps()
    {
        for (int i = 0; i < 10; i++)
        {
            if (powerUps[i])
            {
                powerUps[i]->y -= 2; // Move the power-up down
                if (powerUps[i]->y > 840)
                {
                    delete powerUps[i]; // Delete the power-up object
                    powerUps[i] = nullptr;
                }
                else if (powerUps[i]->y + 20 >= paddle.y && powerUps[i]->y <= paddle.y + 20 && powerUps[i]->x >= paddle.x && powerUps[i]->x <= paddle.x + paddle.width)
                {
                    // Power-up is collected by the paddle
                    switch (powerUps[i]->type)
                    {
                    case 0:
                        paddle.width -= 20; // Smaller Paddle
                        if (paddle.width < 50)
                        {
                            paddle.width = 50;
                        }
                        break;
                    case 1:
                        paddle.width += 20; // Bigger paddle
                        break;
                    case 2:
                        dx += 2; // Faster ball
                        break;
                    }
                    paddle.color = ball.color; // Change paddle color to ball color
                    delete powerUps[i];        // Delete the power-up object
                    powerUps[i] = nullptr;
                }
            }
        }
    }
    void saveScore()
    {
        ofstream file("score.txt");
        if (file.is_open())
        {
            file << "Score: " << score << endl;
        }
        else
        {
            cout << "Unable to open file";
        }
    }
};

Game game; // One global instance of Game Class

void SetCanvasSize(int width, int height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GameDisplay()
{
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    // Draw objects
    game.DrawBricks();
    game.DrawPowerUps();
    game.DrawPaddle();
    game.DrawBall();
    game.topPaddle.draw();

    // Display Score
    DrawString(50, 800, "Score=" + to_string(game.score), colors[MISTY_ROSE]);
    DrawString(700, 800, "Lives=" + to_string(game.lives), colors[MISTY_ROSE]);
    DrawString(350, 800, "Level=" + to_string(game.level), colors[MISTY_ROSE]);
    if (game.lives == 0)
    {
        DrawString(300, 280, "Game Over", colors[WHITE]);
        DrawString(300, 200, "Score=" + to_string(game.score), colors[WHITE]);
    }
    glutSwapBuffers();
}

void NonPrintableKeys(int key, int x, int y)
{
    if (key == GLUT_KEY_UP)
    {
        game.ballLaunched = true;
    }
    else if (key == GLUT_KEY_RIGHT)
    {
        // Move paddle right
        game.topPaddle.x += 10;
    }
    else if (key == GLUT_KEY_LEFT)
    {
        // Move paddle left
        game.topPaddle.x -= 10;
    }

    glutPostRedisplay();
}

void PrintableKeys(unsigned char key, int x, int y)
{
    if (key == 27) // ESC key
    {
        exit(1);
    }
    if (key == 'b' || key == 'B') // Key for placing the bomb
    {
        cout << "b pressed" << endl;
    }
    glutPostRedisplay();
}

void Timer(int m)
{
    if (!(game.ballLaunched))
    {
        // Keep the ball on the paddle until the up arrow key is pressed
        game.ball.x = game.paddle.x + 50; // Center the ball on the paddle
        game.ball.y = game.paddle.y + 33; // Position the ball just above the paddle
    }
    else
    {
        // Update ball's position
        game.ball.x += game.dx; // Move the ball horizontally
        game.ball.y += game.dy; // Move the ball vertically (you can adjust the speed by changing the value)

        // Check for collision with walls
        if (game.ball.x + 10 >= 840 || game.ball.x - 10 <= 0)
        {
            // If the ball hits the right or left wall, reverse its horizontal direction
            game.dx = -(game.dx);
        }

        // Check for collision with the top wall
        if (game.ball.y + 10 >= 840)
        {
            // If the ball hits the top wall, reverse its vertical direction
            game.dy = -(game.dy);
        }

        // Check for collision with the bottom wall (you can adjust this condition if needed)
        if (game.ball.y - 10 <= 0)
        {
            // If the ball hits the bottom wall, reset its position (for example)
            game.ball.x = 420;         // Reset the ball's x-coordinate to the center
            game.ball.y = 420;         // Reset the ball's y-coordinate to the center
            game.dx = 4;               // Reset the ball's horizontal speed
            game.dy = 4;               // Reset the ball's vertical speed
            game.paddle.width = 100;   // Reset the paddle's width
            game.ballLaunched = false; // Reset the launch flag
            game.lives--;
        }

        if (game.ball.y <= game.paddle.y + 20 && game.ball.x >= game.paddle.x && game.ball.x <= game.paddle.x + game.paddle.width)
        {
            // If the ball hits the paddle, reverse its vertical direction
            // You can add additional logic here, like changing the horizontal direction based on where the ball hits the paddle
            game.dy = -(game.dy);
            game.paddle.color = game.ball.color; // Change paddle color to ball color
        }
        if (game.ball.y - 10 <= game.topPaddle.y + 20 && game.ball.y + 10 >= game.topPaddle.y && game.ball.x - 10 >= game.topPaddle.x && game.ball.x + 10 <= game.topPaddle.x + game.topPaddle.width)
        {
            game.dy = -(game.dy);
            game.topPaddle.color = game.ball.color;
        }

        game.hit();
        game.UpdatePowerUps();
    }
    if (game.lives == 0)
    {
        // Stop the game loop
        glutIdleFunc(NULL);
        game.saveScore();
    }
    else
    {
        // Redraw the scene
        glutPostRedisplay();
        // Set up the next call to MoveBall after a delay
        glutTimerFunc(10, Timer, 0);
    }
}

void MousePressedAndMoved(int x, int y)
{
    // Update the paddle's x-coordinate to follow the mouse
    game.paddle.x = x - 50; // Adjust the paddle's position to center it under the mouse

    // Ensure the paddle stays within the window boundaries
    if (game.paddle.x < 0)
    {
        game.paddle.x = 0;
    }
    else if (game.paddle.x + game.paddle.width > 1020)
    { // Assuming the window width is 1020
        game.paddle.x = 1020 - game.paddle.width;
    }

    glutPostRedisplay();
}

void MouseMoved(int x, int y)
{
    // Update the paddle's x-coordinate to follow the mouse
    game.paddle.x = x - 50; // Adjust the paddle's position to center it under the mouse

    // Ensure the paddle stays within the window boundaries
    if (game.paddle.x < 0)
    {
        game.paddle.x = 0;
    }
    else if (game.paddle.x + game.paddle.width > 1020)
    { // Assuming the window width is 1020
        game.paddle.x = 1020 - game.paddle.width;
    }

    glutPostRedisplay();
}

void MouseClicked(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        cout << GLUT_DOWN << " " << GLUT_UP << endl;
    }
    else if (button == GLUT_RIGHT_BUTTON)
    {
        cout << "Right Button Pressed" << endl;
    }
    glutPostRedisplay();
}

int main(int argc, char *argv[])
{
    int width = 840, height = 840;
    InitRandomizer();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(width, height);
    glutCreateWindow("23I-2005");
    SetCanvasSize(width, height);

    glutDisplayFunc(GameDisplay);
    glutSpecialFunc(NonPrintableKeys);
    glutKeyboardFunc(PrintableKeys);
    glutTimerFunc(10, Timer, 0);
    glutMouseFunc(MouseClicked);
    glutPassiveMotionFunc(MouseMoved);
    glutMotionFunc(MousePressedAndMoved);

    glutMainLoop();
    return 1;
}

#endif /* CENTIPEDE_CPP_ */
