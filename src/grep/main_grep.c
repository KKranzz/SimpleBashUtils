#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct option_data {
  int i_flag;
  int v_flag;
  int c_flag;
  int l_flag;
  int n_flag;
} option_data;

typedef struct parse_data {
  int count_files;
  int count_patterns;
  int error_code;

} parse_data;

void Deallocate(char *flags, char **files, char **regex_data);
int HandleOption(char *str, char **flags);
int HandleFlag(char *str, char **flags);
int EFlagExist(int argc, char *argv[]);
void FillDataFlags(char *flags, option_data *df);
parse_data ParseArgs(int argc, char *argv[], char ***regex_data,
                     char ***files_data, char **options_data);
int MatchCheck(char *pattern, char *str, option_data df);
void FileProcess(char *filename, option_data df, char **regex_data,
                 parse_data pd);
void GrepProcessing(int argc, char *argv[]);
void ParseArgHelper(char *argv_elem, int *files_only_mode, int *counter,
                    int argc, int zero_patterns, int *capacity_options,
                    char **options_data, parse_data *result);
void ParseArgHelperForEFlag(char *argv_elem, int *files_only_mode, int *counter,
                            int argc, parse_data *result, int *capacity_options,
                            char **options_data, int *pattern_now);
void ParsePatternDetect(int *capacity_regex, parse_data *result,
                        char ***regex_data, int *pattern_now, char *argv_elem);
void VariantCharReader(int file_reading, int interrupt, int *code_getchar,
                       char *ch, FILE *p_file);

int main(int argc, char *argv[]) {
  GrepProcessing(argc, argv);
  return 0;
}

void Deallocate(char *flags, char **files, char **regex_data) {
  free(flags);
  free(files);
  free(regex_data);
}

int EFlagExist(int argc, char *argv[]) {
  int e_flag = 0;

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--") == 0) {
      i = argc;
    } else if (strcmp(argv[i], "-e") == 0) {
      e_flag = 1;
      i = argc;
    }
  }

  return e_flag;
}

int HandleFlag(char *str, char **flags) {
  int code_option = 0;

  int length = strlen(str);

  int size = strlen(*flags);
  for (int i = 1; i < length; i++) {
    if (str[i] == 'i' || str[i] == 'v' || str[i] == 'c' || str[i] == 'l' ||
        str[i] == 'n') {
      (*flags)[size] = str[i];
      size++;
      (*flags)[size] = '\0';
    } else {
      code_option = 1;
      printf("s21_grep: unknown option -- %c\n", str[i]);
      i = length;
    }
  }

  return code_option;
}

int HandleOption(char *str, char **flags) {
  int index = strlen(*flags);
  int code_option = 0;

  if (strcmp(str, "--help") == 0 || strcmp(str, "--h") == 0) {
    printf("Usage: s21_grep [OPTIONS] PATTERN [FILE]...\n");
    printf("Options:\n");
    printf("-i Ignore case distinctions in pattern matching.\n");
    printf(
        "-v Invert the match; display lines that do not match the pattern.\n");
    printf("-c Count the number of matching lines.\n");
    printf("-l Display only the names of files with matching lines.\n");
    printf("-n Print the line number with output lines.\n");
    printf("-e Specify a pattern for searching.\n");
    code_option = 1;
  } else if (strcmp(str, "--version") == 0 || strcmp(str, "--v") == 0) {
    printf("s21_grep v1.0\n");
    code_option = 1;
  } else if (strcmp(str, "--ignore-case") == 0) {
    (*flags)[index] = 'c';
  } else if (strcmp(str, "--invert-match") == 0) {
    (*flags)[index] = 'i';
  } else if (strcmp(str, "--count") == 0) {
    (*flags)[index] = 'c';
  } else if (strcmp(str, "--files-with-matches") == 0) {
    (*flags)[index] = 'l';
  } else if (strcmp(str, "--live-number") == 0) {
    (*flags)[index] = 'n';
  } else {
    code_option = 1;
    printf("s21_grep: unknown option %s\n", str);
    printf("Usage: s21_grep [OPTION]... PATTERN [FILE]...\n");
    printf("Try 's21_grep --help' for more information.\n");
  }

  return code_option;
}

