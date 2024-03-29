/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handler_a.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sclolus <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/27 12:19:03 by sclolus           #+#    #+#             */
/*   Updated: 2019/10/30 23:39:33 by sclolus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	ft_handler_r(void *param)
{
	(void)param;
	free(g_univers->objects);
	g_univers->nbr_objects = 0;
	g_univers->objects = NULL;
	double	scaling_factor = g_univers->scaling_factor;
	init_univers(g_univers);

	g_univers->scaling_factor = scaling_factor;
}

void	ft_handler_m(void *param)
{
	uint32_t	random_index;

	(void)param;
	random_index = rand() % g_univers->nbr_objects;

	g_univers->cam = g_univers->objects[random_index].pos;
	g_univers->current_follow = random_index;
}
