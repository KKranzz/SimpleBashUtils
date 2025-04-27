#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum option_codes {
  WRONG_OPTION = -1,
  HELP_OPTION_RPO = -2,
  VERSION_OPTION_RPO = -3
};

typedef struct parse_result {
  int code_error;
  int count_files;
} parse_result;

typedef struct data_flags {
  int b_flag;
  int E_flag;
  int v_flag;
  int n_flag;
  int s_flag;
  int T_flag;
} data_flags;

parse_result ParseArgs(int argc, char* argv[], char** flags, char*** files);
void Deallocate(char* flags, char** files);
int HandleOption(char* str, char** flags, int* capacity_flags);
int HandleFlag(char* str, char** flags, int* capacity_flags);
void FillDataFlags(char* flags, data_flags* df);
void StdProcess(data_flags df, int* count_lines);
void FileProcess(char* filename, data_flags df, int* count_lines,
                 int* endliner);
void CatProcess(int argc, char* argv[]);
void HelpOptionOutput();
void ParseCalc(char* argv_elem, int* only_files_mode, int* capacity_flags,
               parse_result* result, char** flags, int* counter,
               int* capacity_files, char*** files, int argc);
void ReallocErrorHandler(const void* const ptr);
void StrProcessingHelper(data_flags df, char ch, char* buffer, int* size,
                         int* s_flag_supporter);
void SFlagProcessHelper(data_flags df, int* count_lines, char* buffer, char ch,
                        int* size);
void SFlagProcessHelperForFiles(data_flags df, int* count_lines, char* buffer,
                                char ch, int* size, int* endliner,
                                int code_get);
void CapacityObserver(int* capacity, char** buffer, int size);
void FileProcessHelper(int* size, data_flags df, char ch, int* s_flag_supporter,
                       int* endliner, char* buffer);
int CountDigits(int number);

int main(int argc, char* argv[]) {
  CatProcess(argc, argv);
  return 0;
}

void Deallocate(char* flags, char** files) {
  free(flags);
  free(files);
}

void ParseCalc(char* argv_elem, int* only_files_mode, int* capacity_flags,
               parse_result* result, char** flags, int* counter,
               int* capacity_files, char*** files, int argc) {
  if (argv_elem[0] == '-' && strlen(argv_elem) > 1 && *only_files_mode == 0) {
    if (argv_elem[1] == '-') {
      if (strcmp("--", argv_elem) == 0) {
        *only_files_mode = 1;
      } else {
        int code_option = HandleOption(argv_elem, flags, capacity_flags);
        if (code_option < 0) {
          *counter = argc;
          result->code_error = code_option;
        }
      }
    } else {
      int code_option = HandleFlag(argv_elem, flags, capacity_flags);
      if (code_option < 0) {
        *counter = argc;
        result->code_error = code_option;
      }
    }
  } else {
    if (*capacity_files <= result->count_files + 1) {
      *capacity_files = (*capacity_files) * 2;
      *files = realloc(*files, sizeof(char*) * ((*capacity_files) + 10));
      if (*files == NULL) {
        printf("calloc error\n");
        result->code_error = -1;
      }
    }

    if (*files != NULL) {
      (*files)[result->count_files] = argv_elem;
      result->count_files++;
    }
  }
}

parse_result ParseArgs(int argc, char* argv[], char** flags, char*** files) {
  parse_result result = {0, 0};
  int capacity_flags = 10;
  int capacity_files = 10;
  *flags = calloc(capacity_flags, sizeof(char));
  if (*flags == NULL) {
    printf("calloc error\n");
    result.code_error = -1;
  }
  *files = calloc(capacity_files, sizeof(char*));
  if (*files == NULL) {
    printf("calloc error\n");
    result.code_error = -1;
  }

  int only_files_mode = 0;

  if (argc != 1 && result.code_error != -1) {
    for (int i = 1; i < argc && result.code_error != -1; i++) {
      ParseCalc(argv[i], &only_files_mode, &capacity_flags, &result, flags, &i,
                &capacity_files, files, argc);
    }
  }

  return result;
}

