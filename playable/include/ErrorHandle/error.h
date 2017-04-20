#define NORMAL_EXIT     000
#define MALLOC_ERROR    100
#define INPUT_ERROR     200
#define GENOME_ERROR    300
#define CREATURE_ERROR  400
#define UNSPECIFIC_ERROR 900

#define quit(code) quit_(code, __LINE__, __FILE__)
#define eMalloc(bytes) Emalloc_(bytes, __LINE__, __FILE__)

void quit_(int code, int line, const char * file);
void * Emalloc_(int bytes, int line, const char * file);
