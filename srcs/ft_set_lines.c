/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_set_lines.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sclolus <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/26 14:41:45 by sclolus           #+#    #+#             */
/*   Updated: 2017/08/16 22:34:35 by sclolus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

t_mem_block		*ft_set_lines(t_mem_block *mem_block)
{
	t_mem_block	*tmp;
	t_mem_block	*lines;
	uint64_t	offset;
	uint64_t	i;

	i = 0;
	offset = 0;
	tmp = mem_block;
	lines = ft_create_mem_block(DEFAULT_MEM_BLOCK_SIZE);
	while ((i + 1) * sizeof(t_point) < mem_block->offset)
	{
		if (((t_point*)mem_block->block + i)->coords.y
			< (((t_point*)mem_block->block + i + 1))->coords.y)
		{
			i += 1;
			break ;
		}
		ft_mem_block_push_back_elem(lines
			, &(t_line){((t_point*)mem_block->block + i)->coords
			, ((t_point*)mem_block->block + i + 1)->coords
			, 0, 0, 0, ((t_point*)mem_block->block + i)
			, ((t_point*)mem_block->block + i + 1)}, sizeof(t_line));
		i++;
		if ((i) * sizeof(t_point) >= mem_block->offset
			&& mem_block->next)
		{
			mem_block = mem_block->next;
			i = 0;
		}
	}
	while ((i) * sizeof(t_point) < mem_block->offset)
	{
		if ((int)((t_point*)mem_block->block + i)->coords.y
			== (int)(((t_point*)mem_block->block + i + 1))->coords.y)
		{
			ft_mem_block_push_back_elem(lines
				, &(t_line){((t_point*)mem_block->block + i)->coords
				, ((t_point*)mem_block->block + i + 1)->coords
				, 0, 0, 0, ((t_point*)mem_block->block + i)
				, ((t_point*)mem_block->block + i + 1)}, sizeof(t_line));
		}
		ft_mem_block_push_back_elem(lines
			, &(t_line){((t_point*)tmp->block + offset)->coords
			, ((t_point*)mem_block->block + i)->coords
			, 0, 0, 0, ((t_point*)tmp->block + offset)
			, ((t_point*)mem_block->block + i)}, sizeof(t_line));
		offset++;
		i++;
		if ((i) * sizeof(t_point) >= mem_block->offset
			&& mem_block->next && !(i = 0))
			mem_block = mem_block->next;
		if (offset * sizeof(t_point) >= tmp->offset
			&& tmp->next && !(offset = 0))
			tmp = tmp->next;
	}
	return (lines);
}