void FillDataFlags(char *flags, option_data *df) {
  int size = strlen(flags);
  for (int i = 0; i < size; i++) {
    if (flags[i] == 'i') {
      df->i_flag = 1;
    } else if (flags[i] == 'v') {
      df->v_flag = 1;
    } else if (flags[i] == 'c') {
      if (df->l_flag != 1) {
        df->c_flag = 1;
        df->n_flag = 0;
      }
    } else if (flags[i] == 'l') {
      df->l_flag = 1;
      df->n_flag = 0;
      df->c_flag = 0;
    } else if (flags[i] == 'n') {
      if (df->c_flag != 1 && df->l_flag != 1) {
        df->n_flag = 1;
      }
    }
  }
}

void ParseArgHelper(char *argv_elem, int *files_only_mode, int *counter,
                    int argc, int zero_patterns, int *capacity_options,
                    char **options_data, parse_data *result) {
  if (strcmp(argv_elem, "--") == 0) {
    *files_only_mode = 1;

    if (*counter == argc - 1 && zero_patterns == 1) {
      result->error_code = 1;
      printf("Usage: s21_grep [OPTION]... PATTERN [FILE]...\n");
      printf("Try 'grep --help' for more information.\n");
    }
  } else if (argv_elem[1] == '-') {
    if (*capacity_options <= (int)strlen(*options_data) + 1) {
      (*capacity_options) += 2;
      (*capacity_options) *= 2;
      *options_data =
          realloc(*options_data, (*capacity_options) * sizeof(char));
    }

    int code_err = HandleOption(argv_elem, options_data);
    if (code_err == 1) {
      result->error_code = 1;
      *counter = argc;
    }
  } else {
    if (*capacity_options <= (int)strlen(*options_data) + 1) {
      (*capacity_options) += 2;
      (*capacity_options) *= 2;
      *options_data =
          realloc(*options_data, (*capacity_options) * sizeof(char));
    }
    int code_err = HandleFlag(argv_elem, options_data);
    if (code_err == 1) {
      result->error_code = 1;
      *counter = argc;
    }
  }
}

void ParseArgHelperForEFlag(char *argv_elem, int *files_only_mode, int *counter,
                            int argc, parse_data *result, int *capacity_options,
                            char **options_data, int *pattern_now) {
  if (strcmp(argv_elem, "--") == 0) {
    *files_only_mode = 1;
  } else if (strcmp(argv_elem, "-e") == 0 ||
             strcmp(argv_elem, "--regexp") == 0) {
    *pattern_now = 1;
    if (*counter == argc - 1) {
      result->error_code = 1;
      printf("s21_grep: option requires an argument -- e\n");
      printf("Usage: s21_grep [OPTION]... PATTERN [FILE]...");
      printf("Try 's21_grep --help' for more information.\n");
    }
  } else if (argv_elem[1] == '-') {
    if (*capacity_options <= (int)strlen(*options_data) + 1) {
      (*capacity_options) += 2;
      (*capacity_options) *= 2;
      *options_data =
          realloc(*options_data, (*capacity_options) * sizeof(char));
    }

    int code_err = HandleOption(argv_elem, options_data);
    if (code_err == 1) {
      result->error_code = 1;
      *counter = argc;
    }
  } else {
    if (*capacity_options <= (int)strlen(*options_data) + 1) {
      (*capacity_options) += 2;
      (*capacity_options) *= 2;
      *options_data =
          realloc(*options_data, (*capacity_options) * sizeof(char));
    }
    int code_err = HandleFlag(argv_elem, options_data);
    if (code_err == 1) {
      result->error_code = 1;
      *counter = argc;
    }
  }
}

