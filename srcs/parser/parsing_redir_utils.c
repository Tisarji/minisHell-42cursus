/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redir_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jikarunw <jikarunw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 00:00:00 by jikarunw          #+#    #+#             */
/*   Updated: 2026/08/28 00:00:00 by jikarunw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_file_list_redir_fail(t_ast *left, t_token *next, t_token *tmp)
{
	if (left)
		free_ast(left);
	if (next)
	{
		if (next->cmd)
			free(next->cmd);
		free(next);
	}
	if (tmp)
	{
		if (tmp->cmd)
			free(tmp->cmd);
		free(tmp);
	}
}

void	free_handle_redirect_fail(t_ast *left, t_token *file_token,
		t_token *next_token)
{
	if (left)
		free_ast(left);
	if (file_token)
	{
		if (file_token->cmd)
			free(file_token->cmd);
		free(file_token);
	}
	if (next_token->cmd)
		free(next_token->cmd);
	free(next_token);
}

t_ast	*finish_redirect_node(t_ast *redirect_node, t_token *tmp,
		t_token *file_token, t_token *next_token)
{
	redirect_node->left = msh_get_redirect(&tmp);
	redirect_node->right = file_ast_node(file_token);
	if (!redirect_node->right)
	{
		free_handle_redirect_fail(redirect_node->left, file_token,
			next_token);
		free(redirect_node);
		return (NULL);
	}
	free(next_token->cmd);
	free(next_token);
	return (redirect_node);
}
