/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocuni-p <jocuni-p@student.42barcelona.com +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 17:00:08 by jocuni-p          #+#    #+#             */
/*   Updated: 2024/06/10 10:51:57 by jocuni-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	control_and_d(char *line)
{
	if (!line)
	{
		if (!isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "exit\n", 5);
		write(STDOUT_FILENO, "exit\n", 5);
		exit(EXIT_SUCCESS);
	}
}

char	*generate_line(void)
{
	char	*line;

	line = readline("minishell$ ");
	if (line && *line)
		add_history(line);
	return (line);
}

static void	main_process(t_env *envlist)
{
	t_tok			*tok;
	t_cmd			*cmd;
	t_exe			exe;
	int				size_pipe;
	char			*line;

	tok = NULL;
	cmd = NULL;
	line = generate_line();
	control_and_d(line);
	if (!line || !*line)
		return (free(line));
	tokenizer(&tok, line);
	free(line);
	if (parser(&cmd, tok) == 1)
		return ;
	tok_free(&tok);
	should_expand(cmd, envlist);
	init_exe(&exe, cmd);
	heredoc(cmd);
	size_pipe = cmd_size(cmd);
	pre_executor(&envlist, cmd, &exe, size_pipe);
	cmd_free(&cmd);
}

/* Detecta si el programa se ejecuta desde una terminal interactiva o desde un pipe/script */ 
void	minishell(t_env *envlist)
{
	if (isatty(STDIN_FILENO))
		main_process(envlist);
	else
	{
		write(2, "run ./minishell (without arguments or operators)\n", 49);
		exit(STDERR_FILENO);
	}
}
