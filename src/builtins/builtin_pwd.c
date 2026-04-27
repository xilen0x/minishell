/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: castorga <castorga@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 17:04:39 by castorga          #+#    #+#             */
/*   Updated: 2024/05/27 17:04:41 by castorga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*Update env after moving to new location with cd*/
t_env	*update_env(t_env *env, char *key, char *val)
{
	t_env	*current;
	t_env	*new_env;

	current = env;
	while (current != NULL)
	{
		if (ft_strcmp(key, current->key) == 0)
		{
			free(current->val);
			current->val = ft_strdup(val);
			return (env);
		}
		current = current->next;
	}
	new_env = lstnew(key, val);
	lstadd_back(&env, new_env);
	return (env);
}

/*checks if the current working directory exists*/
int	exist_cwd(void)
{
	char	*curr_wd;

	curr_wd = getcwd(NULL, 0);
	if (curr_wd == NULL)
		return (0);
	free(curr_wd);
	return (1);
}

/*Function that returns the current path(pwd). 
Use the getcwd function to do this.*/
int	builtin_pwd(t_env *env)
{
	char	*current_wd;

	if (!exist_cwd())
	{
		current_wd = getenv("OLDPWD");
		printf("%s\n", current_wd);
		chdir(current_wd);
		return (1);
	}
	else
	{
		current_wd = getcwd(NULL, 0);
		*env = *update_env(env, "PWD", current_wd);
		printf("%s\n", current_wd);
		free(current_wd);
	}
	set_exit_status(0);
	return (0);
}

/*updates the PWD environment variable*/
void	update_pwd(t_env *env)
{
	char	*current_wd;

	current_wd = getcwd(NULL, 0);
	if (!current_wd)
	{
		perror("getcwd");
		return ;
	}
	(void)update_env(env, "PWD", current_wd);
	free(current_wd);
}
