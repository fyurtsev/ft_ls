/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 04:55:00 by student           #+#    #+#             */
/*   Updated: 2026/11/03 04:55:00 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

static void	get_permissions_string(mode_t mode, char *p)
{
	p[0] = S_ISDIR(mode) ? 'd' :
		S_ISLNK(mode) ? 'l' :
		S_ISCHR(mode) ? 'c' :
		S_ISBLK(mode) ? 'b' :
		S_ISFIFO(mode) ? 'p' :
		S_ISSOCK(mode) ? 's' : '-';
	p[1] = (mode & S_IRUSR) ? 'r' : '-';
	p[2] = (mode & S_IWUSR) ? 'w' : '-';
	if (mode & S_ISUID)
		p[3] = (mode & S_IXUSR) ? 's' : 'S';
	else
		p[3] = (mode & S_IXUSR) ? 'x' : '-';
	p[4] = (mode & S_IRGRP) ? 'r' : '-';
	p[5] = (mode & S_IWGRP) ? 'w' : '-';
	if (mode & S_ISGID)
		p[6] = (mode & S_IXGRP) ? 's' : 'S';
	else
		p[6] = (mode & S_IXGRP) ? 'x' : '-';
	p[7] = (mode & S_IROTH) ? 'r' : '-';
	p[8] = (mode & S_IWOTH) ? 'w' : '-';
	if (mode & S_ISVTX)
		p[9] = (mode & S_IXOTH) ? 't' : 'T';
	else
		p[9] = (mode & S_IXOTH) ? 'x' : '-';
	p[10] = '\0';
}

static void	get_time_string(time_t mtime, char *time_str, size_t max_len)
{
	struct tm	*tm_info;

	tm_info = localtime(&mtime);
	if (!tm_info)
	{
		snprintf(time_str, max_len, "            ");
		return ;
	}
	strftime(time_str, max_len, "%b %e %H:%M", tm_info);
}

void	compute_widths(t_file *files, int count, t_widths *w)
{
	int				i;
	int				d;
	struct passwd	*pw;
	struct group	*gr;

	ft_bzero(w, sizeof(t_widths));
	i = 0;
	while (i < count)
	{
		d = num_digits((long long)files[i].st.st_nlink);
		if (d > w->link)
			w->link = d;
		pw = getpwuid(files[i].st.st_uid);
		d = pw ? (int)strlen(pw->pw_name) : num_digits(files[i].st.st_uid);
		if (d > w->user)
			w->user = d;
		gr = getgrgid(files[i].st.st_gid);
		d = gr ? (int)strlen(gr->gr_name) : num_digits(files[i].st.st_gid);
		if (d > w->group)
			w->group = d;
		if (S_ISCHR(files[i].st.st_mode) || S_ISBLK(files[i].st.st_mode))
		{
			w->has_device = 1;
			d = num_digits(major(files[i].st.st_rdev));
			if (d > w->major)
				w->major = d;
			d = num_digits(minor(files[i].st.st_rdev));
			if (d > w->minor)
				w->minor = d;
		}
		else
		{
			d = num_digits(files[i].st.st_size);
			if (d > w->size)
				w->size = d;
		}
		i++;
	}
	if (w->has_device)
	{
		if (w->major < 3)
			w->major = 3;
		if (w->minor < 3)
			w->minor = 3;
		if (w->major + 2 + w->minor > w->size)
			w->size = w->major + 2 + w->minor;
	}
}

void	print_file_long(const t_file *file, const t_widths *w)
{
	char			perm[11];
	char			time_str[32];
	char			u_buf[32];
	char			g_buf[32];
	char			target[1024];
	struct passwd	*pw;
	struct group	*gr;
	const char		*user;
	const char		*group;
	ssize_t			len;

	get_permissions_string(file->st.st_mode, perm);
	get_time_string(file->st.st_mtime, time_str, sizeof(time_str));
	pw = getpwuid(file->st.st_uid);
	if (pw)
		user = pw->pw_name;
	else
	{
		snprintf(u_buf, sizeof(u_buf), "%u", file->st.st_uid);
		user = u_buf;
	}
	gr = getgrgid(file->st.st_gid);
	if (gr)
		group = gr->gr_name;
	else
	{
		snprintf(g_buf, sizeof(g_buf), "%u", file->st.st_gid);
		group = g_buf;
	}
	printf("%s  %*lu %-*s  %-*s  ",
		perm,
		w->link, (unsigned long)file->st.st_nlink,
		w->user, user,
		w->group, group);
	if (w->has_device)
	{
		if (S_ISCHR(file->st.st_mode) || S_ISBLK(file->st.st_mode))
		{
			int maj_w = w->size - w->minor - 2;
			if (maj_w < w->major)
				maj_w = w->major;
			printf("%*u, %*u ",
				maj_w, (unsigned int)major(file->st.st_rdev),
				w->minor, (unsigned int)minor(file->st.st_rdev));
		}
		else
			printf("%*lld ", w->size, (long long)file->st.st_size);
	}
	else
		printf("%*lld ", w->size, (long long)file->st.st_size);
	printf("%s %s", time_str, file->name);
	if (S_ISLNK(file->st.st_mode))
	{
		len = readlink(file->full_path, target, sizeof(target) - 1);
		if (len != -1)
		{
			target[len] = '\0';
			printf(" -> %s", target);
		}
	}
	printf("\n");
}

void	print_files(t_file *files, int count, t_flags flags,
			int is_dir_listing, long long total_blocks)
{
	t_widths	w;
	int			i;

	if (flags.l)
	{
		if (is_dir_listing)
			printf("total %lld\n", total_blocks);
		if (count > 0)
		{
			compute_widths(files, count, &w);
			i = 0;
			while (i < count)
			{
				print_file_long(&files[i], &w);
				i++;
			}
		}
	}
	else
	{
		i = 0;
		while (i < count)
		{
			printf("%s\n", files[i].name);
			i++;
		}
	}
}
