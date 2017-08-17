/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_reduction_coefficient.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sclolus <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/27 07:34:38 by sclolus           #+#    #+#             */
/*   Updated: 2017/08/17 02:55:04 by sclolus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	ft_sus_reduction_coefficient(void)
{
	*ft_get_reduction_coefficient() -= 0.2;
}

void	ft_add_reduction_coefficient(void)
{
	*ft_get_reduction_coefficient() += 0.2;
}

double	*ft_get_reduction_coefficient(void)
{
	static double	k = K;

	return (&k);
}
