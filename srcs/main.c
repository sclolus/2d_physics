/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sclolus <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/25 16:54:59 by sclolus           #+#    #+#             */
/*   Updated: 2017/08/17 04:02:22 by sclolus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include <assert.h>

static t_mlx_data g_mlx_data;
static t_image_frame g_frame;
univers				*g_univers;

void	rectangle_map(t_rectangle rec, void (*lambda)(int32_t x, int32_t y, void *private), void *private)
{
	int32_t	min_x = (int32_t)rec.min.x;
	int32_t	min_y = (int32_t)rec.min.y;
	int32_t	max_x = (int32_t)rec.max.x;
	int32_t	max_y = (int32_t)rec.max.y;

	for (int32_t y = min_y; y <= max_y; y++) {
		for (int32_t x = min_x; x <= max_x; x++) {

			lambda(x, y, private);
		}
	}
}

inline __attribute__((always_inline)) void pixel_put(int32_t x, int32_t y, uint32_t color)
{
	x -= (int32_t)g_univers->cam.x;
	y -= (int32_t)g_univers->cam.y;
	if (x >= 0 && y >= 0
		&& x < WINDOW_WIDTH && y < WINDOW_HEIGHT)
		((uint32_t *)(g_frame.buffer))[x + y * WINDOW_WIDTH] = color;
}
static void	circle_predicate(int32_t x, int32_t y, void *private)
{
	object *obj = ((object*)private);

	assert(obj->kind == CIRCLE || obj->kind == ATTRACTOR);

	float f_x = (float)x;
	float f_y = (float)y;

	float dx = (obj->pos.x - f_x);
	float dy = (obj->pos.y - f_y);
	if (sqrt(dx * dx + dy * dy) <= obj->circle.radius * g_univers->scaling_factor)
		pixel_put(x, y, obj->color);
}

void	draw_circle(object *obj)
{
	int32_t min_x, min_y;
	int32_t max_x, max_y;

	assert(obj->kind == CIRCLE || obj->kind == ATTRACTOR);

	int32_t delta = obj->circle.radius * g_univers->scaling_factor;
	min_x = obj->pos.x - delta;
	min_y = obj->pos.y - delta;
	max_x = obj->pos.x + delta;
	max_y = obj->pos.y + delta;

	t_rectangle	rec = {
		.min = {
			.x = min_x,
			.y = min_y,
		},
		.max = {
			.x = max_x,
			.y = max_y,
		},
	};
	rectangle_map(rec, &circle_predicate, obj);
}
void	draw_object(object *obj)
{

	switch (obj->kind)
	{
	case CIRCLE:
		draw_circle(obj);
		break;
	case ATTRACTOR:
		draw_circle(obj);
		break;
	default:
		dprintf(2, "draw_object(): Error: Object kind is not supported.");
	}
}


t_2d_vector	vector2d_add(const t_2d_vector a, const t_2d_vector b)
{
	return (t_2d_vector) {
		.x = a.x + b.x,
			.y = a.y + b.y,
			};
}

t_2d_vector vector2d_sub(const t_2d_vector a, const t_2d_vector b)
{
	return (t_2d_vector) {
		.x = a.x - b.x,
			.y = a.y - b.y,
			};
}

t_2d_vector vector2d_scalar_multiply(const t_2d_vector a, float scalar)
{
	return (t_2d_vector) {
		.x = a.x * scalar,
			.y = a.y * scalar,
			};
}

t_2d_vector vector2d_scalar_divide(const t_2d_vector a, float scalar)
{
	return vector2d_scalar_multiply(a, 1.0 / scalar);
}


t_2d_vector vector2d_rotate(const t_2d_vector origin, const t_2d_vector a, float angle)
{
	t_2d_vector vec = vector2d_sub(a, origin);

	float tmp_x = vec.x * cos(angle) - vec.y * sin(angle);
	vec.y =		  tmp_x * sin(angle) + vec.y * cos(angle);


	vec.x = tmp_x;
	return vector2d_add(vec, origin);

}

float		vector2d_magnitude(const t_2d_vector a)
{
	return sqrt(a.x * a.x + a.y * a.y);
}

float		vector2d_distance(const t_2d_vector a, const t_2d_vector b)
{
	float dx = b.x - a.x;
	float dy = b.y - a.y;

	return sqrt(dx * dx + dy * dy);
}

t_2d_vector vector2d_normalize(const t_2d_vector a)
{
	return vector2d_scalar_divide(a, vector2d_magnitude(a));
}

void	apply_velocity(object *obj, float elapsed_time)
{
	const t_2d_vector d_pos = {
		.x = elapsed_time * obj->velocity.x,
		.y = elapsed_time * obj->velocity.y,
	};
	obj->pos = vector2d_add(obj->pos, d_pos);
}

void	apply_elapsed_time(object *obj, float elapsed_time)
{
	t_2d_vector new_pos = obj->pos;

	t_2d_vector d_acceleration = vector2d_scalar_multiply(obj->acceleration, (elapsed_time * elapsed_time) / 2.0);
	t_2d_vector d_velocity = vector2d_scalar_multiply(obj->velocity, elapsed_time);


	/* printf("d_acceleration: {.x = %lf, .y = %lf}\n", d_acceleration.x, d_acceleration.y); */
	new_pos = vector2d_add(new_pos, d_acceleration);
	new_pos = vector2d_add(new_pos, d_velocity);
	obj->pos = new_pos;

	obj->velocity = vector2d_add(vector2d_scalar_multiply(obj->acceleration, elapsed_time), obj->velocity);
}

