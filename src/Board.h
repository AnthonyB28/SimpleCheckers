#include "MovePair.h"
#include <vector>

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
	void AITurn(); // AI makes move
	void PlayerTurn(); // Take player input for turn.
	void DisplayGameOver(); // Displays the end game result
	MovePair GetMovePairFromInput(std::string const & move);
	int GetPositionFromMove(int const move) const;
	void CapturePiece(Pieces const color);
	bool CanAttack(int const position, Pieces const color);
	std::vector<MovePair> GetAvailableMoves(Pieces const color, bool captureOnly);

	std::vector<Pieces> m_GameBoard;
	int m_RedPieces;
	int m_WhitePieces;
	bool m_GameOver;
};