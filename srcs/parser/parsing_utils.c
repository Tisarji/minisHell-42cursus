/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jikarunw <jikarunw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 12:04:29 by jikarunw          #+#    #+#             */
/*   Updated: 2025/03/30 22:47:40 by jikarunw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	validate_tokens(t_token *tokens)
{
	t_token	*prev;
	t_token	*curr;

	if (!tokens)
		return (0);
	prev = NULL;
	curr = tokens;
	while (curr)
	{
		if (prev && prev->type == PIPE && curr->type == PIPE)
			return (0);
		if (prev && prev->type != PIPE && curr->type != PIPE && \
			prev->type >= INDIRECT && prev->type <= HEREDOC && \
			curr->type >= INDIRECT && curr->type <= HEREDOC)
			return (0);
		if (curr->type != PIPE && curr->type >= INDIRECT && \
			curr->type <= HEREDOC && \
			(!curr->next || curr->next->type >= INDIRECT))
			return (0);
		if ((curr->type == PIPE) && (!prev || !curr->next))
			return (0);
		prev = curr;
		curr = curr->next;
	}
	return (1);
}

int	count_cmd_arg(t_token *current)
{
	int	arg_count;

	arg_count = 0;
	while (current && current->type == CMD)
	{
		arg_count++;
		current = current->next;
	}
	return (arg_count);
}

void	add_cmd_arg(t_ast *cmd_node, t_token **tokens, int arg_count)
{
	int		i;
	t_token	*tmp;

	i = 0;
	while (i < arg_count)
	{
		cmd_node->args[i] = ft_strdup((*tokens)->cmd);
		if (!cmd_node->args[i])
		{
			while (i > 0)
				free(cmd_node->args[--i]);
			free(cmd_node->args);
			free(cmd_node);
			return ;
		}
		tmp = *tokens;
		*tokens = (*tokens)->next;
		free(tmp->cmd);
		free_cmd_tokens(&tmp);
		free(tmp);
		i++;
	}
	cmd_node->args[arg_count] = NULL;
}

void	free_cmd_tokens(t_token **tokens)
{
	t_token	*tmp;

	if (!tokens || !*tokens)
		return ;
	while (*tokens)
	{
		tmp = *tokens;
		*tokens = (*tokens)->next;
		if (tmp)
		{
			if (tmp->cmd)
			{
				free(tmp->cmd);
				tmp->cmd = NULL;
			}
			free(tmp);
			tmp = NULL;
		}
	}
	*tokens = NULL;
}

void	free_cmd_args(t_ast *cmd_node)
{
	int	i;

	i = 0;
	if (!cmd_node || !cmd_node->args)
		return ;
	while (cmd_node->args[i])
	{
		free(cmd_node->args[i]);
		i++;
	}
	free(cmd_node->args);
	cmd_node->args = NULL;
}
