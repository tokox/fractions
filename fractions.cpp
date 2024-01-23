#include <stdexcept>

namespace tokox
{

template<FractionCompatible T>
Fraction<T>::Fraction (const T n, const T d):
	_numerator(n),
	_denominator(d)
{
	if(_denominator < T(0)) {
		_numerator = -_numerator;
		_denominator = -_denominator;
	} else if(_denominator == T(0)) {
		throw std::runtime_error("Denominator equals Zero (0) in tokox::Fraction");
	}
}


template<FractionCompatible T>
Fraction<T>::Fraction (const Fraction& other):
	_numerator(other.numerator()),
	_denominator(other.denominator())
{}



template<FractionCompatible T>
Fraction<T>& Fraction<T>::operator= (const Fraction& other)
{
	_numerator = other.numerator();
	_denominator = other.denominator();
	return *this;
}


template<FractionCompatible T>
Fraction<T>& Fraction<T>::operator+= (const Fraction& other)
{
	_numerator = _numerator*other.denominator() + other.numerator()*_denominator;
	_denominator *= other.denominator();
	return *this;
}

template<FractionCompatible T>
Fraction<T> Fraction<T>::operator+ (const Fraction& other) const
{
	return Fraction(*this) += other;
}


template<FractionCompatible T>
Fraction<T>& Fraction<T>::operator-= (const Fraction& other)
{
	return (*this) += -other;
}

template<FractionCompatible T>
Fraction<T> Fraction<T>::operator- (const Fraction& other) const
{
	return Fraction(*this) -= other;
}

template<FractionCompatible T>
Fraction<T> Fraction<T>::operator- () const
{
	return Fraction(-_numerator, _denominator);
}


template<FractionCompatible T>
Fraction<T>& Fraction<T>::operator*= (const Fraction& other)
{
	_numerator *= other.numerator();
	_denominator *= other.denominator();
	return *this;
}

template<FractionCompatible T>
Fraction<T> Fraction<T>::operator* (const Fraction& other) const
{
	return Fraction(*this) *= other;
}


template<FractionCompatible T>
Fraction<T>& Fraction<T>::operator/= (const Fraction& other)
{
	return (*this) *= other.inverted();
}

template<FractionCompatible T>
Fraction<T> Fraction<T>::operator/ (const Fraction& other) const
{
	return Fraction(*this) /= other;
}


template<FractionCompatible T>
Fraction<T>& Fraction<T>::operator%= (const Fraction& other)
{
	if(other.numerator() == T(0)) {
		throw std::runtime_error("Modulo (%) by Zero (0) in tokox::Fraction");
	}
	_numerator = (_numerator*other.denominator())%(other.numerator()*_denominator);
	_denominator *= other.denominator();
	return *this;
}

template<FractionCompatible T>
Fraction<T> Fraction<T>::operator% (const Fraction& other) const
{
	return Fraction(*this) %= other;
}


template<FractionCompatible T>
Fraction<T>& Fraction<T>::operator++ ()
{
	_numerator += _denominator;
	return *this;
}

template<FractionCompatible T>
Fraction<T> Fraction<T>::operator++ (int)
{
	Fraction copy(*this);
	++(*this);
	return copy;
}


template<FractionCompatible T>
Fraction<T>& Fraction<T>::operator-- ()
{
	_numerator -= _denominator;
	return *this;
}

template<FractionCompatible T>
Fraction<T> Fraction<T>::operator-- (int)
{
	Fraction copy(*this);
	--(*this);
	return copy;
}



template<FractionCompatible T>
Fraction<T>& Fraction<T>::reduce ()
{
	if(reduced()) {
		return *this;
	}
	T gcd = std::gcd(_numerator, _denominator);
	_numerator /= gcd;
	_denominator /= gcd;
	return *this;
}

template<FractionCompatible T>
const Fraction<T>& Fraction<T>::reduce () const
{
	if(reduced()) {
		return *this;
	}
	T gcd = std::gcd(_numerator, _denominator);
	_numerator /= gcd;
	_denominator /= gcd;
	return *this;
}

template<FractionCompatible T>
#ifdef __GNUG__
__attribute__(( optimize(0), /* warning is not emmited when optimization is on */
	warning("tokox::Fraction<T>::reduced() is not implemented yet! (always returns false)")
))
#else
#warning tokox::Fraction<T>::reduced() is not implemented yet! (always returns false)
#endif
bool Fraction<T>::reduced () const
{
	return false;
}


template<FractionCompatible T>
Fraction<T>& Fraction<T>::invert ()
{
	T tmp = _numerator;
	_numerator = _denominator;
	_denominator = tmp;
	return (*this);
}

template<FractionCompatible T>
Fraction<T> Fraction<T>::inverted () const
{
	return Fraction(*this).invert();
}



template<FractionCompatible T>
bool Fraction<T>::operator== (const Fraction& other) const
{
	return (*this) <= other && (*this) >= other;
}

template<FractionCompatible T>
bool Fraction<T>::operator!= (const Fraction& other) const
{
	return !((*this) == other);
}


template<FractionCompatible T>
bool Fraction<T>::operator< (const Fraction& other) const
{
	return (_numerator*other.denominator() < other.numerator()*_denominator);
}

template<FractionCompatible T>
bool Fraction<T>::operator> (const Fraction& other) const
{
	return other < (*this);
}

template<FractionCompatible T>
bool Fraction<T>::operator<= (const Fraction& other) const
{
	return !((*this) > other);
}

template<FractionCompatible T>
bool Fraction<T>::operator>= (const Fraction& other) const
{
	return !((*this) < other);
}



template<FractionCompatible T>
T Fraction<T>::value () const
{
	return _numerator/_denominator;
}


template<FractionCompatible T>
T Fraction<T>::numerator () const
{
	return _numerator;
}

template<FractionCompatible T>
void Fraction<T>::numerator (const T n)
{
	_numerator = n;
}


template<FractionCompatible T>
T Fraction<T>::denominator () const
{
	return _denominator;
}

template<FractionCompatible T>
void Fraction<T>::denominator (const T d)
{
	_denominator = d;
	if(_denominator < 0) {
		_numerator = -_numerator;
		_denominator = -_denominator;
	} else if(_denominator == 0) {
		throw std::runtime_error("Denominator equals Zero (0) in tokox::Fraction");
	}
}


template<FractionCompatible T>
Fraction<T>& Fraction<T>::swap (Fraction<T>& other)
{
	T other_numerator = other.numerator();
	other.numerator(_numerator);
	_numerator = other_numerator;
	T other_denominator = other.denominator();
	other.denominator(_denominator);
	_denominator = other_denominator;
	return *this;
}


template <FractionCompatible T>
std::size_t Fraction<T>::hash () const requires Hashable<T>
{
	reduce();
	return (7*std::hash<T>()(_numerator))+(257*std::hash<T>()(_denominator));
}

}
