#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void test_compare_two_str(char* s1, char* s2) {
  printf("%s == %s : %d\n", s1, s2, s1 == s2);
  printf("strcmp(%s, %s) : %d\n", s1, s2, strcmp(s1, s2));
}

char* ascii_string() {
  char* ascii = (char*)malloc(sizeof(char)*129);
  for (int i = 0; i < 128; i++) {
    ascii[i] = (char) i;
  }
  ascii[128] = '\0';
  return ascii;
}

char* string_exclude_btw(char a, char b) { // Pour a = 65 et b = 122
  int length =  128 - ((int)b) + ((int)a) + 2;
  char* s = (char*)malloc(sizeof(char)*length); // Liste de taille 72
  
  for (int i = 0; i <= (int) a; i++) { // De 0 à 65 inclus : 66
    s[i] = (char) i;
  }
  
  for (int i = 0; i < 128 - ((int) b); i++) { // De 122 à 128 exclus : 6
    s[i] = (char) (i + (int) b);
  }
  // total : 72
  
  s[128 - ((int)b) + ((int)a)] = '\0'; // 110 + 1 = 111
  return s;
}


int main() {
  char s_1[] = "hello";
  char s_2[] = "hello";
  test_compare_two_str(s_1, s_2);

  char* ascii = ascii_string();
  printf("%s\n", ascii);

  char* excluded_str = string_exclude_btw('A', 'z');
  printf("%s\n", excluded_str);
  
  free(ascii);
  free(excluded_str);
  return 0;
}
