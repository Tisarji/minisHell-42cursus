/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jikarunw <jikarunw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 12:03:40 by jikarunw          #+#    #+#             */
/*   Updated: 2025/03/17 17:08:57 by jikarunw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_heredoc_nodes(t_ast *node)
{
	if (!node)
		return ;
	free_heredoc_nodes(node->left);
	free_heredoc_nodes(node->right);
	if ((node->type == HEREDOC || node->type == HEREDOC_WORD) && node->args)
	{
		if (node->args[0])
			free(node->args[0]);
		free(node->args);
	}
	free(node);
}

t_ast	*create_heredoc_node(t_token **token)
{
	t_ast	*heredoc_node;

	heredoc_node = msh_init_ast(HEREDOC);
	if (!heredoc_node)
		return (NULL);
	*token = (*token)->next;
	if (!*token || (*token)->type != CMD)
	{
		free(heredoc_node);
		return (NULL);
	}
	return (heredoc_node);
}

static t_ast	*init_heredoc_word_node(void)
{
	t_ast	*heredoc_word_node;

	heredoc_word_node = msh_init_ast(HEREDOC_WORD);
	if (!heredoc_word_node)
		return (NULL);
	heredoc_word_node->args = malloc(sizeof(char *) * 2);
	if (!heredoc_word_node->args)
	{
		free(heredoc_word_node);
		return (NULL);
	}
	heredoc_word_node->args[0] = NULL;
	heredoc_word_node->args[1] = NULL;
	return (heredoc_word_node);
}

t_ast	*create_heredoc_word_node(t_token **token)
{
	t_ast	*heredoc_word_node;
	t_token	*next_token;

	if (!token || !*token)
		return (NULL);
	heredoc_word_node = init_heredoc_word_node();
	if (!heredoc_word_node)
		return (NULL);
	heredoc_word_node->args[0] = ft_strdup((*token)->cmd);
	if (!heredoc_word_node->args[0])
	{
		free(heredoc_word_node->args);
		free(heredoc_word_node);
		return (NULL);
	}
	next_token = (*token)->next;
	free((*token)->cmd);
	free(*token);
	*token = next_token;
	return (heredoc_word_node);
}

t_ast	*msh_get_heredoc_word(t_token **token)
{
	t_ast	*heredoc_node;
	t_ast	*heredoc_word_node;

	if (!token || !*token || (*token)->type != HEREDOC)
		return (NULL);
	heredoc_node = create_heredoc_node(token);
	if (!heredoc_node)
		return (NULL);
	heredoc_word_node = create_heredoc_word_node(token);
	if (!heredoc_word_node)
	{
		free_heredoc_nodes(heredoc_node);
		return (NULL);
	}
	heredoc_node->right = heredoc_word_node;
	if (*token && (*token)->type == HEREDOC)
	{
		heredoc_node->left = msh_get_heredoc_word(token);
		if (!heredoc_node->left)
			free_heredoc_nodes(heredoc_node);
	}
	return (heredoc_node);
}
