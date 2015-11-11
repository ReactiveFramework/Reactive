/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   query2.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gwells <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/11 12:41:45 by gwells            #+#    #+#             */
/*   Updated: 2015/11/11 15:39:51 by gwells           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

class query
{
    public:
        /**
         * Parse the content an put it in linked list
         */

        query() {}
        t_list *parse(const char *query);

};
