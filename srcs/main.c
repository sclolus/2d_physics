/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sclolus <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/25 16:54:59 by sclolus           #+#    #+#             */
/*   Updated: 2017/07/26 21:37:02 by sclolus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int	main(int argc, char **argv)
{
	pthread_t			thread;
	MLX_PTR				connector;
	MLX_WIN				win;
	t_image_frame		*frames;
	t_mem_block			*data;
	t_mem_block			*lines;

	if (argc != 0)
	{
		data = ft_parse_file(argv[1], argv[2]);
		if (!(connector = mlx_init()))
			ft_error_exit(1, (char*[]){MLX_INIT_ERROR}, EXIT_FAILURE);
		if (!(win = mlx_new_window(connector, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME)))
			ft_error_exit(1, (char*[]){MLX_NEW_WIN_ERROR}, EXIT_FAILURE);
		frames = ft_get_image_frames(connector, NBR_IMAGE_FRAME);
		lines = ft_set_lines(data);
		if (pthread_create(&thread, NULL, &ft_pthread_frame_clear_routine, frames) ||
			pthread_create(&thread, NULL, &ft_pthread_frame_clear_routine, frames))
			ft_error_exit(1, (char*[]){ERR_PTHREAD_FAIL}, EXIT_FAILURE);
		ft_quat_rotate_points(&(t_vec){1, 0.5, 1},  -45 * PI / 180, lines);
		ft_draw_lines(connector, win, frames, lines);
		mlx_hook(win, KeyPress, KeyPressMask | KeymapStateMask
				, &ft_handler_keys, (void*[]){connector
				, win, frames, lines, data});
		mlx_hook(win, KeyRelease, KeyReleaseMask | KeymapStateMask
				, &ft_handler_keys_release, (void*[]){connector
				, win, frames, lines, data});
		mlx_loop(connector);
	}
}
