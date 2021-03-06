# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vbespalk <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/08/18 14:12:29 by vbespalk          #+#    #+#              #
#    Updated: 2018/08/18 14:12:31 by vbespalk         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	rt

CC		=	gcc

# source files
SRC =	main.c\
		check.c\
		init.c\
		parser.c\
		source.c\
		sscene.c\
		matrixes.c\
		render.c\
		vectors.c\
		display.c\
		light.c\
		intersect.c\
		inverse.c\
		threads.c\
		operations.c\
		init_sdl.c\
		events.c\
		hook.c\
		key.c\
		features.c\
		mapping.c

# directories
SRC_DIR	=	./src/
INC_DIR	=	-I ./inc/
OBJ_DIR	=	./obj/

# object files
# SRCS	=	$(addprefix $(SRC_D),$(SRC))
# OBJS	=	$(SRCS:.c=.o)
OBJS	=	$(addprefix $(OBJ_DIR),$(SRC:.c=.o))


#FT library
LIB_D	=	./libft/
FT_LIB	=	$(addprefix $(LIB_D),libft.a)
LIB_INC	=	-I $(LIB_D)
LIB_L	=	-L $(LIB_D) -lft

# SDL_LIB
# SDL_INC =	-I frameworks/SDL2.framework/Headers/
# SDL_LNK	=	-F ./frameworks -rpath ./frameworks -framework SDL2 
# SDL_IMG_INC =	-I frameworks/SDL2_image.framework/Headers/
# SDL_IMG_LNK =	-F ./frameworks -rpath ./frameworks -framework SDL2_image 

#LINUX SDL
# -lSDL2 -pthread
SDL_LNK		= -lSDL2
SDL_IMG_LNK = -lSDL2_image

#rules
all: obj $(FT_LIB) $(NAME) 

obj: 
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)%.o:$(SRC_DIR)%.c ./inc/rtv1.h
	$(CC) $(LIB_INC) $(INC_DIR) $(SDL_INC) $(SDL_IMG_INC) -o $@ -c $<

$(MLX_LIB):
	make -C $(MLX_D)

$(FT_LIB):
	make -C $(LIB_D)

$(NAME) : $(OBJS)
	$(CC) $(OBJS) -Wpsabi $(SDL_LNK) $(SDL_IMG_LNK) -pthread  $(LIB_L)  -lm -o $(NAME)

clean:
	rm -rf $(OBJ_DIR)
	make -C $(LIB_D) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIB_D) fclean
			
re:	fclean all

.PHONY: $(FT_LIB)