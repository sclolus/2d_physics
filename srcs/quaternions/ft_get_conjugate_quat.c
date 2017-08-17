/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_conjugate_quat.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sclolus <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/27 09:04:26 by sclolus           #+#    #+#             */
/*   Updated: 2017/08/17 02:55:03 by sclolus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

t_quat	ft_get_conjugate_quat(t_quat *quat)
{
	return ((t_quat){-quat->x, -quat->y, -quat->z, quat->w});
}