void EFlagMissingProcessHelper(int argc, int *files_only_mode, char *argv[],
                               int *zero_patterns, int *capacity_options,
                               char **options_data, parse_data *result,
                               int *capacity_regex, char ***regex_data,
                               int *capacity_files, char ***files_data) {
  for (int i = 1; i < argc; i++) {
    if (*files_only_mode == 0 && argv[i][0] == '-' && strlen(argv[i]) > 1) {
      ParseArgHelper(argv[i], files_only_mode, &i, argc, *zero_patterns,
                     capacity_options, options_data, result);
    } else if (*zero_patterns == 1) {
      if ((*capacity_regex) < result->count_patterns + 1) {
        (*capacity_regex) += 2;
        (*capacity_regex) *= 2;
        *regex_data = realloc(*regex_data, sizeof(char *) * (*capacity_regex));
      }

      (*regex_data)[result->count_patterns] = argv[i];
      result->count_patterns++;
      *zero_patterns = 0;
    } else {
      if (*capacity_files < result->count_files + 1) {
        (*capacity_files) += 2;
        (*capacity_files) *= 2;
        (*files_data) =
            realloc(*files_data, sizeof(char *) * (*capacity_files));
      }

      (*files_data)[result->count_files] = argv[i];
      result->count_files++;
    }
  }
}
void ParsePatternDetect(int *capacity_regex, parse_data *result,
                        char ***regex_data, int *pattern_now, char *argv_elem) {
  if (*capacity_regex < result->count_patterns + 1) {
    (*capacity_regex) += 2;
    (*capacity_regex) *= 2;
    *regex_data = realloc(*regex_data, sizeof(char *) * (*capacity_regex));
  }

  (*regex_data)[result->count_patterns] = argv_elem;
  result->count_patterns++;
  *pattern_now = 0;
}

parse_data ParseArgs(int argc, char *argv[], char ***regex_data,
                     char ***files_data, char **options_data) {
  parse_data result = {0, 0, 0};

  int capacity_options = 10, capacity_files = 10, capacity_regex = 10,
      files_only_mode = 0, e_flag_ex = EFlagExist(argc, argv);

  *options_data = calloc(capacity_options, sizeof(char));
  *files_data = calloc(capacity_files, sizeof(char *));
  *regex_data = calloc(capacity_regex, sizeof(char *));

  if (e_flag_ex == 0) {
    int zero_patterns = 1;
    EFlagMissingProcessHelper(argc, &files_only_mode, argv, &zero_patterns,
                              &capacity_options, options_data, &result,
                              &capacity_regex, regex_data, &capacity_files,
                              files_data);
  } else {
    int pattern_now = 0;
    for (int i = 1; i < argc; i++) {
      if (files_only_mode == 0 && pattern_now == 0 && argv[i][0] == '-' &&
          strlen(argv[i]) > 1) {
        ParseArgHelperForEFlag(argv[i], &files_only_mode, &i, argc, &result,
                               &capacity_options, options_data, &pattern_now);
      } else if (pattern_now == 1) {
        ParsePatternDetect(&capacity_regex, &result, regex_data, &pattern_now,
                           argv[i]);
      } else {
        if (capacity_files < result.count_files + 1) {
          capacity_files += 2;
          capacity_files *= 2;
          *files_data = realloc(*files_data, sizeof(char *) * capacity_files);
        }

        (*files_data)[result.count_files] = argv[i];
        result.count_files++;
      }
    }
  }

  return result;
}

