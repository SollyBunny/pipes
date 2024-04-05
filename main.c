
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>

#include "config.h"

struct winsize _winsize;
#define W _winsize.ws_col
#define H _winsize.ws_row

void sigwinch(int _) {
	(void)_;
	ioctl(0, TIOCGWINSZ, &_winsize);
}

typedef struct {
	int x;
	int y;
	short int dir;
	short int c;
} Pos;

Pos pos;
#ifdef OPTERASERIT
	Pos eraser;
#endif

unsigned char R = DEFAULTR;
unsigned char G = DEFAULTG;
unsigned char B = DEFAULTB;
unsigned char colors[3];
void setcolor() {
	randcolor(colors, R, G, B);
	printf("\x1b[38;2;%hhu;%hhu;%hhum", colors[0], colors[1], colors[2]);
}

char _sgetcharwaiting;
char sgetchar() {
	ioctl(0, FIONREAD, &_sgetcharwaiting);
	if (_sgetcharwaiting == 0) return 0;
	return getchar();
}

void postick(Pos *p) {
	static int randn;
	switch (p->dir) {
		case 0: 
			p->x += 1; 
			if (p->x > W) {
				p->x = 0;
				setcolor();
			}
			break;
		case 1: 
			p->y += 1; 
			if (p->y > H) {
				p->y = 0;
				setcolor();
			}
			break;
		case 2: 
			p->x -= 1; 
			if (p->x < 1) {
				p->x = W;
				setcolor();
			}
			break;
		case 3: 
			p->y -= 1;
			if (p->y < 1) {
				p->y = H;
				setcolor();
			}
			break;
	}
	randn = rand();
	if (randn % 100 > OPTCHANCE) {
		// 0 1 2 3 4 5
		// ┃ ━ ┏ ┓ ┗ ┛;
		if (randn > RAND_MAX / 2) {
			switch (p->dir) {
				case 0: p->dir = 1; p->c = 3; break;
				case 1: p->dir = 2; p->c = 5; break;
				case 2: p->dir = 3; p->c = 4; break;
				case 3: p->dir = 0; p->c = 2; break;
			}
		} else {
			switch (p->dir) { // 
				case 3: p->dir = 2; p->c = 3; break;
				case 2: p->dir = 1; p->c = 2; break;
				case 1: p->dir = 0; p->c = 4; break;
				case 0: p->dir = 3; p->c = 5; break;
			}
		}
	} else if (p->c > 1) {
		p->c = p->dir == 1 || p->dir == 3 ? 0 : 1;
	}
}

int main() {

	// Setup
	sigwinch(0);
	signal(SIGWINCH, sigwinch);
	srand(time(NULL));

	// Check for THEME_COLOR
	{
		const char* theme_color;
		if ((theme_color = getenv("THEME_COLOR"))) {
			sscanf(theme_color, "%hhu;%hhu;%hhu", &R, &G, &B);
		}
	}

	// Term
	#ifdef OPTBOLD
		printf("\x1b[?1049h\x1b[?25l\x1b[1m\x1b[2J");
	#else
		printf("\x1b[?1049h\x1b[?25l\x1b[2J");
	#endif
	struct termios tcur, tres;
	tcgetattr(0, &tcur);
	tcgetattr(0, &tres); // backup the original terminal state to restore later
	tcur.c_lflag &= ~(ICANON|ECHO|ISIG);
	tcsetattr(0, TCSANOW, &tcur);


	// Init screen
	pos.x = rand() % W;
	pos.y = rand() % H;
	#ifdef OPTERASERIT
		eraser.x = rand() % W;
		eraser.y = rand() % H;
	#endif
	setcolor();
	
	// Mainloop
	static char c;
	static unsigned int presim = OPTPRESIM;
	static unsigned char i = 0;
	while (1) {
		if (presim == 0) {
			c = sgetchar();
			switch (c) {
				case 3: // ctrl-c
					goto l_end;
			}
			#ifdef OPTERASERIT
				// do eraser
				if (++i > OPTERASERIT) {
					i = 0;
					postick(&eraser);
					printf("\x1b[%d;%dH ", eraser.y, eraser.x);
				}
			#endif
		}
		postick(&pos);
		printf("\x1b[%d;%dH%s", pos.y, pos.x, chars[pos.c]);
		if (presim == 0) {
			fflush(stdout);
			usleep(OPTTIME);
		} else {
			--presim;
		}
	}

	l_end:

	tcsetattr(0, TCSANOW, &tres);
	#ifdef OPTBOLD
		printf("\x1b[22m\x1b[?25h\x1b[0m\x1b[%dB\n\x1b[?1049l", H - pos.y);
	#else
		printf("\x1b[?25h\x1b[0m\x1b[%dB\n\x1b[?1049l", H - pos.y);
	#endif
	
	return 0;
	
}
