/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_draw_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sclolus <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/25 23:22:11 by sclolus           #+#    #+#             */
/*   Updated: 2017/08/17 02:55:04 by sclolus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

inline static void			ft_case_1(t_mlx_data *mlx_data, t_line *line)
{
	double				e;
	double				derror;
	double				x;
	double				y;
	double				error;

	e = (line->dy / line->dx);
	derror = ABS(e);
	error = derror - 0.5;
	x = line->start.x;
	y = line->start.y;
	while (x < line->end.x)
	{
		((int*)mlx_data->frame->buffer)[(int)(y + WINDOW_HEIGHT / 2)
		* WINDOW_WIDTH + (int)(x) + WINDOW_WIDTH
		/ 2] = ft_get_color_interpolation(line, &(t_vec){x, y, 0});
		error += derror;
		if (error >= 0.5)
		{
			y += 1L;
			error -= 1L;
		}
		x += 1L;
	}
}

inline static void			ft_case_2(t_mlx_data *mlx_data, t_line *line)
{
	double				e;
	double				derror;
	double				x;
	double				y;
	double				error;

	e = (line->dy / line->dx);
	derror = ABS(e);
	error = derror - 0.5;
	x = line->start.x;
	y = line->start.y;
	while (x < line->end.x)
	{
		((int*)mlx_data->frame->buffer)[(int)(y + WINDOW_HEIGHT / 2)
	* WINDOW_WIDTH + (int)x + WINDOW_WIDTH
	/ 2] = ft_get_color_interpolation(line, &(t_vec){x, y, 0});
		error += derror;
		if (error >= 0.5)
		{
			y--;
			error -= 1L;
		}
		x += 1;
	}
}

inline static void			ft_case_3(t_mlx_data *mlx_data, t_line *line)
{
	double				e;
	double				derror;
	double				x;
	double				y;
	double				error;

	e = (line->dx / line->dy);
	derror = ABS(e);
	error = derror - 0.5;
	x = line->start.x;
	y = line->start.y;
	while (y < line->end.y)
	{
		((int*)mlx_data->frame->buffer)[(int)(y + WINDOW_HEIGHT / 2)
	* WINDOW_WIDTH + (int)x + WINDOW_WIDTH
	/ 2] = ft_get_color_interpolation(line, &(t_vec){x, y, 0});
		error += derror;
		if (error >= 0.5)
		{
			x++;
			error -= 1L;
		}
		y += 1;
	}
}

inline static void			ft_case_4(t_mlx_data *mlx_data, t_line *line)
{
	double				e;
	double				derror;
	double				x;
	double				y;
	double				error;

	e = (line->dx / line->dy);
	derror = ABS(e);
	error = derror - 0.5;
	x = line->start.x;
	y = line->start.y;
	while (y > line->end.y)
	{
		((int*)mlx_data->frame->buffer)[(int)(y + WINDOW_HEIGHT / 2)
	* WINDOW_WIDTH + (int)x + WINDOW_WIDTH
	/ 2] = ft_get_color_interpolation(line, &(t_vec){x, y, 0});
		error += derror;
		if (error >= 0.5)
		{
			x++;
			error -= 1L;
		}
		y -= 1;
	}
}

void						ft_draw_line(t_mlx_data *mlx_data, t_line *line)
{
	t_line	tmp;

	tmp = *line;
	if (ft_line_clipping(line))
		return ;
	if (tmp.start.x > tmp.end.x)
		ft_swap_t_point_in_line(&tmp);
	if (tmp.start.y > tmp.end.y)
	{
		if (ABS(tmp.e) < 1)
			ft_case_2(mlx_data, &tmp);
		else
			ft_case_4(mlx_data, &tmp);
	}
	else
	{
		if (ABS(tmp.e) < 1)
			ft_case_1(mlx_data, &tmp);
		else
			ft_case_3(mlx_data, &tmp);
	}
}
