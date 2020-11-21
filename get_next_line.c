/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzhou <hzhou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/08 18:53:37 by hzhou             #+#    #+#             */
/*   Updated: 2020/11/21 13:38:54 by hzhou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static void	ft_freeing(char **str)
{
	free(*str);
	*str = NULL;
}

static int	gnl_verif(char **stack, char **line)
{
	int		i;
	char	*strchr_n;

	i = 0;
	strchr_n = *stack;
	while (strchr_n[i])
	{
		if (strchr_n[i] == '\n')
		{
			strchr_n[i] = '\0';
			*line = ft_strdup(strchr_n);
			*stack = ft_strdup(&strchr_n[i] + 1);
			ft_freeing(&strchr_n);
			return (1);
		}
		i++;
	}
	return (0);
}

static int	gnl_read(int fd, char *heap, char **stack, char **line)
{
	int		result;
	int		p;
	char	*tmp;

	p = 0;
	result = 0;
	while (p == 0 && (result = read(fd, heap, BUFFER_SIZE)) > 0)
	{
		heap[result] = '\0';
		if (*stack)
		{
			tmp = *stack;
			*stack = ft_strjoin(tmp, heap);
			ft_freeing(&tmp);
		}
		if (!*stack)
			*stack = ft_strdup(heap);
		if (gnl_verif(stack, line))
			p = 1;
	}
	return (result > 0 ? 1 : result);
}

static void	ft_result_zero(char **stack, char **line)
{
	if (*stack == NULL || *stack[0] == '\0')
		*line = ft_strdup("");
	else
		*line = ft_strdup(*stack);
}

int			get_next_line(int fd, char **line)
{
	static char	*stack[OPEN_MAX];
	char		*heap;
	int			result;

	if (fd < 0 || fd > OPEN_MAX || read(fd, stack[fd], 0) < 0 || !line \
			|| BUFFER_SIZE < 1 \
			|| !(heap = ft_calloc(sizeof(char), BUFFER_SIZE + 1)))
		return (-1);
	if (stack[fd])
		if (gnl_verif(&stack[fd], line))
		{
			ft_freeing(&heap);
			return (1);
		}
	result = gnl_read(fd, heap, &stack[fd], line);
	ft_freeing(&heap);
	if (result != 0)
		return (result);
	if (result == 0)
	{
		ft_result_zero(&stack[fd], line);
		ft_freeing(&stack[fd]);
	}
	return (0);
}
