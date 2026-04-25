/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils_01.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jikarunw <jikarunw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   */
/*   Created: 2024/12/06 12:03:55 by jikarunw          #+#    #+#             */
/*   Updated: 2025/03/08 01:47:15 by jikarunw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_special_variable_value(t_msh *shell, char **str)
{
	char	*expanded_value;

	if (**str == '?')
	{
		expanded_value = ft_itoa(shell->code);
		(*str)++;
	}
	else if (**str == '$')
	{
		expanded_value = ft_itoa(getpid());
		(*str)++;
	}
	else
		expanded_value = NULL;
	return (expanded_value);
}

char	*get_normal_variable_value(t_msh *shell, char **str)
{
	char	*var_name;
	char	*expanded_value;

	var_name = ft_strdup_while_string(*str, LETTERS_DIGITS);
	if (!var_name || ft_strlen(var_name) == 0)
	{
		free(var_name);
		return (ft_strdup("$"));
	}
	expanded_value = get_env_value(shell, var_name);
	(*str) += ft_strlen(var_name);
	free(var_name);
	return (expanded_value);
}

char	*handle_single_quotes(t_msh *shell, char **str, int expand_vars)
{
	char	*segment;
	char	*temp;
	char	*end;

	(*str)++;
	end = ft_strchr(*str, '\'');
	if (!end)
		end = *str + ft_strlen(*str);
	segment = ft_substr(*str, 0, end - *str);
	if (expand_vars == 2)
	{
		temp = segment;
		segment = expand_string(shell, segment, 1);
		free(temp);
	}
	if (*end == '\'')
		*str = end + 1;
	else
		*str = end;
	return (segment);
}

char	*handle_double_quotes(t_msh *shell, char **str)
{
	char	*segment;
	char	*expanded;
	char	*end;

	(*str)++;
	end = *str;
	while (*end && *end != '"')
		end++;
	segment = ft_substr(*str, 0, end - *str);
	expanded = expand_string(shell, segment, 2);
	free(segment);
	if (*end == '"')
		*str = end + 1;
	else
		*str = end;
	return (expanded);
}

char	*handle_plain_text(char **str)
{
	int		len;
	char	*segment;

	len = 0;
	while ((*str)[len] && (*str)[len] != '$' && (*str)[len] != '\'' \
		&& (*str)[len] != '"')
		len++;
	segment = ft_substr(*str, 0, len);
	*str += len;
	return (segment);
}
