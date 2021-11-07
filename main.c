#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

struct termios orig_termios;

/*
 * The disableRawMode function changes the terminal from raw mode to
 * canonical mode to restore the users terminal original state
 */
void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

/*
 * The enableRawMode function changes the terminal from canonical mode
 * to raw mode so that we don't need to type enter after every character
 * to read it
 */
void enableRawMode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
    enableRawMode();
    char c;
    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q');
    return 0;
}
