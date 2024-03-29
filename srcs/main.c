/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sclolus <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/25 16:54:59 by sclolus           #+#    #+#             */
/*   Updated: 2019/11/04 02:12:11 by sclolus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include "colors.h"
#include <assert.h>

static t_mlx_data g_mlx_data;
static t_image_frame g_frame;
univers				*g_univers;

double	clamp(double x, double min, double max);
int32_t	lerp(double x1, double x2, double x, double y1, double y2);
double		vector2d_magnitude(const t_2d_vector a);
t_2d_vector vector2d_multiply(const t_2d_vector a, const t_2d_vector b);
bool	double_epsilon_eq(double a, double b, double epsilon);
uint32_t	warping_line(double x);


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

uint8_t	g_global_alpha = 0;
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
				((uint32_t *)(g_frame.buffer))[projected_x + projected_y * WINDOW_WIDTH] = ((~0xff000000 & ~color) | alpha_mask) | 0xFF0000;
			}
		}

		if (x >= 0 && y >= 0
			&& x < WINDOW_WIDTH && y < WINDOW_HEIGHT) {
			((uint32_t *)(g_frame.buffer))[x + y * WINDOW_WIDTH] = color | alpha_mask;
		}
	}

}

double		stereographic_projection(t_2d_vector point, double radius)
{
	return point.x / (1.0 - point.y) * radius;
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
	double squared_distance = dx * dx + dy * dy;

	/* double projected_point = stereographic_projection(vector2d_new(f_x, f_y), sqrt(squared_distance)); */

	if (squared_distance <= obj->circle.radius * obj->circle.radius) {
		/* uint32_t	color; */

		/* color = obj->color; */
		/* color = warping_line(projected_point); */

		double	color_space_scale = 256.0 * 20.0;
		uint32_t	color = color_line(vector2d_scalar_divide(obj->pos, color_space_scale),
									   vector2d_scalar_divide(vector2d_normalize(obj->velocity),
															  color_space_scale),
									   vector2d_magnitude(obj->velocity) / color_space_scale);

		/* if (double_epsilon_eq(squared_distance, obj->circle.radius * obj->circle.radius, 2.00)) { */
		/* 	color = lerp(-1.0, 1.0, projected_point, 0x1, 0xff); */
		/* } */

		pixel_put(x, y, color);
	}
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


static void line_predicate(int32_t x, int32_t y, void *private)
{
	object *obj = ((object*)private);

	assert(obj->kind == LINE);
	double c = /* obj->pos.x * obj->line.dir.x + obj->pos.y * obj->line.dir.y */ obj->pos.y;
	double a, b;
	const double scaling_factor = g_univers->scaling_factor;
	const double epsilon = clamp(1.0 / scaling_factor, 0.0, 1.0) * 3;

	a = obj->line.dir.y;
	b = 1.0;

	double f_x = (double)x / scaling_factor;
	double f_y = (double)y / scaling_factor;

	f_x += g_univers->cam.x;
	f_y += g_univers->cam.y;
	f_x = (f_x - obj->pos.x) / obj->line.dir.x;

	if (double_epsilon_eq((f_x) * -a + b * (f_y), c, epsilon)) {
		t_2d_vector	point = vector2d_new(f_x, f_y);
		double mag = vector2d_distance(obj->pos, point);


		uint32_t	color = color_line(obj->pos, obj->line.dir, mag  / (double)256);
		pixel_put(x, y, color);
	}
}

static void	color_plan_predicate(int32_t x, int32_t y, void *private)
{
	const double	scaling_factor = g_univers->scaling_factor;
	const double	f_x = (double)x / (scaling_factor) + g_univers->cam.x;
	const double	f_y = (double)y / (scaling_factor) + g_univers->cam.y;
	object			*plan = (object *)private;
	t_2d_vector		point = vector2d_scalar_divide(vector2d_sub(vector2d_new(f_x, f_y), plan->pos), plan->color_plan.scale );

	point = vector2d_new(fabs(point.x), fabs(point.y));

	uint32_t color = color_plan(point);

# define COLOR_PLAN_GRID_WIDTH 3
# define CPLAN_WIDTH COLOR_PLAN_GRID_WIDTH
	/* uint32_t	grid_x = (uint32_t)(fabs(f_x)) / CPLAN_WIDTH * CPLAN_WIDTH; */
	/* uint32_t	grid_y = (uint32_t)(fabs(f_y)) / CPLAN_WIDTH * CPLAN_WIDTH; */

	/* if ((grid_x) % 256 == 0 */
	/* 	|| (grid_y) % 256 == 0) { */
	/* 	pixel_put(x, y, 0x0); */
	/* } else { */
	pixel_put(x, y, color);
	/* } */

}

