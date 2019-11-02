/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sclolus <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/25 16:55:51 by sclolus           #+#    #+#             */
/*   Updated: 2019/11/02 18:55:45 by sclolus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include <stdint.h>
# include <unistd.h>
# include "key_codes_macos.h"
# include "events.h"
# include "mlx.h"
# include "libft.h"
# include <fcntl.h>
# include <pthread.h>
# include <math.h>
# include <stdio.h>
# include <assert.h>
# include <time.h>
# include <stdbool.h>

typedef void* t_mlx_win;
typedef void* t_mlx_img;
typedef void* t_mlx_ptr;

# define BIN_NAME "2d_physics"

# define WINDOW_NAME BIN_NAME
# define WINDOW_WIDTH 1920
# define WINDOW_HEIGHT 1080

# define PI 3.14159265359
# define K 0.5
# define ANGLE 120

# define MOVE_COEFFICIENT 7
# define ROTATION_SPEED 9
# define ZOOM_RATIO 1.13
# define DEZOOM_RATIO 1 / ZOOM_RATIO
# define FPS 60
# define CLOCK_FRAME_DELTA CLOCKS_PER_SEC / FPS

# define NBR_IMAGE_FRAME 8

# define STDIN_NOFILE 0
# define GRAVITATIONAL_CONSTANT (6.674 / 100000000000.0)
# define BASE_SCALING_FACTOR 1
# define DEFAULT_OBJECT_NUMBER 1
# define SCALING_FACTOR_FACTOR 1.3

# define ABS(x) (x < 0 ? -x : x)

# define ENUM_STRING(x) (#x)

typedef struct	s_color_set
{
	int	color_min;
	int	color_max;
}				t_color_set;

typedef struct	s_mem_block
{
	uint64_t			capacity;
	uint64_t			offset;
	void				*block;
	struct s_mem_block	*next;
}				t_mem_block;

typedef struct	s_vec
{
	double	x;
	double	y;
	double	z;
}				t_vec;

typedef struct	s_point
{
	t_vec				coords;
	int					color;
}				t_point;

typedef struct	s_keycode_f
{
	void	(*f)();
	int		keycode;
	int32_t	used;
}				t_keycode_f;

typedef enum	e_frame_state
{
	ERR = 0,
	USED = 1,
	TO_CLEAR = 2,
	AVAILABLE = 3,
	CLEARING = 4,
}				t_frame_state;

typedef struct	s_image_frame
{
	t_mlx_img		frame;
	void			*buffer;
	t_frame_state	state;
	int32_t			bits_per_pixel;
	int32_t			size_line;
	int32_t			endian;
}				t_image_frame;

typedef struct	s_mlx_data
{
	t_mlx_ptr			connector;
	t_mlx_win			win;
	t_image_frame		*frame;
}				t_mlx_data;

typedef struct	s_pthread_execution_data
{
	t_mlx_data	mlx_data;
	uint32_t	offset;
	t_mem_block	*data;
}				t_pthread_execution_data;

typedef struct	s_quat
{
	double	x;
	double	y;
	double	z;
	double	w;
}				t_quat;


typedef struct s_2d_vector
{
	double	x;
	double	y;
}			   t_2d_vector;

t_2d_vector vector2d_new(const double x, const double y);
t_2d_vector vector2d_zero(void);
t_2d_vector	vector2d_add(const t_2d_vector a, const t_2d_vector b);
t_2d_vector vector2d_sub(const t_2d_vector a, const t_2d_vector b);
t_2d_vector vector2d_scalar_multiply(const t_2d_vector a, double scalar);
t_2d_vector vector2d_scalar_divide(const t_2d_vector a, double scalar);
t_2d_vector vector2d_rotate(const t_2d_vector origin, const t_2d_vector a, double angle);
double		vector2d_magnitude(const t_2d_vector a);
t_2d_vector	vector2d_multiply(const t_2d_vector a, const t_2d_vector b);
double		vector2d_dot_product(const t_2d_vector a, const t_2d_vector b);
double		vector2d_distance(const t_2d_vector a, const t_2d_vector b);
t_2d_vector vector2d_normalize(const t_2d_vector a);
t_2d_vector vector2d_reflect(const t_2d_vector a, const t_2d_vector reflection_axis);
t_2d_vector	vector2d_rotate(const t_2d_vector point, const t_2d_vector rotation_point, const double angle);
t_2d_vector	vector2d_point_symetry(const t_2d_vector point, const t_2d_vector point_of_symetry);

double		bernstein_basis_polynomial(uint64_t n, uint64_t k, double x);
t_2d_vector	bezier_2d_curve(uint64_t order, t_2d_vector *control_points, double t);

typedef struct s_rectangle
{
	t_2d_vector min;
	t_2d_vector max;
}				t_rectangle;

typedef struct	s_circle
{
	double		radius;
}				t_circle;

typedef struct	s_line
{
	t_2d_vector	dir;
}				t_line;


typedef uint32_t	t_color;
typedef enum	s_shape_kind {
	CIRCLE,
	RECTANGLE,
	ATTRACTOR,
	LINE,
	NONE,
	__SHAPE_KIND_VARIANTS_NBR,
}				shape_kind;

# define NBR_SHAPES __SHAPE_KIND_VARIANTS_NBR

typedef struct s_object {
	t_color			color;
	shape_kind		kind;
	t_2d_vector		pos;
	union {
		t_circle	circle;
		t_circle	attractor;
		t_line		line;
	};
	t_2d_vector		velocity;
	t_2d_vector		acceleration;
	double			mass;
	t_2d_vector		applied_forces;
}				object;

typedef bool (*t_f_intersection)(object *a, object *b);

typedef struct	s_univers {
	uint32_t	nbr_objects;
	object		*objects;
	t_2d_vector	cam;
	double		scaling_factor;
	uint32_t	current_follow;
	double		time_ratio;
}				univers;

extern univers	   *g_univers;
extern t_2d_vector symetry_point;
extern t_2d_vector symetry_axis;
extern t_2d_vector defining_point;
extern uint8_t	   g_global_alpha;
extern bool		   symetry_on;

# define ROTATIONS_PER_SEC 0.6 * 2 * M_PI

void	init_univers(univers *univers);
void	univers_remove_object(univers *univers, uint32_t index);
void	univers_add_object(univers *univers, object object);
void	univers_map_objects(univers *univers, void (*lambda)(object *obj, void *private), void *private);
void	univers_map_2d_objects(univers *univers, void (*lambda)(object *a, object *b, void *private), void *private);
void	object_reset_forces(object *object, void *private);
/*
** Mem_block handling
*/

