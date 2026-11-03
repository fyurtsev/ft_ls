/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traverse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 04:55:00 by student           #+#    #+#             */
/*   Updated: 2026/11/03 04:55:00 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

static t_file	*add_entry(t_file *files, int *count, int *capacity,
					const char *dir_path, const char *d_name,
					long long *total_blocks)
{
	t_file	*new_files;

	if (*count >= *capacity)
	{
		*capacity = (*capacity == 0) ? 32 : *capacity * 2;
		new_files = realloc(files, sizeof(t_file) * (*capacity));
		if (!new_files)
			return (NULL);
		files = new_files;
	}
	files[*count].name = strdup(d_name);
	files[*count].full_path = join_path(dir_path, d_name);
	files[*count].has_stat = 0;
	files[*count].is_dir = 0;
	if (files[*count].full_path &&
		lstat(files[*count].full_path, &files[*count].st) == 0)
	{
		files[*count].has_stat = 1;
		*total_blocks += files[*count].st.st_blocks;
		files[*count].is_dir = S_ISDIR(files[*count].st.st_mode);
	}
	else
		ft_bzero(&files[*count].st, sizeof(struct stat));
	(*count)++;
	return (files);
}

static void	handle_recursion(t_file *files, int count, t_flags flags,
				int *exit_status)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (files[i].is_dir &&
			strcmp(files[i].name, ".") != 0 &&
			strcmp(files[i].name, "..") != 0)
		{
			printf("\n");
			traverse_directory(files[i].full_path, flags, 1, exit_status);
		}
		i++;
	}
}

void	traverse_directory(const char *path, t_flags flags,
			int print_header, int *exit_status)
{
	DIR				*dir;
	struct dirent	*entry;
	t_file			*files;
	int				count;
	int				capacity;
	long long		total_blocks;

	dir = opendir(path);
	if (!dir)
	{
		if (print_header)
			printf("%s:\n", path);
		fprintf(stderr, "ft_ls: %s: %s\n", path, strerror(errno));
		if (exit_status)
			*exit_status = 1;
		return ;
	}
	if (print_header)
		printf("%s:\n", path);
	files = NULL;
	count = 0;
	capacity = 0;
	total_blocks = 0;
	while ((entry = readdir(dir)) != NULL)
	{
		if (!flags.a && entry->d_name[0] == '.')
			continue ;
		files = add_entry(files, &count, &capacity, path,
				entry->d_name, &total_blocks);
	}
	closedir(dir);
	sort_files(files, count, flags);
	print_files(files, count, flags, 1, total_blocks);
	if (flags.R)
		handle_recursion(files, count, flags, exit_status);
	free_files(files, count);
}
