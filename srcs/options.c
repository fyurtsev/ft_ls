/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 04:55:00 by student           #+#    #+#             */
/*   Updated: 2026/11/03 04:55:00 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

static int	parse_option_char(char c, t_flags *flags)
{
	if (c == 'l')
		flags->l = 1;
	else if (c == 'R')
		flags->R = 1;
	else if (c == 'a')
		flags->a = 1;
	else if (c == 'r')
		flags->r = 1;
	else if (c == 't')
		flags->t = 1;
	else
	{
		fprintf(stderr, "ft_ls: illegal option -- %c\n", c);
		fprintf(stderr, "usage: ft_ls [-Ralrt] [file ...]\n");
		return (-1);
	}
	return (0);
}

int	parse_flags(int argc, char **argv, t_flags *flags, int *opt_index)
{
	int	i;
	int	j;

	i = 1;
	while (i < argc && argv[i][0] == '-' && argv[i][1] != '\0')
	{
		if (strcmp(argv[i], "--") == 0)
		{
			i++;
			break ;
		}
		j = 1;
		while (argv[i][j] != '\0')
		{
			if (parse_option_char(argv[i][j], flags) == -1)
				return (-1);
			j++;
		}
		i++;
	}
	*opt_index = i;
	return (0);
}

