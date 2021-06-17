/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_set_mlx_hooks.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sclolus <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/16 23:44:21 by sclolus           #+#    #+#             */
/*   Updated: 2017/08/17 02:55:03 by sclolus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	ft_set_mlx_hooks(t_mlx_data *mlx_data, void **params)
{
//	KeyPressMask, KeyReleaseMask, ButtonPressMask, and ButtonReleaseMask
	mlx_hook(mlx_data->win, KeyPress, KeyPressMask
			, &ft_handler_keys, params);
	mlx_hook(mlx_data->win, KeyRelease, KeyReleaseMask
			, &ft_handler_keys_release, params);
}
