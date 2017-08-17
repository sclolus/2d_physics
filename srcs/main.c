/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sclolus <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/25 16:54:59 by sclolus           #+#    #+#             */
/*   Updated: 2017/08/17 04:02:22 by sclolus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int	main(int argc, char **argv)
{
	pthread_t			thread;
	t_mlx_data			mlx_data;
	t_image_frame		*frames;
	t_mem_block			*data;
	t_mem_block			*lines;

	if (argc != 1)
	{
		data = ft_parse_file(argv[1], argv[2]);
		if (!(mlx_data.connector = mlx_init()))
			ft_error_exit(1, (char*[]){MLX_INIT_ERROR}, EXIT_FAILURE);
		if (!(mlx_data.win = mlx_new_window(mlx_data.connector, WINDOW_WIDTH
				, WINDOW_HEIGHT, WINDOW_NAME)))
			ft_error_exit(1, (char*[]){MLX_NEW_WIN_ERROR}, EXIT_FAILURE);
		frames = ft_get_image_frames(mlx_data.connector, NBR_IMAGE_FRAME);
		lines = ft_set_lines(data);
		if (pthread_create(&thread, NULL
				, &ft_pthread_frame_clear_routine, frames))
			ft_error_exit(1, (char*[]){ERR_PTHREAD_FAIL}, EXIT_FAILURE);
		ft_quat_rotate_points(&(t_vec){1, 0.5, 1}, -45 * PI / 180, lines);
		ft_draw_lines(mlx_data.connector, mlx_data.win, frames, lines);
		ft_set_mlx_hooks(&mlx_data, (void*[]){mlx_data.connector
					, mlx_data.win, frames, lines, data});
		mlx_loop(mlx_data.connector);
	}
}
