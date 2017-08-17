/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_draw_lines.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sclolus <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/25 23:20:15 by sclolus           #+#    #+#             */
/*   Updated: 2017/08/17 03:56:09 by sclolus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	ft_draw_lines(t_mlx_ptr connector, t_mlx_win win, t_image_frame *frames
					, t_mem_block *data)
{
	t_image_frame	*frame;
	pthread_t		*thread_tab;
	uint64_t		i;

	i = 0;
	frame = ft_claim_image_frame(frames);
	thread_tab = ft_pthread_create_lines_drawing_threads(connector
					, win, frame, data);
	while (i * sizeof(t_line) < data->offset)
	{
		ft_draw_line(&(t_mlx_data){connector, win, frame}
		, ((t_line*)data->block + i));
		i += DRAWING_THREAD_NBR + 1;
		if (i * sizeof(t_line) >= data->offset
			&& data->next)
		{
			data = data->next;
			i = 0;
		}
	}
	ft_pthread_wait_drawing_threads(thread_tab);
	mlx_put_image_to_window(connector, win, frame->frame, 0, 0);
	frame->state = TO_CLEAR;
}
