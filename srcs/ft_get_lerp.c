/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_lerp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sclolus <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/01 12:28:29 by sclolus           #+#    #+#             */
/*   Updated: 2017/08/17 02:55:04 by sclolus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int32_t	ft_get_lerp(double z1, double z2, double z, t_color_set color_set)
{
	return ((int32_t)(((color_set.color_min * (z2 - z)) + (color_set.color_max
			* (z - z1))) / (z2 - z1)));
}
