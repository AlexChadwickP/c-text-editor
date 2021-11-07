#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <ctype.h>

struct termios orig_termios;

void die(const char *s) {
    perror(s);
    exit(1);
}

/*
 * The disableRawMode function changes the terminal from raw mode to
 * canonical mode to restore the users terminal original state
 */
void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

/*
 * The enableRawMode function changes the terminal from canonical mode
 * to raw mode so that we don't need to type enter after every character
 * to read it
 */
void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
        die("tcsetattr");
    atexit(disableRawMode);

    struct termios raw = orig_termios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= ~(CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
    enableRawMode();

    while (1) {
        char c = '\0';
        if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN)
            die("read");

        if (iscntrl(c))
            printf("%d\r\n", c);
        else
            printf("%d ('%c')\r\n", c, c);

        // Quit if character is q
        if (c == 'q') break;
    }

    return 0;
}
