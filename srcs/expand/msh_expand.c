/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jikarunw <jikarunw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 08:36:37 by jikarunw          #+#    #+#             */
/*   Updated: 2025/03/30 16:40:30 by jikarunw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_env_value(t_msh *shell, char *key)
{
	int		i;
	int		key_len;
	char	*env_value;

	if (!key || !shell->env)
		return (ft_strdup(""));
	if (ft_strcmp(key, "?") == 0)
		return (ft_itoa(shell->code));
	key_len = ft_strlen(key);
	i = 0;
	while (shell->env[i])
	{
		if (ft_strncmp(shell->env[i], key, key_len) == 0
			&& shell->env[i][key_len] == '=')
		{
			env_value = ft_strdup(shell->env[i] + key_len + 1);
			return (env_value);
		}
		i++;
	}
	return (ft_strdup(""));
}

char	*expand_variable_value(t_msh *shell, char **str)
{
	char	*expanded_value;
	char	*result;

	(*str)++;
	expanded_value = get_special_variable_value(shell, str);
	if (expanded_value)
	{
		result = ft_strdup(expanded_value);
		free(expanded_value);
		return (result);
	}
	return (get_normal_variable_value(shell, str));
}

char	*process_segment(t_msh *shell, char **str, int expand_vars)
{
	if (**str == '\'' && expand_vars == 2)
		return (handle_literal_char(str));
	else if (**str == '\'')
		return (handle_single_quotes(shell, str, expand_vars));
	else if (**str == '"')
		return (handle_double_quotes(shell, str));
	else if (**str == '$' && expand_vars)
		return (expand_variable_value(shell, str));
	else if (**str == '$' && !expand_vars)
	{
		(*str)++;
		return (ft_strdup("$"));
	}
	else
		return (handle_plain_text(str));
}

char	*expand_string(t_msh *shell, char *str, int expand_vars)
{
	char	*result;
	char	*temp;
	char	*segment;
	char	*joined;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (*str)
	{
		segment = process_segment(shell, &str, expand_vars);
		if (segment)
		{
			temp = result;
			joined = ft_strjoin_for_other(result, segment);
			if (!joined)
			{
				free(temp);
				free(segment);
				return (NULL);
			}
			result = joined;
			free(temp);
			free(segment);
		}
	}
	return (result);
}

int	is_removable_empty_token(t_token *token, char *expanded)
{
	if (expanded[0] != '\0')
		return (0);
	if (ft_strchr(token->cmd, '\'') || ft_strchr(token->cmd, '"'))
		return (0);
	if (token->prev && (token->prev->type == INDIRECT \
		|| token->prev->type == REDIRECT || token->prev->type == APPEND \
		|| token->prev->type == HEREDOC))
		return (0);
	return (1);
}

void	remove_expanded_token(t_msh *shell, t_token *token)
{
	if (token->prev)
		token->prev->next = token->next;
	else
		shell->token = token->next;
	if (token->next)
		token->next->prev = token->prev;
	free(token->cmd);
	free(token);
}

void	process_expansion(t_msh *shell)
{
	t_token	*current;
	t_token	*next;
	char	*expanded_cmd;
	int		expand_vars;

	current = shell->token;
	while (current)
	{
		next = current->next;
		expand_vars = 1;
		if (current->prev && current->prev->type == HEREDOC)
			expand_vars = 0;
		if (current->cmd && (ft_strchr(current->cmd, '$') || ft_strchr(current->cmd, '"') \
			|| ft_strchr(current->cmd, '\'')))
		{
			expanded_cmd = expand_string(shell, current->cmd, expand_vars);
			if (expanded_cmd && is_removable_empty_token(current, expanded_cmd))
			{
				free(expanded_cmd);
				remove_expanded_token(shell, current);
			}
			else if (expanded_cmd)
			{
				free(current->cmd);
				current->cmd = expanded_cmd;
			}
		}
		current = next;
	}
}
