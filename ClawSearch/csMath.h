#pragma once

#include <math.h>

inline float truncf_to(const float &f, int n)
{
	if (n == 0) { return truncf(f); }
	float m = powf(10.0f, (float)n);
	return truncf(f * m) / m;
}

inline double truncl_to(const double &f, int n)
{
	if (n == 0) { return truncl(f); }
	double m = powl(10.0f, (double)n);
	return truncl(f * m) / m;
}

inline float roundf_to(const float &f, int n)
{
	if (n == 0) { return roundf(f); }
	float m = powf(10.0f, (float)n);
	return roundf(f * m) / m;
}

inline double roundl_to(const double &f, int n)
{
	if (n == 0) { return roundl(f); }
	double m = powl(10.0, (double)n);
	return roundl(f * m) / m;
}

inline float floorf_to(const float &f, int n)
{
	if (n == 0) { return floorf(f); }
	float m = powf(10.0f, (float)n);
	return roundf(f * m) / m;
}

inline double floorl_to(const double &f, int n)
{
	if (n == 0) { return floorl(f); }
	double m = powl(10.0, (double)n);
	return floorl(f * m) / m;
}

inline float ceilf_to(const float &f, int n)
{
	if (n == 0) { return ceilf(f); }
	float m = powf(10.0f, (float)n);
	return ceilf(f * m) / m;
}

inline double ceill_to(const double &f, int n)
{
	if (n == 0) { return ceill(f); }
	double m = powl(10.0, (double)n);
	return ceill(f * m) / m;
}
