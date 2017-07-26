/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pthread_create_lines_drawing_threads.c          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sclolus <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/01 18:41:09 by sclolus           #+#    #+#             */
/*   Updated: 2017/07/26 21:19:27 by sclolus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

pthread_t	*ft_pthread_create_lines_drawing_threads(MLX_PTR connector
					, MLX_WIN win, t_image_frame *frame, t_mem_block *data)
{
	static pthread_t					threat_tab[DRAWING_THREAD_NBR];
	static t_pthread_execution_data		pthread_data[DRAWING_THREAD_NBR];
	uint64_t							i;

	i = 0;
	while (i < DRAWING_THREAD_NBR)
	{
		pthread_data[i] = (t_pthread_execution_data){{connector, win, frame}
						, i + 1, data};
		if (pthread_create(threat_tab + i, NULL
						, &ft_pthread_lines_drawing_routine, pthread_data + i))
			ft_error_exit(1, (char*[]){ERR_PTHREAD_FAIL}, EXIT_FAILURE);
		i++;
	}
	return (threat_tab);
}
