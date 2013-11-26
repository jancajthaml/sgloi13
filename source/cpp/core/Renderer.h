/*
 * Renderer.h
 *
 *  Created on: Nov 26, 2013
 *      Author: jancajthaml
 */

#ifndef RENDERER_H_
#define RENDERER_H_

class Renderer
{/*

	inline void intersection_exist(Ray &ray, HitInfo &hit_info, Scene &scene)
	{
		hit_info.object = 0;
		hit_info.t = FLT_MAX;

		for (int i = 0; i < scene.prim_num; i++)
			scene.primitives[i]->intersect(ray, hit_info);
	}

	inline void opt_intersection_exist(Ray &ray, HitInfo &hit_info, Scene &scene)
	{
		hit_info.object = 0;
		hit_info.t = FLT_MAX;

		for (int i = 0; i < scene.prim_num; i++)
			scene.primitives[i]->opt_intersect(ray, scene.assists[i], hit_info);
	}

	void render(Scene &scene, float *buffer, bool enable_shadow)
	{

		float3 &c = scene.camera->c, &a = scene.camera->a, &b = scene.camera->b;

		#pragma omp parallel for
		for (int y = 0; y < scene.camera->height; y++) {
	    Ray ray;
	    HitInfo hit_info;
	    float *buf = buffer + y * scene.camera->width * 3;
	    ray.origin = scene.camera->eye;

	    float yf = (y + 0.5f);

	    for (int x = 0; x < scene.camera->width; x++) {
	      // setting ray
	      float xf = (x + 0.5f);

	      ray.dir.x = c.x + xf * a.x + yf * b.x;
	      ray.dir.y = c.y + xf * a.y + yf * b.y;
	      ray.dir.z = c.z + xf * a.z + yf * b.z;

	      normalize(ray.dir);

	      // computing intersections
	      //opt_intersection_exist(ray, hit_info, scene);
	      hit_info.object = 0;
	      hit_info.t = FLT_MAX;

	      for (int i = 0; i < scene.prim_num; i++)
	        scene.primitives[i]->opt_intersect(ray, scene.assists[i], hit_info);

	      // one intersection was found at least -> shade ray
	      if (hit_info.object) {
	        if (enable_shadow)
	          shader.shade_ray_with_shadow(ray, hit_info, scene);
	        else
	          shader.shade_ray(ray, hit_info, scene);

	        *buf++ = hit_info.color.x;
	        *buf++ = hit_info.color.y;
	        *buf++ = hit_info.color.z;
	      }
	      // draw background
	      else {
	        *buf++ = scene.bg_color.x;
	        *buf++ = scene.bg_color.y;
	        *buf++ = scene.bg_color.z;
	      }
	    }
	  }
	}


	void render_with_DoF(Scene &scene, float *buffer, float r, float df, int ray_num, bool use_AA)
	{
	  Phong shader;

	  float a = 1.0f / ray_num;

	  scene.camera->setFocus(df);

	  #pragma omp parallel for
	  for (int y = 0; y < scene.camera->height; y++) {
	    HitInfo hit_info;
	    float *buf = buffer + y * scene.camera->width * 3;
	    float yf = (y + 0.5f);

	    for (int x = 0; x < scene.camera->width; x++) {
	      float3 pixel_color, s;
	      Ray ray;

	      float xf = (x + 0.5f);

	      s.x = scene.camera->c.x + xf * scene.camera->a.x + yf * scene.camera->b.x;
	      s.y = scene.camera->c.y + xf * scene.camera->a.y + yf * scene.camera->b.y;
	      s.z = scene.camera->c.z + xf * scene.camera->a.z + yf * scene.camera->b.z;

	      ZEROS(pixel_color);

	      for (int k = 0; k < ray_num; k++) {
	        //setting ray
	        float dx, dy;

	        get_dxdy(dx, dy, r);

	        ray.origin.x = scene.camera->eye.x + dx * scene.camera->right.x + dy * scene.camera->up.x;
	        ray.origin.y = scene.camera->eye.y + dx * scene.camera->right.y + dy * scene.camera->up.y;
	        ray.origin.z = scene.camera->eye.z + dx * scene.camera->right.z + dy * scene.camera->up.z;

	        SUB(ray.dir, s, ray.origin);

	        normalize(ray.dir);

	        //computing intersections
	        intersection_exist(ray, hit_info, scene);

	        //one intersection was found at least -> shade ray
	        if (hit_info.object)
	          shader.shade_ray_with_shadow(ray, hit_info, scene);
	        //draw background
	        else
	          hit_info.color = scene.bg_color;

	        if (use_AA)
	          explore_neighbourhood(ray, xf, yf, 0.25f, scene, hit_info.color, &shader);

	        ADD_FRAGMENT(pixel_color, hit_info.color, a);
	      }
	      //fill buffer
	      *buf++ = pixel_color.x;
	      *buf++ = pixel_color.y;
	      *buf++ = pixel_color.z;
	    }
	  }
	  */
};


#endif /* RENDERER_H_ */
