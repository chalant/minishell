#include "minishell.h"

int	print_grammar(t_ms_grammar *grammar)
{
	int			i;
	int			j;
	t_ms_rule	*rule;

	i = 0;
	while (i < grammar->length)
	{
		rule = grammar->rules[i];
		printf("%s ->", rule->name);
		j = -1;
		while (++j < rule->length)
			printf(" %s", rule->symbols[j]->name);
		printf("\n");
		i++;
	}
	return (1);
}

int	print_earley(t_earley_set **sets, t_ms_grammar *grammar, int size)
{
	int				i;
	int				j;
	int				k;
	t_earley_item	*item;
	t_earley_set	*set;

	i = -1;
	while (++i < size)
	{
		j = -1;
		set = sets[i];
		printf("=================(%d)==================\n\n", i);
		while (++j < set->items->size)
		{
			item = (t_earley_item *)set->items->contents + j;
			printf("%s -> ", grammar->rules[item->rule]->name);
			k = -1;
			while (++k < grammar->rules[item->rule]->length)
			{
				if (k == item->next)
					printf("• ");
				if (k != grammar->rules[item->rule]->length - 1)
					printf("%s ", grammar->rules[item->rule]->symbols[k]->name);
			}
			printf("(%d)\n", item->start);
		}
		printf("\n");
	}
	return (0);
}

int	print_dearley(t_darray *sets, t_ms_grammar *grammar, int size)
{
	int				i;
	int				j;
	int				k;
	t_earley_item	*item;
	t_earley_set	*set;

	i = -1;
	while (++i < size)
	{
		j = -1;
		set = ft_darray_get(sets, i);
		printf("=================(%d)==================\n\n", i);
		while (++j < set->items->size)
		{
			item = (t_earley_item *)set->items->contents + j;
			printf("%s -> ", grammar->rules[item->rule]->name);
			k = -1;
			while (++k < grammar->rules[item->rule]->length)
			{
				if (k == item->next)
					printf("• ");
				if (k != grammar->rules[item->rule]->length - 1)
					printf("%s ", grammar->rules[item->rule]->symbols[k]->name);
			}
			printf("(%d)\n", item->start);
		}
		printf("\n");
	}
	return (0);
}

int	reverse_earley(t_darray *sets, t_ms_grammar *grammar)
{
	int				i;
	int				j;
	int				k;
	t_earley_item	*item;
	t_earley_set	*set;
	t_earley_set	**reversed;

	i = -1;
	reversed = malloc(sizeof(t_earley_set *) * (sets->size));
	ft_bzero(reversed, sets->size);
	while (++i < sets->size)
	{
		reversed[i] = malloc(sizeof(t_earley_set));
		reversed[i]->items = malloc(sizeof(t_darray));
		ft_darray_init(reversed[i]->items, sizeof(t_earley_item), sets->size);
	}
    item = malloc(sizeof(t_earley_item));
	i = -1;
	while (++i < sets->size)
	{
		j = -1;
        set = ft_darray_get(sets, i);
		while (++j < set->items->size)
		{
            ft_memcpy(item, ft_darray_get(set->items, j), sizeof(t_earley_item));
			if (item->completed)
			{
				k = item->start;
				item->start = i;
				ft_darray_append(reversed[k]->items, item);
			}
		}
		reversed[i]->size = j;
	}
	print_earley(reversed, grammar, sets->size);
	return (0);
}

void print_parse_tree(t_parse_tree *node, int depth)
{
	int	i;
	t_parse_tree *child;

	if (!node)
		return ;
	if (!node->rule_name)
		return ;
	i = -1;
	for (int i = 0; i < depth; i++)
		printf("   |");
	printf("%s\n", node->rule_name);
	if (node->terminal)
		return ;
	if (!node->children)
		return ;
	while (++i < node->children->size)
	{
		child = (t_parse_tree *)node->children->contents + i;
		print_parse_tree(child, depth + 1);
	}
}

void	print_commands(t_command *command, int depth)
{
	int	i;

	if (!command)
		return ;
	if (!command->command_name)
		return ;
	for (int i = 0; i < depth; i++)
		printf("   |");
	printf("%s ", command->command_name);
	i = -1;
	if (command->arguments)
	{
		while (++i < command->arguments->size)
			printf("%s ", ((t_token *)ft_darray_get(command->arguments, i))->string);
	}
	i = -1;
	if (command->redirections)
	{
		while (++i < command->redirections->size)
			printf("> %s ", ((t_redirection *)ft_darray_get(command->redirections, i))->file_path);
	}
	printf("input %d output %d\n", command->input, command->output);
	printf("\n");
	if (command->command_flags & MS_OPERAND)
		return ;
	print_commands(command->left, depth + 1);
	print_commands(command->right, depth + 1);
}
