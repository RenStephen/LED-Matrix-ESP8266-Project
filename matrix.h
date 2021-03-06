#include <Adafruit_NeoMatrix.h>
#include "Game_of_Life.h"

// Matrix dimensions
const unsigned int HEIGHT = 8;
const unsigned int WIDTH = 8;
const unsigned int SIZE = HEIGHT*WIDTH;

#define PIN D3

bool rain[SIZE];
int x    = WIDTH;
int pass = 0;

// the game class
GameOfLife game = GameOfLife(SIZE, WIDTH);

// inefficient, but unavoidable
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(1, SIZE, PIN);

void setupMatrix()
{
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(40);
  matrix.setTextColor(matrix.Color(0, 255, 0));
}

void drawRain()
{
  for (int i = SIZE - 1; i >= 0; i--)
  {
    if (rain[i])
    {
      if (i + WIDTH > SIZE - 1)
      {
        rain[i] = false;
        continue;
      }
      rain[i + WIDTH] = true;
    }
    rain[i] = false;
  }

  int x = random(WIDTH);
  rain[x] = 1;
  for (int i = 0; i < SIZE; i++)
  {
    if (rain[i])
    {
      matrix.drawPixel(0, i, matrix.Color(0, 0, 255));
    }
    else
    {
      matrix.drawPixel(0, i, matrix.Color(0, 0, 0));
    }
  }
  matrix.show();
  delay(10);
}

void displayMatrix(int state)
{
  switch (state)
  {
  case 0:
    for (int i = 0; i < SIZE; i++)
    {
      matrix.drawPixel(0, i, matrix.Color(0, 0, 0));
    }
    matrix.show();
    break;
  case 1:
    game.calculateNextState();
  case 2:
    for (int i = 0; i < SIZE; i++)
    {
      if (game.getCellState(i))
      {
        matrix.drawPixel(0, i, matrix.Color(255, 255, 255));
      }
      else
      {
        matrix.drawPixel(0, i, matrix.Color(0, 0, 0));
      }
    }
    delay(100);
    matrix.show();
    break;
  case 3:
    drawRain();
    break;
  case 4:
    // this is going to be the matrix print funciton.
    matrix.fillScreen(0);
    matrix.setCursor(0, 0);
    matrix.print(F("Howdy"));
    if(--x < -36) {
      x = matrix.width();
    }
    matrix.show();
    delay(100);
    break;
  }
}
