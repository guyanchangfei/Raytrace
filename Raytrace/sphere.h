#ifndef SPHEREH
#define SPHEREH
#include"hitable.h"
class sphere:public hitable {
	public:
		sphere() {}
		sphere(vec3 cen, float r,material* m) :center(cen), radious(r),mat_ptr(m) {};
		virtual bool hit(const ray&r, float tmin, float tmax, hit_record& rec)const;
		vec3 center;
		float radious;
		material *mat_ptr;


};
bool sphere::hit(const ray& r, float tmin, float tmax, hit_record& rec)const 
{
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());
	float c = dot(oc, oc) - radious * radious;
	float discrimainant = b * b - a * c;
	if (discrimainant > 0)
	{
		float temp = (-b - sqrt(b*b - a * c)) / a;
		if (temp<tmax&&temp>tmin) {
			rec.t = temp;
			rec.p = r.point_at_parameter(temp);
			rec.normal = (rec.p - center) / radious;
			rec.mat_ptr = mat_ptr;
			return true;
		}
		temp= (-b + sqrt(b*b - a * c)) / a;
		if (temp<tmax&&temp>tmin) {
			rec.t = temp;
			rec.p = r.point_at_parameter(temp);
			rec.normal = (rec.p - center) / radious;
			rec.mat_ptr = mat_ptr;
			return true;
		}

	}
	return false;
}
#endif // !SPHEREH
