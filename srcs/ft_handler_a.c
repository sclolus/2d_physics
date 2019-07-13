/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handler_a.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sclolus <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/27 12:19:03 by sclolus           #+#    #+#             */
/*   Updated: 2017/08/17 03:56:09 by sclolus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	ft_handler_a(void *param)
{
	ft_quat_rotate_points(&(t_vec){1, 0, 0}, ROTATION_SPEED * PI / 180
				, (t_mem_block*)((t_mem_block**)param)[3]);
	ft_draw_lines(((t_mlx_ptr)((char**)param)[0]), (t_mlx_ptr)((char**)param)[1]
				, (t_mlx_img)((char**)param)[2]
				, (t_mem_block*)((t_mem_block**)param)[3]);
}

void	ft_handler_r(void *param)
{
	(void)param;
	free(g_univers->objects);
	g_univers->nbr_objects = 0;
	g_univers->objects = NULL;
	init_univers(g_univers);
}

void	ft_handler_m(void *param)
{
	uint32_t	random_index;

	(void)param;
	random_index = rand() % g_univers->nbr_objects;

	g_univers->cam = g_univers->objects[random_index].pos;
}
