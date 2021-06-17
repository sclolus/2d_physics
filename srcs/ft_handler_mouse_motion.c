#include "fdf.h"

int	ft_handler_mouse_motion(int x, int y, void *param)
{
	univers	*univers = g_univers;

	(void)param;
	(void)x;
	(void)y;
	(void)univers;
	/* 	symetry_point = (t_2d_vector) { */
	/* 	.x = (double)x, */
	/* 	.y = (double)y, */
	/* }; */

	/* symetry_axis = (t_2d_vector) { */
	/* 	.x = (double)x - symetry_point.x, */
	/* 	.y = (double)y - symetry_point.y, */
	/* }; */
	printf("handler_mouse_motion\n");
	/* univers->objects[0].pos = (t_2d_vector){ */
	/* 	.x = (float)x + univers->cam.x, */
	/* 	.y = (float)y + univers->cam.y, */
	/* }; */
	return 0;
}
