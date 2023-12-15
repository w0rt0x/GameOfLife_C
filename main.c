#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>


bool coin_toss(float bias){
    /**
     * Performs a coin toss with a bias
     * @param bias float that alters 50/50 chance
     * @return bool
    */
   int num = rand() % 100 + 1;
   bool coin;

   // Applying bias
   if (num >= (bias * 100)){
        coin = false; 
   } else {
        coin = true;
   }

   return coin;
}


int** create_field(int x, int y){
    /**
     * Creates Field of dim x and dim y by creating a 2D Array
     * @param x integer that represents x-dimension (rows)
     * @param y integer that represents y-dimension (columns)
     * @return Pointer to 2D int Array
     */

    // creating field
    int** field = malloc(sizeof(int*) * x);
    for (int i=0; i < x; ++i){
        field[i] = malloc(sizeof(int) * y);
    }
    // return pointer
    return field;
}


void fill_field(int x, int y, int** field, float chance){
    /**
     * fills 2D array field
     * @param x integer that represents x-dimension (rows)
     * @param y integer that represents y-dimension (columns)
     * @param field int** pointer
     * @param chance float that determines chance of making a cell alive
     * @return Pointer to 2D Array
     */
    for (int row=0; row<x; ++row){
        for (int col=0; col<y; ++col){
            if (coin_toss(chance)){
                field[row][col] = 1; // alive cell
            }else{
                field[row][col] = 0; // dead cell
            }
        }
    }
}


void free_mem(int x, int** ptr){
    /**
     * Frees memory after game is done
     * @param x integer that represents number of rows
     * @param ptr of game field (2D int array)
     * @return void
    */
   for (int i=0; i<x; ++i){
        free(ptr[i]);
   }
   free(ptr);
}


void print_field(int x, int y, int** field){
    /**
     * Flushes console, prints out field of current iteration
     * @param x integer that represents x-dimension (rows)
     * @param y integer that represents y-dimension (columns)
     * @param field pointer of game field (2D int array)
     * @return void
     */

    // clear console
    printf("\e[1;1H\e[2J");

    // print out game field
    for (int row=0; row<x; ++row){
        for (int col=0; col<y; ++col){
            if (field[row][col] == 0){
                printf("%c", ' ');
            }else{
                printf("%c", '#');
            }
        }
        printf("\n");
    }
    fflush(stdout);
}


struct position{
    /*
    * Position structure
    * shows position (row and col) in field
    */
    int row;
    int col;
};
typedef struct position Pos;


int get_num_neighbors(int x, int y, int** field, Pos p){
    /**
     * counts number of neighbor cells that are alive
     * @param x integer that represents x-dimension (rows)
     * @param y integer that represents y-dimension (columns)
     * @param field pointer of game field (2D int array)
     * @param p POS struct that holds row and col coordinate of current cell
     * @return int 
    */
    // Edge-Case: Corner cells
    if (p.row==0 && p.col==0){ // top left
        return field[0][1] + 
               field[1][0] + 
               field[1][1];
    }
    if (p.row==0 && p.col==(y-1)){ // top right
        return field[0][y-2] + 
               field[1][y-2] + 
               field[1][y-1];
    }
    if (p.row==(x-1) && p.col==0){ // bottom left
        return field[x-2][0] + 
               field[x-1][1] + 
               field[x-2][y-2];
    }
    if (p.row==(x-1) && p.col==y-1){ // bottom right
        return field[x-2][y-2] + 
               field[x-2][y-1] + 
               field[x-1][y-2];
    }

    // Edge-Case: Borders
    if (p.row==0){ // Top Border
        return field[0][p.col-1] + 
               field[0][p.col+1] + 
               field[1][p.col-1] +
               field[1][p.col] +
               field[1][p.col+1];
    }
    if (p.row==x-1){ // Bottom Border
        return field[x-2][p.col-1] + 
               field[x-2][p.col] + 
               field[x-2][p.col+1] +
               field[x-1][p.col-1] +
               field[x-1][p.col+1];
    }
    if (p.col==0){ // left border
        return field[p.row-1][0] + 
               field[p.row+1][0] + 
               field[p.row-1][1] +
               field[p.row][1] +
               field[p.row+1][1];
    }
    if (p.col==y-1){ // right border
        return field[p.row-1][y-1] + 
               field[p.row+1][y-1] + 
               field[p.row-1][y-2] +
               field[p.row][y-2] +
               field[p.row+1][y-2];
    }

    // Base case
    return field[p.row-1][p.col-1] +
           field[p.row-1][p.col] +
           field[p.row-1][p.col+1] +
           field[p.row][p.col-1] +
           field[p.row][p.col+1] +
           field[p.row+1][p.col-1] +
           field[p.row+1][p.col] + 
           field[p.row+1][p.col+1]; 
}


int** update(int x, int y, int** field){
    /**field[p.row-1][p.col-1]
    * Updates the field for one time step
    * @param x integer that represents x-dimension (rows)
    * @param y integer that represents y-dimension (columns)
    * @param field int** pointer
    * @return pointer to new field
    */
   // create new field and update it based on the old one
    int** new_field = create_field(x, y);
    
    for (int row=0; row<x; ++row){
        for (int col=0; col<y; ++col){
            Pos p = {row, col};
            int n = get_num_neighbors(x, y, field, p);

            switch (n)
            {
            case 1: // less then 2 neighbors => Death
                new_field[row][col] = 0;
                break;
            case 2: // 2 neighbors => stay dead or alive
                new_field[row][col] = field[row][col]; 
                break;
            case 3: // 3 neighbors => make cell alive
                new_field[row][col] = 1;
                break;
            default: // more than 3 neighbors => death
                new_field[row][col] = 0;
                break;
            }
        }
    }

    // free memory of old field
    free_mem(x, field);
    
    return new_field;
}

int main(){
    int x = 50;
    int y = 100;
    int** field = create_field(x, y);
    float chance = 0.2;
    fill_field(x, y, field, chance);
    print_field(x, y, field);
    for (int i=0; i< 101; ++i){
        field = update(x, y, field);
        print_field(x, y, field);
        int t = sleep(1);
    }

    free_mem(x, field);
    return 0;
}