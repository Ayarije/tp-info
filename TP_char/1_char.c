#include <stdio.h>


void print_all_char() {
  for (int i = 0; i < 128; i++) {
    printf("%c ", (char) i);
  }
  printf("\n");
}


int main() {
  print_all_char();
  
  return 0;
}
