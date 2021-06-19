#include "fdf.h"

int	ft_handler_mouse(int button, int x, int y, void *private)
{
	double scaling_factor = g_universe->scaling_factor;
	object object = /*  =  { */
	/* 	.color = 0x0, */
	/* 	.kind = ATTRACTOR, */
	/* 	.pos = { */
	/* 		.x = (x / scaling_factor + g_universe->cam.x), */
	/* 		.y = (y / scaling_factor + g_universe->cam.y), */
	/* 	}, */
	/* 	.attractor = { */
	/* 		.radius = 200, */
	/* 	}, */
	/* 	.velocity = { */
	/* 		.x = 0, */
	/* 		.y = 0, */
	/* 	}, */
	/* 	.acceleration = { */
	/* 		.x = 0, */
	/* 		.y = 0, */
	/* 	}, */
	/* 	.mass = 1e23, */
	/* 	.applied_forces = { */
	/* 		.x = 0, */
	/* 		.y = 0, */
	/* 	}, */
	/* } */
		random_particle();
	printf("added object at (%lf,%lf)\n", object.pos.x, object.pos.y);
	printf("Calling ft_handler_mouse\n");


	object.pos = (t_2d_vector){
			.x = (x / scaling_factor + g_universe->cam.x),
			.y = (y / scaling_factor + g_universe->cam.y),
	};
	(void)button;
	(void)private;

	universe_add_object(g_universe, object);
	printf("Exited ft_handler_mouse \n");
	return 0;
}
