#define NORMAL_EXIT     000
#define MALLOC_ERROR    100
#define INPUT_ERROR     200
#define GENOME_ERROR    300
#define CREATURE_ERROR  400
#define UNSPECIFC_ERROR 900

#define quit(code) quit_(code, __LINE__, __FILE__)

void quit_(int code, int line, const char * file);
