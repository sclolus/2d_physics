/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handler_a.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sclolus <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/27 12:19:03 by sclolus           #+#    #+#             */
/*   Updated: 2017/07/26 21:07:38 by sclolus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	ft_handler_a(void *param)
{
	ft_quat_rotate_points(&(t_vec){1, 0, 0}, ROTATION_SPEED * PI / 180
				, (t_mem_block*)((t_mem_block**)param)[3]);
	ft_draw_lines(((MLX_PTR)((char**)param)[0]), (MLX_PTR)((char**)param)[1]
				, (MLX_IMG)((char**)param)[2]
				, (t_mem_block*)((t_mem_block**)param)[3]);
}
