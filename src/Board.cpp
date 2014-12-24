#include "Board.h"
#include <iostream>
#include <string>
#include <limits>
#include <sstream>
#include <algorithm>

Board::Board()
	: m_GameBoard()
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
	while (!m_GameOver)
	{
		PlayerTurn();
		Display();
		//AITurn();
		Display();
	}
}

void Board::PlayerTurn()
{
	bool madeMove = false;
	while (!madeMove)
	{
		std::string inputMove;
		std::cout << "Input your white move (from-to format, eg 21-17)";
		std::cin >> inputMove;
		if (inputMove.length() > 0 && IsValidMove(inputMove))
		{
			
		}
	}
}

bool Board::IsValidMove(std::string & const move)
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
			return true;
		}
		else
		{
			std::cout << "Numbers too big or too small. Ranges from 1 to 32.\n";
		}
	}
	else
	{
		std::cout << "Too many arguments, don't use spaces.\n";
	}
	return false;
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
}