/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handler_left.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sclolus <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/26 23:10:15 by sclolus           #+#    #+#             */
/*   Updated: 2017/08/17 03:56:09 by sclolus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	ft_handler_left(void *param)
{
	t_mem_block	*data;
	t_mem_block	*tmp;
	uint32_t	i;

	data = ((t_mem_block**)param)[3];
	tmp = data;
	i = 0;
	while (i * sizeof(t_line) < tmp->offset)
	{
		((t_line*)tmp->block + i)->start.x -= MOVE_COEFFICIENT;
		((t_line*)tmp->block + i)->end.x -= MOVE_COEFFICIENT;
		i++;
		if (i * sizeof(t_line) >= tmp->offset && tmp->next && !(i = 0))
			tmp = tmp->next;
	}
	ft_draw_lines(((t_mlx_ptr)((char**)param)[0]), (t_mlx_ptr)((char**)param)[1]
		, (t_mlx_img)((char**)param)[2]
		, (t_mem_block*)((t_mem_block**)param)[3]);
}
