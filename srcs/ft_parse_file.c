/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_file.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sclolus <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/25 20:57:51 by sclolus           #+#    #+#             */
/*   Updated: 2017/08/16 22:57:49 by sclolus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static t_vec		*ft_get_data_from_line(char *line
											, t_mem_block *data)
{
	static int64_t	y = 0;
	static t_vec	min_max[2];
	int64_t			x;
	int64_t			z;
	uint64_t		i;

	i = 0;
	x = 0;
	while (line[i])
	{
		while (line[i] && !ft_isdigit(line[i]))
			i++;
		if (!line[i])
			break ;
		z = ft_atoi(line + i);
		min_max[0] = (t_vec){x > min_max[0].x ? x : min_max[0].x
	, y > min_max[0].z ? y : min_max[0].y, z > min_max[0].z ? z : min_max[0].z};
		min_max[1] = (t_vec){x < min_max[1].x ? x : min_max[1].x
	, y < min_max[1].z ? y : min_max[1].y, z < min_max[1].z ? z : min_max[1].z};
		ft_mem_block_push_back_elem(data
	, &(t_point){{(double)(x), (double)(y), (double)(-z)}, 0}, sizeof(t_point));
		while (ft_isdigit(line[i]))
			i++;
		x++;
	}
	y++;
	return (min_max);
}

static void		ft_adjust_points(t_mem_block *data, t_vec *min_max
								, t_color_set color_set)
{
	uint64_t		i;
	t_vec			half_max;

	i = 0;
	half_max.x = min_max[0].x / 2;
	half_max.y = min_max[0].y / 2;
	half_max.z = min_max[0].z / 2;
	while ((i) * sizeof(t_point) < data->offset)
	{
		((t_point*)data->block + i)->color = ft_get_lerp(min_max[0].z
	, min_max[1].z, -((t_point*)data->block + i)->coords.z, color_set);
		((t_point*)data->block + i)->coords.z -= half_max.z;
		((t_point*)data->block + i)->coords.x -= half_max.x;
		((t_point*)data->block + i)->coords.y -= half_max.y;
		i++;
		if ((i) * sizeof(t_point) >= data->offset
			&& data->next)
		{
			data = data->next;
			i = 0;
		}
	}
}

t_mem_block		*ft_parse_file(char *filename, char *filename_color)
{
	t_color_set			color_set;
	t_mem_block			*data;
	char				*line;
	t_vec				*min_max;
	int					fd;

	color_set.color_min = 0xFFFFFF;
	color_set.color_max = 0xFFFFFF;
	min_max = NULL;
	data = ft_create_mem_block(DEFAULT_MEM_BLOCK_SIZE);
	if ((fd = open(filename, O_RDONLY)) == -1)
		ft_error_exit(2, (char*[]){filename, OPEN_FILE_FAILED}, EXIT_FAILURE);
	while (get_next_line(fd, &line) > 0)
	{
		min_max = ft_get_data_from_line(line, data);
		free(line);
	}
	free(line);
	if (filename_color)
		color_set = ft_parse_color(filename_color);
	if (min_max)
		ft_adjust_points(data, min_max, color_set);
	if (close(fd) == -1)
		ft_error_exit(3, (char*[]){"Failed to close(): "
					, filename, " file"}, EXIT_FAILURE);
	return (data);
}
