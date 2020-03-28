/*
 * Copyright (C) 2009 Raphael Kubo da Costa <kubito@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "config.h"
#include "mem.h"

/* Error messages */
static const char *const usage_message =
    "\n"
    "Conway's Game of Life\n"
    "Raphael Kubo da Costa, RA 072201\n"
    "\n"
    "Usage: glife GENERATIONS INPUT_FILE < -p MILISECONDS > < -q >\n"
    "\n"
    "  GENERATIONS is the number of generations the game should run\n"
    "  INPUT_FILE  is a file containing an initial board state\n"
    "  MILISECONDS make a pause for the given number of miliseconds\n " "\n";

void game_config_free(GameConfig *config)
{
  if (config) {
    fclose(config->input_file);
    free(config);
  }
}

size_t game_config_get_generations(GameConfig *config)
{
  assert(config);

  return config->generations;
}

size_t game_config_get_miliseconds(GameConfig *config)
{
  assert(config);

  return config->miliseconds;
}

size_t game_config_get_quiet(GameConfig *config)
{
  assert(config);

  return config->quiet;
}

GameConfig *game_config_new_from_cli(int argc, char *argv[])
{
  char *endptr;
  FILE *file;
  GameConfig *config;
  long generations;
  long miliseconds;
  long quiet;

  if (argc != CLI_ARGC && argc != 4 && argc != 5  && argc != 6) {  // caso nao tenho o numero de argumentos necessarios
    fprintf(stderr, usage_message);
    return NULL;
  }

  generations = strtol(argv[1], &endptr, 10);
  if ((*endptr != '\0') || (generations < 0)) {
    fprintf(stderr, "Error: GENERATIONS must be a valid positive integer\n");
    return NULL;
  }

  file = fopen(argv[2], "r");
  if (!file) {
    fprintf(stderr, "Error: could not open '%s'\n", argv[2]);
    return NULL;
  }

  if( (argc == 5 || argc == 6) && ( strcmp(argv[3],"-p") == 0)  ) {
      miliseconds = strtol(argv[4], &endptr, 10);
      if ((*endptr != '\0') || (miliseconds < 0)) {
        fprintf(stderr, "Error: MILISECONDS must be a valid positive integer\n");
        return NULL;
      }
  } else {
    miliseconds = strtol("0", &endptr, 10);   // por defualt o tempo entre screeens é 0
  }

  if( (argc == 4 && ( strcmp(argv[3],"-q") == 0)) || (argc == 6 && ( strcmp(argv[5],"-q") == 0))  ) {
    quiet = 0;
  } else quiet = 1;
  
  config = MEM_ALLOC(GameConfig);
  config->generations = (size_t) generations;
  config->miliseconds = (size_t) miliseconds;
  config->input_file = file;
  config->quiet = quiet;

  return config;
}
