#ifndef SAIPH_H
/* defines */
#define SAIPH_H
/* pathing */
#define COST_CARDINAL 2
#define COST_DIAGONAL 3
#define COST_ICE 8 // slippery and risky, try to find a way around (don't try very hard, though)
#define COST_LAVA 512 // lava, hot!
#define COST_PET 16 // try not to move onto pets
#define COST_TRAP 128 // avoid traps
#define COST_WATER 256 // avoid water if possible
#define PATHING_QUEUE_SIZE 4096 // max amount of nodes in pathing_queue

/* forward declare */
class Saiph;

/* includes */
#include <list>
#include <string>
#include <vector>
#include "Analyzer.h"
#include "Connection.h"
#include "Globals.h"
#include "Item.h"
#include "Map.h"
#include "MonsterTracker.h"
#include "Player.h"
#include "Point.h"
#include "Request.h"
#include "World.h"
/* analyzers */
#include "Analyzers/Door.h"
#include "Analyzers/Elbereth.h"
#include "Analyzers/Explore.h"
#include "Analyzers/Fight.h"
#include "Analyzers/Health.h"
#include "Analyzers/Level.h"
#include "Analyzers/Loot.h"

/* namespace */
using namespace std;

/* struct used for pathing */
struct PathNode {
	PathNode *nextnode;
	unsigned int cost;
	unsigned char move;
};

/* this is our AI */
class Saiph {
	public:
		/* variables */
		MonsterTracker *monstertracker;
		World *world;
		Map map[MAX_BRANCHES][MAX_DUNGEON_DEPTH];
		int current_branch;
		int current_level;
		string command;
		bool engulfed;

		/* constructors */
		Saiph(int interface);

		/* destructor */
		~Saiph();

		/* methods */
		void farlook(const Point &target);
		bool requestAction(const Request &request);
		bool run();
		unsigned char shortestPath(const Point &target, bool allow_illegal_last_move, int *distance, bool *straight_line);

	private:
		/* variables */
		vector<Analyzer *> analyzers;
		Connection *connection;
		PathNode pathmap[MAP_ROW_END + 1][MAP_COL_END + 1];
		Point pathing_queue[PATHING_QUEUE_SIZE];
		int pathcost[UCHAR_MAX + 1];
		bool monster[UCHAR_MAX + 1];
		bool passable[UCHAR_MAX + 1];
		bool static_dungeon_symbol[UCHAR_MAX + 1];

		/* methods */
		void dumpMaps();
		void inspect();
		void parseMessageItem(const string &message);
		void parseMessages();
		void updateMaps();
		void updatePathMap();
		bool updatePathMapHelper(const Point &to, const Point &from);
};
#endif
