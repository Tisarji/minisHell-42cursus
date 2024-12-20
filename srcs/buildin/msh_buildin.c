/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_buildin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jikarunw <jikarunw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 17:47:52 by jikarunw          #+#    #+#             */
/*   Updated: 2024/12/05 15:30:08 by jikarunw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	msh_execute_builtin(t_msh *msh)
{
	if (!msh || !msh->ast || !msh->ast->args || !msh->ast->args[0])
		return (EXIT_FAILURE);
	if (ft_strcmp(msh->ast->args[0], "echo") == 0)
		return (msh_echo(msh));
	else if (ft_strcmp(msh->ast->args[0], "cd") == 0)
		return (msh_cd(msh, msh->ast->args[1]));
	else if (ft_strcmp(msh->ast->args[0], "pwd") == 0)
		return (msh_pwd(msh));
	else if (ft_strcmp(msh->ast->args[0], "export") == 0)
	{
		if (!msh->ast->args[1])
		{
			ft_putstr_fd("minishell: export: not enough arguments\n", STDERR_FILENO);
			return (EXIT_FAILURE);
		}
		return (msh_export(msh, msh->ast->args[1]));
	}
	else if (ft_strcmp(msh->ast->args[0], "unset") == 0)
	{
		if (!msh->ast->args[1])
		{
			ft_putstr_fd("minishell: unset: not enough arguments\n", STDERR_FILENO);
			return (EXIT_FAILURE);
		}
		return (msh_unset(msh, msh->ast->args[1]));
	}
	else if (ft_strcmp(msh->ast->args[0], "env") == 0)
		return (msh_env(msh));
	else if (ft_strcmp(msh->ast->args[0], "exit") == 0)
		return (msh_exit(msh));
	ft_putstr_fd("minishell: command not found\n", STDERR_FILENO);
	return (EXIT_FAILURE);
}
