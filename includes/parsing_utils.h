/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scambier <scambier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 19:37:50 by scambier          #+#    #+#             */
/*   Updated: 2024/06/29 19:41:13 by scambier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_UTILS
# define PARSING_UTILS

int	space_predicat(int c);
int	map_predicat(int c);
int	read_color(char *in);
int	get_longest(char **lines);

#endif