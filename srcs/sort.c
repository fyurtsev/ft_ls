/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 04:55:00 by student           #+#    #+#             */
/*   Updated: 2026/11/03 04:55:00 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

int	compare_files(const t_file *a, const t_file *b, t_flags flags)
{
	int	res;

	res = 0;
	if (flags.t)
	{
		if (a->st.st_mtime != b->st.st_mtime)
		{
			if (b->st.st_mtime > a->st.st_mtime)
				res = 1;
			else
				res = -1;
		}
		else if (STAT_NSEC(a->st) != STAT_NSEC(b->st))
		{
			if (STAT_NSEC(b->st) > STAT_NSEC(a->st))
				res = 1;
			else
				res = -1;
		}
		else
			res = strcmp(a->name, b->name);
	}
	else
		res = strcmp(a->name, b->name);
	if (flags.r)
		res = -res;
	return (res);
}

static void	swap_files(t_file *a, t_file *b)
{
	t_file	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static void	quicksort_files_rec(t_file *arr, int low, int high, t_flags flags)
{
	t_file	pivot;
	int		i;
	int		j;

	if (low >= high)
		return ;
	pivot = arr[low + (high - low) / 2];
	i = low;
	j = high;
	while (i <= j)
	{
		while (compare_files(&arr[i], &pivot, flags) < 0)
			i++;
		while (compare_files(&arr[j], &pivot, flags) > 0)
			j--;
		if (i <= j)
		{
			swap_files(&arr[i], &arr[j]);
			i++;
			j--;
		}
	}
	if (low < j)
		quicksort_files_rec(arr, low, j, flags);
	if (i < high)
		quicksort_files_rec(arr, i, high, flags);
}

void	sort_files(t_file *files, int count, t_flags flags)
{
	if (files && count > 1)
		quicksort_files_rec(files, 0, count - 1, flags);
}

void	sort_errors(t_error *errors, int count)
{
	int		i;
	int		j;
	t_error	tmp;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (strcmp(errors[j].name, errors[j + 1].name) > 0)
			{
				tmp = errors[j];
				errors[j] = errors[j + 1];
				errors[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

