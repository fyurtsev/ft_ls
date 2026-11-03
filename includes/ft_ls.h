/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 04:55:00 by student           #+#    #+#             */
/*   Updated: 2026/11/03 04:55:00 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <dirent.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <pwd.h>
# include <grp.h>
# include <time.h>
# include <unistd.h>
# include <errno.h>

# if defined(__APPLE__)
#  define STAT_NSEC(st) ((st).st_mtimespec.tv_nsec)
# else
#  define STAT_NSEC(st) ((st).st_mtim.tv_nsec)
# endif

typedef struct s_flags {
	int	l;
	int	R;
	int	a;
	int	r;
	int	t;
}	t_flags;

typedef struct s_file {
	char		*name;
	char		*full_path;
	struct stat	st;
	int			has_stat;
	int			is_dir;
}	t_file;

typedef struct s_error {
	char	*name;
	int		err_code;
}	t_error;

typedef struct s_widths {
	int	link;
	int	user;
	int	group;
	int	size;
	int	major;
	int	minor;
	int	has_device;
}	t_widths;

/* options.c */
int		parse_flags(int argc, char **argv, t_flags *flags, int *opt_index);

/* sort.c */
int		compare_files(const t_file *a, const t_file *b, t_flags flags);
void	sort_files(t_file *files, int count, t_flags flags);
void	sort_errors(t_error *errors, int count);

/* display.c */
void	compute_widths(t_file *files, int count, t_widths *w);
void	print_file_long(const t_file *file, const t_widths *w);
void	print_files(t_file *files, int count, t_flags flags,
			int is_dir_listing, long long total_blocks);

/* traverse.c */
void	traverse_directory(const char *path, t_flags flags,
			int print_header, int *exit_status);

/* operands.c */
void	process_operands(int argc, char **argv, int opt_index,
			t_flags flags, int *exit_status);

/* utils.c */
char	*join_path(const char *dir, const char *name);
void	free_files(t_file *files, int count);
void	free_errors(t_error *errors, int count);
int		num_digits(long long n);
void	ft_bzero(void *s, size_t n);

#endif
