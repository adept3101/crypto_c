#include "ecnryptor.h"
#include <ncurses.h>
// #include <stdio.h>
// #include <string.h>

int main() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);

  char input[128] = {0};
  int len = 0;

  int y = 5;
  int x = 10;
  int width = 40;

  mvprintw(y - 1, x, "Input:");

  mvaddch(y, x, '+');
  mvaddch(y, x + width - 1, '+');
  mvhline(y, x + 1, '-', width - 2);

  mvaddch(y + 2, x, '+');
  mvaddch(y + 2, x + width - 1, '+');
  mvhline(y + 2, x + 1, '-', width - 2);

  move(y + 1, x + 1);

  while (1) {
    int ch = getch();

    if (ch == '\n' || ch == KEY_ENTER) {
      break;
    }

    if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
      if (len > 0) {
        input[--len] = '\0';
        mvaddch(y + 1, x + 1 + len, ' ');
        move(y + 1, x + 1 + len);
      }
    } else if (ch >= 32 && ch < 126 && len < width - 3) {
      input[len++] = ch;
      input[len] = '\0';

      addch(ch);
    }
  }

  unsigned char key[crypto_aead_chacha20poly1305_ietf_KEYBYTES];
  randombytes_buf(key, sizeof(key));

  // char *str = "hello";
  size_t size_str = strlen(input);

  encrypted_data obj = encrypt((const unsigned char *)input, key);
  unsigned char *text = encode(obj.cipher_text, size_str);

  move(y + 5, x + 5);

  printw("%s\n", text);

  getch();
  endwin();

  return 0;
}
