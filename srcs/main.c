/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sclolus <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/25 16:54:59 by sclolus           #+#    #+#             */
/*   Updated: 2019/10/31 04:40:33 by sclolus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include <assert.h>

static t_mlx_data g_mlx_data;
static t_image_frame g_frame;
univers				*g_univers;

double	clamp(double x, double min, double max);

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

bool		symetry_on = true;
t_2d_vector	symetry_axis = {
	.x = (double)WINDOW_WIDTH,
	.y = (double)WINDOW_HEIGHT,
};

t_2d_vector symetry_point = {
	.x = WINDOW_WIDTH,
	.y = WINDOW_HEIGHT,
};

uint8_t	g_global_alpha = 256 / 2;
/* t_2d_vector	defining_point = ; */

inline __attribute__((always_inline)) void pixel_put(int32_t x, int32_t y, uint32_t color)
{
	if (1) {
		double	x_f = (double)x;
		double	y_f = (double)y;

		t_2d_vector pixel_pos = vector2d_new(x_f, y_f);

		/* t_2d_vector reflected_point = vector2d_add(vector2d_reflect(vector2d_sub(pixel_pos, symetry_point), symetry_axis), symetry_point); */
		t_2d_vector screen_center = (t_2d_vector) {
			.x = WINDOW_WIDTH / 2,
			.y = WINDOW_HEIGHT / 2,
		};

		uint32_t	alpha_mask = (uint32_t)g_global_alpha << 24;
		t_2d_vector	reflected_point = vector2d_point_symetry(pixel_pos, screen_center);
		if (symetry_on) {
			int32_t	projected_x = (int32_t) (reflected_point.x);
			int32_t	projected_y = (int32_t) (reflected_point.y);

			if (!(projected_x >= 0 && projected_y >= 0
			      && projected_x < WINDOW_WIDTH && projected_y < WINDOW_HEIGHT)) {
				/* if (x < 10 && y < 10) { */
				/* warn("Reflected point is outside of window: .x = %d, .y = %d, orignal_point: .x = %d, .y = %d", projected_x, projected_y, x, y); */
				/* } */
			} else {
				((uint32_t *)(g_frame.buffer))[projected_x + projected_y * WINDOW_WIDTH] = ((~0xff000000 & ~color) | alpha_mask << 24) | 0xFF0000;
			}
		}

		if (x >= 0 && y >= 0
		    && x < WINDOW_WIDTH && y < WINDOW_HEIGHT) {
			((uint32_t *)(g_frame.buffer))[x + y * WINDOW_WIDTH] = color | alpha_mask << 24;
		}
	}

}

static void	circle_predicate(int32_t x, int32_t y, void *private)
{
	object *obj = ((object*)private);
	double scaling_factor = g_univers->scaling_factor;

	assert(obj->kind == CIRCLE || obj->kind == ATTRACTOR);

	double f_x = (double)x / (scaling_factor) + g_univers->cam.x;
	double f_y = (double)y / (scaling_factor) + g_univers->cam.y;

	double dx = (obj->pos.x - f_x);
	double dy = (obj->pos.y - f_y);

	if (dx * dx + dy * dy <= obj->circle.radius * obj->circle.radius * obj->circle.radius)
		pixel_put(x, y, obj->color);
}

