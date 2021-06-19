#include "fdf.h"

int	ft_handler_mouse_motion(int x, int y, void *param)
{
	universe	*universe = g_universe;

	(void)param;
	(void)x;
	(void)y;
	(void)universe;
	/* 	symetry_point = (t_2d_vector) { */
	/* 	.x = (double)x, */
	/* 	.y = (double)y, */
	/* }; */

	/* symetry_axis = (t_2d_vector) { */
	/* 	.x = (double)x - symetry_point.x, */
	/* 	.y = (double)y - symetry_point.y, */
	/* }; */
	printf("handler_mouse_motion\n");
	/* universe->objects[0].pos = (t_2d_vector){ */
	/* 	.x = (float)x + universe->cam.x, */
	/* 	.y = (float)y + universe->cam.y, */
	/* }; */
	return 0;
}
