/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 04:55:00 by student           #+#    #+#             */
/*   Updated: 2026/11/03 04:55:00 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*p;

	p = (unsigned char *)s;
	while (n--)
		*p++ = 0;
}

char	*join_path(const char *dir, const char *name)
{
	size_t	dlen;
	size_t	nlen;
	char	*path;

	dlen = strlen(dir);
	nlen = strlen(name);
	if (dlen > 0 && dir[dlen - 1] == '/')
	{
		path = malloc(dlen + nlen + 1);
		if (!path)
			return (NULL);
		snprintf(path, dlen + nlen + 1, "%s%s", dir, name);
	}
	else
	{
		path = malloc(dlen + 1 + nlen + 1);
		if (!path)
			return (NULL);
		snprintf(path, dlen + 1 + nlen + 1, "%s/%s", dir, name);
	}
	return (path);
}

void	free_files(t_file *files, int count)
{
	int	i;

	if (!files)
		return ;
	i = 0;
	while (i < count)
	{
		free(files[i].name);
		free(files[i].full_path);
		i++;
	}
	free(files);
}

void	free_errors(t_error *errors, int count)
{
	int	i;

	if (!errors)
		return ;
	i = 0;
	while (i < count)
	{
		free(errors[i].name);
		i++;
	}
	free(errors);
}

int	num_digits(long long n)
{
	int	digits;

	if (n == 0)
		return (1);
	if (n < 0)
		n = -n;
	digits = 0;
	while (n > 0)
	{
		digits++;
		n /= 10;
	}
	return (digits);
}
