/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jikarunw <jikarunw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 12:03:47 by jikarunw          #+#    #+#             */
/*   Updated: 2025/04/02 22:14:02 by jikarunw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_ast	*create_file_list_redir(t_token **tokens, t_token *tmp)
{
	t_ast	*redirect_node;
	t_token	*next;

	if (!tokens || !*tokens || !(*tokens)->next)
		return (NULL);
	redirect_node = msh_init_ast((*tokens)->type);
	if (!redirect_node)
		return (NULL);
	next = (*tokens)->next;
	*tokens = next->next;
	redirect_node->left = msh_get_redirect(tokens);
	redirect_node->right = file_ast_node(next);
	if (!redirect_node->right)
	{
		free_file_list_redir_fail(redirect_node->left, next, tmp);
		free(redirect_node);
		return (NULL);
	}
	free(tmp->cmd);
	free(tmp);
	return (redirect_node);
}

t_ast	*handle_redirect(t_token **tokens, t_token *tmp)
{
	t_ast	*redirect_node;
	t_token	*next_token;
	t_token	*file_token;
	t_token	*continuation;

	if (!tokens || !*tokens || !(*tokens)->next || !(*tokens)->next->next)
		return (NULL);
	next_token = (*tokens)->next;
	file_token = next_token->next;
	continuation = file_token->next;
	redirect_node = msh_init_ast((*tokens)->next->type);
	if (!redirect_node)
		return (NULL);
	(*tokens)->next = continuation;
	*tokens = continuation;
	return (finish_redirect_node(redirect_node, tmp, file_token, next_token));
}

t_ast	*build_heredoc_redirect(t_token **tokens, t_token *next_token,
		t_token *tmp)
{
	t_ast	*result;
	t_ast	*last_heredoc;

	(*tokens)->next = NULL;
	*tokens = next_token;
	result = msh_get_heredoc_word(tokens);
	if (result)
	{
		last_heredoc = result;
		while (last_heredoc->left)
			last_heredoc = last_heredoc->left;
		last_heredoc->left = msh_get_redirect(&tmp);
	}
	return (result);
}

t_ast	*process_redirection_tokens(t_token **tokens, t_token *tmp)
{
	t_token	*next_token;
	t_ast	*result;

	if ((*tokens)->type >= INDIRECT && (*tokens)->type <= HEREDOC)
	{
		if ((*tokens)->type == HEREDOC)
			result = msh_get_heredoc_word(tokens);
		else
			result = create_file_list_redir(tokens, tmp);
		return (result);
	}
	while (*tokens && (*tokens)->next)
	{
		next_token = (*tokens)->next;
		if (next_token->type >= INDIRECT && next_token->type <= HEREDOC)
		{
			if (next_token->type == HEREDOC)
				result = build_heredoc_redirect(tokens, next_token, tmp);
			else
				result = handle_redirect(tokens, tmp);
			return (result);
		}
		*tokens = next_token;
	}
	return (NULL);
}

t_ast	*msh_get_redirect(t_token **tokens)
{
	t_token	*tmp;
	t_ast	*result;

	if (!tokens || !*tokens)
		return (NULL);
	tmp = *tokens;
	if (tmp->type == HEREDOC)
	{
		result = msh_get_heredoc_word(tokens);
		if (!result)
			return (NULL);
		if (*tokens && (*tokens)->type >= INDIRECT
			&& (*tokens)->type <= HEREDOC)
			result->right = msh_get_redirect(tokens);
		return (result);
	}
	result = process_redirection_tokens(tokens, tmp);
	if (result)
		return (result);
	result = msh_get_cmd(&tmp);
	if (result)
		return (result);
	return (NULL);
}
