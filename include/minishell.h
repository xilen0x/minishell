/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocuni-p <jocuni-p@student.42barcelona.com +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 16:27:34 by jocuni-p          #+#    #+#             */
/*   Updated: 2024/06/10 11:25:40 by jocuni-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# ifndef READLINE_LIBRARY
#  define READLINE_LIBRARY
# endif

/*--------Libraries---------*/
# include "libft.h"
# include <readline.h>
# include <history.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/wait.h>
# include <limits.h>
# include <errno.h>
# include <sys/stat.h>
# include <stdbool.h>

/*-----------------Defines-------------*/
# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1

# define TRUE 1
# define FALSE 0

# define RED "\033[0;31m"
# define GREEN "\033[1;32m"
# define YELLOW "\033[0;33m"
# define BLUE "\033[1;34m"
# define END "\033[0m"
# define CYAN "\033[1;36m"
# define BLACK "\033[40m"
# define WHITE "\033[1;37m"
# define CLEAN_SCREEN "\033[2J"

/*--------------Signals-------------------*/
# define CTRL_C SIGINT
# define CTRL_D SIGQUIT
# define CTRL_SLASH SIGQUIT
# define CTRL_Z SIGTSTP
// Modes signals
# define PARENT 0
# define CHILD 1

/*--------------------Error messages-------------------*/
# define PRINT_SYNTAX_ERR_1 "syntax error near unexpected token `|'\n"
# define PRINT_SYNTAX_ERR_2 "syntax error near unexpected token `newline'\n"
# define PRINT_SYNTAX_ERR_3 "syntax error\n"

/*------------------Quote status--------*/
# define CLOSED 0
# define OPEN 1

/*-----------global variable------------*/
extern int	g_get_signal;

/*--------------------------- Pipe ---------------------------*/
# define READ 0
# define WRITE 1

//Environment list struct
typedef struct s_env
{
	char			*key;
	char			*val;
	char			**tokens;
	char			**tokens2;
	struct s_env	*next;
}					t_env;

typedef enum e_type
{
	NULL_TYPE = 0,//para saber que se ha inicializado ya 
	WORD,
	PIPE,
	SMALLER,
	D_SMALLER,
	GREATER,
	D_GREATER
}	t_type;

/*------contiene la str y el tipo de cada token-----*/
typedef struct s_tok
{
	char			*str;//contendrá string solo si es WORD, sino será NULLL.
	t_type			type;
	struct s_tok	*next;
}					t_tok;

typedef enum e_redir_type
{
	NULL_REDIR = 0,
	REDIR_INPUT = 1,//<
	HEREDOC_INPUT = 2,//<<
	REDIR_OUTPUT = 3,//>
	REDIR_OUTPUT_APPEND = 4,//>> 
}	t_redir_type;

/*----contiene los datos de cada redirección------*/
typedef struct s_redir
{
	char			*fname;
	t_redir_type	redir_type;
	struct s_redir	*next;
}					t_redir;

/*------contiene los datos de cada pipe de la linea de comando-----*/
typedef struct s_cmd
{
	char			**commands;
	t_redir			*redir;
	struct s_cmd	*next;
}					t_cmd;

/*---variables para expander---*/
typedef struct s_xpdr
{
	size_t	i;//iteradores
	size_t	j;
	size_t	k;
	size_t	len;
	bool	s_quote;//estado de las comillas
	bool	d_quote;
	char	*key;
	char	*val;
	char	*result;//token final expandido
}			t_xpdr;

/*------variables para executor-------*/
typedef struct s_exe
{
	char			**paths;
	char			*cmd_fullpath;
	char			**new_array;
	pid_t			*pid;
	int				num_cmds;
	int				fd_input;
	int				fd_output;
	int				fd[2];
	int				dup_stdin;
	int				dup_stdout;
}					t_exe;

/*--------------------------- minishell.c -------------------------*/
int		set_signals(int mode);
void	minishell(t_env	*envlist);
void	tokenizer(t_tok **tok, char *line);
int		parser(t_cmd **cmd, t_tok *tok);
void	cleaner_envlist(t_env **lst);
void	control_and_d(char *line);

/*--------------------------- init_exe.c -------------------------*/
void	init_exe(t_exe *exe, t_cmd *cmd);	
void	exe_free(t_exe *exe);

