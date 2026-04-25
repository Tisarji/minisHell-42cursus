/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_syntax.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krwongwa <krwongwa@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 18:21:47 by jikarunw          #+#    #+#             */
/*   Updated: 2025/03/13 19:17:54 by krwongwa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	has_invalid_redirections(const char *input)
{
	int	s_q_count;
	int	d_q_count;

	if (has_valid_heredoc(input))
		return (1);
	s_q_count = 0;
	d_q_count = 0;
	while (*input)
	{
		update_quote_counts(*input, &s_q_count, &d_q_count);
		if (!(s_q_count % 2) && !(d_q_count % 2))
		{
			if (*input == '>' || *input == '<')
			{
				if (check_redirection_end(input))
					return (1);
				if (handle_special_redirection(input))
					return (1);
			}
		}
		input++;
	}
	return (0);
}

int	has_unclosed_quotes(const char *input)
{
	char	quote_type;

	quote_type = 0;
	while (*input)
	{
		if (*input == '\'' || *input == '\"')
		{
			if (quote_type == *input)
				quote_type = 0;
			else if (!quote_type)
				quote_type = *input;
		}
		input++;
	}
	return (quote_type != 0);
}

int	has_misplaced_operators(const char *input)
{
	int	expect_command_next;
	int	s_q_count;
	int	d_q_count;

	expect_command_next = 1;
	s_q_count = 0;
	d_q_count = 0;
	while (*input)
	{
		update_quote_counts(*input, &s_q_count, &d_q_count);
		if (!(s_q_count % 2) && !(d_q_count % 2))
		{
			if (*input == '|' || *input == '&')
			{
				if (*input == '&')
					return (1);
				if (expect_command_next || *(input + 1) == '\0')
					return (1);
				expect_command_next = 1;
			}
			else if (*input != ' ' && *input != '\t')
				expect_command_next = 0;
		}
		input++;
	}
	return (expect_command_next);
}

int	has_logical_operators(const char *input)
{
	int	s_q_count;
	int	d_q_count;

	s_q_count = 0;
	d_q_count = 0;
	while (*input)
	{
		update_quote_counts(*input, &s_q_count, &d_q_count);
		if (!(d_q_count % 2) && !(s_q_count % 2) && ((*input == '&' && *(input \
				+ 1) == '&') || (*input == '|' && *(input + 1) == '|')))
			return (1);
		input++;
	}
	return (0);
}

int	syntax_error_checker(const char *input)
{
	if (has_unclosed_quotes(input))
		return (1);
	if (has_logical_operators(input))
		return (1);
	if (has_invalid_redirections(input))
		return (1);
	if (has_misplaced_operators(input))
		return (1);
	return (0);
}
