#include <vector>

class Board
{
public:
	Board();
	~Board();

	void BeginGame();
	void Display();

	enum Pieces{
		EMPTY = 0,
		WHITE = 1,
		RED = 2
	};

private:
	void AITurn();
	void PlayerTurn();
	void DisplayGameOver();

	std::vector<Pieces> m_GameBoard;
	bool m_GameOver;
};