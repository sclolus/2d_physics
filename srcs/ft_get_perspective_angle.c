/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_perspective_angle.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sclolus <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/27 07:37:56 by sclolus           #+#    #+#             */
/*   Updated: 2017/08/17 02:55:04 by sclolus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	ft_sus_perspective_angle(void)
{
	*ft_get_perspective_angle() -= 0.2;
}

void	ft_add_perspective_angle(void)
{
	*ft_get_perspective_angle() += 0.2;
}

double	*ft_get_perspective_angle(void)
{
	static double	angle = ANGLE;

	return (&angle);
}