void	draw_circle(object *obj)
{
	double min_x, min_y;
	double max_x, max_y;

	assert(obj->kind == CIRCLE || obj->kind == ATTRACTOR);

	double scaling_factor = g_univers->scaling_factor;
	double delta = obj->circle.radius;
	min_x = obj->pos.x - delta - g_univers->cam.x;
	min_y = obj->pos.y - delta - g_univers->cam.y;
	max_x = obj->pos.x + delta - g_univers->cam.x;
	max_y = obj->pos.y + delta - g_univers->cam.y;

	min_x *= scaling_factor;
	min_y *= scaling_factor;
	max_x *= scaling_factor;
	max_y *= scaling_factor;

	t_rectangle	rec = {
		.min = {
			.x = /* clamp( */min_x/* , 0, WINDOW_WIDTH) */,
			.y = /* clamp( */min_y/* , 0, WINDOW_HEIGHT) */,
		},
		.max = {
			.x = /* clamp( */max_x/* , 0, WINDOW_WIDTH) */,
			.y = /* clamp( */max_y/* , 0, WINDOW_HEIGHT) */,
		},
	};
	/* printf("object is in (%lf, %lf)\n", obj->pos.x, obj->pos.y); */
	/* printf("object is in (%lf, %lf) relative to the camera\n", obj->pos.x - g_univers->cam.x, obj->pos.y - g_univers->cam.y); */
	/* printf("after  clamping .min.x  = %lf, y = %lf .max = %lf, .y = %lf\n", test.min.x, test.min.y, test.max.x, test.max.y); */

	rectangle_map(rec, &circle_predicate, obj);
}


double		vector2d_magnitude(const t_2d_vector a);
t_2d_vector vector2d_multiply(const t_2d_vector a, const t_2d_vector b);
bool	double_epsilon_eq(double a, double b, double epsilon);
static void line_predicate(int32_t x, int32_t y, void *private)
{
	object *obj = ((object*)private);

	assert(obj->kind == LINE);
	double c = obj->pos.x * obj->line.dir.x + obj->pos.y * obj->line.dir.y;
	double a, b;
	const double scaling_factor = g_univers->scaling_factor;
	const double epsilon = clamp(1 / scaling_factor, 0.0, 1);

	a = obj->line.dir.x;
	b = obj->line.dir.y;

	double f_x = (double)x / scaling_factor;
	double f_y = (double)y / scaling_factor;

	f_x += g_univers->cam.x;
	f_y += g_univers->cam.y;

	/* f_x *= scaling_factor; */
	/* f_y *= scaling_factor; */
	if (double_epsilon_eq((/* obj->pos.x -  */f_x) * a + b * (/* obj->pos.y -  */f_y), c, epsilon))
		pixel_put(x, y, obj->color);
}

void	draw_line(object *obj)
{
	t_rectangle rec = {
		.min = {.x = 0, .y = 0},
		.max = {.x = WINDOW_WIDTH, .y = WINDOW_HEIGHT},
	};

	rectangle_map(rec, &line_predicate, obj);
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
	case LINE:
		draw_line(obj);
		break;
	default:
		/* dprintf(2, "draw_object(): Error: Object kind is not supported."); */
		;
	}
}

