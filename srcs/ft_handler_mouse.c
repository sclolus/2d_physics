#include "fdf.h"

int	ft_handler_mouse(int button, int x, int y, void *private)
{
	double scaling_factor = g_univers->scaling_factor;
	object object =  {
		.color = 0x0,
		.kind = ATTRACTOR,
		.pos = {
			.x = (x / scaling_factor + g_univers->cam.x),
			.y = (y / scaling_factor + g_univers->cam.y),
		},
		.attractor = {
			.radius = 200,
		},
		.velocity = {
			.x = 0,
			.y = 0,
		},
		.acceleration = {
			.x = 0,
			.y = 0,
		},
		.mass = 1e23,
		.applied_forces = {
			.x = 0,
			.y = 0,
		},
	};
	printf("added object at (%lf,%lf)\n", object.pos.x, object.pos.y);

	(void)button;
	(void)private;
	univers_add_object(g_univers, object);
	return 0;
}
