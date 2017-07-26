/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handler_keys.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sclolus <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/26 21:05:42 by sclolus           #+#    #+#             */
/*   Updated: 2017/07/26 21:10:54 by sclolus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static t_keycode_f	*ft_get_keycodes_f(void)
{
	static t_keycode_f	keycodes_f[] = {
		{ft_handler_esc, KEY_ESCAPE, 0},
		{ft_handler_right, KEY_RIGHT, 0},
		{ft_handler_left, KEY_LEFT, 0},
		{ft_handler_up, KEY_UP, 0},
		{ft_handler_down, KEY_DOWN, 0},
		{ft_handler_q, KEY_Q, 0},
		{ft_handler_a, KEY_A, 0},
		{ft_handler_d, KEY_D, 0},
		{ft_handler_w, KEY_W, 0},
		{ft_handler_s, KEY_S, 0},
		{ft_handler_e, KEY_E, 0},
		{ft_handler_pad_up, KEY_PAD_ADD, 0},
		{ft_handler_pad_down, KEY_PAD_SUB, 0}};

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
	uint32_t			bool;

	i = ~0;
	if (!keycodes_f)
		keycodes_f = ft_get_keycodes_f();
	while (++i < NBR_KEY_HOOKS)
	{
		if (keycodes_f[i].keycode == keycode)
		{
			keycodes_f[i].used = 1;
			break ;
		}
	}
	i = 0;
	bool = 0;
	while (i < NBR_KEY_HOOKS)
	{
		if (keycodes_f[i].used && (bool = 1))
			keycodes_f[i].f(param);
		if (++i == NBR_KEY_HOOKS && !bool)
			break ;
	}
	return (0);
}