t_2d_vector vector2d_new(const double x, const double y)
{
	return (t_2d_vector){
		.x = x,
		.y = y,
	};
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

t_2d_vector vector2d_scalar_multiply(const t_2d_vector a, double scalar)
{
	return (t_2d_vector) {
		.x = a.x * scalar,
		.y = a.y * scalar,
	};
}

t_2d_vector vector2d_scalar_divide(const t_2d_vector a, double scalar)
{
	/* return vector2d_scalar_multiply(a, 1.0 / scalar); */
	return (t_2d_vector) {
		.x = a.x / scalar,
		.y = a.y / scalar,
	};
}


t_2d_vector vector2d_rotate(const t_2d_vector a, const t_2d_vector origin, const double angle)
{
	t_2d_vector vec = vector2d_sub(a, origin);

	double tmp_x = vec.x * cos(angle) - vec.y * sin(angle);
	vec.y =		  tmp_x * sin(angle) + vec.y * cos(angle);


	vec.x = tmp_x;
	return vector2d_add(vec, origin);

}

double		vector2d_magnitude(const t_2d_vector a)
{
	return sqrt(a.x * a.x + a.y * a.y);
}

t_2d_vector	vector2d_multiply(const t_2d_vector a, const t_2d_vector b)
{
	return (t_2d_vector){
		.x = a.x * b.x,
		.y = a.y * b.y,

	};
}

double		vector2d_dot_product(const t_2d_vector a, const t_2d_vector b)
{
	return a.x * b.x + b.y * a.y;
}

double		vector2d_distance(const t_2d_vector a, const t_2d_vector b)
{
	double dx = b.x - a.x;
	double dy = b.y - a.y;

	return sqrt(dx * dx + dy * dy);
}

t_2d_vector vector2d_normalize(const t_2d_vector a)
{
	return vector2d_scalar_divide(a, vector2d_magnitude(a));
}

t_2d_vector vector2d_project(const t_2d_vector a, const t_2d_vector projection_axis)
{
	const t_2d_vector normalized_projection_axis = vector2d_normalize(projection_axis);

	return vector2d_scalar_multiply(normalized_projection_axis, vector2d_dot_product(a, normalized_projection_axis));
}

t_2d_vector vector2d_reflect(const t_2d_vector a, const t_2d_vector reflection_axis)
{
	const t_2d_vector	projected_vector = vector2d_project(a, reflection_axis);

	const t_2d_vector	reflected_point = vector2d_sub(vector2d_scalar_multiply(projected_vector, 2.0), a);

	return reflected_point;
}

t_2d_vector	vector2d_point_symetry(const t_2d_vector point, const t_2d_vector point_of_symetry)
{
	t_2d_vector	translation_vector = vector2d_scalar_multiply(vector2d_sub(point_of_symetry, point), 2.0);

	return vector2d_add(point, translation_vector);
}

t_2d_vector	vector2d_slide_scalar(const t_2d_vector point, const t_2d_vector sliding_axis, const double scalar)
{
	t_2d_vector	negative_projection = vector2d_scalar_multiply(vector2d_project(point, sliding_axis), -1 * scalar);

	return vector2d_add(negative_projection, point);
}

t_2d_vector	vector2d_slide(const t_2d_vector point, const t_2d_vector sliding_axis)
{
	return vector2d_slide_scalar(point, sliding_axis, 1);
}


/* t_2d_vector	vector2d_rotate(const t_2d_vector point, const t_2d_vector rotation_point, const double angle) */
/* { */
/* 	t_2d_vector	point_relative_to_rotation_point = vector2d_sub(point, rotation_point); */
/* 	double		point_rotation_distance = vector2d_magnitude(point_relative_to_rotation_point); */

/* 	t_2d_vector rotated_point_relative_to_rotation_point = vector2d_new(cos(angle) * point_rotation_distance, sin(angle) * point_rotation_distance); */

/* 	t_2d_vector rotated_point = vector2d_add(rotated_point_relative_to_rotation_point, rotation_point); */

/* 	return rotated_point; */
/* } */

void	apply_velocity(object *obj, double elapsed_time)
{
	const t_2d_vector d_pos = {
		.x = elapsed_time * obj->velocity.x,
		.y = elapsed_time * obj->velocity.y,
	};
	obj->pos = vector2d_add(obj->pos, d_pos);
}

void	apply_elapsed_time(object *obj, double elapsed_time)
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

	double distance = vector2d_distance(obj->pos, attractor->pos);
	if (distance <= obj->circle.radius)
	{
		distance = obj->circle.radius;
	}
	double f = (GRAVITATIONAL_CONSTANT * attractor->mass * obj->mass) / (distance * distance);
	obj->applied_forces = vector2d_add(obj->applied_forces, vector2d_scalar_multiply(vector2d_normalize(a), f));
}

uint32_t	collisions_number = 0;

