#ifndef EXPLORE_H
#define EXPLORE_H
/* various */
#define EXPLORE_SEARCH_COUNT 32 // how many times we should search a square
/* priorities */
#define EXPLORE_DESCEND 35
#define EXPLORE_VISIT_CORRIDOR 60
#define EXPLORE_VISIT_OPEN_DOOR 60
#define EXPLORE_VISIT_UNLIT_AREA 60
#define EXPLORE_VISIT_UNKNOWN_TILE 25
#define EXPLORE_SEARCH_CORRIDOR_DEAD_END 60
#define EXPLORE_SEARCH_DOOR_DEAD_END 60
#define EXPLORE_SEARCH_ROOM_CORNER 25
#define EXPLORE_SEARCH_CORRIDOR_CORNER 15
#define EXPLORE_SEARCH_WALL 10
#define EXPLORE_UNKNOWN_STAIRS 100
/* this should go */
#define MAX_DUNGEON_DEPTH 64

#include <list>
#include <string>
#include "../Analyzer.h"
#include "../Globals.h"
#include "../Point.h"

class Saiph;

class Explore : public Analyzer {
	public:
		Explore(Saiph *saiph);

		void analyze();
		void complete();
		void inspect(const Point &point);
		void parseMessages(const std::string &messages);

	private:
		Saiph *saiph;
		int search[MAX_DUNGEON_DEPTH][MAP_ROW_END + 1][MAP_COL_END + 1];
		bool visited[MAX_DUNGEON_DEPTH][MAP_ROW_END + 1][MAP_COL_END + 1];
		std::list<Point> explore;
};
#endif
