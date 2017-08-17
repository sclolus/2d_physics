/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handler_w.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sclolus <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/27 12:21:35 by sclolus           #+#    #+#             */
/*   Updated: 2017/08/17 03:04:39 by sclolus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	ft_handler_w(void *param)
{
	ft_quat_rotate_points(&(t_vec){0, 1, 0}, ROTATION_SPEED * PI / 180
				, (t_mem_block*)((t_mem_block**)param)[3]);
	ft_draw_lines(((mlx_ptr)((char**)param)[0]), (mlx_ptr)((char**)param)[1]
				, (mlx_img)((char**)param)[2]
				, (t_mem_block*)((t_mem_block**)param)[3]);
}
