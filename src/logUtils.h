#ifndef LOGGER_H
#define LOGGER_H
#include <stdbool.h>
#include "treeUtils.h"

void log_message(const char* tag, const char* message);

void log_value(const char* tag, const char* message, int parameter);

void log_char_string(const char* tag, const char* message,const char* string);

void log_string(const char* tag, const char* message,const char* string);

void log_char_value(const char* tag, const char* message, char parameter);

void log_tree(Node * node);
#endif /* LOG_H */