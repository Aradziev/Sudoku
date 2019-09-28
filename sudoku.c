/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sudoku.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleradzi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/26 12:59:51 by aleradzi          #+#    #+#             */
/*   Updated: 2019/09/28 16:05:16 by aleradzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void	print(char **array)
/* this function prints the array with lines dividing the different boxes */
{
	int y;
	int x;
	int hori;

	hori = -1;
	x = -1;
	y = -1;
	while (++y < 9)
	{
		if (y == 3 || y == 6)
		{
			while (++hori < 13)
				write(1, "- ", 2);
			write(1, "\n", 1);
		}
		while (++x < 9)
		{
			write(1, &array[y][x], 1);
			write(1, " ", 1);
			if (x == 2 || x == 5)
				write(1, " |  ", 4);
		}
		write(1, "\n", 1);
		x = -1;
		hori = -1;
	}
	write(1, "\n", 1);
}

int		checkBox(char **array, int x, int y, int n)
/* checkBox checks if number 'n' is anywhere in the current box or not */
{
	int		xnew;
	int		ynew;
	int		xcounter;
	int		ycounter;

	n += '0';
	xnew = (x / 3) * 3;
	ynew = (y / 3) * 3;
	xcounter = xnew + 3;
	ycounter = ynew + 3;

	while (ynew < ycounter)
	{
		while (xnew < xcounter)
		{
			if (array[ynew][xnew] == n)
				return (0);
			xnew++;
		}
		xnew = xcounter - 3;
		ynew++;
	}
	return (1);
}

int		isntDot(char **array, int x, int y)
/* isntDot returns a 1 for true if the current position is not a dot and a 0 for false if it is */
{
	if (array[y][x] != '.')
		return (1);
	return (0);
}


int		checkXY(char **array, int x, int y, int n)
/* checkXY checks if number 'n' is anywhere on the x or y axis */
{
	int		xnew;
	int		ynew;

	n += '0';
	xnew = 0;
	ynew = 0;
	while (xnew < 9)
	{
		if (array[y][xnew] == n)
			return (0);
		xnew++;
	}
	while (ynew < 9)
	{
		if (array[ynew][x] == n)
			return (0);
		ynew++;
	}
	return (1);
}

int		recursive(char **array, int x, int y)
/* this is the function that uses recursion making this whole thing work */
{
	int		n;

	n = 1;
	if (x > 8)
	{
		x = 0;
		y += 1;
	}
	if (y > 8)
		return (1);
	if (isntDot(array, x, y))
		return (recursive(array, x + 1, y));
	while (n <= 9)
	{
		if (checkXY(array, x, y, n) && checkBox(array, x, y, n))
		{
			array[y][x] = n + '0';
//			print(array);
//			printf("\n");
			if (recursive(array, x + 1, y))
				return (1);
			array[y][x] = '.';
		}
		n++;
	}
	return (0);	
}

char	**xferArray(char *buff)
/* xferArray turns the test file from zzzz\nzzzz\n... to an array with x and y coordinates */
{
	char	**array;
	int		y;
	int		x;

	array = malloc(sizeof(char **) * 9);
	y = -1;
	while (++y < 9)
	{
		x = 0;
		array[y] = malloc(sizeof(char *) * 10);
		while (*buff != '\n')
		{
			if (*buff != ' ')
			{
				array[y][x] = *buff;
				x++;
			}
			buff++;
		}
		buff++;
		array[y][9] = '\0';
	}
	return (array);
}

char	**readfile(int fd)
/* readfile */
{
	char buffer[163];

	read(fd, buffer, 162);
	buffer[162] = '\0';
	return (xferArray(buffer));
}

int		main(int ac, char **av)
{
	int		fd;
	char	**array;

	 if (ac == 2)
	 {
		fd = open(av[1], O_RDONLY);
		array = readfile(fd);
		printf("\n PUZZLE: \n\n");
		print(array);
		recursive(array, 0, 0);
		printf("\n RESULT: \n\n");
		print(array);
	 }
	return (0);
}
