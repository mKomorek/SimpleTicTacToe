#include <iostream>
#include <vector>

namespace AI
{
    std::pair<int, int> takeBestMove(std::vector<std::vector<char>> board, const int& moveIndex);
    int minMax(std::vector<std::vector<char>> board, int depth, bool isAIMove);
    int computerMove(std::vector<std::vector<char>>& board, int& depth, int& score, int& bestScore);
    int humanMove(std::vector<std::vector<char>>& board, int& depth, int& score, int& bestScore);
}

namespace game
{
    void drawInstruction();
    void drawBoard(const std::vector<std::vector<char>>& board);
    bool rowCrossed(const std::vector<std::vector<char>>& board);
    bool columnCrossed(const std::vector<std::vector<char>>& board);
    bool diagonalCrossed(const std::vector<std::vector<char>>& board);
    bool checkIsGameOver(const std::vector<std::vector<char>>& board);
    void run(bool isComputerTurn);
    void checkInput(int& move);
}

std::pair<int, int> AI::takeBestMove(std::vector<std::vector<char>> board, const int& moveIndex)
{
    int x=-1, y=-1;
    int score=0, bestScore=-999;
    for(int i=0; i<board.size(); i++)
    {
        for(int j=0; j<board.size(); j++)
        {
            if(board[i][j] == ' ')
            {
                board[i][j] = 'O';
                score = AI::minMax(board, moveIndex+1, false);
                board[i][j] = ' ';
                if(score > bestScore)
                {
                    bestScore = score;
                    x = i;
                    y = j;
                }
            }
        }
    }
    return std::make_pair(x, y);
}

int AI::minMax(std::vector<std::vector<char>> board, int depth, bool isAIMove)
{
    int score = 0;
    int bestScore = 0;
    if (game::checkIsGameOver(board) == true)
    {
        if (isAIMove == true)
            return -1;
        if (isAIMove == false)
            return 1;
    }
    else
    {
        if(depth < 9)
        {
            if(isAIMove == true)
                computerMove(board, depth, score, bestScore);
            else
                humanMove(board, depth, score, bestScore);
        }
        else
            return 0;
    }
}

int AI::computerMove(
    std::vector<std::vector<char>>& board, int& depth, int& score, int& bestScore)
{
    bestScore = -999;
    for(int i=0; i<board.size(); i++)
    {
        for(int j=0; j<board.size(); j++)
        {
            if(board[i][j] == ' ')
            {
                board[i][j] = 'O';
                score = minMax(board, depth+1, false);
                board[i][j] = ' ';
                if(score > bestScore)
                    bestScore = score;
            }
        }
    }
    return bestScore;
}

int AI::humanMove(
    std::vector<std::vector<char>>& board, int& depth, int& score, int& bestScore)
{
    bestScore = 999;
    for (int i=0; i<board.size(); i++)
    {
        for (int j=0; j<board.size(); j++)
        {
            if (board[i][j] == ' ')
            {
                board[i][j] = 'X';
                score = minMax(board, depth + 1, true);
                board[i][j] = ' ';
                if (score < bestScore)
                {
                    bestScore = score;
                }
            }
        }
    }
    return bestScore;
}

void game::drawInstruction()
{
    std::cout << "\n[INFO] How to play? Just pick cell number to make move";
    std::cout << "and win against AI!" << std::endl;
    std::cout << "\n---------------" << std::endl;
    std::cout << " | 1 | 2 | 3 | " << std::endl;
    std::cout << "---------------" << std::endl;
    std::cout << " | 4 | 5 | 6 | " << std::endl;
    std::cout << "---------------" << std::endl;
    std::cout << " | 7 | 8 | 9 | " << std::endl;
    std::cout << "---------------" << std::endl;
}

void game::drawBoard(const std::vector<std::vector<char>>& board)
{
    std::cout << "---------------" << std::endl;
    for(const auto& row : board)
    {
        for(const auto& elem : row)
        {
            std::cout << " | " << elem;
        }
        std::cout << " |";
        std::cout << std::endl << "---------------" << std::endl;
    }
}

bool game::rowCrossed(const std::vector<std::vector<char>>& board)
{
    for(int i=0; i<3; ++i)
    {
        if(board[i][0] == board[i][1] &&
		   board[i][1] == board[i][2] &&
		   board[i][0] != ' ')
           return true;
    }
    return false;
}

bool game::columnCrossed(const std::vector<std::vector<char>>& board)
{
    for(int i=0; i<3; ++i)
    {
        if(board[0][i] == board[1][i] &&
           board[1][i] == board[2][i] &&
           board[0][i] != ' ')
           return true;
    }
    return false;
}

bool game::diagonalCrossed(const std::vector<std::vector<char>>& board)
{
    if(board[0][0] == board[1][1] &&
       board[1][1] == board[2][2] &&
       board[0][0] != ' ')
       return(true);

    if(board[0][2] == board[1][1] &&
       board[1][1] == board[2][0] &&
       board[0][2] != ' ')
       return(true);

    return(false);
}

bool game::checkIsGameOver(const std::vector<std::vector<char>>& board)
{
    return(rowCrossed(board) || columnCrossed(board) || diagonalCrossed(board));
}

std::vector<std::vector<char>> initBoard()
{
    auto board = std::vector<std::vector<char>> {{' ', ' ', ' '},
                                                 {' ', ' ', ' '},
                                                 {' ', ' ', ' '}};
    return board;
}

void game::run(bool isComputerTurn)
{
    int moveIndex=0, x=0, y=0;
    auto board = initBoard();
    drawInstruction();

    while(checkIsGameOver(board) == false && moveIndex != 3 * 3)
    {
        if(isComputerTurn)
        {
            auto moveCoordinates = AI::takeBestMove(board, moveIndex);
            board[moveCoordinates.first][moveCoordinates.second] = 'O';
            std::cout << "[INFO] Computer made move! Your turn:\n";
            drawBoard(board);
            ++moveIndex;
            isComputerTurn = false;
        }
        else
        {
            int move;
            game::checkInput(move);
            x = move/3;
            y = move%3;
            if(move>=0 && move<9)
            {
                if(board[x][y]==' ')
                {
                    board[x][y] = 'X';
                    drawBoard(board);
                    ++moveIndex;
                    isComputerTurn = true;
                }
                else
                    std::cout << "\n[ERROR] Choose a free place! \n ";
            }
            else
                std::cout << "\n[ERROR] Invalid position! \n";
        }
    }
}

void game::checkInput(int& move)
{
    std::cout << "Make move: ";
    std::cin >> move;
    if(std::cin.fail())
    {
         std::cin.clear();
         std::cin.ignore();
        std::cout << "\n[ERROR] Invalid position" << std::endl;
    }
    --move;
}

int main()
{
    system("clear");
    char isHumanStart;
    while(isHumanStart != 'q')
    {
        std::cout << "\nDo you want to start? (y/n) : ";
        std::cin >> isHumanStart;

        if(isHumanStart == 'y')
            game::run(false);
        else if(isHumanStart == 'n')
            game::run(true);
        else
            std::cout << "\n[ERROR] Invalid choice! \n";

        std::cout << "\n[INFO] The game is over! Quit? (q/n)";
        std::cin >> isHumanStart;
    }
    return 0;
}
