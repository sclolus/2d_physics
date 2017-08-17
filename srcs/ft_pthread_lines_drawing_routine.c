/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pthread_lines_drawing_routine.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sclolus <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/01 18:40:32 by sclolus           #+#    #+#             */
/*   Updated: 2017/08/17 02:55:03 by sclolus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	*ft_pthread_lines_drawing_routine(void *arg)
{
	t_mlx_data	mlx_data;
	t_mem_block *data;
	uint32_t	i;
	uint32_t	offset;

	mlx_data = ((t_pthread_execution_data*)arg)->mlx_data;
	offset = ((t_pthread_execution_data*)arg)->offset;
	data = ((t_pthread_execution_data*)arg)->data;
	i = offset;
	while (i * sizeof(t_line) < data->offset)
	{
		ft_draw_line(&mlx_data, ((t_line*)data->block + i));
		i += DRAWING_THREAD_NBR + 1;
		if (i * sizeof(t_line) >= data->offset
			&& data->next)
		{
			data = data->next;
			i = offset;
		}
	}
	pthread_exit(data);
}