void	draw_line(object *obj)
{
	t_rectangle rec = {
		.min = {.x = 0, .y = 0},
		.max = {.x = WINDOW_WIDTH, .y = WINDOW_HEIGHT},
	};

	rectangle_map(rec, &line_predicate, obj);
}

void	draw_color_plan(object *obj)
{
	t_rectangle rec = {
		.min = {.x = 0, .y = 0},
		.max = {.x = WINDOW_WIDTH, .y = WINDOW_HEIGHT - WINDOW_HEIGHT / 4},
	};

	rectangle_map(rec, &color_plan_predicate, obj);
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
	case COLOR_PLAN:
		draw_color_plan(obj);
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

t_2d_vector vector2d_zero(void)
{
	return vector2d_new(0.0, 0.0);
}

t_2d_vector vector2d_abs(const t_2d_vector a)
{
	return (t_2d_vector) {
		.x = fabs(a.x),
			.y = fabs(a.y),
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
	static char		buffer[256];
	const uint32_t	hud_color = 0x0;

	snprintf(buffer, sizeof(buffer) - 1, "objects: %u", univers->nbr_objects);
	mlx_string_put(g_mlx_data.connector, g_mlx_data.win, 50, 100, hud_color, buffer);

	t_2d_vector pos = univers->objects[univers->current_follow].pos;
	snprintf(buffer, sizeof(buffer) - 1, "pos: .x = %lf .y = %lf", pos.x, pos.y);
	mlx_string_put(g_mlx_data.connector, g_mlx_data.win, 50, 115, hud_color, buffer);

	pos = univers->cam;
	snprintf(buffer, sizeof(buffer) - 1, "pos: .x = %lf .y = %lf", pos.x, pos.y);
	mlx_string_put(g_mlx_data.connector, g_mlx_data.win, 50, 190, hud_color, buffer);

	snprintf(buffer, sizeof(buffer) - 1, ".scaling_factor: %lf", univers->scaling_factor);
	mlx_string_put(g_mlx_data.connector, g_mlx_data.win, 50, 130, hud_color, buffer);

	snprintf(buffer, sizeof(buffer) - 1, ".time_ratio: %lf", univers->time_ratio);
	mlx_string_put(g_mlx_data.connector, g_mlx_data.win, 50, 145, hud_color, buffer);

	snprintf(buffer, sizeof(buffer) - 1, ".collisions_number: %u", collisions_number);
	mlx_string_put(g_mlx_data.connector, g_mlx_data.win, 50, 160, hud_color, buffer);

	snprintf(buffer, sizeof(buffer) - 1, ".alpha: %hhx", g_global_alpha);
	mlx_string_put(g_mlx_data.connector, g_mlx_data.win, 50, 175, hud_color, buffer);

	if (univers->objects[univers->current_follow].kind == LINE)
	{		snprintf(buffer, sizeof(buffer) - 1, ".type: %s", ENUM_STRING(LINE));
	mlx_string_put(g_mlx_data.connector, g_mlx_data.win, 50, 25, hud_color, buffer);
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
	object->color = lerp(0.0, 100.0, vector2d_magnitude(object->velocity), 0, 0xFF) | 0xFF00;
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

void	univers_apply_elapsed_time(univers *univers, double elapsed_time)
{
	const double	angle = ROTATIONS_PER_SEC * (elapsed_time / g_univers->time_ratio);
	t_2d_vector	screen_center = vector2d_new((double)WINDOW_WIDTH / 2.0, (double)WINDOW_HEIGHT / 2.0);
	symetry_point = vector2d_rotate(symetry_point, screen_center, angle);
	symetry_axis = vector2d_sub(symetry_point, screen_center);


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
	/* object->line.dir = vector2d_rotate(object->line.dir, center_of_rotation, rotation->angle); */
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

uint32_t	warping_line(double x)
{
	double		color = x + WINDOW_WIDTH / 2;
	uint32_t	dist = (uint32_t)(color / WINDOW_WIDTH);

	color = dist << 16 | dist << 8 | dist;
	return color;
}

void		print_wrapping_line(void)
{
	uint32_t	y = WINDOW_HEIGHT / 2 - 10;

	while (y < WINDOW_HEIGHT / 2) {
		uint32_t	i = 0;

		while (i < WINDOW_WIDTH)
		{
			uint32_t	color = warping_line((double)i - (double)(WINDOW_WIDTH/2));
			pixel_put(i, y, color);
			i++;
		}
		y++;
	}
}

void	init_univers(univers *univers)
{
	struct s_object line =  {
		.color = 0x0,
		.kind = LINE,
		.pos = {
			.x = 0,
			.y = 0,
		},
		.line = {
			.dir = {
				.x = 0,
				.y = 1,
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

	(void)line;
	for (uint32_t i = 0; i < DEFAULT_OBJECT_NUMBER; i++) {
		struct s_object object = {
			.color = 0x2000FFFF,
			.kind = CIRCLE,
			/* .kind = COLOR_PLAN, */
			/* .kind = LINE, */
			.pos = {
				.x = rand() % (int32_t)(WINDOW_WIDTH / BASE_SCALING_FACTOR) * 10,
				.y = rand() % (int32_t)(WINDOW_HEIGHT / BASE_SCALING_FACTOR) * 10,
				/* .x = 0, */
				/* .y = 0, */
				/* .x = 0 + rand() % 2, */
				/* .y = 0 + rand() % 2, */
			},
			.circle = {
				.radius = 100 + i / 60 * 10,
			},
			/* .line = { */
			/* 	.dir = { */
			/* 		.x = rand() % 200 - 100.0, */
			/* 		.y = rand() % 200 - 100.0, */
			/* 	}, */
			/* }, */
			/* .color_plan = { */
			/* 	.scale = 256.0 */
			/* }, */
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
		};
		univers_add_object(univers, object);
	}

	/* univers_add_object(univers, line); */

	collisions_number = 0;
	univers->cam.x = 0;
	univers->cam.y = 0;
	univers->scaling_factor = BASE_SCALING_FACTOR;
	/* univers->time_ratio = 1; */
}

void	limit_position_by_radius(object *obj, void *private)
{
	const double	limiting_radius = *(double *)private;

	t_2d_vector		zero = vector2d_zero();

	if (vector2d_distance(obj->pos, zero) > limiting_radius) {
		t_2d_vector	dir = vector2d_scalar_multiply(vector2d_normalize(vector2d_sub(obj->pos, zero)), -1 * limiting_radius);

		(void)dir;
		/* obj->pos = dir; */
		obj->velocity = vector2d_scalar_multiply(obj->velocity, -1);
	}
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
		.current_follow = 0,
	};


	/* memset(g_frame.buffer, 0xff, WINDOW_WIDTH * WINDOW_HEIGHT * 4); */
	uint32_t	*pixels = g_frame.buffer;
	for (uint32_t y = 0; y < WINDOW_HEIGHT; y++) {
		for (uint32_t x = 0; x < WINDOW_WIDTH; x++) {
			pixels[y * WINDOW_WIDTH + x] = 0x00ffffff;
		}
	}
	if (old == 0) {
		g_univers = &univers;
		init_univers(&univers);
		old = clock();
		last_time = old;
	}
	new = clock();
	double elapsed_time = (double)(new - last_time) / (double)CLOCKS_PER_SEC * g_univers->time_ratio;
	/* t_2d_vector	*other_points = malloc(sizeof(t_2d_vector) * (DEFAULT_OBJECT_NUMBER - 1)); */
	/* static double	periodic = 0.0; */

	/* periodic += elapsed_time; */

	/* if (periodic > 10.0) { */
	/* 	periodic = 0.0; */
	/* } */

	/* assert(other_points); */
	/* for (uint32_t i = 0; i < DEFAULT_OBJECT_NUMBER - 1; i++) { */
	/* 	other_points[i] = g_univers->objects[i + 1].pos; */
	/* } */

	/* /\* double current_time = ; *\/ */
	/* g_univers->objects[0].pos = bezier_2d_curve(DEFAULT_OBJECT_NUMBER - 2, other_points, periodic / 10.0); */
	/* free(other_points); */
	/* const double	angle = ROTATIONS_PER_SEC * (elapsed_time * g_univers->time_ratio); */
	/* struct folding_rotation rotation = { */
	/* 	.angle = angle, */
	/* }; */



	/* univers_map_objects(g_univers, &objects_folding_rotation, (void *)&rotation); */
	/* g_univers->objects[1].line.dir = vector2d_rotate(g_univers->objects[1].line.dir, vector2d_zero(), angle); */

	univers_apply_elapsed_time(&univers, elapsed_time);
	univers_apply_gravity(&univers);
	univers_apply_acceleration(&univers);
	const double	limiting_radius = WINDOW_WIDTH * 10;
	(void)limiting_radius;
	univers_map_objects(g_univers, &limit_position_by_radius, (void *)&limiting_radius);

	(void)elapsed_time;
	last_time = new;
	if (new - old < CLOCK_FRAME_DELTA)
		return 0;
	g_univers->cam = g_univers->objects[g_univers->current_follow].pos;
	g_univers->cam.x -= ((float)WINDOW_WIDTH / (g_univers->scaling_factor * 2.0));
	g_univers->cam.y -= ((float)WINDOW_HEIGHT / (g_univers->scaling_factor * 2.0));
	draw_univers(&univers);
	draw_trajectory(&univers.objects[univers.current_follow]);
	/* print_wrapping_line(); */

	struct s_object line =  {
		.color = 0xFF0000,
		.kind = LINE,
		.pos = {
			.x = 0,
			.y = WINDOW_HEIGHT - WINDOW_HEIGHT / 8,
		},
		.line = {
			.dir = {
				.x = 1,
				.y = 0,
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

	draw_object(&line);
	mlx_put_image_to_window(g_mlx_data.connector, g_mlx_data.win, g_frame.frame, 0, 0);
	draw_univers_hud(&univers);
	old = new;
	return (1);
}

uint64_t	factorial(uint64_t n)
{
	uint64_t	fac = 1;

	while (n > 1) {
		fac *= n;
		n--;
	}
	return fac;
}

uint64_t	binomial_coefficient(uint64_t n, uint64_t k)
{
	assert(n >= k);
	return factorial(n) / (factorial(k) * factorial(n - k));
}

void		bernstein_basis_polynomials(uint64_t n)
{
	uint64_t	current_v = 0;
	char		polynomial[256];

	while (current_v <= n)
	{
		uint64_t   coef = binomial_coefficient(n, current_v);

		snprintf(polynomial, sizeof (polynomial), "%llux^%llu (1 - x)^%llu", coef, current_v, n - current_v);
		current_v++;
	}
}

double		bernstein_basis_polynomial(uint64_t n, uint64_t k, double x)
{
	assert(n >= k);
	return (double)binomial_coefficient(n, k) * pow(x, k) * pow(1.0 - x, n - k);
}

t_2d_vector	bezier_2d_curve(uint64_t order, t_2d_vector *control_points, double t)
{
	uint64_t	i = 0;
	t_2d_vector	bezier_point = vector2d_zero();

	while (i <= order)
	{
		double		current_bernstein_polynomial = bernstein_basis_polynomial(order, i, t);
		t_2d_vector	current_point				 = vector2d_scalar_multiply(control_points[i], current_bernstein_polynomial);

		bezier_point = vector2d_add(bezier_point, current_point);
		i++;
	}

	return bezier_point;
}

int	main(int argc, char **argv)
{
	/* pthread_t			thread; */
	t_mlx_data			mlx_data;
	t_image_frame		*frames;
	/* t_mem_block			*data; */

	color_automata	automata = color_automata_new(vector3d_new(1/2.0, 1/2.0, 1/2.0), vector3d_new(-2, 0.5, 0));

	uint32_t color = evaluate_color_automata(&automata, 0.0);
	printf("Automata returned color: %x\n", color);

	(void)argv;
	/* bernstein_basis_polynomials(4); */
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
		mlx_hook(mlx_data.win, BUTTONPRESS, BUTTONPRESSMASK, &ft_handler_mouse
				 , NULL);
		mlx_hook(mlx_data.win, MOTIONNOTIFY, 0, &ft_handler_mouse_motion
				 , NULL);

		mlx_loop(mlx_data.connector);
	}
}
