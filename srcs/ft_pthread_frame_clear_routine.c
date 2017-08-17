/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pthread_frame_clear_routine.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sclolus <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/28 19:16:58 by sclolus           #+#    #+#             */
/*   Updated: 2017/08/17 02:55:03 by sclolus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	*ft_pthread_frame_clear_routine(void *arg)
{
	t_image_frame	*frames;
	uint32_t		i;

	i = 0;
	frames = arg;
	while (42)
	{
		if (frames[i].state == TO_CLEAR)
		{
			frames[i].state = CLEARING;
			ft_bzero(frames[i].buffer, WINDOW_WIDTH * WINDOW_HEIGHT * 4);
			frames[i].state = AVAILABLE;
		}
		i++;
		i %= NBR_IMAGE_FRAME;
	}
}
