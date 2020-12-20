#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

typedef struct	s_zone
{
	int		w;
	int		h;
	char	b;
}				t_zone;

typedef struct s_shape
{
	char	type;
	float	x;
	float	y;
	float	w;
	float	h;
	char	c;
}				t_shape;

int		ft_putstr(char *s)
{
	while(*s)
	{
		write(1, s, 1);
		s++;
	}
	return (0);
}

int		put_error(char *s)
{
	ft_putstr(s);
	return (1);
}

int		set_zone(FILE *file, t_zone *zone)
{
	//read
	if (fscanf(file, "%d %d %c\n", &zone->w, &zone->h, &zone->b) != 3)
		return (1);
	//check
	if (zone->w < 1 || zone->w > 300 || zone->h < 1 || zone->h > 300)
		return (1);
	return (0);
}

int		draw_rec(int i, int j, t_shape *shape)
{
	//printf("draw_rec i [%d]\n", i);
	//printf("draw_rec j [%d]\n", j);
	//printf("draw_rec type [%c]\n", shape->type);
	//printf("draw_rec x    [%lf]\n", shape->x);
	//printf("draw_rec y    [%lf]\n", shape->y);
	//printf("draw_rec w    [%lf]\n", shape->w);
	//printf("draw_rec h    [%lf]\n", shape->h);
	//printf("draw_rec c    [%c]\n", shape->c);	
	if (i < shape->y || (shape->y + shape->h) < i || 
	 j < shape->x || (shape->x + shape->w) < j)
	{
		//printf("draw_rec: out\n");
		return (0);
	}
	if (i - shape->y < 1.0 || (shape->y + shape->h) - i < 1.0 ||
	 j - shape->x < 1.0 || (shape->x + shape->w) - j < 1.0)
	{
		if (shape->type == 'r' || shape->type == 'R')
		{
			//printf("draw_rec: on\n");
			return (1);
		}
	}
	if (shape->type == 'R')
	{
		//printf("draw_rec: in\n");
		return (1);
	}
	return (0);
}

int		draw_shape(t_zone *zone, char **img, t_shape *shape)
{
	int		i;
	int		j;

	//printf("draw_shape type [%c]\n", shape->type);
	//printf("draw_shape x    [%lf]\n", shape->x);
	//printf("draw_shape y    [%lf]\n", shape->y);
	//printf("draw_shape w    [%lf]\n", shape->w);
	//printf("draw_shape h    [%lf]\n", shape->h);
	//printf("draw_shape c    [%c]\n", shape->c);	
	i = 0;
	while (i < zone->h)
	{
		j = 0;
		while (j < zone->w)
		{
			if (draw_rec(i, j, shape))
				(*img)[i * zone->w + j] = shape->c;
			j++;
		}
		i++;
	}
	return (0);
}

int		set_shape(FILE *file, t_zone *zone, char **img)
{
	t_shape	shape;
	int		rv;

	while ((rv = fscanf(file, "%c %f %f %f %f %c\n", &shape.type, &shape.x, &shape.y, &shape.w, &shape.h, &shape.c)) == 6)
	{
		//printf("rv: %d", rv);
		if (shape.w < 0 || shape.h < 0)
			return (1);
		if (shape.type != 'r' && shape.type != 'R')
			return (1);		
		draw_shape(zone, img, &shape);
	}
	//printf("rv: %d", rv);
	if (rv != -1)
		return (1);
	return (0);
}

int		write_all(t_zone *zone, char **img)
{
	int		i;
	
	i = 0;
	while (i < zone->h)
	{
		write(1, (*img) + i * zone->w, zone->w);
		ft_putstr("\n");
		i++;
	}
	return (0);
}

int		read_to_draw(char **argv)
{
	FILE		*file;
	t_zone		zone;
	char		*img;
	int			i;

	if (!(file = fopen(argv[1], "r")))
		return (1);
	if (set_zone(file, &zone))
		return (1);
	//malloc
	if (!(img = (char *)malloc(sizeof(char) * (zone.w * zone.h))))
		return (1);
	//draw
	i = 0;
	while (i < zone.w * zone.h)
		img[i++] = zone.b;
	if (set_shape(file, &zone, &img))
	{
		free (img);
		return (1);
	}	
	write_all(&zone, &img);
	free(img);
	return (0);
}

int		main(int argc, char **argv)
{
	if (argc != 2)
		return (put_error("Error: argument\n"));
	if (read_to_draw(argv))
		return (put_error("Error: Operation file corrupted\n"));
	return (0);
}