/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_parsing_heredoc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jikarunw <jikarunw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 22:45:13 by jikarunw          #+#    #+#             */
/*   Updated: 2025/04/02 22:47:28 by jikarunw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_ast	*create_heredoc_node_command(t_token **tokens)
{
	t_ast	*heredoc_word_node;

	if (!tokens || !*tokens)
		return (NULL);
	heredoc_word_node = msh_init_ast(HEREDOC_WORD);
	if (!heredoc_word_node)
		return (NULL);
	heredoc_word_node->args = malloc(sizeof(char *) * 2);
	if (!heredoc_word_node->args)
	{
		free(heredoc_word_node);
		return (NULL);
	}
	heredoc_word_node->args[0] = ft_strdup((*tokens)->cmd);
	if (!heredoc_word_node->args[0])
	{
		free_cmd_args(heredoc_word_node);
		free(heredoc_word_node);
		return (NULL);
	}
	heredoc_word_node->args[1] = NULL;
	*tokens = (*tokens)->next;
	return (heredoc_word_node);
}

t_ast	*handle_heredoc(t_token **tokens, t_ast *cmd_node)
{
	t_ast	*heredoc_node;
	t_ast	*heredoc_word_node;
	t_ast	*current;

	if (!tokens || !*tokens || !cmd_node)
		return (NULL);
	heredoc_node = msh_init_ast(HEREDOC);
	if (!heredoc_node)
		return (NULL);
	*tokens = (*tokens)->next;
	if (*tokens && (*tokens)->type == CMD)
	{
		heredoc_word_node = create_heredoc_node_command(tokens);
		if (!heredoc_word_node)
		{
			free(heredoc_node);
			return (NULL);
		}
		heredoc_node->right = heredoc_word_node;
	}
	current = cmd_node;
	while (current->right)
		current = current->right;
	current->right = heredoc_node;
	return (cmd_node);
}

void	process_heredoc_if_needed(t_token **tokens, t_ast *cmd_node)
{
	if (*tokens && (*tokens)->type == HEREDOC)
	{
		if (!handle_heredoc(tokens, cmd_node))
			cleanup_cmd_node(cmd_node);
	}
}

int	init_cmd_node_args(t_ast *cmd_node, t_token **tokens)
{
	cmd_node->args = malloc(sizeof(char *) * 2);
	if (!cmd_node->args)
		return (0);
	cmd_node->args[0] = ft_strdup((*tokens)->cmd);
	if (!cmd_node->args[0])
	{
		free(cmd_node->args);
		return (0);
	}
	cmd_node->args[1] = NULL;
	*tokens = (*tokens)->next;
	return (1);
}
