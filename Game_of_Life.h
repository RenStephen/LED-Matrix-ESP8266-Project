class GameOfLife {
    private:
        int SIZE;
        int WIDTH;
        bool* state;
        bool* nextState;
    public:
        GameOfLife(int matrixSize, int matrixWidth) {
            this->WIDTH = matrixWidth;
            this->SIZE = matrixSize;
            state = new bool[SIZE];
        }

        void setRandomGameState() {
          for (int i = 0; i < SIZE; i++) {
              state[i] = ((int)random(4)%2 == 0);
          }
        }
        
        bool getCellState(int cell) {
            return this->state[cell];
        }
          
        int check(int index) {
            // if the neighbors are out of bounds
            if (index>SIZE || index <0) {
                return 0;
            }
            return (int)state[index];
        }

        void calculateNextState() {
            for (int i = 0 ; i < SIZE ; i++) {
                this->state[i] = lifeRules(i);
            }
        }

        int getNeighbors(int index) {
            /*
             * if index%WIDTH == 0 
             * 
             * index-WIDTH, index-(WIDTH-1)
             * index,       index+1
             * index+WIDTH, index+(WIDTH+1)
             * 
             * if index%WIDTH-1 == 0
             * 
             * index-(WIDTH+1), index-WIDTH
             *         index-1,       index
             * index+(WIDTH-1), index+WIDTH
             * 
             * base case
             * 
             * index-(WIDTH+1), index-WIDTH, index-(WIDTH-1)
             * index-1,         index,       index+1
             * index+(WIDTH-1), index+WIDTH, index+(WIDTH+1)
             */
            if (index%WIDTH == 0) {
                return check(index-WIDTH)+check(index-(WIDTH-1))+check(index+1)+check(index+WIDTH)+check(index+(WIDTH+1));
            } else if (index%(WIDTH-1) == 0) {
                return check(index-(WIDTH+1))+check(index-WIDTH)+check(index-1)+check(index+(WIDTH-1))+ check(index+WIDTH);
             } else {
                return check(index-1)+check(index+1)+check(index-(WIDTH+1))+check(index-WIDTH)+check(index-(WIDTH-1))+check(index+WIDTH+1)+check(index+WIDTH)+check(index+(WIDTH-1));
            }
        }

        bool lifeRules(int index) {
            //  Every cell interacts with its eight neighbours, which are the cells that are horizontally, vertically, or diagonally adjacent
            /*
             * index-(WIDTH+1), index-WIDTH, index-(WIDTH-1)
             * index-1,         index,       index+1
             * index+(WIDTH-1), index+WIDTH, index+(WIDTH+1)
             */
            //gets the number of neighbors
            int neighbors = getNeighbors(index);
            
            // if the cell is alive
            if (state[index]) {
                //  Any live cell with fewer than two live neighbors dies, as if by underpopulation.
                //  Any live cell with more than three live neighbors dies, as if by overpopulation.
                if (neighbors<2 || neighbors >3) {
                    return false;
                }
                //  Any live cell with two or three live neighbors lives on to the next generation.
                if (neighbors==2 || neighbors==3) {
                    return true;
                }
            }
            //  Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.
            if (!state[index] && neighbors==3) {
                return true;
            }
            return false;
        }
};
