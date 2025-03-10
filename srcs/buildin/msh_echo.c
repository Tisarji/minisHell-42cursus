/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jikarunw <jikarunw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 18:11:22 by jikarunw          #+#    #+#             */
/*   Updated: 2025/03/10 08:37:56 by jikarunw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	loop_echo(t_p *list, int i)
{
	int	i;
	int	n_flag;
	int saved_stdout;

	saved_stdout = dup(STDOUT_FILENO);
	n_flag = 0;
	i = 1;
	handle_fd(list);
	if (list->args[i] && !ft_strncmp(list->args[i], "-n", 2))
	{
		n_flag = 1;
		i++;
	}
	while (list->args[i])
	{
		ft_putstr_fd(list->args[i], STDOUT_FILENO);
		if (list->args[i + 1] != NULL)
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
}

int	msh_echo(t_p *list)
{
	int	i;
	int	n_flag;
	int	saved_stdout;

	saved_stdout = dup(STDOUT_FILENO);
	n_flag = 0;
	i = 1;
	handle_fd(list);
	if (!ft_strncmp(list->args[i], "-n", 2))
	{
		n_flag = 1;
		i++;
	}
	loop_echo(list, i);
	if (!n_flag)
		ft_putstr_fd("\n", STDOUT_FILENO);
	if (list->fd_out != 1)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
	return (0);
}
