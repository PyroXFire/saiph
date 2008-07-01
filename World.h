#ifndef WORLD_H
/* defines */
#define WORLD_H
/* buffer */
#define BUFFER_SIZE 65536
/* text telling us there are more messages */
#define MORE "--More--"
#define MORE_LENGTH 8
/* we need these to determine if we're looking at a menu */
#define END "(end)"
#define END_LENGTH 6 // seems to be a bug here, there's an space after; "(end) "
#define PAGE " of "
#define PAGE_LENGTH 6 // must add 2 because of "y)" in "(x of y)"
/* and these for looking for questions */
#define QUESTION_YN "[yn]"
#define QUESTION_YNQ "[ynq]"
#define QUESTION_DY "(y)"
#define QUESTION_DN "(n)"
#define QUESTION_DQ "(q)"
#define QUESTION_LENGTH 20

/* forward declare */
class World;

/* includes */
#include <string>
#include "Connection.h"
#include "Globals.h"
#include "Player.h"

/* namespace */
using namespace std;

/* the world in our eyes */
class World {
	public:
		/* variables */
		char map[ROWS][COLS + 1]; // + 1 because we'll make the last character on each line '\0' (for easier parsing)
		int color[ROWS][COLS]; // not used for string reading, no need for + 1
		int row; // cursor position, row
		int col; // cursor position, col
		Player player;
		char messages[BUFFER_SIZE];
		bool menu;
		bool question;
		char data[BUFFER_SIZE];

		/* constructors */
		World(Connection *connection);

		/* destructors */
		~World();

		/* methods */
		bool executeCommand(const string &command);

	private:
		/* variables */
		Connection *connection;
		ssize_t data_size;
		int messages_pos; // used for concatenation multiple messages together
		string msg_str; // used for fetching messages

		/* methods */
		void handleEscapeSequence(int *pos, int *color);
		void fetchMessages();
		void fetchMessagesHelper(int row, int startcol);
		void update();
};
#endif
