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

int main(int argc, char **argv)
{
    if (argc != 2)
        ft_quit("Enter one argument Pls", 2, EXIT_FAILURE);

    query test;

    test.parse(argv[1]);

    return (0);
}
