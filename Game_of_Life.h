

// Matrix dimensions
const unsigned int HEIGHT = 8;
const unsigned int WIDTH  = 8;
const unsigned int SIZE   = 64;

class GameOfLife {
    private:
        bool state[SIZE];
        bool nextState[SIZE];
    public:
        GameOfLife(bool[] startingState) {
            this->state = startingState;
        }

        void setGamestate(bool[] newState) {
            this->state = newState;
        }

        void calculateNextState() {

        }
        void lifeRules(uint8_t index){
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
                    state[index]=false;
                }
                //  Any live cell with two or three live neighbors lives on to the next generation.
                if(neighbors==2 || neighbors==3){
                    state[index]=true;
                }
            }
            //  Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.
            if( !state[index] && neighbors==3){
                state[index]=true;
            }
        }
};