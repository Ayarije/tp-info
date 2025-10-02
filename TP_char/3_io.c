#include <stdio.h>


void q10_write_nb(int a, int b) {
  FILE* f = fopen("nb_list.txt", "w");
  for (int i = a; i <= b; i++) { fprintf(f, "%d\n", i); }
  fclose(f);
}

void q11_ask_sum() {
  double a, b;
  printf("Enter two float (a b): ");
  scanf("%lf %lf", &a, &b);
  printf("%f + %f = %f\n", a, b, a+b);
}

int q13_sum_file(char* filename) {
  FILE* f = fopen(filename, "r");
  int sum = 0;
  int value;
  int return_value = fscanf(f, "%d\n", &value);
  while (return_value != EOF) {
    sum = sum + value;
    return_value = fscanf(f, "%d\n", &value);
  }
  return sum;
}

void q16_cp_file(char* src_filename, char* dst_filename) {
  FILE* src_f = fopen(src_filename, "r");
  FILE* dst_f = fopen(dst_filename, "w");

  char c;
  int return_value = fscanf(src_f, "%c", &c);

  while (return_value != EOF) {
    fprintf(dst_f, "%c", c);
    return_value = fscanf(src_f, "%c", &c);
  }
  
  fclose(src_f);
  fclose(dst_f);
}

int main() {
  q10_write_nb(0, 100);
  q11_ask_sum();
  printf("sum of q10 file : %d\n", q13_sum_file("nb_list.txt"));
  q16_cp_file("nb_list.txt", "nb_list_copied.txt");
  
  return 0;
}