void	draw_univers_hud(univers *univers)
{
	static char buffer[256];

	snprintf(buffer, sizeof(buffer) - 1, "objects: %u", univers->nbr_objects);
	mlx_string_put(g_mlx_data.connector, g_mlx_data.win, 50, 100, 0xFFFFFF, buffer);

	t_2d_vector pos = univers->objects[univers->current_follow].pos;
	snprintf(buffer, sizeof(buffer) - 1, "pos: .x = %lf .y = %lf", pos.x, pos.y);
	mlx_string_put(g_mlx_data.connector, g_mlx_data.win, 50, 115, 0xFFFFFF, buffer);

	pos = univers->cam;
	snprintf(buffer, sizeof(buffer) - 1, "pos: .x = %lf .y = %lf", pos.x, pos.y);
	mlx_string_put(g_mlx_data.connector, g_mlx_data.win, 50, 190, 0xFFFFFF, buffer);

	snprintf(buffer, sizeof(buffer) - 1, ".scaling_factor: %lf", univers->scaling_factor);
	mlx_string_put(g_mlx_data.connector, g_mlx_data.win, 50, 130, 0xFFFFFF, buffer);

	snprintf(buffer, sizeof(buffer) - 1, ".time_ratio: %lf", univers->time_ratio);
	mlx_string_put(g_mlx_data.connector, g_mlx_data.win, 50, 145, 0xFFFFFF, buffer);

	snprintf(buffer, sizeof(buffer) - 1, ".collisions_number: %u", collisions_number);
	mlx_string_put(g_mlx_data.connector, g_mlx_data.win, 50, 160, 0xFFFFFF, buffer);

	snprintf(buffer, sizeof(buffer) - 1, ".alpha: %hhx", g_global_alpha);
	mlx_string_put(g_mlx_data.connector, g_mlx_data.win, 50, 175, 0xFFFFFF, buffer);

	snprintf(buffer, sizeof(buffer) - 1, ".lifetime: %lf", univers->lifetime);
	mlx_string_put(g_mlx_data.connector, g_mlx_data.win, 50, 205, 0xFFFFFF, buffer);


	if (univers->objects[univers->current_follow].kind == LINE)
	{		snprintf(buffer, sizeof(buffer) - 1, ".type: %s", ENUM_STRING(LINE));
		mlx_string_put(g_mlx_data.connector, g_mlx_data.win, 50, 175, 0xFFFFFF, buffer);
	}



}
int32_t	lerp(double x1, double x2, double x, double y1, double y2)
{
	return ((int32_t)(((y1 * (x2 - x)) + (y2
					      * (x - x1))) / (x2 - x1)));
}

bool	double_epsilon_eq(double a, double b, double epsilon)
{
	return b >= a - epsilon && b <= a + epsilon;
}

bool	circle_intersection(object *a, object *b)
{
	/* assert(a->kind == CIRCLE && b->kind == CIRCLE); */
	/* double radius_a = (a->circle.radius/\*  * g_univers->scaling_factor *\/) * (a->circle.radius/\*  * g_univers->scaling_factor *\/); */
	/* double radius_b = (b->circle.radius/\*  * g_univers->scaling_factor *\/) * (b->circle.radius/\*  * g_univers->scaling_factor *\/); */

	/* double x_a = a->pos.x * a->pos.x; */
	/* double y_a = a->pos.y * a->pos.y; */

	/* double x_b = b->pos.x * b->pos.x; */
	/* double y_b = b->pos.y * b->pos.y; */

	/* double left_hand = radius_b - radius_a; */
	/* double right_hand = x_b - x_a + y_b - y_a; */

	/* const double epsilon = 5e-1; */

	t_2d_vector total = vector2d_sub(a->pos, b->pos);

	double right_hand = a->circle.radius + b->circle.radius;
	double left_hand = vector2d_magnitude(total);

	/* printf("left_hand = %lf, right_hand = %lf\n", left_hand, right_hand); */
	return /* double_epsilon_eq(left_hand, right_hand, epsilon); */
		right_hand >= left_hand;

}

