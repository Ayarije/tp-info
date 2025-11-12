#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int count_char(char* filename) {
  FILE* f = fopen(filename, "r");

  int count = 0;
  char c;
  int return_value = fscanf(f, "%c", &c);
  
  while (return_value != EOF) { // tant qu'on atteint pas la fin du fichier
    count++; // On incrémente count
    return_value = fscanf(f, "%c", &c); // on update return value
  }
  return count;
}

int search_word(char* word, char* text) {
  int word_length = (int) strlen(word);
  int current_length = 0;

  for (int i = 0; i < (int) strlen(text); i++) { // On parcours le texte donné 
    if (text[i] == word[current_length]) { // Pour chaque lettre consécutive valide
      current_length++; // On incrémente la longueur valide
    } else {
      current_length = 0; // Sinon on réinitialise celle-ci
    }

    if (current_length == word_length) { // Si il y a toutes les lettres du mots consécutivement
      return i - word_length + 1; // On renvoie la position de la première lettre
    }
  }
  return -1;
}

int search_word_until_end_line(char* word, char* text) {
  int word_length = strlen(word);
  int current_length = 0;

  for (int i = 0; text[i] != '\n' && text[i] != '\0'; i++) { // On parcours le texte jusqu'à la fin ou une fin de ligne
    if (text[i] == word[current_length]) { // On execute la meme methode que pour search word
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
  // On alloue une chaine de charactère de la taille du nb de charactères dans le fichier pour stocker les lignes
  char* line = (char*) malloc(sizeof(char) * count_char(filename));
  // on récupère une ligne dans "line"
  int return_value = fscanf(f, "%[^\n]\n", line);
  
  while (return_value != EOF) { // Tant qu'on a pas atteint la fin du fichier
    if (search_word_until_end_line(word, line) == -1) { continue; }

    // Si on trouve le mot on affiche la ligne correspondante
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
