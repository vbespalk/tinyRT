/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbespalk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 21:49:13 by vbespalk          #+#    #+#             */
/*   Updated: 2018/08/17 21:49:15 by vbespalk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void	configuration(void)
{
	write(1, "-------------------------------------\n", 38);
	write(1, "MOVE XYZ   : 8 2 4 6 + - (numeric)   \n", 38);
	write(1, "FOV/ZOOM   : * / (numeric)           \n", 38);
	write(1, "ROTATE     : (Arrows, PgUp, PgDn)    \n", 38);
	write(1, "RESET      : 0 (numeric)             \n", 38);
	write(1, "-------------------------------------\n", 38);
}

int		load_tex(t_sdl *sdl, SDL_Surface **tex, char *path)
{
	SDL_Surface *src;
	// SDL_Surface *surf;

	src = IMG_Load(path);
	if(!src) 
	{
   		printf("IMG_Load: %s\n", IMG_GetError());
   		return (-1);
   	}
   	*tex = SDL_ConvertSurface(src, sdl->format, 0);
   	// tex = SDL_CreateTextureFromSurface(sdl->renderer, surf);
   	// if (!tex) 
   	// {
    // 	fprintf(stderr, "CreateTextureFromSurface failed: %s\n", SDL_GetError());
    // 	return (-1);
    // }
    SDL_FreeSurface(src);
    // SDL_FreeSurface(surf);
    printf("TEXTURE loading SUCCESSFULLY\n");
    return (0);
}

int		init_env(t_env *env, t_scene *scene, t_obj **obj_pix, t_sdl *sdl)
{
	t_obj *tmp;

	env->scene = scene;
	env->light = scene->light;
	env->obj = scene->obj;
	tmp = env->obj;
	while (tmp)
	{
		if (tmp->type == 1 && !tmp->refract && !tmp->reflect) // 
			if (load_tex(sdl, &tmp->texture, SUN_PATH))
				return (-1);
		if (tmp->type == 2 && !tmp->refract && !tmp->reflect) // 
			if (load_tex(sdl, &tmp->texture, SUN_PATH))
				return (-1);
		tmp = tmp->next;
	}
	env->asp_rat = (double)SCR_WID / (double)SCR_HEI;
	env->pix_obj = obj_pix;
	env->sdl = sdl;
	configuration();
	return (0);
}

void	rt_loop(t_env *env, t_sdl *sdl)
{
	sdl->event_loop = 1;
	render(env, env->scene);
	SDL_UpdateTexture(sdl->screen, NULL, sdl->pixels, SCR_WID * sizeof(Uint32));
	SDL_RenderClear(sdl->renderer);
	SDL_RenderCopy(sdl->renderer, sdl->screen, NULL, NULL);
	SDL_RenderPresent(sdl->renderer);
	while (sdl->event_loop)
	{
		if (event_handler(env))
		{
			//Update the surface
			// if (SDL_UpdateWindowSurface(sdl->window))
			// 	sdl_error("Window could not be updated! ");
			render(env, env->scene);
			SDL_UpdateTexture(sdl->screen, NULL, sdl->pixels, SCR_WID * sizeof(Uint32));
			SDL_RenderClear(sdl->renderer);
			SDL_RenderCopy(sdl->renderer, sdl->screen, NULL, NULL);
			SDL_RenderPresent(sdl->renderer);
		}
	}
}

int		main(int argc, char **argv)
{
	int		fd;
	t_scene	scene;
	t_env	env;
	t_sdl	sdl;
	t_obj	*obj_pix[SCR_HEI * SCR_WID];


	if (argc != 2)
		ft_usage("RT scene\n");
	if ((fd = open(argv[1], O_RDONLY)) == -1)
		ft_error(NULL);
	if (!ft_strequ(ft_strchr(argv[1], '.'), ".rt"))
		ft_error("Invalid file\n");
	if (!(parser(fd, &scene)))
		ft_error("Scene is incomplete or incorrect\n");
	close(fd);
	if (sdl_init(&sdl) < 0)
	{
		struct_del(&scene);
		exit(-1);
	}
	if (init_env(&env, &scene, &obj_pix[0], &sdl))
	{
		struct_del(&scene);
		exit(-1);
	}
	rt_loop(&env, &sdl);
	printf("Works\n");
	sdl_close(&sdl);
	return (0);
}
