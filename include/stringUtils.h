char *substr(char *src,int pos,int len);

int find_index_off_first_occurence_in_string(char *src, char charToFind);

bool log_in_file(char * string, char * file);
 
void trim_leading(char * str);

void trim_last(char * str);

void remove_space_at_beginning_and_end(char * string);

char** str_split_and_add_path(char * args,char * path);

char** str_split(char* a_str, const char a_delim);

int string_contain_and_at_end(char * command);

void remove_newline_ch(char *line);

void removeChar(char *str, char garbage);