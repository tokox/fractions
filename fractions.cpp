#include "fractions.hpp"
namespace tokox
{

template <Fraction_compatible T>
T common_denominator (const Fraction<T>& a,
	const Fraction<T>& b,
	std::string where = "common_denominator",
	std::function<bool(T, T)> check = [] (T, T) { return true; }
)
{
	if (can_mul<T>(a.numerator(), b.denominator())
		&& can_mul<T>(b.numerator(), a.denominator())
		&& can_mul<T>(a.denominator(), b.denominator())
		&& check(a.numerator() * b.denominator(), b.numerator() * a.denominator())
		)
	{
		return a.denominator() * b.denominator();
	}
	if (!a.reduced())
	{
		a.reduce();
		if (can_mul<T>(a.numerator(), b.denominator())
			&& can_mul<T>(b.numerator(), a.denominator())
			&& can_mul<T>(a.denominator(), b.denominator())
			&& check(a.numerator() * b.denominator(), b.numerator() * a.denominator())
			)
		{
			return a.denominator() * b.denominator();
		}
	}
	if (!b.reduced())
	{
		b.reduce();
		if (can_mul<T>(a.numerator(), b.denominator())
			&& can_mul<T>(b.numerator(), a.denominator())
			&& can_mul<T>(a.denominator(), b.denominator())
			&& check(a.numerator() * b.denominator(), b.numerator() * a.denominator())
			)
		{
			return a.denominator() * b.denominator();
		}
	}
	T _lcm;
	try
	{
		_lcm = lcm<T>(a.denominator(), b.denominator());
	}
	catch (std::overflow_error&)
	{
		throw FractionOverflowError<T>("common_denominator");
	}
	if (can_mul<T>(a.numerator(), _lcm / a.denominator())
		&& can_mul<T>(b.numerator(), _lcm / b.denominator()))
	{
		if (check(a.numerator() * (_lcm / a.denominator()), b.numerator() * (_lcm / b.denominator())))
		{
			return _lcm;
		}
		throw FractionOverflowError<T>(where);
	}
	throw FractionOverflowError<T>("common_denominator");
}

template <Fraction_compatible T>
Fraction<T>::Fraction (const T n, const T d):
	_numerator(n),
	_denominator(d),
	_flags(0)
{
	if (_denominator < T(0))
	{
		if (!can_neg<T>(_numerator) || !can_neg<T>(_denominator))
		{
			reduce();
			if (!can_neg<T>(_numerator) || !can_neg<T>(_denominator))
			{
				throw FractionOverflowError<T>("Fraction::Fraction");
			}
		}
		_numerator = -_numerator;
		_denominator = -_denominator;
	}
	else if (_denominator == T(0))
	{
		throw FractionDenominatorIsZeroError<T>("Fraction::Fraction");
	}
}

template<Fraction_compatible T>
Fraction<T>::Fraction (const T n, const T d, const uint8_t f):
	_numerator(n),
	_denominator(d),
	_flags(f)
{}

template <Fraction_compatible T>
Fraction<T>::Fraction (const Fraction& other):
	_numerator(other.numerator()),
	_denominator(other.denominator()),
	_flags(other.reduced() ? REDUCED : 0)
{}



template <Fraction_compatible T>
Fraction<T>& Fraction<T>::operator= (const Fraction& other)
{
	_numerator = other.numerator();
	_denominator = other.denominator();
	_flags = other.reduced() ? REDUCED : 0;
	return *this;
}


template <Fraction_compatible T>
Fraction<T>& Fraction<T>::operator+= (const Fraction& other)
{
	T common_denom = common_denominator(*this, other, "Fraction::operator+=", can_add<T>);
	_numerator = (_numerator * (common_denom / _denominator)) + (other.numerator() * (common_denom / other.denominator()));
	_denominator = common_denom;
	_flags &= ~REDUCED;
	return *this;
}

template <Fraction_compatible T>
Fraction<T> Fraction<T>::operator+ (const Fraction& other) const
{
	return Fraction(*this) += other;
}

template<Fraction_compatible T>
Fraction<T> Fraction<T>::operator+ () const
{
	reduce();
	return Fraction(*this);
}


template <Fraction_compatible T>
Fraction<T>& Fraction<T>::operator-= (const Fraction& other)
{
	T common_denom = common_denominator(*this, other, "Fraction::operator-=", can_sub<T>);
	_numerator = (_numerator * (common_denom / _denominator)) - (other.numerator() * (common_denom / other.denominator()));
	_denominator = common_denom;
	_flags &= ~REDUCED;
	return *this;
}

template <Fraction_compatible T>
Fraction<T> Fraction<T>::operator- (const Fraction& other) const
{
	return Fraction(*this) -= other;
}

template <Fraction_compatible T>
Fraction<T> Fraction<T>::operator- () const
{
	if (!can_neg<T>(_numerator))
	{
		reduce();
		if (!can_neg<T>(_numerator))
		{
			throw FractionOverflowError<T>("Fraction::operator-");
		}
	}
	return Fraction(-_numerator, _denominator, _flags);
}


template <Fraction_compatible T>
Fraction<T>& Fraction<T>::operator*= (const Fraction& other)
{
	if (can_mul<T>(_numerator, other.numerator())
		&& can_mul<T>(_denominator, other.denominator()))
	{
		_numerator *= other.numerator();
		_denominator *= other.denominator();
		_flags &= ~REDUCED;
		return *this;
	}
	if (!reduced())
	{
		reduce();
		if (can_mul<T>(_numerator, other.numerator())
			&& can_mul<T>(_denominator, other.denominator()))
		{
			_numerator *= other.numerator();
			_denominator *= other.denominator();
			_flags &= ~REDUCED;
			return *this;
		}
	}
	if (!other.reduced())
	{
		other.reduce();
		if (can_mul<T>(_numerator, other.numerator())
			&& can_mul<T>(_denominator, other.denominator()))
		{
			_numerator *= other.numerator();
			_denominator *= other.denominator();
			_flags &= ~REDUCED;
			return *this;
		}
	}
	T other_numerator = other.numerator();
	T other_denominator = other.denominator();
	T _gcd = gcd<T>(_numerator, other_denominator);
	_numerator /= _gcd;
	other_denominator /= _gcd;
	if (can_mul<T>(_numerator, other_numerator)
		&& can_mul<T>(_denominator, other_denominator))
	{
		_numerator *= other_numerator;
		_denominator *= other_denominator;
		_flags &= ~REDUCED;
		return *this;
	}
	_gcd = gcd<T>(_denominator, other_numerator);
	_denominator /= _gcd;
	other_numerator /= _gcd;
	if (can_mul<T>(_numerator, other_numerator)
		&& can_mul<T>(_denominator, other_denominator))
	{
		_numerator *= other_numerator;
		_denominator *= other_denominator;
		_flags |= REDUCED;
		return *this;
	}
	throw FractionOverflowError<T>("Fraction::operator*");
}

template <Fraction_compatible T>
Fraction<T> Fraction<T>::operator* (const Fraction& other) const
{
	return Fraction(*this) *= other;
}


template <Fraction_compatible T>
Fraction<T>& Fraction<T>::operator/= (const Fraction& other)
{
	return (*this) *= other.inverted();
}

template <Fraction_compatible T>
Fraction<T> Fraction<T>::operator/ (const Fraction& other) const
{
	return Fraction(*this) /= other;
}


template <Fraction_compatible T>
Fraction<T>& Fraction<T>::operator%= (const Fraction& other)
{
	T common_denom = common_denominator(*this, other);
	_numerator = (_numerator * (common_denom / _denominator)) % (other.numerator() * (common_denom / other.denominator()));
	_denominator = common_denom;
	_flags &= ~REDUCED;
	return *this;
}

template <Fraction_compatible T>
Fraction<T> Fraction<T>::operator% (const Fraction& other) const
{
	return Fraction(*this) %= other;
}


template <Fraction_compatible T>
Fraction<T>& Fraction<T>::operator++ ()
{
	if (!can_add<T>(_numerator, _denominator))
	{
		reduce();
		if (!can_add<T>(_numerator, _denominator))
		{
			throw FractionOverflowError<T>("Fraction::operator++");
		}
	}
	_numerator += _denominator;
	return *this;
}

template <Fraction_compatible T>
Fraction<T> Fraction<T>::operator++ (int)
{
	Fraction copy(*this);
	++(*this);
	return copy;
}


template <Fraction_compatible T>
Fraction<T>& Fraction<T>::operator-- ()
{
	if (!can_sub<T>(_numerator, _denominator))
	{
		reduce();
		if (!can_sub<T>(_numerator, _denominator))
		{
			throw FractionOverflowError<T>("Fraction::operator--");
		}
	}
	_numerator -= _denominator;
	return *this;
}

template <Fraction_compatible T>
Fraction<T> Fraction<T>::operator-- (int)
{
	Fraction copy(*this);
	--(*this);
	return copy;
}



template <Fraction_compatible T>
Fraction<T>& Fraction<T>::reduce ()
{
	if (!reduced())
	{
		T _gcd = gcd<T>(_numerator, _denominator);
		_numerator /= _gcd;
		_denominator /= _gcd;
		_flags |= REDUCED;
	}
	return *this;
}

template <Fraction_compatible T>
const Fraction<T>& Fraction<T>::reduce () const
{
	if (!reduced())
	{
		T _gcd = gcd<T>(_numerator, _denominator);
		_numerator /= _gcd;
		_denominator /= _gcd;
		_flags |= REDUCED;
	}
	return *this;
}

template <Fraction_compatible T>
bool Fraction<T>::reduced () const
{
	return _flags & REDUCED;
}


template <Fraction_compatible T>
Fraction<T>& Fraction<T>::invert ()
{
	T tmp = _numerator;
	_numerator = _denominator;
	_denominator = tmp;
	if (_denominator < T(0))
	{
		_numerator = -_numerator;
		_denominator = -_denominator;
	}
	else if (_denominator == T(0))
	{
		throw FractionDenominatorIsZeroError<T>("Fraction::invert");
	}

	return (*this);
}

template <Fraction_compatible T>
Fraction<T> Fraction<T>::inverted () const
{
	return Fraction(*this).invert();
}



template <Fraction_compatible T>
bool Fraction<T>::operator== (const Fraction& other) const
{
	return (*this) <= other && (*this) >= other;
}

template <Fraction_compatible T>
bool Fraction<T>::operator!= (const Fraction& other) const
{
	return !((*this) == other);
}


template <Fraction_compatible T>
bool Fraction<T>::operator< (const Fraction& other) const
{
	T common_denom = common_denominator(*this, other);
	return (_numerator * (common_denom / _denominator)) < (other.numerator() * (common_denom / other.denominator()));
}

template <Fraction_compatible T>
bool Fraction<T>::operator> (const Fraction& other) const
{
	return other < (*this);
}

template <Fraction_compatible T>
bool Fraction<T>::operator<= (const Fraction& other) const
{
	return !((*this) > other);
}

template <Fraction_compatible T>
bool Fraction<T>::operator>= (const Fraction& other) const
{
	return !((*this) < other);
}



template <Fraction_compatible T>
T Fraction<T>::value () const
{
	return _numerator / _denominator;
}


template <Fraction_compatible T>
T Fraction<T>::numerator () const
{
	return _numerator;
}

template <Fraction_compatible T>
void Fraction<T>::numerator (const T n)
{
	_numerator = n;
	_flags &= ~REDUCED;
}


template <Fraction_compatible T>
T Fraction<T>::denominator () const
{
	return _denominator;
}

template <Fraction_compatible T>
void Fraction<T>::denominator (const T d)
{
	_denominator = d;
	_flags &= ~REDUCED;
	if (_denominator < 0)
	{
		_numerator = -_numerator;
		_denominator = -_denominator;
	}
	else if (_denominator == 0)
	{
		throw FractionDenominatorIsZeroError<T>("Fraction::denominator");
	}
}


template <Fraction_compatible T>
void Fraction<T>::swap (Fraction<T>& other)
{
	const Fraction<T> other_copy(other);
	other = *this;
	*this = other_copy;
}


template <Fraction_compatible T>
std::size_t Fraction<T>::hash () const requires Hashable<T>
{
	reduce();
	return (7 * std::hash<T>()(_numerator)) + (((((size_t) 257) << 32) + 1023) * std::hash<T>()(_denominator));
}

}
