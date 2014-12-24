#define NO_CAPTURE -1

#include "Board.h"
#include <iostream>
#include <string>
#include <limits>
#include <sstream>
#include <algorithm>

Board::Board()
	: m_GameBoard()
	, m_PlayerCapture(NO_CAPTURE)
	, m_RedPieces(12)
	, m_WhitePieces(12)
	, m_GameOver(false)
{
	for (int i = 0; i < 64; ++i)
	{
		m_GameBoard.push_back(Pieces::EMPTY);
	}

	m_GameBoard[1] = Pieces::RED;
	m_GameBoard[3] = Pieces::RED;
	m_GameBoard[5] = Pieces::RED;
	m_GameBoard[7] = Pieces::RED;
	m_GameBoard[8] = Pieces::RED;
	m_GameBoard[10] = Pieces::RED;
	m_GameBoard[12] = Pieces::RED;
	m_GameBoard[14] = Pieces::RED;
	m_GameBoard[17] = Pieces::RED;
	m_GameBoard[19] = Pieces::RED;
	m_GameBoard[21] = Pieces::RED;
	m_GameBoard[23] = Pieces::RED;

	m_GameBoard[40] = Pieces::WHITE;
	m_GameBoard[42] = Pieces::WHITE;
	m_GameBoard[44] = Pieces::WHITE;
	m_GameBoard[46] = Pieces::WHITE;
	m_GameBoard[49] = Pieces::WHITE;
	m_GameBoard[51] = Pieces::WHITE;
	m_GameBoard[53] = Pieces::WHITE;
	m_GameBoard[55] = Pieces::WHITE;
	m_GameBoard[56] = Pieces::WHITE;
	m_GameBoard[58] = Pieces::WHITE;
	m_GameBoard[60] = Pieces::WHITE;
	m_GameBoard[62] = Pieces::WHITE;
}

Board::~Board()
{

}

void Board::BeginGame()
{
	Display();
	while (!m_GameOver)
	{
		PlayerTurn();
		AITurn();
	}
}

void Board::PlayerTurn()
{
	bool madeMove = false;
	// Input loop
	while (!madeMove)
	{
		std::string inputMove;
		std::cout << "Input your white move (from-to format, eg 21-17)\n";
		std::cin >> inputMove;
		MovePair movePair = GetMovePair(inputMove);

		// If input is valid, check the validity of the move and if captures are made...
		if (IsValidMove(movePair))
		{
			// TODO move generation should return a list of moves we could just compare against
			// TODO make sure the player HAS To take captures....
			m_GameBoard[movePair.from] = Pieces::EMPTY;
			if (m_PlayerCapture != NO_CAPTURE)
			{
				m_GameBoard[m_PlayerCapture] = Pieces::EMPTY;
				CapturePiece(Pieces::RED);
				m_PlayerCapture = NO_CAPTURE; // Reset flag, TODO too manual & needs improvement
			}
			m_GameBoard[movePair.to] = Pieces::WHITE;
			// TODO check to make sure player cannot make more jumps, then ask to make more jumps if so
			madeMove = true;
			std::cout << "Result of your move: \n";
			Display();
		}
	}
}

void Board::AITurn()
{
	if (!m_GameOver)
	{
		std::vector<MovePair> movesToMake = GetAvailableMoves(Pieces::RED, false);
		int randMove = rand() % movesToMake.size();
		MovePair moveToMake = movesToMake.at(randMove);
		m_GameBoard[moveToMake.from] = Pieces::EMPTY;
		if (moveToMake.capture != NO_CAPTURE)
		{
			m_GameBoard[moveToMake.capture] = Pieces::EMPTY;
			CapturePiece(Pieces::WHITE);
		}
		m_GameBoard[moveToMake.to] = Pieces::RED;
		std::cout << "AI Red turn:\n";
		Display();
	}
}

void Board::CapturePiece(Pieces const color)
{
	// TODO make notification for color winner
	if (color == Pieces::RED)
	{
		--m_RedPieces;
		if (m_RedPieces <= 0)
		{
			m_GameOver = true;
		}
	}
	else if (color == Pieces::WHITE)
	{
		--m_WhitePieces;
		if (m_WhitePieces <= 0)
		{
			m_GameOver = true;
		}
	}
}

