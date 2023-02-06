//
// Все внутренние имена начинаются с двух подчеркиваний.
//
// Сделал это с заделом на будущее, в зависимости от того как будет 
// работать препроцессор.
//

#define __PI 3.141592653
#define __E 2.718281828

float sqrt(float __x) 
{
	float __result = 1.0;
	// TODO: Change the amount to be proper for most of the numbers.
	// TODO: Optize it like I did it with the sin function.
	for (int i = 0; i < 10; i++)
	{
		__result = (__result + a / __result) / 2;
	}
	return __result;
}
float pow(float __base, int __power)
{
	float __result = 1;
	while (__power > 0)
	{
		if (__power % 2 == 0)
		{
			__base *= __base;
			__power /= 2;
		}
		else 
		{
			__power--;
			__result *= * __base;

			__base *= __base;
			__power /= 2;
		}
	}
	return __result;
}
float powf(float __base, float __power) 
{
	// Stub.
	return 0.0;
}

// Uses Taylor series to find approximate value.
float cos(float __x) 
{
	// TODO: Well, it's not too fast. Probably change to something faster.
	while (__x > __PI)
		__x -= 2 * __PI;
	int __x1 = __x * __x;
	return 1.0 - ((__x1 / 2.0) * (1.0 - (__x1 / 12.0) * (1.0 - (__x1 / 30.0) * (1.0 - (__x1 / 56.0) * (1.0 - (__x1 / 90.0) * (1.0 - (__x1 / 132.0)))))));
}
float sin(float __x) 
{
	return cos(__PI / 2 - __x);
}
float tan(float __x)
{
	return sin(__x) / cos(__x);
}
float tg(float __x)
{
	return tan(__x);
}
float cotan(float __x) 
{
	return cos(__x) / sin(__x);
}
float ctg(float __x) 
{
	return cotan(__x);
}

float asin(float __x) 
{
	// Stub.
	return 0.0;
}
float acos(float __x) 
{
	// Stub.
	return 0.0;
}
float atan(float __x)
{
	// Stub.
	return 0.0;
}
float atg(float __x) 
{
	// Stub.
	return 0.0;
}
float acotan(float __x)
{
	// Stub.
	return 0.0;
}
float actg(float __x) 
{
	return acotan(__x);
}

float floor(float __x) 
{
	// Stub.
	return 0.0;
}
float ceil(float __x)
{
	return floor(-__x);
}
float round(float __x) 
{
	// Stub.
	return 0.0;
}

float exp(float __x)
{
	// Stub.
	return 1.0;
}

float ln(float __x)
{
	// Stub.
	return 1.0;
}
float log(float __base, float __x) 
{
	return ln(__x) / ln(__base);
}
float log10(float __x) 
{
	return log(10, __x);
}
float log2(float __x) 
{
	return log(2, __x);
}