# define MEM_BLOCK_LIMIT 256
# define DEFAULT_MEM_BLOCK_SIZE sizeof(t_point) * sizeof(t_line) * (100000)

void			*ft_mem_block_push_back_elem(t_mem_block *mem_block
									, void *elem, uint32_t size);
t_mem_block		*ft_create_mem_block(uint64_t capacity);
t_mem_block		*ft_set_lines(t_mem_block *mem_block);

/*
** Frames handling
*/

t_image_frame	*ft_get_image_frames(t_mlx_ptr connector, uint32_t nbr_frames);
void			*ft_pthread_frame_clear_routine(void *arg);
t_image_frame	*ft_claim_image_frame(t_image_frame *frames);

/*
** Isometric Perspective
*/

void			ft_sus_perspective_angle(void);
void			ft_add_perspective_angle(void);
double			*ft_get_perspective_angle(void);

double			*ft_get_reduction_coefficient(void);
void			ft_sus_reduction_coefficient(void);
void			ft_add_reduction_coefficient(void);

/*
** Parsing
*/

# define HEXA_CHARSET "0123456789ABCDEF"
# define ERR_COLOR_NBR "Invalid number of color in color file"
# define ERR_COLOR_INVALID_FILE_SIZE "Invalid file size for color file"
# define ERR_INVALID_CHAR_COLOR_FILE "Invalid characters in color file"

t_mem_block		*ft_parse_file(char *filename, char *filename_color);
t_color_set		ft_parse_color(char *filename_color);

/*
** Line drawing
*/

typedef void	(*t_draw_line)(t_mlx_data *, t_point, t_point);

void			ft_set_3d(t_mem_block *data);
void			ft_draw_line(t_mlx_data *mlx_data
						, t_line *line) __attribute__((hot));
void			ft_draw_lines(t_mlx_ptr connector, t_mlx_win win
						, t_image_frame *frames, t_mem_block *data);
