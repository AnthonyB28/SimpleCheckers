#define NO_CAPTURE -1

#include "Board.h"
#include <iostream>
#include <string>
#include <limits>
#include <sstream>
#include <algorithm>

Board::Board()
	: m_GameBoard()
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
	// Nothing explicitly allocated, doesn't need cleanup
}

void Board::BeginGame()
{
	std::cout << "Input is Red/Black top, white bottom. You are WHITE.\nhttp://www.bobnewell.net/nucleus/checkers.php?itemid=289 \n\n";
	Display();
	while (!m_GameOver)
	{
		PlayerTurn();
		AITurn();
	}
	std::cout << "Game over! Thanks for playing.\n";
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
	std::cout << "\n\n";
}

void Board::PlayerTurn()
{
	bool madeMove = false;
	bool multiJump = false;
	// Input loop
	while (!madeMove)
	{
		std::string playerInput;
		std::cout << "Input your white move (from-to format, eg 21-17)\n";
		std::cin >> playerInput;

		// Get valid moves & search them for player input. If not there, invalid move!
		MovePair inputMovePair = GetMovePairFromInput(playerInput);

		// If multijump is true, we're going to only check for chain moves!!
		// There may be a bug here, player may be able to capture from something else during multi move.
		std::vector<MovePair> movesToMake = GetAvailableMoves(Pieces::WHITE, multiJump);
		std::sort(movesToMake.begin(), movesToMake.end());
		std::vector<MovePair> captureMoves;
		if (movesToMake.size() != 0)
		{
			for (std::vector<MovePair>::iterator it = movesToMake.begin(); it != movesToMake.end(); ++it)
			{
				if (it->capture != NO_CAPTURE)
				{
					captureMoves.push_back(*it);
				}
				if (*it == inputMovePair)
				{
					// Valid move input from user.
					bool capture = it->capture != NO_CAPTURE ? true : false;
					if (!capture && captureMoves.size() > 0)
					{
						std::cout << "There are capture moves available.\n";
					}
					else
					{
						m_GameBoard[it->from] = Pieces::EMPTY;
						if (capture)
						{
							m_GameBoard[it->capture] = Pieces::EMPTY;
							CapturePiece(Pieces::RED);
						}
						m_GameBoard[it->to] = Pieces::WHITE;
						std::cout << "Result of your move: \n";
						Display();
						if (capture && CanPositionCapture(it->to, Pieces::WHITE))
						{
							// Additional moves available! Continue the loop.
							std::cout << "You have at least 1 extra jump. Please enter next jump move.... \n";
							multiJump = true;
						}
						else
						{
							madeMove = true;
						}
					}
					break;
				}
			}
		}
		else
		{
			std::cout << "White has no moves to make. Red wins!\n";
			madeMove = true;
			m_GameOver = true;
		}

		if (!madeMove && !multiJump)
		{
			std::cout << "Invalid move. Please try again.\n";
		}
	}
}

void Board::AITurn()
{
	if (!m_GameOver)
	{
		bool madeMove = false;
		bool multiJump = false;
		while (!madeMove)
		{
			// If multijump is true, we're going to only check for chain moves!!
			// There may be a bug here, player may be able to capture from something else during multi move.
			std::vector<MovePair> movesToMake = GetAvailableMoves(Pieces::RED, multiJump);
			if (movesToMake.size() != 0)
			{
				std::sort(movesToMake.begin(), movesToMake.end());
				MovePair moveToMake = movesToMake.at(0);
				m_GameBoard[moveToMake.from] = Pieces::EMPTY;
				if (moveToMake.capture != NO_CAPTURE)
				{
					m_GameBoard[moveToMake.capture] = Pieces::EMPTY;
					CapturePiece(Pieces::WHITE);
				}
				m_GameBoard[moveToMake.to] = Pieces::RED;

				// Check for double chain and do next move if possible!
				if (moveToMake.capture != NO_CAPTURE && CanPositionCapture(moveToMake.to, Pieces::RED))
				{
					madeMove = false;
					multiJump = true;
					std::cout << "Red can double jump.\n";
				}
				else
				{
					madeMove = true;
				}
				std::cout << "AI Red turn:\n";
				Display();
			}
			else
			{
				std::cout << "Red has no moves to make. White wins!\n";
				madeMove = true;
				m_GameOver = true;
			}
		}
	}
}


