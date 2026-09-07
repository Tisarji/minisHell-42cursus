/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jikarunw <jikarunw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 15:28:33 by krwongwa          #+#    #+#             */
/*   Updated: 2025/03/17 20:40:59 by jikarunw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	run_cmd(t_p *list, int status)
{
	char	*cmd_path;

	cmd_path = find_path(list->cmd, list->path);
	dup2(list->fd_in, 0);
	safe_fd(list, 0);
	if (status == -1)
	{
		*list->code = 1;
		exit(*list->code);
	}
	if (execve(cmd_path, list->args, list->env) == -1)
	{
		*list->code = ft_puterrorcmd(list->cmd, errno);
		if (cmd_path != NULL)
			free(cmd_path);
		exit(*list->code);
	}
	free(cmd_path);
}

void	child_process(t_p *list, int status)
{
	int	a;

	a = -1;
	mode_signal_exe(2);
	if (list->fd_out != -1)
	{
		dup2(list->fd_out, 1);
		safe_close(&list->pipe[0]);
		safe_close(&list->pipe[1]);
		safe_fd(list, 1);
	}
	else
		pipe_write(list);
	if (status == -1)
	{
		*list->code = 1;
		exit(*list->code);
	}
	check_build_in_command(list->cmd, &a);
	if (a == 0)
	{
		*list->code = msh_execute_builtin(list);
		exit(*list->code);
	}
	else
		run_cmd(list, status);
}

void	parent_process(t_p *list)
{
	if (list->fd_in > -1)
		safe_fd(list, 0);
	safe_fd(list, 1);
	list->fd_in = dup(list->pipe[0]);
	safe_close(&list->pipe[0]);
	safe_close(&list->pipe[1]);
}

void	mutiple_exe(t_ast *ast, t_p *list)
{
	int	status;

	status = 1;
	prepare_cmd(ast, list, &status);
	list->process_pid[list->iter] = fork();
	if (list->process_pid[list->iter] == -1)
	{
		perror("FORK : ");
		*list->code = errno;
	}
	if (list->process_pid[list->iter] == 0)
		child_process(list, status);
	else
		parent_process(list);
	clear_list(list);
	list->iter++;
}

void	pipe_task(t_ast *ast, t_p *list)
{
	if (!ast)
		return ;
	if (ast->type == CMD_GROUP)
		mutiple_exe(ast, list);
	else if (ast->type == PIPE)
	{
		if (pipe(list->pipe) == -1)
		{
			perror("PIPE : ");
			*list->code = errno;
			return ;
		}
		pipe_task(ast->left, list);
		pipe_task(ast->right, list);
	}
}
