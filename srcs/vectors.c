/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vectors.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sclolus <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/02 22:39:32 by sclolus           #+#    #+#             */
/*   Updated: 2019/11/03 02:12:09 by sclolus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fdf.h>

# define VEC3_OP(a, b, op) (t_3d_vector) { \
		.x = a.x op b.x,					   \
			.y = a.y op b.y,				   \
			.z = a.z op b.z,				   \
}

# define VEC3_SCALAR_OP(a, scalar, op) (t_3d_vector) { \
		.x = a.x op scalar,							   \
			.y = a.y op scalar,				   \
			.z = a.z op scalar,				   \
}

t_3d_vector	vector3d_add(const t_3d_vector a, const t_3d_vector b)
{
	return VEC3_OP(a, b, +);
}

t_3d_vector	vector3d_sub(const t_3d_vector a, const t_3d_vector b)
{
	return VEC3_OP(a, b, -);
}

t_3d_vector	vector3d_scalar_multiply(const t_3d_vector a, const double scalar)
{
	return VEC3_SCALAR_OP(a, scalar, *);
}

t_3d_vector	vector3d_scalar_divide(const t_3d_vector a, const double scalar)
{
	return VEC3_SCALAR_OP(a, scalar, /);
}

double		vector3d_dot_product(const t_3d_vector a, const t_3d_vector b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

double		vector3d_magnitude(const t_3d_vector a)
{
	return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}

double		vector3d_distance(const t_3d_vector a, const t_3d_vector b)
{
	t_3d_vector ab = vector3d_sub(a, b);

	return vector3d_magnitude(ab);
}

t_3d_vector	vector3d_normalize(const t_3d_vector a)
{
	double	magnitude = vector3d_magnitude(a);

	return vector3d_scalar_divide(a, magnitude);
}

t_3d_vector	vector3d_new(double x, double y, double z)
{
	return (t_3d_vector){
		.x = x,
			.y = y,
			.z = z,
	};
}

t_3d_vector	vector3d_zero(void)
{
	return vector3d_new(0.0, 0.0, 0.0);
}

t_3d_vector	vector3d_reflect(const t_3d_vector a, const plan reflection_plan)
{
	t_3d_vector	a_to_plan_point = vector3d_sub(reflection_plan.point, a);
	t_3d_vector	to_plan = vector3d_scalar_multiply(reflection_plan.normal, vector3d_dot_product(a_to_plan_point, reflection_plan.normal));
	t_3d_vector	to_reflected_point = vector3d_scalar_multiply(to_plan, 2.0);

	t_3d_vector	reflected_point = vector3d_add(a, to_reflected_point);

	return reflected_point;
}
