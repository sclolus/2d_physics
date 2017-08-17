/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_z_coord.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sclolus <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/01 12:32:01 by sclolus           #+#    #+#             */
/*   Updated: 2017/08/17 02:55:04 by sclolus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

double	ft_get_z_coord(t_line *line, t_vec *point)
{
	return (((line->end.z - line->start.z) * ((point->x - line->start.x)
			/ (line->end.x - line->start.x))) + line->start.z);
}