bool Board::CanPositionCapture(int const position, Pieces const color)
{
	std::vector<MovePair> captureMoves = GetAvailableMoves(color, true);
	if (captureMoves.size() > 0)
	{
		for (std::vector<MovePair>::const_iterator it = captureMoves.cbegin(); it != captureMoves.cend(); ++it)
		{
			// If the starting position is the input, then this piece can capture!!!
			if (it->from == position)
			{
				return true;
			}
		}
	}

	return false;
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
			std::cout << "White wins!\n";
		}
	}
	else if (color == Pieces::WHITE)
	{
		--m_WhitePieces;
		if (m_WhitePieces <= 0)
		{
			m_GameOver = true;
			std::cout << "Red wins!\n";
		}
	}
}

MovePair Board::GetMovePairFromInput(std::string const & move)
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
			//std::cout << "From: " << piecePositionFrom << " To: " << piecePositionTo << "\n";
			return MovePair ( GetPositionFromCheckersMove(piecePositionFrom), GetPositionFromCheckersMove(piecePositionTo), false );
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

int Board::GetPositionFromCheckersMove(int const checkersMove) const
{
	// http://www.bobnewell.net/nucleus/checkers.php?itemid=289
	// Black top and white bottom notation.
	// If row is even, then only index off one.
	// If row is odd, index off two.
	int row = (checkersMove * 2 - 1) / 8;
	int inBoard = ((row & 1) == 1) ? checkersMove * 2 - 2 : checkersMove * 2 - 1;
	return inBoard;
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
					if (position != 8 && position != 24 && position != 40 && position != 56)
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
				if (position != 7 && position != 23 && position != 39 && position != 55)
				{
					// Jump over the up right diag
					if (diagUpRight != 7 && diagUpRight != 23 && diagUpRight != 39 && diagUpRight != 55)
					{
						int diagJumpUpRight = diagUpRight - 7;
						if (diagJumpUpRight >= 0 && diagJumpUpRight <= 63)
						{
							if (((color == Pieces::WHITE && m_GameBoard.at(diagUpRight) == Pieces::RED)
								|| (color == Pieces::RED && m_GameBoard.at(diagUpRight) == Pieces::WHITE))
								&& m_GameBoard.at(diagJumpUpRight) == Pieces::EMPTY)
							{
								moveList.push_back(MovePair(position, diagJumpUpRight, diagUpRight));
							}
						}
					}

					// Jump over the up left diag
					if (diagDownRight != 7 && diagDownRight != 23 && diagDownRight != 39 && diagDownRight != 55)
					{
						int diagJumpDownRight = diagDownRight + 9;
						if (diagJumpDownRight >= 0 && diagJumpDownRight <= 63)
						{
							if (((color == Pieces::WHITE && m_GameBoard.at(diagDownRight) == Pieces::RED)
								|| (color == Pieces::RED && m_GameBoard.at(diagDownRight) == Pieces::WHITE))
								&& m_GameBoard.at(diagJumpDownRight) == Pieces::EMPTY)
							{
								moveList.push_back(MovePair(position, diagJumpDownRight, diagDownRight));
							}
						}
					}
				}

				if (position != 8 && position != 24 && position != 40 && position != 56)
				{
					// Jump over left up diag
					if (diagUpLeft != 8 && diagUpLeft != 24 && diagUpLeft != 40 && diagUpLeft != 56)
					{
						int diagJumpUpLeft = diagUpLeft - 9;
						if (diagJumpUpLeft >= 0 && diagJumpUpLeft <= 63)
						{
							if (((color == Pieces::WHITE && m_GameBoard.at(diagUpLeft) == Pieces::RED)
								|| (color == Pieces::RED && m_GameBoard.at(diagUpLeft) == Pieces::WHITE))
								&& m_GameBoard.at(diagJumpUpLeft) == Pieces::EMPTY)
							{
								moveList.push_back(MovePair(position, diagJumpUpLeft, diagUpLeft));
							}
						}
					}

					// Jump over the up left diag
					if (diagDownLeft != 8 && diagDownLeft != 24 && diagDownLeft != 40 && diagDownLeft != 56)
					{
						int diagJumpDownLeft = diagDownLeft + 7;
						if (diagJumpDownLeft >= 0 && diagJumpDownLeft <= 63)
						{
							if (((color == Pieces::WHITE && m_GameBoard.at(diagDownLeft) == Pieces::RED)
								|| (color == Pieces::RED && m_GameBoard.at(diagDownLeft) == Pieces::WHITE))
								&& m_GameBoard.at(diagJumpDownLeft) == Pieces::EMPTY)
							{
								moveList.push_back(MovePair(position, diagJumpDownLeft, diagDownLeft));
							}
						}
					}
				}
			}
		}
	}

	return moveList;
}