void HelpOptionOutput() {
  printf("Usage: s21_cat [OPTIONS] [FILE]..\n");
  printf("Options:\n");
  printf("-b Number non-empty output lines.\n");
  printf("-E Display a dollar sign ($) at the end of each line.\n");
  printf("-n Number all output lines.\n");
  printf("-s Suppress repeated empty output lines.\n");
  printf("-T Display tabs as ^I.\n");
  printf("-v Display special symbols except tab and $\n");
}

void ReallocErrorHandler(const void* const ptr_1) {
  if (ptr_1 == NULL) {
    printf("realloc error\n");
  }
}

int HandleOption(char* str, char** flags, int* capacity_flags) {
  int code_option = 0;
  if (*capacity_flags <= (int)strlen(*flags) + 1) {
    (*capacity_flags) += 2;
    (*capacity_flags) *= 2;
    *flags = realloc(*flags, sizeof(char) * ((*capacity_flags) + 10));
    ReallocErrorHandler(*flags);
  }
  if (strcmp("--help", str) == 0 || strcmp("--h", str) == 0) {
    code_option = HELP_OPTION_RPO;
    HelpOptionOutput();
  } else if (strcmp("--version", str) == 0 || strcmp("--v", str) == 0) {
    code_option = VERSION_OPTION_RPO;
    printf("s21_cat version 1.0\n");
  } else if (strcmp("--number-nonblank", str) == 0) {
    (*flags)[strlen(*flags)] = 'b';
  } else if (strcmp("--show-ends", str) == 0) {
    (*flags)[strlen(*flags)] = 'E';
  } else if (strcmp("--number", str) == 0) {
    (*flags)[strlen(*flags)] = 'n';
  } else if (strcmp("--squeeze-blank", str) == 0) {
    (*flags)[strlen(*flags)] = 's';
  } else if (strcmp("--show-tabs", str) == 0) {
    (*flags)[strlen(*flags)] = 'T';
  } else if (strcmp("--show-nonprinting", str) == 0) {
    (*flags)[strlen(*flags)] = 'v';
  } else {
    code_option = WRONG_OPTION;
    printf("s21_cat: unknown option %s", str);
    printf("Try 's21_cat --help' for more information");
  }
  return code_option;
}

int HandleFlag(char* str, char** flags, int* capacity_flags) {
  int code_option = 0;

  int length = strlen(str);

  int size = strlen(*flags);
  for (int i = 1; i < length; i++) {
    if (str[i] == 'b' || str[i] == 'e' || str[i] == 'v' || str[i] == 'E' ||
        str[i] == 'n' || str[i] == 's' || str[i] == 't' || str[i] == 'T') {
      if (*capacity_flags <= (int)strlen(*flags) + 1) {
        (*capacity_flags) += 2;
        (*capacity_flags) *= 2;
        int cap = *capacity_flags;
        *flags = realloc(*flags, sizeof(char) * cap);
        if (*flags == NULL) {
          printf("realloc error\n");
          i = length;
        }
      }
      if (*flags != NULL) {
        (*flags)[size] = str[i];
        size++;
        (*flags)[size] = '\0';
      }
    } else {
      code_option = WRONG_OPTION;
      printf("s21_cat: unknown option -- %c", str[i]);
      i = length;
    }
  }

  return code_option;
}

void FillDataFlags(char* flags, data_flags* df) {
  int size = strlen(flags);

  for (int i = 0; i < size; i++) {
    if (flags[i] == 'b') {
      df->b_flag = 1;
      df->n_flag = 0;
    } else if (flags[i] == 'e') {
      df->E_flag = 1;
      df->v_flag = 1;
    } else if (flags[i] == 'v') {
      df->v_flag = 1;
    } else if (flags[i] == 'E') {
      df->E_flag = 1;
    } else if (flags[i] == 'n') {
      if (df->b_flag == 1) {
        df->n_flag = 0;
      } else {
        df->n_flag = 1;
      }
    } else if (flags[i] == 's') {
      df->s_flag = 1;
    } else if (flags[i] == 't') {
      df->T_flag = 1;
      df->v_flag = 1;
    } else if (flags[i] == 'T') {
      df->T_flag = 1;
    }
  }
}

