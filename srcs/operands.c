/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 04:55:00 by student           #+#    #+#             */
/*   Updated: 2026/11/03 04:55:00 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

static void	add_error(t_error **errors, int *count, const char *name, int err)
{
	t_error	*new_errors;

	new_errors = realloc(*errors, sizeof(t_error) * (*count + 1));
	if (!new_errors)
		return ;
	*errors = new_errors;
	(*errors)[*count].name = strdup(name);
	(*errors)[*count].err_code = err;
	(*count)++;
}

static void	add_file(t_file **files, int *count, const char *name,
				struct stat st, int is_dir)
{
	t_file	*new_files;

	new_files = realloc(*files, sizeof(t_file) * (*count + 1));
	if (!new_files)
		return ;
	*files = new_files;
	(*files)[*count].name = strdup(name);
	(*files)[*count].full_path = strdup(name);
	(*files)[*count].st = st;
	(*files)[*count].has_stat = 1;
	(*files)[*count].is_dir = is_dir;
	(*count)++;
}

static void	classify_arg(const char *arg, t_flags flags,
				t_error **errs, int *err_cnt,
				t_file **files, int *f_cnt,
				t_file **dirs, int *d_cnt)
{
	struct stat	st;
	DIR			*test_dir;
	size_t		len;
	int			is_dir;

	if (lstat(arg, &st) == -1)
	{
		add_error(errs, err_cnt, arg, errno);
		return ;
	}
	is_dir = 0;
	if (S_ISDIR(st.st_mode))
		is_dir = 1;
	else if (S_ISLNK(st.st_mode))
	{
		len = strlen(arg);
		if (len > 0 && arg[len - 1] == '/')
		{
			test_dir = opendir(arg);
			if (test_dir)
			{
				closedir(test_dir);
				is_dir = 1;
			}
			else
			{
				add_error(errs, err_cnt, arg, errno);
				return ;
			}
		}
		else if (!flags.l)
		{
			test_dir = opendir(arg);
			if (test_dir)
			{
				closedir(test_dir);
				is_dir = 1;
			}
		}
	}
	if (is_dir)
		add_file(dirs, d_cnt, arg, st, 1);
	else
		add_file(files, f_cnt, arg, st, 0);
}

void	process_operands(int argc, char **argv, int opt_index,
			t_flags flags, int *exit_status)
{
	t_error	*errors = NULL;
	t_file	*files = NULL;
	t_file	*dirs = NULL;
	int		err_cnt = 0;
	int		f_cnt = 0;
	int		d_cnt = 0;
	int		printed_something = 0;
	int		total_ops;
	int		print_header;
	int		i;

	if (opt_index == argc)
	{
		traverse_directory(".", flags, 0, exit_status);
		return ;
	}
	i = opt_index;
	while (i < argc)
	{
		classify_arg(argv[i], flags, &errors, &err_cnt, &files, &f_cnt, &dirs, &d_cnt);
		i++;
	}
	sort_errors(errors, err_cnt);
	i = 0;
	while (i < err_cnt)
	{
		fprintf(stderr, "ft_ls: %s: %s\n", errors[i].name, strerror(errors[i].err_code));
		*exit_status = 1;
		i++;
	}
	if (f_cnt > 0)
	{
		sort_files(files, f_cnt, flags);
		print_files(files, f_cnt, flags, 0, 0);
		printed_something = 1;
	}
	total_ops = err_cnt + f_cnt + d_cnt;
	print_header = (total_ops > 1);
	if (d_cnt > 0)
	{
		sort_files(dirs, d_cnt, flags);
		i = 0;
		while (i < d_cnt)
		{
			if (printed_something)
				printf("\n");
			traverse_directory(dirs[i].name, flags, print_header, exit_status);
			printed_something = 1;
			i++;
		}
	}
	free_errors(errors, err_cnt);
	free_files(files, f_cnt);
	free_files(dirs, d_cnt);
}
