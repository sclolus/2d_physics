/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handler_pad_up.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sclolus <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/27 06:49:50 by sclolus           #+#    #+#             */
/*   Updated: 2017/08/17 03:59:25 by sclolus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	ft_handler_pad_up(void *param)
{
	t_mem_block	*data;
	t_mem_block	*tmp;
	uint32_t	i;

	data = ((t_mem_block**)param)[3];
	tmp = data;
	i = 0;
	while (i * sizeof(t_line) < tmp->offset)
	{
		((t_line*)tmp->block + i)->start.x *= ZOOM_RATIO;
		((t_line*)tmp->block + i)->start.z *= ZOOM_RATIO;
		((t_line*)tmp->block + i)->start.y *= ZOOM_RATIO;
		((t_line*)tmp->block + i)->end.x *= ZOOM_RATIO;
		((t_line*)tmp->block + i)->end.z *= ZOOM_RATIO;
		((t_line*)tmp->block + i)->end.y *= ZOOM_RATIO;
		i++;
		if (i * sizeof(t_line) >= tmp->offset && tmp->next && !(i = 0))
			tmp = tmp->next;
	}
	ft_draw_lines(((t_mlx_ptr)((char**)param)[0]), (t_mlx_ptr)((char**)param)[1]
	, (t_mlx_img)((char**)param)[2], (t_mem_block*)((t_mem_block**)param)[3]);
}