bool	circle_line_intersection(object *circle, object *line/* , uint32_t *intersection_number, t_2d_vector *intersections */)
{
	(void)circle;
	(void)line;
	if (line->kind == CIRCLE)
	{
		object *tmp = circle;
		circle = line;
		line = tmp;
	}
	assert(circle->kind == CIRCLE || circle->kind == ATTRACTOR);
	assert(line->kind == LINE);

	double r, b, c, a;

	r = circle->circle.radius;
	a = line->line.dir.x;
	b = line->line.dir.y;
	c = line->pos.x * a + line->pos.y * b - a * circle->pos.x - b * circle->pos.y;
	double r_squared = r * r;

	double delta = r_squared * (b * b + a * a) - c * c;

	const double epsilon = 1e-10;

	double x1, y1, x2, y2;

	if (double_epsilon_eq(delta, 0.0, epsilon)) {
		x1 = a * c / (a * a + b * b);
		x1 = b * c / (a * a + b * b);
		return true;
	} else if (delta > 0.0) {
		x1 = (a * c + b * sqrt(delta)) / (b * b + a * a);
		x2 = (a * c - b * sqrt(delta)) / (b * b + a * a);
		y1 = (b * c + a * sqrt(delta)) / (b * b + a * a);
		y2 = (b * c - a * sqrt(delta)) / (b * b + a * a);
		return true;
	}
	return false;
}

void	apply_elapsed_time_wrapper(object *object, void *private)
{
	double elapsed_time = *((double *)private);
	apply_elapsed_time(object, elapsed_time);

}

void	color_object(object *object, void *private)
{
	(void)private;
	object->color = lerp(0.0, 100.0, vector2d_magnitude(object->velocity), 0xA, 0xFF) << 8;
}

void	lifetime_update(object *object, void *private)
{
	double elapsed_time = *((double *)private);
	
	object->lifetime -= elapsed_time;
}


void	particle_death(object *object, void *private)
{
	univers *univers = private;
	uint32_t index = (object - univers->objects) / sizeof(object);
	
	if (object->lifetime <= 0.0f) {
		univers_remove_object(univers, index);
	}
}


void	apply_collision(object *a, object *b, void *private)
{
	univers *univers = private;
	t_f_intersection	intersections[NBR_SHAPES][NBR_SHAPES] = {
		{&circle_intersection, NULL, &circle_intersection, &circle_line_intersection, NULL},
		{NULL, NULL, NULL, NULL, NULL},
		{&circle_intersection, NULL, &circle_intersection, &circle_line_intersection, NULL},
		{&circle_line_intersection, NULL, &circle_line_intersection,NULL /* &line_intersection */, NULL},
		{NULL, NULL, NULL, NULL, NULL},
	};

	(void)univers;

	if (intersections[a->kind][b->kind] == NULL) {
		dprintf(2, "Could not apply_collision because F_intersection_(%d,%d) does not exists\n", a->kind, b->kind);
		return ;
	}
	if (intersections[a->kind][b->kind](a, b))
	{
		/* g_univers->time_ratio = 0; */

		collisions_number++;
		/* a->circle.radius += b->circle.radius; */
		/* a->mass += b->mass; */
		/* b->circle.radius += i_r; */
		/* b->mass += i_m; */

		/* if (i < u) */
		/* { */
		/* 	univers_remove_object(univers, u); */
		/* 	univers_remove_object(univers, i); */

		/* } else { */
		/* 	univers_remove_object(univers, i); */
		/* 	univers_remove_object(univers, u); */
		/* } */
		/* printf("There are now %u objects\n", univers->nbr_objects); */
	}
}

object	random_particle(void) {

	object object = {
		.color = 0x2000FFFF,
		.kind = CIRCLE,
		.pos = {
			.x = rand() % (int32_t)(WINDOW_WIDTH / BASE_SCALING_FACTOR * 5),
			.y = rand() % (int32_t)(WINDOW_HEIGHT / BASE_SCALING_FACTOR * 5),
			/* .x = 0 + rand() % 2, */
			/* .y = 0 + rand() % 2, */
		},
		.circle = {
			.radius = 10 + (rand() * 3 / DEFAULT_OBJECT_NUMBER) % 60,
		},
		.velocity = {
			.x = 0,
			.y = 0,
		},
		.acceleration = {
			.x = 0,
			.y = 0,
		},
		.mass = 1e18,
		.applied_forces = {
			.x = 0,
			.y = 0,
		},
		.lifetime = (double)(rand() % 5000 + 1),
	};

