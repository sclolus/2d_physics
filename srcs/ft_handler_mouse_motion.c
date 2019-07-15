#include "fdf.h"

int	ft_handler_mouse_motion(int x, int y, void *param)
{
	univers	*univers = g_univers;

	(void)param;
	(void)x;
	(void)y;
	(void)univers;
	/* univers->objects[0].pos = (t_2d_vector){ */
	/* 	.x = (float)x + univers->cam.x, */
	/* 	.y = (float)y + univers->cam.y, */
	/* }; */
	return 0;
}