void StrProcessingHelper(data_flags df, char ch, char* buffer, int* size,
                         int* s_flag_supporter) {
  if (df.T_flag == 1 && ch == '\t') {
    buffer[(*size)] = '^';
    (*size)++;
    buffer[(*size)] = 'I';
    (*size)++;
    buffer[(*size)] = '\0';
  } else if (df.v_flag == 1 && ((ch >= 0 && ch <= 31) || ch == 127) &&
             ch != 10 && ch != 9) {
    if (ch >= 0 && ch <= 31) {
      buffer[(*size)] = '^';
      (*size)++;
      buffer[(*size)] = 64 + (int)ch;
      (*size)++;
      buffer[(*size)] = '\0';
    } else if (ch == 127) {
      buffer[(*size)] = '^';
      (*size)++;
      buffer[(*size)] = '?';
      (*size)++;
      buffer[(*size)] = '\0';
    }
  } else {
    buffer[(*size)] = ch;
    (*size)++;
    buffer[(*size)] = '\0';
  }
  if (ch != '\n') {
    *s_flag_supporter = 0;
  }
}

void SFlagProcessHelper(data_flags df, int* count_lines, char* buffer, char ch,
                        int* size) {
  if (df.n_flag == 1) {
    for (int i = 0; i < 6 - CountDigits(*count_lines); i++) {
      printf(" ");
    }
    printf("%d\t", *count_lines);
    (*count_lines)++;
  } else if (df.b_flag == 1 && (*size) > 1) {
    for (int i = 0; i < 6 - CountDigits(*count_lines); i++) {
      printf(" ");
    }
    printf("%d\t", *count_lines);
    (*count_lines)++;
  }
  if (df.E_flag == 1 && ch == '\n') {
    (*size)--;
    buffer[(*size)] = '$';
    (*size)++;
    buffer[(*size)] = '\n';
    (*size)++;
    buffer[(*size)] = '\0';
  }
  printf("%s", buffer);
}

void SFlagProcessHelperForFiles(data_flags df, int* count_lines, char* buffer,
                                char ch, int* size, int* endliner,
                                int code_get) {
  if (df.n_flag == 1 && *endliner == 0) {
    for (int i = 0; i < 6 - CountDigits(*count_lines); i++) {
      printf(" ");
    }
    printf("%d\t", *count_lines);
    (*count_lines)++;
    *endliner = 1;
  } else if (df.b_flag == 1 &&
             ((*size) > 1 || (code_get == EOF && (*size) > 0)) &&
             *endliner == 0) {
    for (int i = 0; i < 6 - CountDigits(*count_lines); i++) {
      printf(" ");
    }
    printf("%d\t", *count_lines);
    (*count_lines)++;
    *endliner = 1;
  }
  if (df.E_flag == 1 && ch == '\n') {
    (*size)--;
    buffer[(*size)] = '$';
    (*size)++;
    buffer[(*size)] = '\n';
    (*size)++;
    buffer[(*size)] = '\0';
  }
  for (int i = 0; i < (*size); i++) {
    printf("%c", buffer[i]);
  }
}

