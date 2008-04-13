#ifdef DEBUG
#define debug(...)  { fprintf(stderr, __VA_ARGS__); fprintf(stderr,"\n");}
#else
#define debug(...)
#endif
