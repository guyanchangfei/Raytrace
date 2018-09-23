#include<iostream>
#include<fstream>
#include<limits>

using namespace std;
#include "vec3.h"
#include "hitable_list.h"
#include "camera.h"
#include "sphere.h"
#include "hitable_list.h"
#include "material.h"

hitable *random_scene() {
	int n = 500;
	hitable **list = new hitable*[n + 1];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
	int i = 1;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			float choose_mat = drand48();
			vec3 center(a + 0.9*drand48(), 0.2, b + 0.9*drand48());
			if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
				if (choose_mat < 0.8) {  // diffuse
					list[i++] = new sphere(center, 0.2, new lambertian(vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48())));
				}
				else if (choose_mat < 0.95) { // metal
					list[i++] = new sphere(center, 0.2,
						new metal(vec3(0.5*(1 + drand48()), 0.5*(1 + drand48()), 0.5*(1 + drand48())), 0.5*drand48()));
				}
				else {  // glass
					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
	}

	list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
	list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

	return new hitable_list(list, i);
}

vec3 color(const ray& r,hitable *world,int depth) {
	hit_record rec;
	//float t = hit_sphere(vec3(0, 0, -1), 0.5, r);
	if (world->hit(r,0.001,(numeric_limits<float>::max)(), rec))
	{
		ray scattered;
		vec3 attenuation;
		if (depth < 50&& rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		{
			return attenuation * color(scattered, world, depth + 1);
		}
		else 
		{
			return vec3(0, 0, 0);
		}
		//vec3 target = rec.p + rec.normal + random_int_unit_Sphere();

		//return 0.5*color(ray(rec.p,target-rec.p),world);
	}
	else
	{
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5*(unit_direction.y() + 1.0);
		return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
	
}

int main() {

	int nx = 1200;
	int ny = 800;
	int ns = 10;
	ofstream outfile("mytxt.ppm",std::ios_base::out);
	outfile << "P3\n" << nx << " " << ny << "\n255\n";
	cout << "P3\n" << nx << " " << ny << "\n255\n";

	//hitable *list[5];
	//hitable *world = new hitable_list(list, 5);
	hitable *world;
	world = random_scene();
	vec3 lookfrom(13, 2, 3);
	vec3 lookat(0, 0, 0);
	float dis_to_focus = 10.0;
	float aperture = 0.1;
	camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, float(nx) / float(ny),aperture,dis_to_focus);
	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++)
		{
			vec3 vec(0, 0, 0);
			for (int s = 0; s < ns; s++)
			{
				float random = rand() % (100) / (float)(100);
				float u = float(i+random) / float(nx);
				float v = float(j+random) / float(ny);
				ray r = cam.get_ray(u, v);
				vec3 p = r.point_at_parameter(2.0);
				vec += color(r,world,0);
			}
			
			vec /=(float) ns;
			vec = vec3(sqrt(vec[0]), sqrt(vec[1]), sqrt(vec[2]));
			//vec3 vec(float(i) / float(nx), float(j) / float(ny), 0.2);
			int ir = int(255.99*vec[0]);
			int ig = int(255.99*vec[1]);
			int ib = int(255.99*vec[2]);
			cout << ir << " " << ig << " " << ib << "\n";
			outfile<< ir << " " << ig << " " << ib << "\n";
		}
	}
	
	
}