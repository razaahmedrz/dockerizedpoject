#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define MAX_WORD_LEN 50
#define MAX_GUESSES 6

char word[MAX_WORD_LEN+1];  // word to be guessed
char guesses[MAX_WORD_LEN+1];  // correct guesses
int wrong_guesses;  // number of incorrect guesses

void draw_hangman(int wrong_guesses) {
  // ASCII art for hangman stages
  char *hangman_art[] = {
    "   _____\n"
    "   |   |\n"
    "       |\n"
    "       |\n"
    "       |\n"
    "       |\n"
    "       |\n"
    "_______|_",
    "   _____\n"
    "   |   |\n"
    "   O   |\n"
    "       |\n"
    "       |\n"
    "       |\n"
    "       |\n"
    "_______|_",
    "   _____\n"
    "   |   |\n"
    "   O   |\n"
    "   |   |\n"
    "       |\n"
    "       |\n"
    "       |\n"
    "_______|_",
    "   _____\n"
    "   |   |\n"
    "   O   |\n"
    "  /|   |\n"
    "       |\n"
    "       |\n"
    "       |\n"
    "_______|_",
    "   _____\n"
    "   |   |\n"
    "   O   |\n"
    "  /|\\  |\n"
    "       |\n"
    "       |\n"
    "       |\n"
    "_______|_",
    "   _____\n"
    "   |   |\n"
    "   O   |\n"
    "  /|\\  |\n"
    "  /    |\n"
    "       |\n"
    "       |\n"
    "_______|_",
    "   _____\n"
    "   |   |\n"
    "   O   |\n"
    "  /|\\  |\n"
    "  / \\  |\n"
    "       |\n"
    "       |\n"
    "_______|_",
  };

  printf("%s", hangman_art[wrong_guesses]);
}

void init_game() {
  // read in word to be guessed
  printf("Enter a word to be guessed: ");
  scanf("%s", word);

  // initialize guesses array with underscores
  for (int i = 0; i < strlen(word); i++) {
    guesses[i] = '_';
  }
  guesses[strlen(word)] = '\0';

  wrong_guesses = 0;

}

void print_game_state() {
  draw_hangman(wrong_guesses);
  printf("\nWord: %s\n", guesses);
  printf("Incorrect Guesses: %d\n", wrong_guesses);
}

int check_guess(char guess) {
  int correct = 0;

#pragma omp parallel for reduction(+:correct)
  for (int i = 0; i < strlen(word); i++) {
    int thread_num = omp_get_thread_num();  // get the thread number of the current thread
    printf("Thread %d checking index %d\n", thread_num, i);  // print the thread number and loop index
    if (word[i] == guess) {
      guesses[i] = guess;
      correct = 1;
    }
  }

  if (!correct) {
    wrong_guesses++;
  }

  return correct;
}

int main(int argc, char* argv[]) {
  init_game();

  while (wrong_guesses < MAX_GUESSES && strchr(guesses, '_') != NULL) {
    print_game_state();

    char guess;
    printf("Enter a guess: ");
    scanf(" %c", &guess);

    int correct = check_guess(guess);
    if (correct) {
      printf("Correct guess!\n");
    } else {
      printf("Incorrect guess.\n");
    }
  }

  if (wrong_guesses == MAX_GUESSES) {
    printf("You lost! The word was %s.\n", word);
  } else {
    printf("You won! The word was %s.\n", word);
  }

  return 0;
}

// gcc -fopenmp hangman.c -o hangman
// ./hangman
//docker build -t hangman .
//docker run -it hangman
