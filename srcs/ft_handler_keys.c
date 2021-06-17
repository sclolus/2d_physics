/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handler_keys.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sclolus <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/26 21:05:42 by sclolus           #+#    #+#             */
/*   Updated: 2019/10/31 04:33:59 by sclolus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static t_keycode_f	*ft_get_keycodes_f(void)
{
	static t_keycode_f	keycodes_f[] = {
		{ft_handler_esc, XK_Escape, 0},
		/* {ft_handler_right, KEY_RIGHT, 0}, */
		/* {ft_handler_left, KEY_LEFT, 0}, */
		/* {ft_handler_up, KEY_UP, 0}, */
		/* {ft_handler_down, KEY_DOWN, 0}, */
		/* {ft_handler_q, KEY_Q, 0}, */
		{ft_handler_alpha_up, XK_t, 0},
		{ft_handler_alpha_down, XK_y, 0},
		{ft_handler_toggle_symetry, XK_f, 0},
		{ft_handler_r, XK_r, 0},
		{ft_handler_s, XK_s, 0},
		{ft_handler_m, XK_m, 0},
		/* {ft_handler_a, KEY_A, 0}, */
		/* {ft_handler_d, KEY_D, 0}, */
		/* {ft_handler_w, KEY_W, 0}, */
		/* {ft_handler_s, KEY_S, 0}, */
		/* {ft_handler_e, KEY_E, 0}, */
		{ft_handler_left, XK_Left, 0},
		{ft_handler_right, XK_Right, 0},
		{ft_handler_pad_up, XK_Up, 0},
		{ft_handler_pad_down, XK_Down, 0}};

	if (sizeof(keycodes_f) / sizeof(t_keycode_f) != NBR_KEY_HOOKS)
		ft_error_exit(1, (char*[]){INVALID_KEYS_HOOKS_NBR}, EXIT_FAILURE);
	return (keycodes_f);
}

int					ft_handler_keys_release(int keycode
						, void __attribute__((unused)) *param)
{
	static t_keycode_f	*keycodes_f = NULL;
	uint32_t			i;

	i = 0;
	if (!keycodes_f)
		keycodes_f = ft_get_keycodes_f();
	while (i < NBR_KEY_HOOKS)
	{
		if (keycodes_f[i].keycode == keycode)
		{
			keycodes_f[i].used = 0;
			break ;
		}
		i++;
	}
	return (0);
}

int					ft_handler_keys(int keycode, void *param)
{
	static t_keycode_f	*keycodes_f = NULL;
	uint32_t			i;
	uint32_t			_bool;

	i = ~0;
	if (!keycodes_f)
		keycodes_f = ft_get_keycodes_f();
	printf("Calling keycode handler: %d\n", keycode);

	while (++i < NBR_KEY_HOOKS)
	{
		if (keycodes_f[i].keycode == keycode)
		{
			keycodes_f[i].used = 1;

			break ;
		}
	}




	i = 0;
	_bool = 0;
	while (i < NBR_KEY_HOOKS)
	{
		if (keycodes_f[i].used && (_bool = 1)) {
			keycodes_f[i].f(param);

		}
		if (++i == NBR_KEY_HOOKS && !_bool)
			break ;
	}
	return (0);
}