/*--------------------------- wellcome_msg.c -------------------------*/
int		bg_color(void);
void	init_msg(void);
int		help_mini(void);

/*---------------------------array 2d -------------------------*/
size_t	size_arr2d(char **arr2d);
char	**dup_arr2d(char **arr2d);
char	**add_one_arr2d(char **arr2d, char *new);
char	**rm_one_arr2d(char **arr2d, int index);
void	free_arr2d(char **arr2d);
void	print_arr2d(char **arr2d);//ELIMINAR ANTES DE ENTREGA

/*-------------------t_env-------------------*/
t_env	*lstlast(t_env *lst);
void	lstadd_back(t_env **lst, t_env *new);
t_env	*lstnew(char *key, char *value);
void	env_init_list(char **envp, t_env **envlist);
void	env_delone(t_env **env, char **node_to_del, void (*del)(void*));
void	cleaner_envlist(t_env **lst);

/*--------------------t_tok ------------------*/
t_tok	*tok_new_node(char *str, int type);
t_tok	*tok_last(t_tok *lst);
void	tok_add_back(t_tok **lst, t_tok *new);
void	tok_free(t_tok **lst);
int		tok_size(t_tok *lst);//ELIMINAR ANTES DE ENTREGA

/*--------------------t_cmd--------------------*/
t_cmd	*cmd_new_node(void);
t_cmd	*cmd_last(t_cmd *lst);
void	cmd_add_back(t_cmd **lst, t_cmd *new);
void	cmd_free(t_cmd **lst);
int		cmd_size(t_cmd *lst);//ELIMINAR ANTES DE ENTREGA

/*--------------------t_redir------------------*/
t_redir	*redir_new_node(char *str, int redir_type);
t_redir	*redir_last(t_redir *lst);
void	redir_add_back(t_redir **lst, t_redir *new);
void	redir_free(t_redir **lst);
int		redir_size(t_redir *lst);//ELIMINAR ANTES DE ENTREGA

/*-------------------tokenizer------------------*/
int		init_operator_type(char *line, t_tok **new_tok);
int		tok_len(char *line, t_tok **new_tok);
void	init_word_str(size_t len, t_tok *new_tok, char *line, size_t i);

/*---------------------parser-------------------*/
int		is_operator(t_tok *node);
int		is_redirection(t_tok *node);
void	handle_error(char *str, t_tok **tok);
size_t	commands_counter(t_tok *tok);
int		syntax_check_1(t_tok *tok);
int		syntax_check_2(t_tok *tok);
void	commands_creator(t_tok *tok, t_cmd *node);
void	commands_filler(t_tok **tok, t_cmd *node);

/*------------expander & quote removal----------*/
void	should_expand(t_cmd *cmd, t_env *envlist);
char	*expander(char *str, t_env *envlist);
void	init_xpdr(t_xpdr *xpdr);
size_t	new_tok_len(char *str, t_xpdr *xpdr, t_env *envlist);
void	init_xpdr_except_result(t_xpdr *xpdr);
void	get_dollar_len(char *str, t_xpdr *xpdr, t_env *envlist);
char	*get_env_key(char *str);
char	*get_env_val(char *env_key, t_env *envlist);
void	handle_quote_len(char c, t_xpdr *xpdr);
void	handle_quote_builder(char c, t_xpdr *xpdr);
void	handle_quote_after_dollar(char c, t_xpdr *xpdr);
size_t	get_len_and_free(char *str);
void	*new_tok_builder(char *str, t_xpdr *xpdr, t_env *envlist);
void	get_dollar_builder(char *str, t_xpdr *xpdr, t_env *envlist);
void	handle_dollar_question(t_xpdr *xpdr);
void	handle_dollar_invalid_syntax(char *str, t_xpdr *xpdr);

/*--------------------------utils t_env-------------------*/
t_env	*lstlast(t_env *lst);
void	lstadd_back(t_env **lst, t_env *new);
t_env	*lstnew(char *key, char *value);
void	env_init_list(char **envp, t_env **envlist);
void	env_delone(t_env **env, char **node_to_del, void (*del)(void*));
void	cleaner_envlist(t_env **lst);
int		no_path_env(t_cmd *cmd, t_exe exe, t_env *env);

