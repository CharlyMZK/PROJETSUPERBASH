#include "stringUtils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

void empty_file(char* path);

void delete_file(char* path);

bool is_file_empty(char* fp);

bool switch_from_file_content_to_file(char* fromFile, char* toFile);

void display_file_content(char* path);