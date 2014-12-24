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
	std::vector<int> GetMovePair(std::string const & move);
	bool IsValidMove(std::vector<int> const & movePair);
	int GetPositionFromMove(int const move) const;

	std::vector<Pieces> m_GameBoard;
	int m_PlayerCapture;
	int m_RedPieces;
	int m_WhitePieces;
	bool m_GameOver;
};