#define CURRENT_DIR "."
#define PARENT_DIR ".."
#define FORWARD_SLASH '/'
#define DEFAULT_PERM "----------"
#define DATE_FORMAT "%a %b %e %H:%M:%S %G"
#define DATE_SIZE 50
#define FORWARD_SLASH 47
#define COPY_PRINT_FORMAT "%s -> %s\n"

bool file_exists(char *);
bool is_directory(char *);
bool has_trailing_slash(const char *);
int create_absolute_symlink(const char *, char *);
void copy_file(const char *, char *, bool, bool);
void copy_files(const char *, char *, bool, bool);
void print_file_info(char *);
void print_files_info(char *source);