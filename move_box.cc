#include <vector>
#include <iostream>
#include <stdio.h>
#include <string.h>
using namespace std;


class Board {
public:
    int width;
	int height;
    
private:
    
	int * board;
    bool * match_mask;

    void clearMasks(){
        int n =  width * height;
        for(int i  = 0 ; i < n ; i++){
            match_mask[i] = false;
        }
        return;
    }


	bool clearMatches(){
        int last_type = 0;
        int box_type;
        int consec_count = 0;
        clearMasks();
        //find consecutive horizontal matches
        bool found_match = false;
        for(int r = 0 ; r < height ; r++){
            for(int c  = 0 ; c < width ; c++){
                int i = r*width+c;
                box_type = board[i];
                if(box_type == last_type && c > 0 && box_type > 0 ){
                    consec_count++;
                    if(consec_count==2){
                        match_mask[i - 1] = true;
                        match_mask[i - 2] = true;
                    }
                    if(consec_count>=2){
                        match_mask[i] = true;
                        found_match = true;
                    }
                }else{
                    consec_count = 0;
                }
                last_type = box_type;
            }
        }
        
        //find consecutive vertical matches
        consec_count = 0;
        for(int c = 0 ; c  < width ; c++){
            for(int r  = 0 ; r < height ; r++){
                int i = r*width+c;
                box_type = board[i];
                if(box_type == last_type && r > 0 && box_type > 0 ){
                    consec_count++;
                    if(consec_count==2){
                        match_mask[i - width] = true;
                        match_mask[i - 2*width] = true;
                    }
                    if(consec_count>=2){
                        match_mask[i] = true;
                        found_match = true;
                    }
                }else{
                    consec_count = 0;
                }
                last_type = box_type;
                
            }
        }
        
        //clear the board whereever there matching boxes 3 in a row
        int n = width*height;
        if(found_match){
            for(int i = 0 ; i < n ; i++){
                if (match_mask[i]) {
                    board[i] = 0;
                }
            }
        }

        return found_match;
	}
    
    //make the boxes fall down
    void gravitate(){
        int f;
        for(int c = 0 ; c < width; c++){
            f = 0;
            for(int r  = 0 ; r < height ; r++){
                if(board[r*width+c]!=0){
                    board[f*width + c] = board[r*width + c];
                    f++;
                }
            }
            for(int r = f ; r < height ; r++){
                board[r*width + c] = 0 ; 
            }
        }
        return;
    }
    
public:
    
    enum colors {
        S,G,V,L,B,P,R
    }; 
   #define MAX_MOVES 4
	Board():width(7), height(5) {
        
	int temp [] = {
P,S,P,R,G,S,S,
S,S,R,L,P,S,S,
S,S,P,L,R,S,S,
S,S,L,G,P,S,S,
S,S,P,S,G,S,S
        };
         
        
       /* int temp [] = {
            0,0,1,1,0,0,0,
            0,0,0,1,0,0,0,
            0,0,0,2,0,0,0,
            0,0,0,2,0,0,0,
            0,0,0,1,0,0,0,
            0,0,0,2,0,0,0,
            0,0,0,2,0,0,0
          };  
		*/
        
        board = new int[width * height];
        memcpy(board, &temp, sizeof(int)*width*height);
        
        match_mask = new bool[width * height];
	}
    
    ~Board(){
		delete [] board;
        delete [] match_mask;
	}

    void copyBoard( Board & b){
        /*int n = width * height;
        for(int i = 0 ; i < n ; i++){
            board[i] = b.board[i];
        }*/
        memcpy(board, b.board, sizeof(int)*width*height);
    }
    void print(){
        for(int i = 0 ; i < width; i++){
            printf("-");
        }
        printf("\n");
        for(int r = height - 1; r >= 0 ; r--){
            for(int c = 0; c < width ; c++){
                printf("%d", board[r*width+c]);
            }
            printf("\n");
        }
    }
    void simulateMove(int r1, int c1, int cell){
        int i1 = r1*width + c1;
        
        int temp = board[i1];
        
        if(temp == board[cell]){
            //types are same, dont simulate
            return;
        }
        
        board[i1] = board[cell];
        board[cell] = temp;
        
        //printf("After moved:\n");
        do {
          //  printf("After cleared matches:\n");
            //print();
            gravitate();
            //printf("After gravitate matches:\n");
            //print();
        } while (clearMatches());
        //printf("Finished simulate.\n");
        
    }
    bool hasBoxes(){
        int n =  width * height;
        for(int i  = 0 ; i < n ; i++){
            if(board[i]!=0){
                return true;
            }
        }
        return false;
    }
    
