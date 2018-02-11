#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define VERBOSE 1

void log_message(const char* tag, const char* message) {
   if(VERBOSE == 1){
      printf("[%s]: %s \n", tag, message);
   }
}

void log_value(const char* tag, const char* message, int parameter) {
   if(VERBOSE == 1){
      printf("[%s]: %s ( %d ) \n", tag, message,parameter);
   }
}

void log_string(const char* tag, const char* message,const char* parameter) {
   if(VERBOSE == 1){
      printf("[%s]: %s ( %s ) \n", tag, message,parameter);
   }
}

void log_char_value(const char* tag, const char* message, char parameter) {
   if(VERBOSE == 1){
      printf("[%s]: %s ( %c ) \n", tag, message,parameter);
   }
}