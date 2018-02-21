#ifndef LOGGER_H
#define LOGGER_H

void log_message(const char* tag, const char* message);

void log_value(const char* tag, const char* message, int parameter);

void log_char_string(const char* tag, const char* message,const char* string);

void log_string(const char* tag, const char* message,const char* string);

void log_char_value(const char* tag, const char* message, char parameter);

#endif /* LOG_H */