    int moves_possible(int r, int c){
        if(r==0 || r == height - 1){
            if(c==0 || c== width - 1){
                return 2;
            }else{
                return 3;
            }
        }else if(c==0 || c == width - 1){
            return 3;
        }else{
            return 4;
        }
    }

    bool hasNorth(int r, int c){
        if(r!=(height - 1)){
            return board[r+1]!=0;
        }else{
            return false;
        }
    }
    int getNorth(int r, int c){
        return (r+1)*width + c;
    }
    bool hasSouth(int r){
        return r > 0;
    }
    int getSouth(int r, int c){
        return (r - 1) * width + c;
    }
    bool hasEast(int c){
        return c != (width - 1);
    }
    int getEast(int r, int c){
        return r*width + c + 1;
    }
    bool hasWest(int c){
        return c > 0;
    }
    int getWest(int r, int c){
        return r*width + c - 1;
    }
    
    bool getBoxType(int r, int c){
        return  board[r*width + c];
    }
};

struct Move{
    enum Direction {N, S, E, W} dir;
    int r;
    int c;
};



bool recurse(int depth, Board & board, vector<Move> & moves);

__inline__ bool didFindSolution(Board & b, int depth, vector<Move> & moves, Move::Direction dir, int r, int c){
    Move move;
    move.dir = dir;
    move.r = r;
    move.c = c;
    if(b.hasBoxes()){
        moves.push_back(move);
        if( recurse(depth, b, moves)){
            return true;
        }else{
            moves.pop_back();
            return false;
        }
    }else{
        moves.push_back(move);
        return true;
    }
}
int solution_count;

bool recurse(int depth, Board & board, vector<Move> & moves){
    depth++;
    if(depth > MAX_MOVES){
        return false;
    }
    Board temp;
    solution_count++;
    for(int r = 0 ; r < board.height ; r++){
        for(int c = 0 ; c < board.width ; c++){
            if(board.getBoxType(r,c)!=0){
                           
                //printf("original board North, depth %d :\n", depth);
                //board.print();
                if(board.hasNorth(r, c)){
                    temp.copyBoard(board);
                    temp.simulateMove(r,c, temp.getNorth(r,c));
                    if(didFindSolution(temp, depth, moves, Move::N, r, c)){
                        return true;
                    }
                }
                
                //printf("original board South, depth %d :\n", depth);
                //board.print();
                if(board.hasSouth(r)){
                    temp.copyBoard(board);
                    temp.simulateMove(r,c, temp.getSouth(r,c));
                    if(didFindSolution(temp, depth, moves, Move::S, r, c)){
                        return true;
                    }
                }
                
                //printf("original board East, depth %d, r: %d, c: %d\n", depth, r, c);
                //board.print();
                if(board.hasEast(c)){
                    temp.copyBoard(board);;
                    //printf("get east cell: %d\n", temp.getEast(r,c));
                    temp.simulateMove(r,c, temp.getEast(r,c));
                    if(didFindSolution(temp, depth, moves, Move::E, r, c)){
                        return true;
                    }
                }
                
                //printf("original board West, depth  %d, r: %d, c: %d\n", depth, r, c);
                //board.print();
                if(board.hasWest(c)){
                    temp.copyBoard(board);
                    //printf("get west cell: %d\n", temp.getWest(r,c));
                    temp.simulateMove(r,c, temp.getWest(r,c));
                    if(didFindSolution(temp, depth, moves, Move::W, r, c)){
                        return true;
                    }
                }
                
            }//end if box type
        }//end for c
    }//end for r
    
    return false;
}
                   
int main(int argc, char ** argv){
    solution_count = 0;
    Board b;
    vector<Move> moves;
    printf("start\n");
    
    bool found = recurse(0,b, moves);
    printf("NSEW: %d %d %d %d \n", Move::N,Move::S, Move::E, Move::W );
    printf("Moves: %d\n", moves.size());
    printf("Searched through %d solutions\n",solution_count);
    if(found){
        printf("Found solution\n");
    }else{
        printf("did not find solution\n");
    }
    for(int i = 0 ; i < moves.size() ; i++){
        printf("r: %d, c: %d, d: %d \n", moves[i].r, moves[i].c, moves[i].dir);
    }
    
}
