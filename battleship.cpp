#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

const int GRID_SIZE = 10;

// Ship structure
struct Ship {
    string name;
    int size;
    char symbol;
    int hits;
    bool sunk;
    
    Ship(string n, int s, char sym) : name(n), size(s), symbol(sym), hits(0), sunk(false) {}
};

// Board class
class Board {
private:
    char oceanGrid[GRID_SIZE][GRID_SIZE];
    char targetGrid[GRID_SIZE][GRID_SIZE];
    vector<Ship> ships;
    
public:
    Board() {
        for(int i = 0; i < GRID_SIZE; i++) {
            for(int j = 0; j < GRID_SIZE; j++) {
                oceanGrid[i][j] = '.';
                targetGrid[i][j] = '.';
            }
        }
        initializeShips();
    }
    
    void initializeShips() {
        ships.push_back(Ship("Carrier", 5, 'C'));
        ships.push_back(Ship("Battleship", 4, 'B'));
        ships.push_back(Ship("Cruiser", 3, 'R'));
        ships.push_back(Ship("Submarine", 3, 'S'));
        ships.push_back(Ship("Destroyer", 2, 'D'));
    }
    
    void displayOceanGrid() {
        cout << "\n   ";
        for(int i = 1; i <= GRID_SIZE; i++) {
            if(i < 10) cout << i << " ";
            else cout << i;
        }
        cout << "\n";
        
        for(int i = 0; i < GRID_SIZE; i++) {
            cout << char('A' + i) << "  ";
            for(int j = 0; j < GRID_SIZE; j++) {
                cout << oceanGrid[i][j] << " ";
            }
            cout << "\n";
        }
    }
    
    void displayTargetGrid() {
        cout << "\n   ";
        for(int i = 1; i <= GRID_SIZE; i++) {
            if(i < 10) cout << i << " ";
            else cout << i;
        }
        cout << "\n";
        
        for(int i = 0; i < GRID_SIZE; i++) {
            cout << char('A' + i) << "  ";
            for(int j = 0; j < GRID_SIZE; j++) {
                cout << targetGrid[i][j] << " ";
            }
            cout << "\n";
        }
    }
    
    bool isValidPlacement(Ship& ship, int row, int col, char direction) {
        if(row < 0 || col < 0 || row >= GRID_SIZE || col >= GRID_SIZE) {
            return false;
        }
        
        if(direction == 'H' || direction == 'h') {
            if(col + ship.size > GRID_SIZE) return false;
            
            for(int i = 0; i < ship.size; i++) {
                if(oceanGrid[row][col + i] != '.') 
                    return false;
            }
        } else if(direction == 'V' || direction == 'v') {
            if(row + ship.size > GRID_SIZE) return false;
            
            for(int i = 0; i < ship.size; i++) {
                if(oceanGrid[row + i][col] != '.') 
                    return false;
            }
        } else {
            return false;
        }
        return true;
    }
    
    bool placeShip(Ship& ship, int row, int col, char direction) {
        if(!isValidPlacement(ship, row, col, direction)) {
            return false;
        }
        
        if(direction == 'H' || direction == 'h') {
            for(int i = 0; i < ship.size; i++) {
                oceanGrid[row][col + i] = ship.symbol;
            }
        } else {
            for(int i = 0; i < ship.size; i++) {
                oceanGrid[row + i][col] = ship.symbol;
            }
        }
        return true;
    }
    
    bool receiveAttack(int row, int col) {
        if(row < 0 || col < 0 || row >= GRID_SIZE || col >= GRID_SIZE) {
            return false;
        }
        
        if(oceanGrid[row][col] == '.' || oceanGrid[row][col] == 'O') {
            oceanGrid[row][col] = 'O';
            return false;
        } else if(oceanGrid[row][col] == 'X') {
            return false;
        } else {
            char hitShip = oceanGrid[row][col];
            oceanGrid[row][col] = 'X';
            
            for(auto& ship : ships) {
                if(ship.symbol == hitShip) {
                    ship.hits++;
                    if(ship.hits == ship.size) {
                        ship.sunk = true;
                        cout << "\n*** You sunk the " << ship.name << "! ***\n";
                    }
                    break;
                }
            }
            return true;
        }
    }
    
    bool allShipsSunk() {
        for(const auto& ship : ships) {
            if(!ship.sunk) return false;
        }
        return true;
    }
    
    bool alreadyShot(int row, int col) {
        return (oceanGrid[row][col] == 'X' || oceanGrid[row][col] == 'O');
    }
    
    vector<Ship>& getShips() {
        return ships;
    }
    
    void setTargetGrid(int row, int col, char value) {
        targetGrid[row][col] = value;
    }
};

// Game class
class Game {
private:
    Board player1Board;
    Board player2Board;
    
