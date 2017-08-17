/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_color.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sclolus <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/03 04:10:29 by sclolus           #+#    #+#             */
/*   Updated: 2017/08/17 02:55:03 by sclolus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static uint32_t			ft_atoi_base(char *str, char *base)
{
	uint64_t		nbr;
	uint32_t		base_len;

	nbr = 0;
	base_len = ft_strlen(base);
	while (*str == ' ' || *str == '\t' || *str == '\n'
			|| *str == '\v' || *str == '\r' || *str == '\f')
		str++;
	while (*str && ft_strchr(base, *str))
		nbr = (nbr * base_len) + (ft_strchr(base, *str++) - base);
	return ((uint32_t)nbr);
}

static void				ft_check_color_nbr(uint32_t *nbr_color)
{
	if (*nbr_color != 2)
		ft_error_exit(1, (char*[]){ERR_COLOR_NBR}, EXIT_FAILURE);
}

static t_color_set		ft_get_color_set(char *buf)
{
	t_color_set			color_set;
	uint32_t			nbr_color __attribute__((cleanup(ft_check_color_nbr)));
	uint32_t			i;

	nbr_color = 0;
	i = 0;
	while (buf[i] && nbr_color < 2)
	{
		while (buf[i] && !ft_strchr(HEXA_CHARSET, buf[i]))
		{
			if (!ft_strchr(" ,", buf[i]))
				ft_error_exit(1, (char*[]){ERR_INVALID_CHAR_COLOR_FILE}, 1);
			i++;
		}
		if (!buf[i])
			break ;
		if (nbr_color)
			color_set.color_max = ft_atoi_base(buf + i, HEXA_CHARSET);
		else
			color_set.color_min = ft_atoi_base(buf + i, HEXA_CHARSET);
		while (buf[i] && ft_strchr(HEXA_CHARSET, buf[i]))
			i++;
		nbr_color++;
	}
	return (color_set);
}

/*
** Sizeof(buf) == 19 because format ft_strlen("FFFFFFFF, FFFFFFFF") == 18
*/

t_color_set				ft_parse_color(char *filename_color)
{
	static char			buf[19];
	t_color_set			color_set;
	int					fd;
	int32_t				ret;

	if ((fd = open(filename_color, O_RDONLY)) == -1)
		ft_error_exit(2, (char*[]){filename_color, OPEN_FILE_FAILED}
		, EXIT_FAILURE);
	if ((ret = read(fd, buf, 19)) == -1)
		ft_error_exit(2, (char*[]){"ft_parse_color() failed: "
					, ERR_PARSE_COLOR_READ}, EXIT_FAILURE);
	if (ret > 18)
		ft_error_exit(1, (char*[]){ERR_COLOR_INVALID_FILE_SIZE}, EXIT_FAILURE);
	buf[ret] = '\0';
	color_set = ft_get_color_set(buf);
	if (close(fd) == -1)
		ft_error_exit(3, (char*[]){"Failed to close(): "
					, filename_color, " file"}, EXIT_FAILURE);
	return (color_set);
}
