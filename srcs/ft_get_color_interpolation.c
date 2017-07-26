/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_color_interpolation.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sclolus <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/01 15:54:27 by sclolus           #+#    #+#             */
/*   Updated: 2017/07/26 21:17:56 by sclolus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int32_t	ft_get_color_interpolation(t_line *line, t_vec *point)
{
	double	z1;
	double	z2;
	double	z3;

	z1 = line->start.z;
	z2 = line->end.z;
	if (line->point_1->coords.z == line->point_2->coords.z)
		return (line->point_1->color);
	else
		z3 = ((line->point_1->coords.z * (z2 - ft_get_z_coord(line, point))
			+ line->point_2->coords.z
			* (ft_get_z_coord(line, point) - z1)) / (z2 - z1));
	return ((int32_t)((line->point_1->color * (line->point_2->coords.z - z3)
					+ line->point_2->color
					* (z3 - line->point_1->coords.z))
					/ (line->point_2->coords.z - line->point_1->coords.z)));
}
