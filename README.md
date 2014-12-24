SimpleCheckers
==============

Simplistic C++ checkers game made in 4 hours.

* The input is using English Draught notation (from-to) found on Wikipedia and [this] site.

* The Red/Black AI takes kind of random moves, but prioritizes captures and chained moves. If given an extra hour, I could implement a basic evaluation function and minimax/negamax to make it have a primitive strategy.

* From what I was able to test, you the player as white, can only make valid moves and if a capture move is available, you must take it.

* Chained jump moves appear to work for both AI and the player. I tested out a few scenarios of the AI, and one with the player, but there could be edge cases I didn't catch. If the player enters a move that begins a chain move, they must follow up with the next chain move by typing it manually. In the code, I potentially see a bug where a player or the AI begins a chain move, but instead of continuing the chain jumps, they move a different piece that can capture as well but I couldn't test it in time.

* During the time limit, I never got to finish a full game against the AI, so I'm not 100% sure if game over works as intended. When testing with only 2 pieces on the board, it appeared to work.


[this]: http://www.bobnewell.net/nucleus/checkers.php?itemid=289
