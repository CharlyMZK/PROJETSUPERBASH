char* getAliasName(char* handledAlias, int aliasNameSize);

void displayAliases();

void updateAliases(char** splitedBySpacesCommand);

void handleAlias(char** splitedBySpacesCommand);

void removeAlias(char** splitedBySpacesCommand);

void modifyCommandDependingOnAliasDefined(char* c, char** splitedBySpacesCommand);

int getAliasNameSize(char* handledAlias);

int getCommandNameSize(char* handledAlias, int indexToStart);