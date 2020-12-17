#define CURRENT_DIR "."
#define PARENT_DIR ".."
#define DEFAULT_PERM "----------"
#define DATE_FORMAT "%a %b %e %H:%M:%S %G"
#define DATE_SIZE 50

bool is_directory(const char *);
void copy_file(const char *, char *);
void foreach_file(const char *, void (*)(char *));
void print_file_info(char *);