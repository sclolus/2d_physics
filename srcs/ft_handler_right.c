/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handler_right.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sclolus <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/26 21:27:20 by sclolus           #+#    #+#             */
/*   Updated: 2017/08/17 03:59:40 by sclolus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	ft_handler_right(void *param)
{
	(void)param;
	if (g_univers->time_ratio == 0) {
		g_univers->time_ratio = 0.05;
	}
	g_univers->time_ratio *= 2.0;
}
