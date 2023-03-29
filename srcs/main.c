#include "minishell.h"

int	main(int ac, char **av, char **envp)
{
	char			*str;
	t_envp			tenvp;
//	t_parsed_data	*parsed_data;
	t_env_node  	*head;

	// ft_basic_signal();

	head = init_node(envp);
	if (ac != 1)
		return (0);
	if (!ft_strcmp(av[0], "minishell"))
		return (0);
	envp_init(&tenvp, envp);
	//signal_setting();
	while (1)
	{
		str = readline("minishell$ ");
		if (!str)
		{
			printf("exit\n");
			break ;
		}
//		parsed_data = parsing(str);
		// print_queue(parsed_data);
		if (ft_strcmp(str, "echo $?") == 0)
		{
			printf("%d\n", tenvp.exit_status);
			tenvp.exit_status = 0;
		}
		else if (ft_strchr(str, '|'))
			pipex(str, &tenvp);
		else if (ft_strncmp(str, "echo ", 5) == 0)
			ft_echo(ft_split(str, ' '));// TODO 토큰을 사용하도록 변경해야 함
		else if (ft_strcmp(str, "pwd") == 0) // TODO 토큰을 사용하도록 변경해야 함
			ft_pwd();
		else if (ft_strncmp(str, "exit ", 5) == 0 || ft_strcmp(str, "exit") == 0)
			ft_exit(ft_split(str, ' '));// TODO 토큰을 사용하도록 변경해야 함
		else if (ft_strcmp(str, "env") == 0)
			ft_env(head);
		else if (ft_strncmp(str, "cd ", 3) == 0 || ft_strcmp(str, "cd") == 0)
			ft_cd(ft_split(str, ' '), &tenvp);
		else if (ft_strncmp(str, "export ", 7) == 0 || ft_strcmp(str, "export") == 0)
			ft_export(head, ft_split(str, ' '));
		else if (ft_strncmp(str, "unset ", 6) == 0 || ft_strcmp(str, "unset") == 0)
			ft_unset(head, ft_split(str, ' ')[1]);
		else
		{
			char **tmp2;
			tmp2 = convert_array(head);
			tenvp.envp = tmp2;
			run_cmd(str, &tenvp);
		}
		if (str[0] != '\0')
			add_history(str);
		free(str);
	}
	return (0);
}