int MatchCheck(char *pattern, char *str, option_data df) {
  int match = 0;
  regex_t regex;
  int code_compile_regex = 0;
  if (df.i_flag == 1) {
    code_compile_regex = regcomp(&regex, pattern, REG_EXTENDED | REG_ICASE);
  } else {
    code_compile_regex = regcomp(&regex, pattern, REG_EXTENDED);
  }

  if (code_compile_regex) {
    printf("Non compile regex (regcomp_error)\n");
    match = -1;
  }

  int len = strlen(str);

  char *buffer_for_regexec = calloc(len + 1, sizeof(char));
  strcpy(buffer_for_regexec, str);

  if (len > 0 && (buffer_for_regexec[len - 1] == '\n' ||
                  buffer_for_regexec[len - 1] == EOF)) {
    buffer_for_regexec[len - 1] = '\0';
  }
  buffer_for_regexec[len] = '\0';
  if (match != -1) {
    match = regexec(&regex, buffer_for_regexec, 0, NULL, 0);  // match = 0,
  }
  if (!match) {
    match = 1;
  } else if (match == REG_NOMATCH) {
    match = 0;
  } else {
    match = -1;
    printf("regexec error\n");
  }

  regfree(&regex);
  free(buffer_for_regexec);
  return match;
}

void PositiveMatchProcessing(option_data df, int *interrupt, int file_reading,
                             char *filename, int *count_match_lines,
                             parse_data *pd, char *buffer, int *counter,
                             int total_count_line) {
  if (df.l_flag == 1) {
    *interrupt = 1;
    if (file_reading == 0) {
      printf("(standart ouptut)\n");
    } else {
      printf("%s\n", filename);
    }
  } else if (df.v_flag != 1 && df.c_flag == 1) {
    (*count_match_lines)++;
  } else if (df.v_flag != 1) {
    if (df.n_flag == 1) {
      if (pd->count_files <= 1) {
        printf("%d:", total_count_line);
      } else {
        if (file_reading == 0) {
          printf("(standart_otput):%d:", total_count_line);
        } else {
          printf("%s:%d:", filename, total_count_line);
        }
      }
    } else if (pd->count_files > 1) {
      printf("%s:", filename);
    }
    printf("%s", buffer);
  }
  *counter = pd->count_patterns;
}

void NegativeMatchProcessHelper(option_data df, int *interrupt,
                                int file_reading, char *filename, parse_data pd,
                                char *buffer, int *count_match_lines,
                                int total_count_line) {
  if (df.l_flag == 1) {
    *interrupt = 1;
    if (file_reading == 0) {
      printf("(standart ouptut)\n");
    } else {
      printf("%s\n", filename);
    }
  } else if (df.c_flag != 1) {
    if (df.n_flag == 1) {
      if (pd.count_files <= 1) {
        printf("%d:", total_count_line);
      } else {
        if (file_reading == 0) {
          printf("(standart_otput):%d:", total_count_line);
        } else {
          printf("%s:%d:", filename, total_count_line);
        }
      }
    } else if (pd.count_files > 1) {
      printf("%s:", filename);
    }
    printf("%s", buffer);
  } else {
    (*count_match_lines)++;
  }
}

void TextProcess(int *capacity, int *size, char **buffer, char *ch,
                 parse_data *pd, option_data df, int *interrupt,
                 int file_reading, char *filename, int *count_match_lines,
                 char **regex_data, int *total_count_line, int *code_getchar,
                 FILE *p_file, int non_eof_reach) {
  if (*capacity <= (*size) + 1) {
    (*capacity) += 2;
    (*capacity) *= 2;
    char *temp = realloc(*buffer, sizeof(char) * (*capacity));
    if (temp != NULL) {
      (*buffer) = temp;
    }
  }

  if (*code_getchar == EOF) {
    (*buffer)[(*size)] = '\n';
  } else {
    (*buffer)[(*size)] = *ch;
  }
  (*size)++;
  (*buffer)[(*size)] = '\0';
  if ((*ch) == '\n' || non_eof_reach == 0) {
    for (int i = 0; i < pd->count_patterns; i++) {
      if (MatchCheck(regex_data[i], *buffer, df) == 1) {
        PositiveMatchProcessing(df, interrupt, file_reading, filename,
                                count_match_lines, pd, *buffer, &i,
                                *total_count_line);
      } else if (i == pd->count_patterns - 1 && df.v_flag == 1) {
        NegativeMatchProcessHelper(df, interrupt, file_reading, filename, *pd,
                                   *buffer, count_match_lines,
                                   *total_count_line);
      }
    }
    for (int i = 0; i < (*capacity); i++) {
      (*buffer)[i] = '\0';
    }
    (*size) = 0;
    (*total_count_line)++;
  }
  if (non_eof_reach == 1) {
    VariantCharReader(file_reading, *interrupt, code_getchar, ch, p_file);
  }
}

