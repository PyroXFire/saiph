#include "LevelAnalyzer.h"

/* constructors */
LevelAnalyzer::LevelAnalyzer(Saiph *saiph) {
	this->saiph = saiph;
	action = ILLEGAL_MOVE;
}

/* methods */
int LevelAnalyzer::finish() {
	/* time to descend */
	int branch = saiph->current_branch;
	int dungeon = saiph->world->player.dungeon;
	if (saiph->map[branch][dungeon].dungeon[saiph->world->player.row][saiph->world->player.col] == STAIRS_DOWN) {
		/* standing on downstairs, descend */
		action = MOVE_DOWN;
		return LA_DESCEND_PRIORITY;
	}
	for (int r = MAP_ROW_START; r <= MAP_ROW_END; ++r) {
		for (int c = 0; c < COLS; ++c) {
			if (saiph->map[branch][dungeon].dungeon[r][c] == STAIRS_DOWN) {
				int distance = 0;
				bool direct_line = false;
				Point tmp;
				tmp.row = r;
				tmp.col = c;
				unsigned char move = saiph->shortestPath(tmp, false, &distance, &direct_line);
				if (move != ILLEGAL_MOVE) {
					action = move;
					return LA_DESCEND_PRIORITY;
				}
			}
		}
	}
	return 0;
}

void LevelAnalyzer::command(string *command) {
	command->push_back(action);
}
