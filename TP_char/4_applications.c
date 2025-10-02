#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int count_char(char* filename) {
  FILE* f = fopen(filename, "r");

  int count = 0;
  char c;
  int return_value = fscanf(f, "%c", &c);
  
  while (return_value != EOF) {
    count++;
    return_value = fscanf(f, "%c", &c);
  }
  return count;
}

int search_word(char* word, char* text) {
  int word_length = (int) strlen(word);
  int current_length = 0;

  for (int i = 0; i < (int) strlen(text); i++) {
    if (text[i] == word[current_length]) {
      current_length++;
    } else {
      current_length = 0;
    }

    if (current_length == word_length) {
      return i - word_length + 1;
    }
  }
  return -1;
}

int search_word_until_end_line(char* word, char* text) {
  int word_length = strlen(word);
  int current_length = 0;

  for (int i = 0; text[i] != '\n' && text[i] != '\0'; i++) {
    if (text[i] == word[current_length]) {
      current_length++;
    } else {
      current_length = 0;
    }

    if (current_length == word_length) {
      return i - word_length + 1;
    }
  }
  return -1;
}

void search_file(char* word, char* filename) {
  FILE* f = fopen(filename, "r");
  char* line = (char*) malloc(sizeof(char) * count_char(filename));
  int return_value = fscanf(f, "%[^\n]\n", line);
  
  while (return_value != EOF) {
    if (search_word_until_end_line(word, line) == -1) { continue; }

    printf("%s\n", line);
    return_value = fscanf(f, "%[^\n]\n", line);
  }
  free(line);
}

int main() {
  char* filename = "TP_char/simple.txt";
  char* word = "do";

  search_file(word, filename);
  
  return 0;
}
