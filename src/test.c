#include "raylib.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define BOARD_SQUARES 15
#define SQUARE_PX 40

typedef struct {
  int x;
  int y;
} Vec2;

bool vec2_equals(Vec2 v1, Vec2 v2) { return v1.x == v2.x && v1.y == v2.y; }

typedef struct {
  size_t length;
  Vec2 body[BOARD_SQUARES * BOARD_SQUARES];
} Snake;

typedef enum {
  LEFT,
  UP,
  RIGHT,
  DOWN,
} Direction;

bool move(Snake *s, Direction d, bool add_piece);
Vec2 new_apple_coords(Snake *s);

int main(void) {
  const int screen_edge_px = BOARD_SQUARES * SQUARE_PX;
  InitWindow(screen_edge_px, screen_edge_px, "example project");

  Direction curr_dir = DOWN;
  Snake snake = {
      .length = 3,
      .body[0] = {.x = (int)(BOARD_SQUARES / 2) + 0,
                  .y = (int)(BOARD_SQUARES / 2) + 1},
      .body[1] = {.x = (int)(BOARD_SQUARES / 2) + 1,
                  .y = (int)(BOARD_SQUARES / 2) + 1},
      .body[2] = {.x = (int)(BOARD_SQUARES / 2) + 1,
                  .y = (int)(BOARD_SQUARES / 2) + 2},
  };

  Vec2 apple = {.x = -1};
  bool surviving = true;

  SetTargetFPS(5);
  while (!WindowShouldClose() && surviving) {
    switch (GetKeyPressed()) {
    case KEY_LEFT:
      curr_dir = curr_dir != RIGHT ? LEFT : RIGHT;
      break;
    case KEY_UP:
      curr_dir = curr_dir != DOWN ? UP : DOWN;
      break;
    case KEY_RIGHT:
      curr_dir = curr_dir != LEFT ? RIGHT : LEFT;
      break;
    case KEY_DOWN:
      curr_dir = curr_dir != UP ? DOWN : LEFT;
      break;
    }

    if (apple.x == -1) {
      apple = new_apple_coords(&snake);
    }

    bool piece_eaten = vec2_equals(snake.body[0], apple);
    if (piece_eaten) {
      apple = (Vec2){.x = -1, .y = -1};
    }
    surviving = move(&snake, curr_dir, piece_eaten);

    BeginDrawing();
    ClearBackground(BLACK);
    // drawing the lines for the board
    for (int i = 1; i < BOARD_SQUARES; ++i) {
      int line_px_pos = SQUARE_PX * i;
      DrawLine(line_px_pos, 0, line_px_pos, screen_edge_px, DARKPURPLE);
      DrawLine(0, line_px_pos, screen_edge_px, line_px_pos, DARKPURPLE);
    }
    // drawing the food piece
    DrawRectangle(apple.x * SQUARE_PX, apple.y * SQUARE_PX, SQUARE_PX,
                  SQUARE_PX, BLUE);
    // drawing the snake
    for (int i = 0; i < snake.length; ++i) {
      DrawRectangle(snake.body[i].x * SQUARE_PX, snake.body[i].y * SQUARE_PX,
                    SQUARE_PX, SQUARE_PX, RED);
    }
    EndDrawing();
  }
  CloseWindow();
  return 0;
}

bool move(Snake s[static 1], Direction d, bool add_piece) {
  Vec2 *head = &(s->body[0]);
  Vec2 prev_pos = *head;
  switch (d) {
  case LEFT:
    --(head->x);
    break;
  case UP:
    --(head->y);
    break;
  case RIGHT:
    ++(head->x);
    break;
  case DOWN:
    ++(head->y);
    break;
  }

  // checking if the head went out of bound
  if (head->x < 0 || head->y < 0 || head->x > BOARD_SQUARES - 1 ||
      head->y > BOARD_SQUARES - 1) {
    return false;
  }
  // every piece gets the position of the previous piece
  for (int i = 1; i < s->length; ++i) {
    if (vec2_equals(*head, s->body[i])) {
      return false;
    }
    Vec2 tmp = s->body[i];
    s->body[i] = prev_pos;
    prev_pos = tmp;
  }

  if (add_piece) {
    s->body[s->length] = s->body[s->length - 1];
    ++(s->length);
  }

  return true;
}

Vec2 new_apple_coords(Snake *s) {
  bool board_occupied[BOARD_SQUARES][BOARD_SQUARES];
  memset(board_occupied, false, sizeof(board_occupied));
  for (int i = 0; i < s->length; ++i) {
    board_occupied[s->body[i].x][s->body[i].y] = true;
  }

  Vec2 empty_coords[BOARD_SQUARES * BOARD_SQUARES];
  int j = 0;
  for (int i = 0; i < BOARD_SQUARES * BOARD_SQUARES; ++i) {
    int x = i / BOARD_SQUARES;
    int y = i % BOARD_SQUARES;
    if (!board_occupied[x][y]) {
      empty_coords[j++] = (Vec2){.x = x, .y = y};
    }
  }
  return empty_coords[GetRandomValue(0, j - 1)];
}
