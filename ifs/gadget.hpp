#ifndef GADGET_HPP
#define GADGET_HPP

inline static int global_counter = 42;

class Gadget
{
public:
	inline static size_t counter = 0;

	Gadget() 
	{
		++counter;
	}
};

#endif