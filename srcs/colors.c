/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sclolus <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/02 23:43:23 by sclolus           #+#    #+#             */
/*   Updated: 2019/11/03 02:12:51 by sclolus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "colors.h"

line			line_new(t_3d_vector point, t_3d_vector direction)
{
	return (line) {
		.point = point,
			.dir = direction,
			};
}

color_automata	color_automata_new(t_3d_vector starting_color, t_3d_vector direction)
{
	assert(!(starting_color.x < 0.0 || starting_color.x > 1.0 || // Is outside cube function ?
			starting_color.y < 0.0 || starting_color.y > 1.0 ||
			 starting_color.z < 0.0 || starting_color.y > 1.0));

	return (color_automata) {
		.cube_faces = {
			{{1/2, 0, 1/2}, {0, 1, 0}},
			{{1/2, 1, 1/2}, {0, -1, 0}},
			{{1/2, 1/2, 0}, {0, 0, 1}},
			{{1/2, 1/2, 1}, {0, 0, -1}},
			{{0, 1/2, 1/2}, {1, 0, 0}},
			{{1, 1/2, 1/2}, {-1, 0, 0}},
		},
		.starting_line = line_new(starting_color, direction),
	};
}

uint32_t	evaluate_color_automata(color_automata *automata, double t)
{
	double		distance_from_intersection;
	t_3d_vector	intersection_point;

	while (42) {
		line	*line = &automata->starting_line;
		plan	*face;


		for (uint32_t current_face = 0; current_face < 6; current_face++) {
			face = &automata->cube_faces[current_face];

			double line_normal_plan_parallelisme = vector3d_dot_product(line->dir, face->normal);

			if (double_epsilon_eq(line_normal_plan_parallelisme, 0.0, 0.01)) { // change this magic epsilon
				continue ;
			}
			t_3d_vector	line_to_plan = vector3d_sub(face->point, line->point);

			double points_normal_parallelisme = vector3d_dot_product(line_to_plan, face->normal);

			distance_from_intersection = points_normal_parallelisme / line_normal_plan_parallelisme;

			if (distance_from_intersection < 0.0) {
				continue ;
			}

			intersection_point = vector3d_add(vector3d_scalar_multiply(line->dir, distance_from_intersection), line->point);
			if (intersection_point.x < 0.0 || intersection_point.x > 1.0 ||
				intersection_point.y < 0.0 || intersection_point.y > 1.0 ||
				intersection_point.z < 0.0 || intersection_point.y > 1.0) {
				continue ; // Skip intersection outside the color cube.
			}

		printf("Found intersection_point at: " VEC3_FORMAT"\n", VEC3_FA_LIST(intersection_point));
		break ;
		}

		if (distance_from_intersection > t) {
			t -= distance_from_intersection;
			// Collision code
			return 0;
		} else {
			t_3d_vector color_point = vector3d_add(vector3d_scalar_multiply(line->dir, distance_from_intersection), line->point);

			// Map it to 256x256x256 color cube space.
			color_point = vector3d_scalar_multiply(color_point, 256.0);

			uint32_t	blue_component = (uint32_t)color_point.x;
			uint32_t	green_component = (uint32_t)color_point.y << 8;
			uint32_t	red_component = (uint32_t)color_point.z << 16;

			assert(blue_component < 256);
			assert((green_component >> 8) < 256);
			assert((red_component >> 16) < 256);

			uint32_t	color = blue_component |
				green_component |
				red_component;

			return color;
		}
	}
}
