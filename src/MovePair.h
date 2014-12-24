/*
	Representation of a checker move with from and to positions.
	If a capture is available (not -1), it is position of piece to erase.
*/

struct MovePair
{
public:
	MovePair(int from, int to, int capture)
		: from(from)
		, to(to)
		, capture(capture)
	{}

	inline bool operator< (MovePair const & right)
	{
		return capture > right.capture;
	}

	inline bool operator== (MovePair const & right)
	{
		return (from == right.from) && (to == right.to);
	}

	int from;
	int to;
	int capture;
};