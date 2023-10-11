/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 09:31:06 by ychalant          #+#    #+#             */
/*   Updated: 2023/04/13 09:31:06 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static inline t_list	*clear_lst(t_list **lst, void (*del)(void *),
		void *content)
{
	ft_lstclear(lst, del);
	del(content);
	return (NULL);
}

static inline t_list	*clear_content(void (*del)(void *), void *content)
{
	del(content);
	return (NULL);
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*current;
	t_list	*first;
	void	*content;

	if (!lst || !f || !del)
		return (NULL);
	content = f(lst->content);
	first = ft_lstnew(content);
	if (!first)
		return (clear_content(del, content));
	current = first;
	while (lst->next)
	{
		lst = lst->next;
		content = f(lst->content);
		current->next = ft_lstnew(content);
		if (!current->next)
			return (clear_lst(&first, del, content));
		current = current->next;
	}
	return (first);
}