int				ft_draw_lines_hook(void *param);
int32_t			ft_line_clipping(t_line *tmp);
void			ft_swap_t_point_in_line(t_line *line);

/*
** Pthread drawing
*/

# define DRAWING_THREAD_NBR 8
# define ERR_PTHREAD_FAIL "pthread_create() failed"

pthread_t		*ft_pthread_create_lines_drawing_threads(t_mlx_ptr connector
				, t_mlx_win win, t_image_frame *frame, t_mem_block *data);
void			ft_pthread_wait_drawing_threads(pthread_t *thread_tab);
void			*ft_pthread_lines_drawing_routine(void *arg);

/*
** Key handling
*/

# define NBR_KEY_HOOKS 11
# define INVALID_KEYS_HOOKS_NBR "Invalid keys number given in macro expansion"

void			ft_set_mlx_hooks(t_mlx_data *mlx_data, void **params);
int				ft_handler_mouse(int button, int x, int y, void *private);
int				ft_handler_keys(int keycode, void *param);
int				ft_handler_keys_release(int keycode
					, void __attribute__((unused)) *param);
void			ft_handler_esc(void __attribute__((unused)) *param);
void			ft_handler_right(void *param);
void			ft_handler_left(void *param);
void			ft_handler_down(void *param);
void			ft_handler_up(void *param);
void			ft_handler_pad_down(void *param);
void			ft_handler_pad_up(void *param);
void			ft_handler_q(void *param);
void			ft_handler_s(void *param);
void			ft_handler_e(void *param);
void			ft_handler_w(void *param);
void			ft_handler_s(void *param);
void			ft_handler_toggle_symetry(void *param);
void			ft_handler_d(void *param);
void			ft_handler_a(void *param);
void			ft_handler_alpha_up(void *param);
void			ft_handler_alpha_down(void *param);
void			ft_handler_r(void *param);
void			ft_handler_m(void *param);
void			ft_handler_left(void *param);
void			ft_handler_right(void *param);
int			   	ft_handler_mouse_motion(int x, int y, void *param);

/*
** Quaternions
*/

double			ft_get_quat_magnitude(t_quat *quat);
t_quat			ft_normalize_quat(t_quat *quat);
t_quat			ft_get_conjugate_quat(t_quat *quat);
t_quat			ft_multiply_quat(t_quat a, t_quat b);
void			ft_quat_rotate_points(t_vec *axis, double angle
				, t_mem_block *data) __attribute__((hot));

/*
** Interpolation
*/

# define COLOR_MIN_Z 0xF00000
# define COLOR_MAX_Z 0xFFFFFF

int32_t			ft_get_color_interpolation(t_line *line, t_vec *point);
double			ft_get_z_coord(t_line *line, t_vec *point);
int32_t			ft_get_lerp(double z1, double z2
				, double z, t_color_set color_set);

/*
** Error handling
*/

# define err(format, ...) do {											\
		dprintf(2, BIN_NAME ": %s::%s:(%d): " format "\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__); \
		exit(EXIT_FAILURE);												\
	} while (0);

# define err_errno(format, ...) do {									\
		dprintf(2, BIN_NAME ": %s::%s:(%d): " format ": %s\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__, strerror(errno)); \
		exit(EXIT_FAILURE);												\
	} while (0);

# define warn(format, ...) do {											\
		dprintf(2, BIN_NAME ": %s::%s:(%d): Warning: " format "\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__); \
	} while (0);

# define warn_errno(format, ...) do {									\
		dprintf(2, BIN_NAME ": %s::%s:(%d): Warning: " format ": %s\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__, strerror(errno)); \
	} while (0);

# define INVALID_MEM_CAPACITY "Invalid capacity given to ft_create_mem_block()"
# define MALLOC_FAILURE "malloc() failed due to insufficient ressources left"
# define MLX_INIT_ERROR "mlx_init() failed"
# define MLX_NEW_WIN_ERROR "mlx_new_window() failed"
# define MLX_NEW_IMG_ERROR "mlx_new_image() failed"
# define T_MLX_IMG_FRAMES_ERROR "malloc() failed to alloc image frames"
# define OPEN_FILE_FAILED ": open() failed"
# define ERR_PARSE_COLOR_READ "read() failed"

#endif
