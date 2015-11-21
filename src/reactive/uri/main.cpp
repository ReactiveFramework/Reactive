/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gwells <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/11 14:09:44 by gwells            #+#    #+#             */
/*   Updated: 2015/11/11 14:29:51 by gwells           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "query2.hpp"
#include <stdlib.h>
#include "libft.h"

typedef void (*ptrfunc)(void *, size_t);
void    dellist(void *, size_t);

typedef struct  s_content
{
    char        *path;
    char        *value;
}               t_content;

int main(int argc, char **argv)
{
    if (argc != 2)
        ft_quit("Enter one argument Pls", 2, EXIT_FAILURE);

    query test;
    t_list  *lst;

    ptrfunc delfunc;

    delfunc = dellist;

    lst = test.parse(argv[1]);
    ft_lstdel(&lst, delfunc);
    return (0);
}

void    dellist(void* content, size_t size)
{
    t_content *tmp;

    tmp = (t_content*)content;
    free(tmp->path);
    free(tmp->value);
    free(content);
}
