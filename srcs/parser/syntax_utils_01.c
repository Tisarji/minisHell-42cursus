/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils_01.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jikarunw <jikarunw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 14:55:35 by jikarunw          #+#    #+#             */
/*   Updated: 2025/03/14 12:31:17 by jikarunw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	update_quote_counts(char c, int *s_q_count, int *d_q_count)
{
	if (c == '\'' && !(*d_q_count % 2))
		*s_q_count = (*s_q_count + 1) % 2;
	else if (c == '\"' && !(*s_q_count % 2))
		*d_q_count = (*d_q_count + 1) % 2;
}

int	is_invalid_operator(const char **input)
{
	if (!input || !*input || !**input)
		return (0);
	if (**input == '>' && *(*input + 1) == '>')
		(*input)++;
	else if (**input == '<' && *(*input + 1) == '<')
		(*input)++;
	else if (**input == '>' || **input == '<')
	{
		(*input)++;
		if (**input == '\0' || ft_isspace(**input))
			return (1);
	}
	return (0);
}

int	check_redirection_end(const char *input)
{
	if (*(input + 1) == '\0')
		return (1);
	if (*(input + 1) == '|' || *(input + 1) == '&')
		return (1);
	return (0);
}

int	handle_special_redirection(const char *input)
{
	if ((*input == '>' && *(input + 1) == '>') || (*input == '<' && *(input
				+ 1) == '<'))
	{
		input++;
		if (check_redirection_end(input))
			return (1);
	}
	else if (*(input + 1) == '|' || *(input + 1) == '&')
	{
		return (1);
	}
	return (0);
}

int	has_valid_heredoc(const char *input)
{
	int	s_q_count;
	int	d_q_count;

	s_q_count = 0;
	d_q_count = 0;
	while (*input)
	{
		update_quote_counts(*input, &s_q_count, &d_q_count);
		if (!(s_q_count % 2) && !(d_q_count % 2))
		{
			if (*input == '<' && *(input + 1) == '<')
			{
				input += 2;
				while (*input == ' ' || *input == '\t')
					input++;
				if (*input == '\0' || *input == '|' || *input == '&'
					|| *input == '<' || *input == '>')
					return (1);
				while (*input && !ft_isspace(*input) && *input != '|'
					&& *input != '<' && *input != '>')
					input++;
				if (!*input)
					break ;
				continue ;
			}
		}
		input++;
	}
	return (0);
}
