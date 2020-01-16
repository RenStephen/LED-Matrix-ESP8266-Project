#include <Adafruit_NeoMatrix.h>

// Matrix dimensions
const unsigned int HEIGHT = 8;
const unsigned int WIDTH  = 8;
const unsigned int SIZE   = 64;

#define PIN D3

// inefficient, but unavoidable
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(1, SIZE, PIN);

void setupMatrix() {
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(40);
}

class GameOfLife {
    private:
        bool* state;
        bool* nextState;
    public:
        GameOfLife() {
            randomSeed(analogRead(0)); 
            bool initState[SIZE];
            for (int i = 0; i < SIZE; i++) {
                initState[i] = ((int)random(20)%2 == 0);
            }
        }

        void setGamestate(bool* newState) {
            this->state = newState;
        }
          
        int check(int index) {
            // if the neighbors are out of bounds
            if (index>SIZE || index <0) {
                return 0;
            }
            //this means the neighbor cell is alive
            else if (state[index]) {
                return 1;
            }
            //cell is dead
            else {
                return 0;
            }
        }

        void calculateNextState() {
            for (int i = 0 ; i < SIZE ; i++) {
                nextState[i] = lifeRules(i);
            }
            this->state = nextState;
        }

        bool lifeRules(int index){
            //  Every cell interacts with its eight neighbours, which are the cells that are horizontally, vertically, or diagonally adjacent
            /*
             * index-(WIDTH+1), index-WIDTH, index-(WIDTH-1)
             * index-1,         index,       index+1
             * index+(WIDTH-1), index+WIDTH, index+(WIDTH+1)
             */
            //gets the number of neighbors
            int neighbors=check(index-1)+check(index+1)+check(index-(WIDTH+1))+check(index-WIDTH)+check(index-(WIDTH-1))+check(index+WIDTH+1)+check(index+WIDTH)+check(index+(WIDTH-1));
            
            // if the cell is alive
            if(state[index]){
                //  Any live cell with fewer than two live neighbors dies, as if by underpopulation.
                //  Any live cell with more than three live neighbors dies, as if by overpopulation.
                if(neighbors<2 || neighbors >3){
                    return false;
                }
                //  Any live cell with two or three live neighbors lives on to the next generation.
                if(neighbors==2 || neighbors==3){
                    return true;
                }
            }
            //  Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.
            if( !state[index] && neighbors==3){
                return true;
            }
            return false;
        }
};
