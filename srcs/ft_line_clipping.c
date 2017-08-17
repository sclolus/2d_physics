/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_line_clipping.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sclolus <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/17 00:08:50 by sclolus           #+#    #+#             */
/*   Updated: 2017/08/17 02:55:03 by sclolus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int32_t	ft_line_clipping(t_line *tmp)
{
	if (tmp->start.x + (WINDOW_WIDTH / 2) + 1 >= WINDOW_WIDTH || tmp->start.y
	+ (WINDOW_HEIGHT / 2) + 1 >= WINDOW_HEIGHT || tmp->start.x + (WINDOW_WIDTH
	/ 2) < 0 || tmp->start.y + (WINDOW_HEIGHT / 2) < 0 || tmp->end.x
	+ (WINDOW_WIDTH / 2) + 1 >= WINDOW_WIDTH || tmp->end.y + (WINDOW_HEIGHT / 2)
	+ 1 >= WINDOW_HEIGHT || tmp->end.x + (WINDOW_WIDTH / 2) < 0
	|| tmp->end.y + (WINDOW_HEIGHT / 2) < 0)
		return (1);
	return (0);
}
