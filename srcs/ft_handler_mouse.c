#include "fdf.h"

int	ft_handler_mouse(int button, int x, int y, void *private)
{
	object object =  {
		.color = 0x0,
		.kind = ATTRACTOR,
		.pos = {
			.x = x + g_univers->cam.x,
			.y = y + g_univers->cam.y,
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
	(void)button;
	(void)private;
	univers_add_object(g_univers, object);
	return 0;
}
