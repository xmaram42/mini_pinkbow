/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maram <maram@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:29:10 by ashaheen          #+#    #+#             */
/*   Updated: 2025/08/31 11:45:13 by maram            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


// char	**dup_envp(char **src)
// {
// 	int		n;
// 	int		i;
// 	char	**dst;

// 	n = 0;
// 	while (src[n])
// 		n++;
// 	dst = (char **)malloc(sizeof(char *) * (n + 1));
// 	if (!dst)
// 		return (NULL);
// 	i = 0;
// 	while (i < n)
// 	{
// 		dst[i] = ft_strdup(src[i]);
// 		if (!dst[i])
// 			return (NULL); // simple version; we can add cleanup later
// 		i++;
// 	}
// 	dst[n] = NULL;
// 	return (dst);
// }


void	update_shell_level(t_shell *shell)
{
	char	*new_shlvl;

	// Always start from 1 for minishell, regardless of inherited value
	// This ensures minishell starts fresh every time
	new_shlvl = ft_itoa(1);
	if (new_shlvl)
	{
		update_env_var("SHLVL", new_shlvl, shell);
		free(new_shlvl);
	}
}

char	**dup_envp(char **src)
{
	char	**dst;
	int		n;
	int		i;

	n = env_count(src);
	dst = (char **)malloc(sizeof(char *) * (n + 1));
	if (!dst)
		return (NULL);
	i = 0;
	while (i < n)
	{
		dst[i] = ft_strdup(src[i]);
		if (!dst[i])
		{
			// roll back on failure
			while (i > 0)
			{
				i--;
				free(dst[i]);
			}
			free(dst);
			return (NULL);
		}
		i++;
	}
	dst[n] = NULL;
	return (dst);
}

int main(int ac, char **av, char **envp)
{
    char    *line;
    t_shell shell;
    t_token *token_list;
    t_cmd   *cmd_list;

    (void)av;
    if (ac > 1)
    {
        printf("You Should Write ./minishell ");
        return (0);
    }

    shell.envp = dup_envp(envp);                    // make a private copy
    if (!shell.envp)
    {
        ft_putstr_fd("minishell: failed to duplicate environment\n", 2);
        return (1);
    }
    shell.exit_code = 0;
    shell.exp = NULL;                               // your expander state (if any)
    
	// Update SHLVL (shell level)
	update_shell_level(&shell);

    while (1)
    {
        setup_signals();
        line = readline("pink_bow🎀$ ");
        if (!line)                                  // Ctrl-D / EOF
        {
            printf("exit\n");
            break;
        }
        if (*line)                                  // non-empty line
        {
            add_history(line);

            token_list = NULL;
            cmd_list = NULL;

            tokens(line, &token_list, &shell);              // LEXER
            if (token_list)
            {
                if (!validate_syntax(token_list))   // SYNTAX VALIDATION (before type classification)
                {
                    shell.exit_code = 2;
                    free_tokens(token_list);
                    continue;
                }
                set_token_types(token_list);            // classify tokens
                expand_token_list(token_list, &shell);  // expand $VAR, ~, etc.
                remove_empty_tokens(&token_list);
                if (token_list)
                {
                    set_token_types(token_list);
                    cmd_list = parse_pipeline(token_list);  // PARSER
                    execute_pipeline(cmd_list, &shell);     // EXECUTOR
                    free_cmd_list(cmd_list);
                }
                free_tokens(token_list);
            }
        }
        free(line);
    }

    free_envp(shell.envp);                          // free your private env
    return (0);
}