void OutputCFlagInfo(option_data df, int interrupt, parse_data pd,
                     char *filename, int count_match_lines, FILE *p_file,
                     int file_reading) {
  if (df.c_flag == 1 && interrupt != -1) {
    if (pd.count_files > 1) {
      printf("%s:%d\n", filename, count_match_lines);
    } else {
      printf("%d\n", count_match_lines);
    }
  }

  if (file_reading == 1 && interrupt != -1) {
    fclose(p_file);
  }
}

void FileProcess(char *filename, option_data df, char **regex_data,
                 parse_data pd) {
  FILE *p_file = NULL, *freopen_code = NULL;

  int file_reading = 1, interrupt = 0;

  if (strcmp("-", filename) == 0) {
    freopen_code = freopen("/dev/tty", "r", stdin);
    if (freopen_code == NULL) {
      printf("freopen error\n");
    }
    file_reading = 0;
  } else {
    p_file = fopen(filename, "r");
    if (p_file == NULL) {
      printf("s21_grep: %s: No such file or directory\n", filename);
      interrupt = -1;
    }
  }

  char ch = 'z';
  int capacity = 10, size = 0, count_match_lines = 0, total_count_line = 1,
      code_getchar = 0;

  char *buffer = calloc(capacity, sizeof(char) * capacity);

  if (file_reading == 0 && interrupt == 0) {
    code_getchar = getchar();
    ch = (char)code_getchar;
  } else if (interrupt == 0 && p_file != NULL) {
    code_getchar = fgetc(p_file);
    ch = (char)code_getchar;
  }

  while ((code_getchar != EOF) && (interrupt == 0)) {
    TextProcess(&capacity, &size, &buffer, &ch, &pd, df, &interrupt,
                file_reading, filename, &count_match_lines, regex_data,
                &total_count_line, &code_getchar, p_file, 1);
  }

  if (code_getchar == EOF && strlen(buffer) != 0) {
    TextProcess(&capacity, &size, &buffer, &ch, &pd, df, &interrupt,
                file_reading, filename, &count_match_lines, regex_data,
                &total_count_line, &code_getchar, p_file, 0);
  }

  OutputCFlagInfo(df, interrupt, pd, filename, count_match_lines, p_file,
                  file_reading);

  free(buffer);
}

void GrepProcessing(int argc, char *argv[]) {
  char **regex_data = NULL;
  char **files_data = NULL;
  char *options_data = NULL;

  parse_data pd =
      ParseArgs(argc, argv, &regex_data, &files_data, &options_data);

  option_data od = {0};
  FillDataFlags(options_data, &od);

  if (pd.error_code == 1 || pd.count_patterns == 0) {
    if (pd.count_patterns == 0) {
      printf("Missing pattern\n");
    }
  } else if (pd.count_files == 0) {
    FileProcess("-", od, regex_data, pd);
  } else {
    for (int i = 0; i < pd.count_files; i++) {
      FileProcess(files_data[i], od, regex_data, pd);
    }
  }

  Deallocate(options_data, files_data, regex_data);
}

void VariantCharReader(int file_reading, int interrupt, int *code_getchar,
                       char *ch, FILE *p_file) {
  if (file_reading == 0 && interrupt == 0) {
    (*code_getchar) = getchar();
    (*ch) = (char)(*code_getchar);
  } else if (interrupt == 0) {
    (*code_getchar) = fgetc(p_file);
    (*ch) = (char)(*code_getchar);
  }
}