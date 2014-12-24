#include "MovePair.h"
#include <vector>

/*
	Checkerboard functionality.
	Uses English Draught rules with no kings.
	For input, uses the Black top and White bottom notation found below:
	http://www.bobnewell.net/nucleus/checkers.php?itemid=289
*/

class Board
{
public:
	Board();
	~Board();

	void BeginGame(); // Begins checkers game loop
	void Display(); // Prints board to console

	// Piece representations on the board
	enum Pieces{
		EMPTY = 0,
		WHITE = 1,
		RED = 2
	};

private:
	MovePair GetMovePairFromInput(std::string const & move); // Parses input for move
	std::vector<MovePair> GetAvailableMoves(Pieces const color, bool captureOnly); // Returns all available moves for color, or only capture moves
	int GetPositionFromCheckersMove(int const move) const; // Using Black at Top, white at bottom notation, gets the 2D array location
	bool CanPositionCapture(int const position, Pieces const color); // Checks if piece has any available captures at location
	void AITurn(); // AI makes move
	void PlayerTurn(); // Take player input for turn.
	void DisplayGameOver(); // Displays the end game result
	void CapturePiece(Pieces const color); // Reduces score count of the color, game over check

	std::vector<Pieces> m_GameBoard;
	int m_RedPieces;
	int m_WhitePieces;
	bool m_GameOver;
};