std::vector<MovePair> Board::GetAvailableMoves(Pieces const color, bool captureOnly)
{
	std::vector<MovePair> moveList;
	if (color != Pieces::EMPTY)
	{
		for (std::vector<Pieces>::const_iterator it = m_GameBoard.cbegin(); it != m_GameBoard.cend(); ++it)
		{
			if (*it == color)
			{
				int position = it - m_GameBoard.cbegin();
				int diagUpRight = position - 7;
				int diagUpLeft = position - 9;
				int diagDownLeft = position + 7;
				int diagDownRight = position + 9;
				if (!captureOnly)
				{
					if (position != 7 && position != 23 && position != 39 && position != 55)
					{
						if (diagUpRight >= 0 && diagUpRight <= 63
							&& m_GameBoard.at(diagUpRight) == Pieces::EMPTY)
						{
							moveList.push_back(MovePair(position, diagUpRight, NO_CAPTURE));
						}
						if (diagDownRight >= 0 && diagDownRight <= 63
							&& m_GameBoard.at(diagDownRight) == Pieces::EMPTY)
						{
							moveList.push_back(MovePair(position, diagDownRight, NO_CAPTURE));
						}
					}
					if (position != 23 && position != 39 && position != 55)
					{
						if (diagUpLeft >= 0 && diagUpLeft <= 63
							&& m_GameBoard.at(diagUpLeft) == Pieces::EMPTY)
						{
							moveList.push_back(MovePair(position, diagUpLeft, NO_CAPTURE));
						}
						if (diagDownLeft >= 0 && diagDownLeft <= 63 
							&& m_GameBoard.at(diagDownLeft) == Pieces::EMPTY)
						{
							moveList.push_back(MovePair(position, diagDownLeft, NO_CAPTURE));
						}
					}
				}

				// Capture jumps
				// Jump over the up right diag
				int diagJumpUpRight = diagUpRight - 7;
				if (diagJumpUpRight >= 0 && diagJumpUpRight <= 63)
				{
					if ((color == Pieces::WHITE && m_GameBoard.at(diagJumpUpRight) == Pieces::RED)
						|| (color == Pieces::RED && m_GameBoard.at(diagJumpUpRight) == Pieces::WHITE))
					{
						moveList.push_back(MovePair(position, diagJumpUpRight, diagUpRight));
					}
				}

				// Jump over left up diag
				int diagJumpUpLeft = diagUpLeft - 9;
				if (diagJumpUpLeft >= 0 && diagJumpUpLeft <= 63)
				{
					if ((color == Pieces::WHITE && m_GameBoard.at(diagJumpUpLeft) == Pieces::RED)
						|| (color == Pieces::RED && m_GameBoard.at(diagJumpUpLeft) == Pieces::WHITE))
					{
						moveList.push_back(MovePair(position, diagJumpUpLeft, diagUpLeft));
					}
				}

				// Jump over the up left diag
				int diagJumpDownLeft = diagDownLeft + 7;
				if (diagJumpDownLeft >= 0 && diagJumpDownLeft <= 63)
				{
					if ((color == Pieces::WHITE && m_GameBoard.at(diagJumpDownLeft) == Pieces::RED)
						|| (color == Pieces::RED && m_GameBoard.at(diagJumpDownLeft) == Pieces::WHITE))
					{
						moveList.push_back(MovePair(position, diagJumpDownLeft, diagDownLeft));
					}
				}
		
				// Jump over the up left diag
				int diagJumpDownRight = diagDownRight + 9;
				if (diagJumpDownRight >= 0 && diagJumpDownRight <= 63)
				{
					if ((color == Pieces::WHITE && m_GameBoard.at(diagJumpDownRight) == Pieces::RED)
						|| (color == Pieces::RED && m_GameBoard.at(diagJumpDownRight) == Pieces::WHITE))
					{
						moveList.push_back(MovePair(position, diagJumpDownRight, diagDownRight));
					}
				}
			}
		}
	}

	return moveList;
}

