/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handler_alpha.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sclolus <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/31 03:09:59 by sclolus           #+#    #+#             */
/*   Updated: 2019/10/31 03:39:45 by sclolus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	ft_handler_alpha_up(void *param)
{
	(void)param;
	g_global_alpha += 3;
}

void	ft_handler_alpha_down(void *param)
{
	(void)param;
	g_global_alpha -= 3;
}
