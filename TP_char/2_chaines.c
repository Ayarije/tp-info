#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int min(int a, int b) {
  if (a < b) {
    return a;
  } else {
    return b;
  }
}

int b_cmp_nb(int* n1, int* n2, int n1_len, int n2_len) {
  // Compare deux nombre de base quelconque renvoie -1 si n1 < n2, 1 si n1 > n2 et 0 si n1 = n2
  if (n1_len < n2_len) {
    return -1;
  }
  if (n1_len > n2_len) {
    return 1;
  }

  for (int i = n1_len - 1; i >= 0; i--) { // Parcours n1 et n2 du début vers la fin
    if (n1[i] == n2[i]) { continue; } // Si les 2 chiffres sont égaux passer au chiffre suivant

    if (n1[i] < n2[i]) {
      return -1;
    }
    if (n1[i] > n2[i]) {
      return 1;
    }
  }

  return 0;
}

void test_compare_two_str(char* s1, char* s2) {
  printf("%s == %s : %d\n", s1, s2, s1 == s2);
  printf("strcmp(%s, %s) : %d\n", s1, s2, strcmp(s1, s2));
}

char* ascii_string() {
  char* ascii = (char*)malloc(sizeof(char)*128);
  for (int i = 1; i < 128; i++) {
    ascii[i-1] = (char) i;
  }
  ascii[127] = '\0';
  return ascii;
}

char* string_only_btw(char a, char b) { // Pour a = 65 et b = 122
  int length =  ((int) b) - ((int) a) + 1;
  char* s = (char*) malloc(sizeof(char)*(length + 1)); // Liste de taille 
  
  for (int i = 0; i < length; i++) {
    s[i] = ((char) i + ((int) a));
  }
  
  s[length] = '\0'; // 110 + 1 = 111
  return s;
}

int my_strlen(char* s) { // Linéaire (P = nP)
  int i = 0;
  while (s[i] != '\0') {
    i++;
  }
  return i;
}

int my_strcmp(char* s1, char* s2) { // Linéaire (P = 5nP)
  int len_s1 = my_strlen(s1);
  int len_s2 = my_strlen(s2);

  int* int_s1 = (int*) malloc(sizeof(int) * len_s1);
  int* int_s2 = (int*) malloc(sizeof(int) * len_s2);

  for (int i = 0; i < len_s1; i++) {
    int_s1[i] = (int) s1[i];
  }

  for (int i = 0; i < len_s2; i++) {
    int_s2[i] = (int) s2[i];
  }
  
  int result = b_cmp_nb(int_s1, int_s2, len_s1, len_s2) * -1;

  free(int_s1);
  free(int_s2);
  return result;
}

char* my_strcpy(char* dst_string, char* src_string) { // Linéaire (P = nP)
  int len = my_strlen(src_string);
  for (int i = 0; i < len; i++) {
    dst_string[i] = src_string[i];
  }
  dst_string[len] = '\0';
  return dst_string;
}

char* my_strcat(char* s1, char* s2) {
  int len_s1 = my_strlen(s1);
  int len_s2 = my_strlen(s2);

  char* result = (char*) malloc(sizeof(char) * (len_s1 + len_s2 + 1));

  for (int i = 0; i < len_s1; i++)
  {
    result[i] = s1[i];
  }
  for (int i = len_s1; i < len_s1 + len_s2; i++)
  {
    result[i] = s2[i - len_s1];
  }
  result[len_s1 + len_s2] = '\0';
  
  return result;
}

int main() {
  char s_1[] = "hellkqn,fd";
  char s_2[] = "hello";
  test_compare_two_str(s_1, s_2);
  printf("my_strcmp(%s, %s) : %d\n", s_1, s_2, my_strcmp(s_1, s_2));

  char* concatenated = my_strcat(s_1, s_2);
  printf("\"%s\" + \"%s\" = \"%s\"\n", s_1, s_2, concatenated);

  char* dst = malloc(sizeof(char) * (my_strlen(s_1) + 1));
  dst = my_strcpy(dst, s_1);
  printf("my_strcpy(%s) = %s\n", s_1, dst);
  
  char* ascii = ascii_string();
  printf("ASCII table: %s\n", ascii);
  
  char* excluded_str = string_only_btw('A', 'z');
  printf("btw A and z: %s\n", excluded_str);
  
  free(ascii);
  free(excluded_str);
  free(dst);
  free(concatenated);
  return 0;
}