/*---------------------------executor.c -------------------------*/
char	**get_paths(t_env *env);
int		pre_executor(t_env **env, t_cmd *cmd, t_exe *exe, int size_pipe);
int		search_command_path(t_cmd *cmd, t_exe *exe);
void	error_exe(int num);
int		list_to_array(t_env *env, t_exe *exe);
int		close_fd(t_exe	*exe);
int		executor(t_cmd *cmd, t_exe	*exe, t_env **env);

/*---------------------------redirections.c -------------------------*/
int		pre_redirections(t_cmd *cmd, t_exe *exe);

/*---------------------------utils0.c -------------------------*/
int		ft_msgs(int n, t_cmd *cmd);
void	set_exit_status(int n);

/*---------------------utils1.c-------------------*/
int		ca_strchr(const char *s, int c);
char	*ft_strncpy(char *dest, char *src, unsigned int n);
void	*p_malloc(size_t size);
void	str_free_and_null(char **str);

void	signal_child(int sig);
void	signal_parent(int sig);

/*-------------------exit_status------------------*/
int		get_exit_status_len(void);
char	*get_exit_status_val(void);
void	command_not_found(t_cmd *cmd, const char *prefix, size_t prefix_len);
void	no_file_or_dir(t_cmd *cmd, const char *prefix, size_t prefix_len);

/*--------------------------- builtins -------------------------*/
int		builtins(t_cmd *cmd, t_exe exe, t_env **env);
int		builtin_exit(t_cmd *cmd);
int		builtin_pwd(t_env *env);
int		builtin_cd(t_cmd	*cmd, t_env **env);
int		builtin_env(t_cmd *cmd, t_exe exe, t_env *env);
int		builtin_echo(t_cmd *cmd);
int		builtin_export(t_cmd *cmd, t_env **env);
int		builtin_unset(t_cmd *cmd, t_env **env);
int		is_builtins(t_cmd *cmd);
int		exist_cwd(void);

/*--------------------------- utils_cd utils_cd_2 ---------------*/
int		handle_no_argument(t_cmd *cmd);
int		handle_tilde(t_cmd *cmd);
int		handle_dash(t_cmd *cmd);
int		handle_dot(t_cmd *cmd);
int		handle_invalid_path(t_cmd *cmd);
void	update_environment(t_env *env, char *current_wd);
int		free_current_wd(char *current_wd);
int		go_home(void);
/*--------------------------- builtin export -------------------------*/
int		check_export(char *arg);
int		variable_exists(t_env **env, char **variable);
int		variable_exists_op2(t_env *env, char *variable);
int		variable_exists_op3(t_env *env, char *variable);
t_env	*variable_exists_op4(t_env *env, char *key);
t_env	*update_env(t_env *env, char *key, char *val);

/*--------------------builtin unset-----------------*/
int		check_syntax(char *cmd);

/*-----------------------oldpwd---------------------*/
int		var_exists_oldpwd(t_env *env, char *variable);
void	update_pwd(t_env *env);
int		set_old_pwd(void);
int		get_old_pwd(char *current_wd, t_env *env);
int		go_path(t_cmd *cmd);
int		update_oldpwd(t_env *env, char *current_wd);

/*----------------------prints--------------------*/
void	print_arr2d(char **arr2d);//ELIMINAR ANTES DE ENTREGA
void	print_tok(t_tok *lst);//ELIMINAR ANTES DE ENTREGA
void	print_cmd(t_cmd *list);//ELIMINAR ANTES DE ENTREGA
void	print_redir(t_redir *lst);//ELIMINAR ANTES DE ENTREGA
void	ft_print_keys(t_env *env_struct);//ELIMINAR ANTES DE ENTREGA
void	ft_print_values(t_env *env_struct);//ELIMINAR ANTES DE ENTREGA
void	ft_printstack(t_env *env_struct);//ELIMINAR ANTES DE ENTREGA
void	print_cmd_para_executor(t_cmd *lst);//ELIMINAR ANTES DE ENTREGA

/*------------------redirections---------------*/
int		exist_redirections(t_cmd *cmd);

/*-------------------heredoc-------------------*/
int		heredoc_create(t_redir *redir, int hd_nbr);
int		heredoc(t_cmd *cmd);

#endif