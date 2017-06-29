/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sclolus <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/25 16:54:59 by sclolus           #+#    #+#             */
/*   Updated: 2017/06/29 12:30:49 by sclolus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int	main(int argc, char **argv)
{
//	pthread_t			thread;
	MLX_PTR				connector;
	MLX_WIN				win;
	t_image_frame		*frames;
	MLX_IMG				image;
	t_mem_block			*data;
//	t_mem_block			*lines;

	if (argc != 0 && argv)
	{
		data = ft_parse_file(argv[1]);
		ft_putendl_fd("Charged map", 2);
		if (!(connector = mlx_init()))
			ft_error_exit(1, (char*[]){MLX_INIT_ERROR}, EXIT_FAILURE);
		if (!(win = mlx_new_window(connector, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME)))
			ft_error_exit(1, (char*[]){MLX_NEW_WIN_ERROR}, EXIT_FAILURE);
		if (!(image = mlx_new_image(connector, WINDOW_WIDTH, WINDOW_HEIGHT)))
			ft_error_exit(1, (char*[]){MLX_NEW_IMG_ERROR}, EXIT_FAILURE);
		frames = ft_get_image_frames(connector, NBR_IMAGE_FRAME);
//		lines = ft_set_lines(data);
//		pthread_create(&thread, NULL, &ft_pthread_frame_clear_routine, frames);
//		pthread_create(&thread, NULL, &ft_pthread_frame_clear_routine, frames);
//		ft_draw_line(&(t_mlx_data){connector, win, frames[0].frame}, &(t_line){{10, 10, 0}, {WINDOW_WIDTH - 10, WINDOW_HEIGHT - 10, 0}, 0, 0, 0, NULL, NULL});
		ft_draw_line(&(t_mlx_data){connector, win, &frames[0]}, &(t_line){{10, WINDOW_HEIGHT - 10, 0}, {WINDOW_WIDTH - 10, 10, 0}, 0, 0, 0, NULL, NULL});
		/* ft_quat_rotate_points(&(t_vec){1, 0.5, 1},  -45 * PI / 180, lines); */
		/* ft_draw_lines(connector, win, frames, lines); */
		/* mlx_hook(win, KeyPress, KeyPressMask | KeymapStateMask */
		/* 		, &ft_handler_keys, (void*[]){connector */
		/* 		, win, frames, lines, data}); */
		/* mlx_hook(win, KeyRelease, KeyReleaseMask | KeymapStateMask */
		/* 		, &ft_handler_keys_release, (void*[]){connector */
		/* 		, win, frames, lines, data}); */
		mlx_put_image_to_window(connector, win, frames[0].frame, 0, 0);
		mlx_loop(connector);
	}

}
