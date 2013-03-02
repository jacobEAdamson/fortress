#include <iostream>
#include <queue>
#include <curses.h>
using namespace std;

const int CHUNK_SIZE = 256;

enum event_t {
	UP,
	DOWN,
	LEFT,
	RIGHT,
	UP_LEFT,
	UP_RIGHT,
	DOWN_LEFT,
	DOWN_RIGHT,
	QUIT
};

class Event {
public:
	event_t type;

	Event(event_t p_type){
		type = p_type;
	}
};

struct Unit {
	int x;
	int y;
	char token;
};

enum terrain_t {
	EMPTY,
	WALL
};

struct Terrain {
	terrain_t type;
	bool visible;
};

struct Chunk {
	Terrain grid[CHUNK_SIZE][CHUNK_SIZE];
};

void GetInput();
void HandleEvents();
bool IsPassable(int x, int y);
void CreateRoom(Terrain grid[CHUNK_SIZE][CHUNK_SIZE], int width, int height);

queue<Event> event_queue;

Unit units[20];
int units_length = 0;

int map_width_x, map_width_y;
int max_row, max_column;

Chunk test_chunk;

bool shutdown = false;

int main(){
	int i, j, k;
	i = j = k = 0;

	initscr();
	keypad(stdscr, true);

	getmaxyx(stdscr, max_row, max_column);
	if(max_column-1 > CHUNK_SIZE)
		map_width_x = CHUNK_SIZE;
	else
		map_width_x = max_column-1;

	if(max_row-1 > CHUNK_SIZE)
		map_width_y = CHUNK_SIZE;
	else
		map_width_y = max_row-1;

	for(int i = 0; i < CHUNK_SIZE; i++){
		for(int j = 0; j < CHUNK_SIZE; j++){
			test_chunk.grid[i][j].type = WALL;
			test_chunk.grid[i][j].visible = false;
		}
	}
	CreateRoom(test_chunk.grid, 8, 8);

	Unit player;
	player.x = 3;
	player.y = 3;
	player.token = '@';

	units[units_length++] = player;


	while(!shutdown){
		clear();
		//Print terrain for the current Z level
		for(i = 0; i < map_width_y; i++){
			for(j = 0; j < map_width_x; j++){
				if(test_chunk.grid[j][i].visible == false)
					addch(' ');
				else if(test_chunk.grid[j][i].type == EMPTY)
					addch('.');
				else if(test_chunk.grid[j][i].type == WALL)
					addch('#');
			}
			addch('\n');
		}

		for(i = 0; i < units_length; i++)
		{
			mvaddch(units[i].y, units[i].x, units[i].token);
		}
		move(max_row-1, 0);
		refresh();

		GetInput();
		HandleEvents();
	}

	endwin();
	return 0;
}

void GetInput(){
	int input_char = getch();

	switch(input_char){
	case 'q':
		event_queue.push(Event(QUIT));
		break;
	case KEY_LEFT:
		event_queue.push(Event(LEFT));
		break;
	case KEY_RIGHT:
		event_queue.push(Event(RIGHT));
		break;
	case KEY_DOWN:
		event_queue.push(Event(DOWN));
		break;
	case KEY_UP:
		event_queue.push(Event(UP));
		break;
	case '7':
		event_queue.push(Event(UP_LEFT));
		break;
	case '8':
		event_queue.push(Event(UP));
		break;
	case '9':
		event_queue.push(Event(UP_RIGHT));
		break;
	case '4':
		event_queue.push(Event(LEFT));
		break;
	case '6':
		event_queue.push(Event(RIGHT));
		break;
	case '1':
		event_queue.push(Event(DOWN_LEFT));
		break;
	case '2':
		event_queue.push(Event(DOWN));
		break;
	case '3':
		event_queue.push(Event(DOWN_RIGHT));
		break;
	default:
		break;
	}
}

void HandleEvents(){
	while(event_queue.size() > 0){
		Event current_event = event_queue.front();
		event_queue.pop();

		switch (current_event.type) {
		case UP_LEFT:
			if(IsPassable(units[0].x-1, units[0].y-1)){
				units[0].x--;
				units[0].y--;
			}
			break;
		case UP:
			if(IsPassable(units[0].x, units[0].y-1)){
				units[0].y--;
			}
			break;
		case UP_RIGHT:
			if(IsPassable(units[0].x+1, units[0].y-1)){
				units[0].x++;
				units[0].y--;
			}
			break;
		case LEFT:
			if(IsPassable(units[0].x-1, units[0].y)){
				units[0].x--;
			}
			break;
		case RIGHT:
			if(IsPassable(units[0].x+1, units[0].y)){
				units[0].x++;
			}
			break;
		case DOWN_LEFT:
			if(IsPassable(units[0].x-1, units[0].y+1)){
				units[0].x--;
				units[0].y++;
			}
			break;
		case DOWN:
			if(IsPassable(units[0].x, units[0].y+1)){
				units[0].y++;
			}
			break;
		case DOWN_RIGHT:
			if(IsPassable(units[0].x+1, units[0].y+1)){
				units[0].x++;
				units[0].y++;
			}
			break;
		case QUIT:
			shutdown = true;
			break;
		default:
			break;
		}

	}
}

bool IsPassable(int x, int y){
	if(x == map_width_x || x == -1){
		return false;
	}
	else if(y == map_width_y || y == -1){
		return false;
	}
	else if(test_chunk.grid[x][y].type == WALL){
		return false;
	}
	else{
		return true;
	}
}

void CreateRoom(Terrain grid[CHUNK_SIZE][CHUNK_SIZE], int width, int height){
	for(int i = 0; i < width; i++){
		grid[i][0].type = WALL;
		grid[i][0].visible = true;
	}

	for(int i = 1; i < height-1; i++){
		grid[0][i].type = WALL;
		grid[0][i].visible = true;
		for(int j = 1; j < width-1; j++){
			grid[j][i].type = EMPTY;
			grid[j][i].visible = true;
		}
		grid[width-1][i].type = WALL;
		grid[width-1][i].visible = true;
	}

	for(int i = 0; i < width; i++){
		grid[i][height-1].type = WALL;
		grid[i][height-1].visible = true;
	}
}
