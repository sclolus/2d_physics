/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pthread_wait_drawing_threads.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sclolus <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/02 18:09:54 by sclolus           #+#    #+#             */
/*   Updated: 2017/08/17 02:55:03 by sclolus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	ft_pthread_wait_drawing_threads(pthread_t *thread_tab)
{
	uint32_t	i;
	void		*return_status;

	i = 0;
	while (i < DRAWING_THREAD_NBR)
	{
		pthread_join(thread_tab[i], &return_status);
		i++;
	}
}
