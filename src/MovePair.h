struct MovePair
{
public:
	MovePair(int from, int to, int capture)
		: from(from)
		, to(to)
		, capture(capture)
	{}

	int from;
	int to;
	int capture;
};