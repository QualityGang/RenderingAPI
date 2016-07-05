#pragma once

template <void(*ctor)()>
struct static_constructor
{
	struct constructor { constructor() { ctor(); } };
	static constructor c;
};

template <void(*dtor)()>
struct static_destructor
{
	struct destructor { ~destructor() { dtor(); } };
	static destructor d;
};

template <void(*ctor)()>
typename static_constructor<ctor>::constructor static_constructor<ctor>::c;

template <void(*dtor)()>
typename static_destructor<dtor>::destructor static_destructor<dtor>::d;