    void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }
    
    void setupPhase(Board& board, string playerName) {
        clearScreen();
        cout << "\n========================================\n";
        cout << "    " << playerName << "'s Setup Phase\n";
        cout << "========================================\n";
        
        auto& ships = board.getShips();
        
        for(auto& ship : ships) {
            bool placed = false;
            while(!placed) {
                board.displayOceanGrid();
                cout << "\n----------------------------------------\n";
                cout << "Place your " << ship.name << " (size " << ship.size << ")\n";
                cout << "----------------------------------------\n";
                cout << "Enter starting coordinate (e.g., A5): ";
                
                string coord;
                cin >> coord;
                
                if(coord.length() < 2) {
                    cout << "Invalid input! Try again.\n";
                    continue;
                }
                
                char rowChar = toupper(coord[0]);
                if(rowChar < 'A' || rowChar > 'J') {
                    cout << "Invalid row! Use A-J.\n";
                    continue;
                }
                
                int row = rowChar - 'A';
                int col;
                try {
                    col = stoi(coord.substr(1)) - 1;
                } catch(...) {
                    cout << "Invalid column! Try again.\n";
                    continue;
                }
                
                if(col < 0 || col >= GRID_SIZE) {
                    cout << "Invalid column! Use 1-10.\n";
                    continue;
                }
                
                cout << "Enter direction (H for horizontal, V for vertical): ";
                char direction;
                cin >> direction;
                
                if(board.placeShip(ship, row, col, direction)) {
                    placed = true;
                    clearScreen();
                    cout << "\n" << ship.name << " placed successfully!\n";
                } else {
                    cout << "\nInvalid placement! Try again.\n";
                    cout << "Press Enter to continue...";
                    cin.ignore();
                    cin.get();
                    clearScreen();
                }
            }
        }
        
        board.displayOceanGrid();
        cout << "\n" << playerName << ", your fleet is ready!\n";
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
    }
    
public:
    void playGame() {
        cout << "\n";
        cout << "================================================\n";
        cout << "          B A T T L E S H I P                  \n";
        cout << "================================================\n";
        cout << "\n";
        cout << "Press Enter to start...";
        cin.get();
        
        setupPhase(player1Board, "Player 1");
        setupPhase(player2Board, "Player 2");
        
        bool gameOver = false;
        int currentPlayer = 1;
        
        while(!gameOver) {
            clearScreen();
            cout << "\n========================================\n";
            cout << "       Player " << currentPlayer << "'s Turn\n";
            cout << "========================================\n";
            
            Board& attackingBoard = (currentPlayer == 1) ? player1Board : player2Board;
            Board& defendingBoard = (currentPlayer == 1) ? player2Board : player1Board;
            
            cout << "\nYour Target Grid (Enemy Waters):";
            attackingBoard.displayTargetGrid();
            
            bool validShot = false;
            while(!validShot) {
                cout << "\nEnter target coordinates (e.g., B7): ";
                string coord;
                cin >> coord;
                
                if(coord.length() < 2) {
                    cout << "Invalid input! Try again.\n";
                    continue;
                }
                
                char rowChar = toupper(coord[0]);
                if(rowChar < 'A' || rowChar > 'J') {
                    cout << "Invalid row! Use A-J.\n";
                    continue;
                }
                
                int row = rowChar - 'A';
                int col;
                try {
                    col = stoi(coord.substr(1)) - 1;
                } catch(...) {
                    cout << "Invalid column! Try again.\n";
                    continue;
                }
                
                if(col < 0 || col >= GRID_SIZE) {
                    cout << "Invalid column! Use 1-10.\n";
                    continue;
                }
                
                if(defendingBoard.alreadyShot(row, col)) {
                    cout << "You already shot there! Choose another target.\n";
                    continue;
                }
                
                if(defendingBoard.receiveAttack(row, col)) {
                    cout << "\n*** HIT! ***\n";
                    attackingBoard.setTargetGrid(row, col, 'X');
                } else {
                    cout << "\n*** MISS! ***\n";
                    attackingBoard.setTargetGrid(row, col, 'O');
                }
                validShot = true;
            }
            
            if(defendingBoard.allShipsSunk()) {
                clearScreen();
                cout << "\n";
                cout << "================================================\n";
                cout << "            V I C T O R Y !                     \n";
                cout << "================================================\n";
                cout << "\n";
                cout << "       *** PLAYER " << currentPlayer << " WINS! ***\n\n";
                cout << "================================================\n";
                gameOver = true;
            } else {
                cout << "\nPress Enter to end your turn...";
                cin.ignore();
                cin.get();
                currentPlayer = (currentPlayer == 1) ? 2 : 1;
            }
        }
    }
};

// Main function
int main() {
    srand(time(0));
    
    Game game;
    game.playGame();
    
    cout << "\nThanks for playing Battleship!\n";
    cout << "Press Enter to exit...";
    cin.get();
    
    return 0;
}