void apply_gravity(object *obj, object *attractor)
{
	if (obj->kind == ATTRACTOR)
		return ;
	t_2d_vector a = vector2d_sub(attractor->pos, obj->pos);

	float distance = vector2d_distance(obj->pos, attractor->pos);
	if ((obj->kind == CIRCLE) && distance <= obj->circle.radius)
	{
		distance = obj->circle.radius;
	}
	float f = (GRAVITATIONAL_CONSTANT * attractor->mass * obj->mass) / (distance * distance);
	obj->applied_forces = vector2d_add(obj->applied_forces, vector2d_scalar_multiply(vector2d_normalize(a), f));
}

void	draw_univers_hud(univers *univers)
{
	static char buffer[256];

	snprintf(buffer, sizeof(buffer) - 1, "objects: %u", univers->nbr_objects);
	mlx_string_put(g_mlx_data.connector, g_mlx_data.win, 50, 100, 0xFFFFFF, buffer);

	snprintf(buffer, sizeof(buffer) - 1, "pos: .x = %lf .y = %lf", univers->cam.x, univers->cam.y);
	mlx_string_put(g_mlx_data.connector, g_mlx_data.win, 50, 115, 0xFFFFFF, buffer);

}
int32_t	lerp(double x1, double x2, double x, double y1, double y2)
{
	return ((int32_t)(((y1 * (x2 - x)) + (y2
										  * (x - x1))) / (x2 - x1)));
}

bool	float_epsilon_eq(float a, float b, float epsilon)
{
	return b >= a - epsilon && b <= a + epsilon;
}

bool	circle_intersection(object *a, object *b)
{
	/* assert(a->kind == CIRCLE && b->kind == CIRCLE); */

	float radius_a = (a->circle.radius/*  * g_univers->scaling_factor */) * (a->circle.radius/*  * g_univers->scaling_factor */);
	float radius_b = (b->circle.radius/*  * g_univers->scaling_factor */) * (b->circle.radius/*  * g_univers->scaling_factor */);

	float x_a = a->pos.x * a->pos.x;
	float y_a = a->pos.y * a->pos.y;

	float x_b = b->pos.x * b->pos.x;
	float y_b = b->pos.y * b->pos.y;

	float left_hand = radius_b - radius_a;
	float right_hand = x_b - x_a + y_b - y_a;

	float epsilon = 5e-1;

	return float_epsilon_eq(left_hand, right_hand, epsilon);

}

void	apply_elapsed_time_wrapper(object *object, void *private)
{
	float elapsed_time = *((float *)private);
	apply_elapsed_time(object, elapsed_time);

}

void	color_object(object *object, void *private)
{
	(void)private;
	object->color = lerp(0.0, 10000.0, vector2d_magnitude(object->velocity), 0xFF, 0xFF0000);
}

