/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handler_q.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sclolus <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/27 07:47:52 by sclolus           #+#    #+#             */
/*   Updated: 2017/07/26 21:06:06 by sclolus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	ft_handler_q(void *param)
{
	ft_quat_rotate_points(&(t_vec){0, 0, 1}, ROTATION_SPEED * PI / 180
				, (t_mem_block*)((t_mem_block**)param)[3]);
	ft_draw_lines(((MLX_PTR)((char**)param)[0]), (MLX_PTR)((char**)param)[1]
				, ((t_image_frame**)param)[2]
				, (t_mem_block*)((t_mem_block**)param)[3]);
}
