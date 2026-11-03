/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 04:55:00 by student           #+#    #+#             */
/*   Updated: 2026/11/03 04:55:00 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

int	main(int argc, char **argv)
{
	t_flags	flags;
	int		opt_index;
	int		exit_status;

	ft_bzero(&flags, sizeof(t_flags));
	opt_index = 1;
	exit_status = 0;
	if (parse_flags(argc, argv, &flags, &opt_index) == -1)
		return (1);
	process_operands(argc, argv, opt_index, flags, &exit_status);
	return (exit_status);
}
