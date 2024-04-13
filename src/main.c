#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const int screen_width = 1280;
const int screen_height = 720;
const int grid_size = 40;
const int grid_cols = screen_width/grid_size;
const int grid_rows = screen_height/grid_size;

int **alloc_grid(void);
void free_grid(int **grid);

int sum_neighbors(int **grid, int x, int y);

int main(void) {
    int **grid = alloc_grid();
    if (grid == NULL) return 1;

    srand(time(NULL));
    for (int i = 0; i < grid_cols; ++i) {
        for (int j = 0; j < grid_rows; ++j) {
            grid[i][j] = rand()%2;
        }
    }

    InitWindow(screen_width, screen_height, "game of life");
    SetTargetFPS(60);
    
    int gen = 0;
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        for (int i = 0; i < grid_cols; ++i) {
            for (int j = 0; j < grid_rows; ++j) {
                if (grid[i][j] == 1) {
                    DrawRectangle(i*grid_size, j*grid_size, grid_size, grid_size, GRAY);
                }
            }
        }
        DrawText(TextFormat("Generation: %i", gen), 5, 5, 16, PINK);
        EndDrawing();

        if (IsKeyPressed(KEY_RIGHT)) {
            int **next_grid = alloc_grid();
            for (int i = 0; i < grid_cols; ++i) {
                for (int j = 0; j < grid_rows; ++j) {
                    int sum = sum_neighbors(grid, i, j);
                    if (sum == -1) {
                        next_grid[i][j] = grid[i][j];
                    }

                    switch (grid[i][j]) {
                    case 0: {
                        if (sum == 3) {
                            next_grid[i][j] = 1;
                        }
                    } break;
                    case 1: {
                        if (sum <= 1 || sum >= 4) {
                            next_grid[i][j] = 0;
                        } else {
                            next_grid[i][j] = 1;
                        }
                    } break;
                    }
                }
            }
            free_grid(grid);
            grid = next_grid;
            gen++;
        }
    }
    CloseWindow();
    free_grid(grid);
    return 0;
}

int **alloc_grid(void) {
    int **grid = malloc(grid_cols*sizeof(int*));
    if (grid == NULL) {
        printf("No mem :(\n");
        return NULL;
    }

    for (int i = 0; i < grid_cols; ++i) {
        grid[i] = malloc(grid_rows*sizeof(int));
        if (grid[i] == NULL) {
            printf("No mem :(\n");
            return NULL;
        }
    }
    return grid;
}

void free_grid(int **grid) {
    if (grid != NULL) {
        for (int i = 0; i < grid_cols; ++i) {
            if (grid[i] != NULL) {
                free(grid[i]);
            }
        }
        free(grid);
    }
}

int sum_neighbors(int **grid, int x, int y) {
    if (grid == NULL || grid[x] == NULL) return -1;
    int sum = 0;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == x && j == y) continue;
            int col = (x+i+grid_cols)%grid_cols;
            int row = (y+j+grid_rows)%grid_rows;
            sum += grid[col][row];
        }
    }
    return sum;
}
