#include "minishell.h"

int	print_error_for_invalid_name(char *cmd)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
	return (0);
}

char	**ft_simple_split(char *tmp, char *equal_address)
{
	char	**str;
	int		i;
	int		j;

	i = 0;
	str = malloc(sizeof(char *) * 3);
	str[0] = malloc(sizeof(char) * (equal_address - tmp + 1));
	str[1] = malloc(sizeof(char) * (ft_strlen(tmp) - (equal_address - tmp) + 1));
	str[2] = NULL;
	while (tmp + i < equal_address)
	{
		str[0][i] = tmp[i];
		++i;
	}
	str[0][i] = '\0';
	if (str[0][i - 1] == '+')
		str[0][i - 1] = '\0';
	j = 0;
	++i;
	while (*(tmp + i) != '\0')
	{
		str[1][j] = tmp[i];
		++i;
		++j;
	}
	str[1][j] = '\0';
	return (str);
}

int	arr_len(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		i++;
	return (i);
}

void	print_export(t_env_node *head)
{
	char	**arr;
	int 	i;

	i = 0;
	arr = convert_array_for_export(head);
	quick_sort(arr, 0, arr_len(arr) - 1);
	while (arr[i])
	{
		printf("declare -x %s\n", arr[i]);
		++i;
	}
	i = 0;
	while (arr[i] != NULL)
	{
		free(arr[i]);
		++i;
	}
	free(arr);
}

t_env_node	*get_old_key_address(t_env_node *head, char *key)
{
	head = head->next;
	while (head != NULL)
	{
		if (ft_strcmp(head->key, key) == 0)
			return (head);
		head = head->next;
	}
	return (NULL);
}
