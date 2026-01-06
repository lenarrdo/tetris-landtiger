#include "gui.h"
#include "GLCD.h"
#include "tetris/core/stats.h"

#define MAX_DIGITS 6

static void visualize_stat(int x, int y, int num);
static void itoa(int n, char *s);

void visualize_top(void)
{
	visualize_stat(170, 110, high_score);
}

void visualize_score(void)
{
	visualize_stat(170, 160, score);
}

void visualize_lines(void)
{
	visualize_stat(170, 210, tot_cleared_lines);
}

void show_default_stats(void)
{
	GUI_Text(170, 90, (uint8_t *)"TOP", TEXT_COLOR, BACKGROUND_COLOR);
	visualize_top();

	GUI_Text(170, 140, (uint8_t *)"SCORE", TEXT_COLOR, BACKGROUND_COLOR);
	visualize_score();

	GUI_Text(170, 190, (uint8_t *)"LINES", TEXT_COLOR, BACKGROUND_COLOR);
	visualize_lines();
}

/********************
 * HELPER FUNCTIONS *
 ********************/

static void itoa(int n, char *s)
{
	int i = 0;
	s[MAX_DIGITS] = '\0';

	// maxout
	if (n >= 999999) {
		while (i < MAX_DIGITS)
			s[i++] = '9';
		return;
	}

	do {
		s[i++] = n % 10 + '0';
	} while ((n /= 10) > 0);

	while (i < MAX_DIGITS)
		s[i++] = '0';

	char temp;
	for (int j = 0, k = i - 1; j < k; j++, k--) {
		temp = s[j];
		s[j] = s[k];
		s[k] = temp;
	}
}

static void visualize_stat(int x, int y, int num)
{
	char strnum[10];
	itoa(num, strnum);
	GUI_Text(x, y, (uint8_t *)strnum, TEXT_COLOR, BACKGROUND_COLOR);
}