void	apply_collision(object *a, object *b, void *private)
{
	univers *univers = private;

	if (circle_intersection(a, b))
	{
		float i_r = a->circle.radius;
		float i_m = a->mass;

		a->circle.radius += b->circle.radius;
		a->mass += b->mass;
		b->circle.radius += i_r;
		b->mass += i_m;

		/* univers_remove_object(univers, u); */
		printf("There are now %u objects\n", univers->nbr_objects);
	}
}

void	univers_apply_elapsed_time(univers *univers, float elapsed_time)
{
	univers_map_objects(univers, &apply_elapsed_time_wrapper, &elapsed_time);
	univers_map_objects(univers, &color_object, NULL);
	univers_map_2d_objects(univers, &apply_collision, univers);
}

void	apply_acceleration(object *object, void *private)
{
	(void)private;
	object->acceleration = vector2d_scalar_divide(object->applied_forces, object->mass);
}

void	univers_apply_acceleration(univers *univers)
{
	univers_map_objects(univers, &apply_acceleration, NULL);
}


void	univers_map_objects(univers *univers, void (*lambda)(object *obj, void *private), void *private)
{
	uint32_t i = 0;

	while (i < univers->nbr_objects)
	{
		lambda(&univers->objects[i], private);
		i++;
	}
}


void univers_map_2d_objects(univers *univers, void (*lambda)(object *a, object *b, void *private), void *private)
{
	uint32_t	i = 0;
	object		*objects = univers->objects;

	assert(objects);
	while (i < univers->nbr_objects)
	{
		uint32_t u = 0;
		while (u < univers->nbr_objects)
		{
			if (i != u)
				lambda(&objects[i], &objects[u], private);
			u++;
		}
		i++;
	}
}


void	object_reset_forces(object *object, void *private)
{
	(void)private;
	object->acceleration = (t_2d_vector){ 0, 0 };
	object->applied_forces = (t_2d_vector){ 0, 0 };

}

void	apply_gravity_wrapper(object *a, object *b, void *private)
{
	(void)private;
	apply_gravity(a, b);
}

void	univers_apply_gravity(univers *univers)
{
	univers_map_objects(univers, &object_reset_forces, NULL);
	univers_map_2d_objects(univers, &apply_gravity_wrapper, NULL);
}

void	univers_remove_object(univers *univers, uint32_t index)
{
	if (index != univers->nbr_objects - 1)
		memmove(univers->objects + index, univers->objects + index + 1, (univers->nbr_objects - index - 1) * sizeof(object));
	univers->nbr_objects--;
}

void	univers_add_object(univers *univers, object object)
{
	uint32_t nbr_objects = univers->nbr_objects;
	if (univers->objects == NULL)
	{
		univers->nbr_objects = 0;
		assert(univers->objects = malloc(sizeof(object)));
	}
	univers->objects = realloc(univers->objects, sizeof(struct s_object) * (nbr_objects + 1));
	univers->objects[nbr_objects] = object;
	univers->nbr_objects++;
}

void	draw_object_wrapper(object *object, void *private)
{
	(void)private;
	draw_object(object);
}

void	draw_univers(univers *univers)
{
	univers_map_objects(univers, &draw_object_wrapper, NULL);
}

