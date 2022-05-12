/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/23 19:45:02 by plouvel           #+#    #+#             */
/*   Updated: 2022/05/12 16:12:45 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <unistd.h>

time_t	get_mlsec_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000 + tv.tv_usec / 1000));
}

void	precise_sleep(uint64_t ms)
{
	time_t	wake_up_time;

	wake_up_time = get_mlsec_time() + ms;
	while (get_mlsec_time() < wake_up_time)
		usleep(100);
}
