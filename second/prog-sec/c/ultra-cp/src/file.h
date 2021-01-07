#define CURRENT_DIR "."
#define PARENT_DIR ".."
#define FORWARD_SLASH '/'
#define DEFAULT_PERM "----------"
#define DATE_FORMAT "%a %b %e %H:%M:%S %G"
#define DATE_SIZE 50

bool file_exists(char *);
bool is_directory(char *);
bool has_trailing_slash(const char *);
void copy_file(const char *, char *);
void foreach_file(const char *, void (*)(char *));
void print_files_info(char *source);
void print_file_info(char *);