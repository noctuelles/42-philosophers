/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/23 19:45:02 by plouvel           #+#    #+#             */
/*   Updated: 2022/04/23 19:54:25 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>

time_t	diff_mlsec(struct timeval t1, struct timeval t2)
{
	time_t	diff_in_mlsec;

	diff_in_mlsec = (t2.tv_sec - t1.tv_sec) * 1000;
	diff_in_mlsec += (t2.tv_usec - t1.tv_usec) * 0.001;
	return (diff_in_mlsec);
}
