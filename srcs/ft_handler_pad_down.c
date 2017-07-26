/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handler_pad_down.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sclolus <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/27 06:51:08 by sclolus           #+#    #+#             */
/*   Updated: 2017/07/26 21:05:43 by sclolus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	ft_handler_pad_down(void *param)
{
	t_mem_block	*data;
	t_mem_block	*tmp;
	uint32_t	i;

	data = ((t_mem_block**)param)[3];
	tmp = data;
	i = 0;
	while (i * sizeof(t_line) < tmp->offset)
	{
		((t_line*)tmp->block + i)->start.x *= DEZOOM_RATIO;
		((t_line*)tmp->block + i)->start.z *= DEZOOM_RATIO;
		((t_line*)tmp->block + i)->start.y *= DEZOOM_RATIO;
		((t_line*)tmp->block + i)->end.x *= DEZOOM_RATIO;
		((t_line*)tmp->block + i)->end.z *= DEZOOM_RATIO;
		((t_line*)tmp->block + i)->end.y *= DEZOOM_RATIO;
		i++;
		if (i * sizeof(t_line) >= tmp->offset && tmp->next && !(i = 0))
			tmp = tmp->next;
	}
	ft_draw_lines(((MLX_PTR)((char**)param)[0]), (MLX_PTR)((char**)param)[1]
		, (MLX_IMG)((char**)param)[2], (t_mem_block*)((t_mem_block**)param)[3]);
}
