NAME= physics
SRC= srcs/main.c \
	srcs/ft_create_mem_block.c \
	srcs/ft_mem_block_push_back_elem.c \
	srcs/ft_get_image_frames.c \
	srcs/ft_pthread_frame_clear_routine.c \
	srcs/ft_claim_frame.c \
	srcs/ft_pthread_wait_drawing_threads.c \
	srcs/ft_set_mlx_hooks.c \
	srcs/ft_handler_keys.c \
	srcs/ft_handler_mouse.c \
	srcs/ft_handler_esc.c \
	srcs/ft_handler_right.c \
	srcs/ft_handler_left.c \
	srcs/ft_handler_up.c \
	srcs/ft_handler_down.c \
	srcs/ft_handler_pad_up.c \
	srcs/ft_handler_pad_down.c \
	srcs/ft_handler_q.c \
	srcs/ft_handler_e.c \
	srcs/ft_handler_a.c \
	srcs/ft_handler_alpha.c \
	srcs/ft_handler_d.c \
	srcs/ft_handler_s.c \
	srcs/ft_handler_w.c \
	srcs/ft_handler_symetry.c \
	srcs/ft_handler_mouse_motion.c \
	srcs/ft_get_reduction_coefficient.c \
	srcs/ft_get_perspective_angle.c \
	srcs/ft_get_lerp.c

# srcs/ft_set_3d.c \
# srcs/ft_parse_file.c \
# srcs/ft_parse_color.c \
# srcs/quaternions/ft_get_conjugate_quat.c \
# srcs/quaternions/ft_get_quat_magnitude.c \
# srcs/quaternions/ft_multiply_quat.c \
# srcs/quaternions/ft_normalize_quat.c \
# srcs/quaternions/ft_quat_rotate_points.c \


HDRS= includes/fdf.h
OBJ= $(SRC:.c=.o)
HDR_PATH= ./libft/includes/
CC= gcc
CC_FLAGS= -v -Wall -Wextra -isystem /usr/include -L /usr/lib/x86_64-linux-gnu/ -lm -lXext -lX11 -lGL -lpthread

ifeq ($(optim),yes)
	CC_FLAGS += -Ofast -march=native
endif


ifeq ($(debug),yes)
	CC_FLAGS +=  -g3 -fsanitize=address -fsanitize-blacklist=my_ignores.txt
endif

MLX_PATH=./minilibx-linux/
LIBFT_PATH=./libft/
FLAGS=  -isystem /usr/include -L$(MLX_PATH)  -I$(MLX_PATH) -lm -lmlx -L$(LIBFT_PATH) -lft -I$(HDR_PATH) -I./includes -lGL -lX11  -lXext  -lpthread # -framework OpenGL -framework AppKit 

all: submodule $(NAME)

submodule:
	@make -C $(MLX_PATH)
	@make -C libft/

$(NAME): $(OBJ)
	$(CC) $(CC_FLAGS) $^ $(FLAGS) -o $(NAME)
%.o : %.c $(HDRS)
	$(CC) $(CC_FLAGS) $< -c -I$(HDR_PATH) -I./includes -I$(MLX_PATH) -o $@

clean:
	rm -f $(OBJ)
	make -C $(LIBFT_PATH) clean
	make -C $(MLX_PATH) clean
fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_PATH) fclean

re: fclean all