MovePair Board::GetMovePair(std::string const & move)
{
	// Convert input syntax to useable one.
	// Split on '-'
	std::vector<std::string> fromToSyntax;
	std::stringstream ss(move);
	std::string item;
	while (std::getline(ss, item, '-')) {
		fromToSyntax.push_back(item);
	}
	
	if (fromToSyntax.size() == 2)
	{
		int piecePositionFrom;
		int piecePositionTo;
		std::istringstream(fromToSyntax.at(0)) >> piecePositionFrom;
		std::istringstream(fromToSyntax.at(1)) >> piecePositionTo;
		if (piecePositionFrom >= 1 && piecePositionFrom <= 32 &&
			piecePositionTo >= 1 && piecePositionTo <= 32)
		{
			std::cout << "From: " << piecePositionFrom << " To: " << piecePositionTo << "\n";
			return MovePair ( GetPositionFromMove(piecePositionFrom), GetPositionFromMove(piecePositionTo), false );
		}
		else
		{
			std::cout << "Numbers too big or too small. Ranges from 1 to 32.\n";
		}
	}
	else
	{
		std::cout << "Invalid input, don't use spaces.\n";
	}

	return MovePair(-1,-1, false); // TODO make an invalid movepair recognizeable instead of magic nums
}

bool Board::IsValidMove(MovePair const movePair)
{
	if (movePair.from != -1 && movePair.to != -1)
	{
		int from = movePair.from;
		if (m_GameBoard.at(from) == Pieces::WHITE)
		{
			int to = movePair.to;

			// 3 things to check for here....
			// If the player is making a normal adjacent move
			// If the player is capturing a piece
			// If the player is making a string of jumps.... Maybe discourage this and make it auto?

			int diagUpRight = from - 7;
			int diagUpLeft = from - 9;
			int diagDownLeft = from + 7;
			int diagDownRight = from + 9;
			if (m_GameBoard.at(to) == Pieces::EMPTY)
			{
				// Adjacent move
				if (to == diagUpRight || to == diagUpLeft || to == diagDownLeft || to == diagDownRight)
				{
					if (m_GameBoard.at(to) == Pieces::EMPTY)
					{
						std::cout << "Valid move.\n";
						return true;
					}
				}

				// Capture jumps
				// Jump over the up right diag
				else if (to == diagUpRight - 7)
				{
					if (m_GameBoard.at(diagUpRight) == Pieces::RED)
					{
						m_PlayerCapture = diagUpRight;
						return true;
					}
				}
				// Jump over the up left diag
				else if (to == diagUpLeft - 9)
				{
					if (m_GameBoard.at(diagUpLeft) == Pieces::RED)
					{
						m_PlayerCapture = diagUpLeft;
						return true;
					}
				}
				// Jump over the up left diag
				else if (to == diagDownLeft + 7)
				{
					if (m_GameBoard.at(diagDownLeft) == Pieces::RED)
					{
						m_PlayerCapture = diagDownLeft;
						return true;
					}
				}
				// Jump over the up left diag
				else if (to == diagDownRight + 9)
				{
					if (m_GameBoard.at(diagUpLeft) == Pieces::RED)
					{
						m_PlayerCapture = diagUpLeft;
						return true;
					}
				}
			}
			else
			{
				std::cout << "Not an empty destination.\n";
			}
			
		}
		else
		{
			std::cout << "Not a white piece.\n";
		}
	}

	std::cout << "Invalid move. Try again.\n";
	m_PlayerCapture = NO_CAPTURE;
	return false;
}

int Board::GetPositionFromMove(int const checkersMove) const
{
	// If move is even, then only index off one.
	// If move is odd, index off two.
	int row = (checkersMove * 2 - 1) / 8;
	int inBoard = ((row & 1) == 1) ? checkersMove * 2 - 2 : checkersMove * 2 - 1;
	return inBoard;
}

void Board::Display()
{
	std::string boardDisplay; //TODO: faster implementation of strings
	int counter = 1;
	for (std::vector<Pieces>::const_iterator it = m_GameBoard.cbegin(); it != m_GameBoard.cend(); ++it)
	{
		if (*it == Pieces::EMPTY)
		{
			boardDisplay += ".";
		}
		else if (*it == Pieces::RED)
		{
			boardDisplay += "R";
		}
		else
		{
			boardDisplay += "W";
		}

		if (counter % 8 == 0)
		{
			boardDisplay += "\n";
		}
		++counter;
	}
	std::cout << boardDisplay;
	std::cout << "Press enter to continue";
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cout << "\n";
}