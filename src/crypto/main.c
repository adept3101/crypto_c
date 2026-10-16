#include "encryptor.h"
#include <ncurses.h>
#include <sodium.h>
#include <string.h>

#define BOX_W 40
#define MAX_LEN (BOX_W - 2)

static void draw_button(int y, int x, const char *text, int selected) {
  if (selected)
    attron(A_REVERSE);
  mvprintw(y, x, "[ %s ]", text);
  if (selected)
    attroff(A_REVERSE);
}

static void draw_box(int y, int x, int w) {
  /* 3 строки: верх, середина, низ */
  mvaddch(y, x, ACS_ULCORNER);
  mvhline(y, x + 1, ACS_HLINE, w - 2);
  mvaddch(y, x + w - 1, ACS_URCORNER);

  mvaddch(y + 1, x, ACS_VLINE);
  mvaddch(y + 1, x + w - 1, ACS_VLINE);

  mvaddch(y + 2, x, ACS_LLCORNER);
  mvhline(y + 2, x + 1, ACS_HLINE, w - 2);
  mvaddch(y + 2, x + w - 1, ACS_LRCORNER);
}

static void draw_ui(int y, int x, const char *input, int len, int selected,
                    const char *status) {
  mvprintw(y - 2, x, "Encryptor");
  mvprintw(y - 1, x, "Input:");

  draw_box(y, x, BOX_W);
  /* %-*s затирает остатки после backspace */
  mvprintw(y + 1, x + 1, "%-*s", MAX_LEN, input);

  draw_button(y + 4, x, "Encrypt", selected);

  mvprintw(y + 6, x, "%-*s", BOX_W, status);
  mvprintw(y + 12, x, "Tab/Down/Up - move, Enter - confirm");

  if (selected) {
    curs_set(0);
  } else {
    curs_set(1);
    move(y + 1, x + 1 + len);
  }
  refresh();
}

int main(void) {
  if (sodium_init() < 0)
    return 1;

  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);

  char input[128] = {0};
  int len = 0;
  int selected = 0;
  const char *status = "";

  const int y = 5;
  const int x = 10;

  while (1) {
    draw_ui(y, x, input, len, selected, status);
    status = "";

    int ch = getch();

    if (ch == '\t') {
      selected = !selected;
    } else if (ch == KEY_DOWN) {
      selected = 1;
    } else if (ch == KEY_UP) {
      selected = 0;
    } else if (ch == '\n' || ch == KEY_ENTER) {
      if (!selected) {
        selected = 1; /* Enter в поле -> на кнопку */
      } else if (len == 0) {
        status = "Input is empty!";
      } else {
        break;
      }
    } else if (!selected) {
      if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
        if (len > 0)
          input[--len] = '\0';
      } else if (ch >= 32 && ch <= 126 && len < MAX_LEN) {
        input[len++] = (char)ch;
        input[len] = '\0';
      }
    }
  }

  unsigned char key[crypto_aead_chacha20poly1305_ietf_KEYBYTES];
  randombytes_buf(key, sizeof(key));

  size_t size_str = strlen(input);

  encrypted_data obj = encrypt((const unsigned char *)input, key);
  unsigned char *text = encode(obj.cipher_text, size_str);

  /* Результат: перенос по ширине поля */
  curs_set(0);
  mvprintw(y + 8, x, "Encrypted:");
  size_t total = strlen((const char *)text);
  int row = y + 9;
  for (size_t off = 0; off < total; off += MAX_LEN, row++) {
    int n = (total - off > MAX_LEN) ? MAX_LEN : (int)(total - off);
    mvprintw(row, x, "%.*s", n, (const char *)text + off);
  }

  mvprintw(row + 1, x, "Press any key to exit...");
  refresh();
  getch();

  endwin();
  return 0;
}
