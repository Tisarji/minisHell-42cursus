/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jikarunw <jikarunw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 22:07:28 by jikarunw          #+#    #+#             */
/*   Updated: 2024/07/24 15:31:49 by jikarunw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "./minishell.h"

# define EXIT_PROGRAM "exit"

typedef struct s_token
{
	char	*input_cmd;
	char	**cmd;
	int		code_out;
}	t_token;

#endif