void	init_univers(univers *univers)
{
	static struct s_object moon = {
		.color = 0x0100FF00,
		.kind = CIRCLE,
		.pos = {
			.x = 100,
			.y = WINDOW_HEIGHT - 100,
		},
		.circle = {
			.radius = 5,
		},
		.velocity = {
			.x = 0,
			.y = -52,
		},
		.acceleration = {
			.x = 0,
			.y = 0,
		},
		.mass = 1,
		.applied_forces = {
			.x = 0,
			.y = 0,
		},
	};

	static struct s_object attractor = {
		.color = 0x00FF00,
		.kind = ATTRACTOR,
		.pos = {
			.x = WINDOW_WIDTH / 2,
			.y = WINDOW_HEIGHT / 2,
		},
		.velocity = {
			.x = 0,
			.y = 0,
		},
		.attractor = {
			.radius = 100,
		},
		.acceleration = {
			.x = 0,
			.y = 0,
		},
		.mass = 5e16,
		.applied_forces = {
			.x = 0,
			.y = 0,
		},
	};



	univers_add_object(univers, moon);
	univers_add_object(univers, attractor);

	for (uint32_t i = 0; i < DEFAULT_OBJECT_NUMBER; i++) {
		object object = {
			.color = 0x2000FFFF,
			.kind = CIRCLE,
			.pos = {
				.x = rand() % WINDOW_WIDTH,
				.y = rand() % WINDOW_HEIGHT,
			},
			.circle = {
				.radius = 10 + (i * 2) % 60,
			},
			.velocity = {
				.x = 0,
				.y = -52,
			},
			.acceleration = {
				.x = 0,
				.y = 0,
			},
			.mass = 1e16,
			.applied_forces = {
				.x = 0,
				.y = 0,
			},
		};
		univers_add_object(univers, object);
	}

	univers->cam.x = 0;
	univers->cam.y = 0;
	univers->scaling_factor = BASE_SCALING_FACTOR;
	univers->time_ratio = 1;
}

int	draw_stuff()
{
	static time_t old = 0;
	static time_t last_time = 0;
	static time_t new = 0;
	static univers	univers = {
		0,
		NULL,
		{
			.x = 0,
			.y = 0,
		},
		.scaling_factor = BASE_SCALING_FACTOR,
		.time_ratio = 1,
	};


	memset(g_frame.buffer, 0, WINDOW_WIDTH * WINDOW_HEIGHT * 4);
	if (old == 0) {
		g_univers = &univers;
		init_univers(&univers);
		old = clock();
		last_time = old;
	}
	new = clock();
	float elapsed_time = (float)(new - last_time) / (float)CLOCKS_PER_SEC * g_univers->time_ratio;

	univers_apply_elapsed_time(&univers, elapsed_time);
	univers_apply_gravity(&univers);
	univers_apply_acceleration(&univers);
	last_time = new;
	if (new - old < CLOCK_FRAME_DELTA)
		return 0;
	g_univers->cam = g_univers->objects[g_univers->current_follow].pos;
	g_univers->cam.x -= WINDOW_WIDTH / 2;
	g_univers->cam.y -= WINDOW_HEIGHT / 2;
	draw_univers(&univers);
	mlx_put_image_to_window(g_mlx_data.connector, g_mlx_data.win, g_frame.frame, 0, 0);

	draw_univers_hud(&univers);
	old = new;
	return (1);
}

int	main(int argc, char **argv)
{
	/* pthread_t			thread; */
	t_mlx_data			mlx_data;
	t_image_frame		*frames;
	/* t_mem_block			*data; */

	(void)argv;
	if (argc == 1)
	{
		srand(time(NULL));
		/* data = ft_parse_file(argv[1], argv[2]); */
		if (!(mlx_data.connector = mlx_init()))
			ft_error_exit(1, (char*[]){MLX_INIT_ERROR}, EXIT_FAILURE);
		if (!(mlx_data.win = mlx_new_window(mlx_data.connector, WINDOW_WIDTH
											, WINDOW_HEIGHT, WINDOW_NAME)))
			ft_error_exit(1, (char*[]){MLX_NEW_WIN_ERROR}, EXIT_FAILURE);
		frames = ft_get_image_frames(mlx_data.connector, NBR_IMAGE_FRAME);
		/* lines = ft_set_lines(data); */
		/* if (pthread_create(&thread, NULL */
		/* 				   , &ft_pthread_frame_clear_routine, frames)) */
		/* 	ft_error_exit(1, (char*[]){ERR_PTHREAD_FAIL}, EXIT_FAILURE); */
		/* ft_quat_rotate_points(&(t_vec){1, 0.5, 1}, -45 * PI / 180, lines); */
		/* ft_draw_lines(mlx_data.connector, mlx_data.win, frames, lines); */
		g_mlx_data = mlx_data;
		g_frame = frames[0];
		draw_stuff();
		ft_set_mlx_hooks(&mlx_data, (void*[]){mlx_data.connector
					, mlx_data.win, frames, NULL, NULL});
		mlx_loop_hook(mlx_data.connector, &draw_stuff, NULL);
		mlx_loop(mlx_data.connector);
	}
}
