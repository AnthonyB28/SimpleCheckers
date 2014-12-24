struct MovePair
{
public:
	MovePair(int from, int to, int capture)
		: from(from)
		, to(to)
		, capture(capture)
	{}

	// Returns if the left captures something
	inline bool operator< (MovePair const & right)
	{
		return capture > right.capture;
	}

	// Returns if from and to are the same
	inline bool operator== (MovePair const & right)
	{
		return (from == right.from) && (to == right.to);
	}

	int from;
	int to;
	int capture;
};