void StdProcess(data_flags df, int* count_lines) {
  char ch;
  int capacity = 10;
  int size = 0;
  FILE* code_freopen = freopen("/dev/tty", "r", stdin);
  if (code_freopen == NULL) {
    printf("freopen error\n");
  }
  int s_flag_supporter = 0;
  char* buffer = calloc(capacity, sizeof(char) * capacity);
  if (buffer == NULL) {
    printf("calloc error\n");
  }
  int code_get = getchar();
  ch = (char)code_get;
  while (code_get != EOF || size != 0) {
    if (capacity <= size + 1) {
      CapacityObserver(&capacity, &buffer, size);
    }

    if (code_get != EOF) {
      StrProcessingHelper(df, ch, buffer, &size, &s_flag_supporter);
    }
    if ((ch == '\n' || code_get == EOF)) {
      if (s_flag_supporter == 0) {
        SFlagProcessHelper(df, count_lines, buffer, ch, &size);
      }
      for (int i = 0; i < size; i++) {
        buffer[i] = '\0';
      }

      if (df.E_flag == 1 && ch == '\n') {
        size--;
      }

      if (df.s_flag == 1 && size <= 1) {
        s_flag_supporter = 1;
      }

      size = 0;
    }

    code_get = getchar();
    ch = (char)code_get;
  }

  free(buffer);
}

void CapacityObserver(int* capacity, char** buffer, int size) {
  if (*capacity <= size + 1) {
    (*capacity) += 2;
    (*capacity) *= 2;
    *buffer = realloc(*buffer, (*capacity) * (sizeof(char) + 10));
    ReallocErrorHandler(*buffer);
    (*buffer)[size + 1] = '\0';
  }
}

void FileProcessHelper(int* size, data_flags df, char ch, int* s_flag_supporter,
                       int* endliner, char* buffer) {
  for (int i = 0; i < (*size); i++) {
    buffer[i] = '\0';
  }

  if (df.E_flag == 1 && ch == '\n') {
    (*size)--;
  }

  if (df.s_flag == 1 && (*size) <= 1) {
    *s_flag_supporter = 1;
  }

  if (ch == '\n') {
    *endliner = 0;
  }
}

void FileProcess(char* filename, data_flags df, int* count_lines,
                 int* endliner) {
  FILE* p_file = fopen(filename, "r");
  if (p_file != NULL) {
    char ch;
    int capacity = 10;
    int size = 0;

    int s_flag_supporter = 0;
    char* buffer = calloc(capacity, sizeof(char) * capacity);
    ReallocErrorHandler(buffer);
    int code_get = fgetc(p_file);
    ch = (char)code_get;
    while (code_get != EOF || size != 0) {
      if (capacity <= size + 1) {
        CapacityObserver(&capacity, &buffer, size);
      }

      if (code_get != EOF) {
        StrProcessingHelper(df, ch, buffer, &size, &s_flag_supporter);
      }
      if ((ch == '\n' || code_get == EOF)) {
        if (s_flag_supporter == 0) {
          SFlagProcessHelperForFiles(df, count_lines, buffer, ch, &size,
                                     endliner, code_get);
        }
        FileProcessHelper(&size, df, ch, &s_flag_supporter, endliner, buffer);
        size = 0;
      }

      code_get = fgetc(p_file);
      ch = (char)code_get;
    }

    free(buffer);
    fclose(p_file);
  } else {
    printf("s21_cat: %s: no such file or directory", filename);
  }
}

void CatProcess(int argc, char* argv[]) {
  char* flags = NULL;
  char** files = NULL;
  int count_lines = 1;
  int endliner = 0;
  parse_result pr = ParseArgs(argc, argv, &flags, &files);
  data_flags df = {0, 0, 0, 0, 0, 0};
  FillDataFlags(flags, &df);

  if (pr.code_error < 0) {
  } else if (pr.count_files == 0) {
    StdProcess(df, &count_lines);
  } else {
    for (int i = 0; i < pr.count_files; i++) {
      if (strcmp(files[i], "-") == 0) {
        StdProcess(df, &count_lines);
      } else {
        FileProcess(files[i], df, &count_lines, &endliner);
      }
    }
  }

  Deallocate(flags, files);
}

int CountDigits(int number) {
  if (number == 0) return 1;
  int count = 0;
  while (number != 0) {
    number /= 10;
    count++;
  }
  return count;
}