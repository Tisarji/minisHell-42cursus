/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jikarunw <jikarunw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 13:20:30 by krwongwa          #+#    #+#             */
/*   Updated: 2025/03/17 17:16:04 by jikarunw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clear_list(t_p *list)
{
	if (list->args)
		list->args = NULL;
	if (list->cmd)
		list->cmd = NULL;
}

void	free2d(char **str)
{
	int	i;

	if (!str)
		return ;
	i = 0;
	while (str[i])
	{
		if (str[i])
			free(str[i]);
		i++;
	}
	free(str);
}

void	free_list(t_p *list)
{
	clear_list(list);
	free(list->process_pid);
	if (list->path)
		free2d(list->path);
	list->path = NULL;
	list->msh = NULL;
	list->env = NULL;
	safe_close(&list->pipe[0]);
	safe_close(&list->pipe[1]);
	list->pipe[0] = -1;
	list->pipe[1] = -1;
	safe_fd(list, 0);
	safe_fd(list, 1);
}

void	prepare_cmd(t_ast *ast, t_p *list, int *status)
{
	if (!ast)
		return ;
	if (*status != -1
		&& (ast->type == REDIRECT || ast->type == APPEND || ast->type == INDIRECT))
	{
		if (ast->type == REDIRECT)
			open_out_file(ast->right->args[0], list, 1, status);
		else if (ast->type == APPEND)
			open_out_file(ast->right->args[0], list, 0, status);
		else if (ast->type == INDIRECT)
			open_in_file(ast->right->args[0], list, status);
	}
	if (ast->type == CMD)
	{
		if (list->cmd == NULL)
			list->cmd = ast->args[0];
		if (list->args == NULL)
			list->args = ast->args;
	}
	prepare_cmd(ast->left, list, status);
	prepare_cmd(ast->right, list, status);
}
