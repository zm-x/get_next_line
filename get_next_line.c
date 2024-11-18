/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zael-mou <zael-mou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 10:15:04 by zael-mou          #+#    #+#             */
/*   Updated: 2024/11/18 14:23:17 by zael-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	get_line(char **reminder, char *cpy, int *byt_read, int fd)
{
	char	*tmp;
	int		nm;

	nm = read(fd, cpy, BUFFER_SIZE);
	if (nm < 0)
	{
		free(cpy);
		free(*reminder);
		*reminder = NULL;
		cpy = NULL;
		return (-1);
	}
	cpy[nm] = '\0';
	*byt_read = nm;
	tmp = ft_strjoin(*reminder, cpy);
	if (*reminder)
		free(*reminder);
	*reminder = tmp;
	if (check(*reminder))
	{
		free(cpy);
		cpy = NULL;
		return (1);
	}
	return (0);
}

char	*return_line(char **reminder)
{
	int		i;
	int		j;
	char	*cpy;
	char	*line;

	line = ft_strndup(*reminder, &i);
	j = 0;
	cpy = malloc(ft_strlen(*reminder) - i + 1);
	if (!cpy)
	{
		free(*reminder);
		*reminder = NULL;
		return (NULL);
	}
	while ((*reminder)[i])
		cpy[j++] = (*reminder)[i++];
	cpy[j] = '\0';
	if (*reminder)
		free(*reminder);
	*reminder = cpy;
	return (line);
}

char	*free_after_last_line(char **reminder, char *cpy)
{
	if (cpy)
	{
		free(cpy);
		cpy = NULL;
	}
	if (*reminder)
	{
		free(*reminder);
		*reminder = NULL;
	}
	return (NULL);
}

char	*ft_check(int *status, char **reminder, char *cpy, int fd)
{
	while (status[1] > 0)
	{
		status[0] = get_line(reminder, cpy, &status[1], fd);
		if (status[0] == 1)
			break ;
		else if (status[0] == -1)
			return (NULL);
	}
	return (*reminder);
}

char	*get_next_line(int fd)
{
	char		*cpy;
	static char	*reminder;
	int			status[2];

	if ((fd < 0) || (BUFFER_SIZE <= 0))
		return (NULL);
	if (!reminder)
		reminder = ft_strdup("");
	if (!check(reminder))
	{
		cpy = malloc(BUFFER_SIZE + 1);
		if (!cpy)
		{
			(free(reminder), reminder = NULL);
			return (NULL);
		}
		status[1] = 1;
		if (!ft_check(status, &reminder, cpy, fd))
			return (NULL);
		if (status[1] <= 0 && cpy)
			(free(cpy), cpy = NULL);
	}
	if (*reminder == '\0' && status[1] <= 0)
		return (free_after_last_line(&reminder, cpy));
	return (return_line(&reminder));
}
/*
#include <fcntl.h>
#include <stdio.h>

int	main(void)
{
	int n = open("file.txt", O_RDONLY);
	char *line = get_next_line(n);
	while (line)
	{
		printf("%s" , line);
		free(line);
		line = get_next_line(n);
	}
}*/
