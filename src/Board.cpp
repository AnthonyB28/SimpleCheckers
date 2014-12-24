#include "Board.h"
#include <iostream>
#include <string>

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
void Board::Display()
{
	std::string boardDisplay; //TODO: faster implementation of this
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
}