	return object;

}

void	univers_respawn_dead_particles(univers *univers) {
	uint32_t particles_to_spawn = DEFAULT_OBJECT_NUMBER - univers->nbr_objects;

	for (uint32_t i = 0; i < particles_to_spawn; i++) {
		
		univers_add_object(univers, random_particle());
	}
}

void	univers_apply_elapsed_time(univers *univers, double elapsed_time)
{
	const double	angle = ROTATIONS_PER_SEC * (elapsed_time / g_univers->time_ratio);
	t_2d_vector	screen_center = vector2d_new((double)WINDOW_WIDTH / 2.0, (double)WINDOW_HEIGHT / 2.0);
	symetry_point = vector2d_rotate(symetry_point, screen_center, angle);
	symetry_axis = vector2d_sub(symetry_point, screen_center);


	univers_map_objects(univers, &apply_elapsed_time_wrapper, &elapsed_time);
	univers_map_objects(univers, &color_object, NULL);
	
	univers_map_objects(univers, &lifetime_update, &elapsed_time);
	univers->lifetime += elapsed_time;
	
	univers_map_objects(univers, &particle_death, univers);
	univers_respawn_dead_particles(univers);
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

struct	folding_rotation {
	double	angle;
	object	*previous_object;
};

void	objects_folding_rotation(object *object, void *private)
{
	assert(private);

	struct folding_rotation	*rotation = (struct folding_rotation*)private;

	t_2d_vector	center_of_rotation;

	if (!rotation->previous_object) {
		center_of_rotation = vector2d_new(0, 0);
	} else {
		center_of_rotation = rotation->previous_object->pos;
	}

	object->pos = vector2d_rotate(object->pos, center_of_rotation, rotation->angle);
	rotation->previous_object = object;
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

static void	trajectory_predicate(int32_t x, int32_t y, void *private)
{
	object		*object = private;

	double		epsilon = 0.01;
	uint32_t	iter = 0;

	struct s_object		copy = *object;

	while (iter < 100)
	{
		apply_elapsed_time(&copy, epsilon);
		if (vector2d_distance(copy.pos, (t_2d_vector){.x = (double)x, .y = (double)y}) <= copy.circle.radius / 4)
			pixel_put(x + g_univers->cam.x, + g_univers->cam.y, 0xFF0000);
		iter++;
	}
}

double	clamp(double x, double min, double max)
{
	if (x < min) {
		return min;
	} else if (x > max) {
		return max;
	}
	return x;
}

void	draw_trajectory(object *object)
{
	t_2d_vector cam_dist = vector2d_sub(g_univers->cam, object->pos);
	t_rectangle rec = {
		.min = cam_dist,
		.max = cam_dist,
	};

	double		epsilon = 0.01;
	uint32_t	iter = 0;


	struct s_object		copy = *object;
	/* printf("before - .min { .x = %lf, .y = %lf} .max = {.x = %lf, .y = %lf}\n", rec.min.x, rec.min.y, rec.max.x, rec.max.y); */
	while (iter > 0)
	{
		apply_elapsed_time(&copy, epsilon);
		cam_dist = vector2d_sub(g_univers->cam, copy.pos);
		double x = cam_dist.x;
		double y = cam_dist.y;

		rec.min.x = clamp(x, 0, rec.min.x);
		rec.min.y = clamp(y, 0, rec.min.y);

		rec.max.x = clamp(x, rec.max.x, WINDOW_WIDTH);
		rec.max.y = clamp(y, rec.max.y, WINDOW_HEIGHT);

		iter++;
	}

	/* printf("after -   .min { .x = %lf, .y = %lf} .max = {.x = %lf, .y = %lf}\n", rec.min.x, rec.min.y, rec.max.x, rec.max.y); */
	rectangle_map(rec, &trajectory_predicate, object);
}

void	init_univers(univers *univers)
{
	struct s_object object =  {
		.color = 0xFFFF,
		.kind = LINE,
		.pos = {
			.x = -1,
			.y = -1,
		},
		.line = {
			.dir = {
				.x = 1,
				.y = -1,
			},
		},
		.velocity = {
			.x = 0,
			.y = 0,
		},
		.acceleration = {
			.x = 0,
			.y = 0,
		},
		.mass = 1e10,
		.applied_forces = {
			.x = 0,
			.y = 0,
		},
	};

	(void)object;
	/* univers_add_object(univers, object); */

	for (uint32_t i = 0; i < DEFAULT_OBJECT_NUMBER; i++) {
		univers_add_object(univers, random_particle());
	}

	collisions_number = 0;
	univers->cam.x = 0;
	univers->cam.y = 0;
	univers->scaling_factor = BASE_SCALING_FACTOR;
	/* univers->time_ratio = 1; */
}

static double	framerate = 0.0f;

void	draw_frame_counter() {
	static char buffer[256];

	snprintf(buffer, sizeof(buffer) - 1, "fps: %lf", framerate);
	mlx_string_put(g_mlx_data.connector, g_mlx_data.win, 50, 250, 0xFFFFFF, buffer);
}

int	draw_stuff()
{
	static time_t old = 0;
	static time_t last_time = 0;
	static time_t new = 0;

	static double	current_second = 0;
	static uint32_t	frame_count  = 0;
	static univers	univers = {
		0,
		NULL,
		{
			.x = 0,
			.y = 0,
		},
		.scaling_factor = BASE_SCALING_FACTOR,
		.time_ratio = 1,
		.current_follow = 0,
	};


	memset(g_frame.buffer, 0, WINDOW_WIDTH * WINDOW_HEIGHT * 4);
	if (old == 0) {
		g_univers = &univers;
		init_univers(&univers);
		old = clock();
		last_time = old;
	}
	new = clock();
	double elapsed_time = (double)(new - last_time) / (double)CLOCKS_PER_SEC * g_univers->time_ratio;
	current_second += elapsed_time;
	

	/* const double	angle = ROTATIONS_PER_SEC * (elapsed_time * g_univers->time_ratio); */
	/* struct folding_rotation rotation = { */
	/* 	.angle = angle, */
	/* }; */

	/* univers_map_objects(g_univers, &objects_folding_rotation, (void *)&rotation); */

	univers_apply_elapsed_time(&univers, elapsed_time);
	univers_apply_gravity(&univers);
	univers_apply_acceleration(&univers);

	(void)elapsed_time;
	last_time = new;
	if (new - old < CLOCK_FRAME_DELTA)
		return 0;
	/* g_univers->cam = g_univers->objects[g_univers->current_follow].pos; */
	/* g_univers->cam.x -= ((float)WINDOW_WIDTH / (g_univers->scaling_factor * 2.0)); */
	/* g_univers->cam.y -= ((float)WINDOW_HEIGHT / (g_univers->scaling_factor * 2.0)); */
	draw_univers(&univers);
	draw_trajectory(&univers.objects[univers.current_follow]);
	mlx_put_image_to_window(g_mlx_data.connector, g_mlx_data.win, g_frame.frame, 0, 0);

	draw_univers_hud(&univers);
	draw_frame_counter();
	frame_count++;

	if (current_second >= 1.0f) {
		framerate = (double)frame_count / current_second;
		frame_count = 0;
		current_second = 0.0f;
	}
	
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
		mlx_hook(mlx_data.win, ButtonPress, ButtonPressMask, &ft_handler_mouse
			 , NULL);
		mlx_hook(mlx_data.win, MotionNotify, 0, &ft_handler_mouse_motion
			 , NULL);

		mlx_loop(mlx_data.connector);
	}
}
