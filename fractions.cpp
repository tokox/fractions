#include <stdexcept>
#include <numeric>
#include <utility>
#include <functional>

#include "fractions.hpp"

namespace tokox
{

template<typename T>
Fraction<T>::Fraction (const T n, const T d):
	_numerator(n),
	_denominator(d)
{
	if(_denominator < 0) {
		_numerator = -_numerator;
		_denominator = -_denominator;
	} else if(_denominator == 0) {
		throw std::runtime_error("Denominator equals Zero (0) in tokox::Fraction");
	}
}


template<typename T>
Fraction<T>::Fraction (const Fraction& other):
	_numerator(other.numerator()),
	_denominator(other.denominator())
{}



template<typename T>
Fraction<T>& Fraction<T>::operator= (const Fraction& other)
{
	_numerator = other.numerator();
	_denominator = other.denominator();
	return *this;
}


template<typename T>
Fraction<T>& Fraction<T>::operator+= (const Fraction& other)
{
	_numerator = _numerator*other.denominator() + other.numerator()*_denominator;
	_denominator *= other.denominator();
	return *this;
}

template<typename T>
Fraction<T> Fraction<T>::operator+ (const Fraction& other) const
{
	return Fraction(*this) += other;
}


template<typename T>
Fraction<T>& Fraction<T>::operator-= (const Fraction& other)
{
	return (*this) += -other;
}

template<typename T>
Fraction<T> Fraction<T>::operator- (const Fraction& other) const
{
	return Fraction(*this) -= other;
}

template<typename T>
Fraction<T> Fraction<T>::operator- () const
{
	return Fraction(-_numerator, _denominator);
}


template<typename T>
Fraction<T>& Fraction<T>::operator*= (const Fraction& other)
{
	_numerator *= other.numerator();
	_denominator *= other.denominator();
	return *this;
}

template<typename T>
Fraction<T> Fraction<T>::operator* (const Fraction& other) const
{
	return Fraction(*this) *= other;
}


template<typename T>
Fraction<T>& Fraction<T>::operator/= (const Fraction& other)
{
	return (*this) *= other.inverted();
}

template<typename T>
Fraction<T> Fraction<T>::operator/ (const Fraction& other) const
{
	return Fraction(*this) /= other;
}


template<typename T>
Fraction<T>& Fraction<T>::operator%= (const Fraction& other)
{
	if(other.numerator() == 0) {
		throw std::runtime_error("Modulo (%) by Zero (0) in tokox::Fraction");
	}
	_numerator = (_numerator*other.denominator())%(other.numerator()*_denominator);
	_denominator *= other.denominator();
	return *this;
}

template<typename T>
Fraction<T> Fraction<T>::operator% (const Fraction& other) const
{
	return Fraction(*this) %= other;
}


template<typename T>
Fraction<T>& Fraction<T>::operator++ ()
{
	_numerator += _denominator;
	return *this;
}

template<typename T>
Fraction<T> Fraction<T>::operator++ (int)
{
	Fraction copy(*this);
	++(*this);
	return copy;
}


template<typename T>
Fraction<T>& Fraction<T>::operator-- ()
{
	_numerator -= _denominator;
	return *this;
}

template<typename T>
Fraction<T> Fraction<T>::operator-- (int)
{
	Fraction copy(*this);
	--(*this);
	return copy;
}



template<typename T>
Fraction<T>& Fraction<T>::reduce ()
{
	const T gcd = std::gcd(_numerator, _denominator);
	_numerator /= gcd;
	_denominator /= gcd;
	return *this;
}

template<typename T>
Fraction<T> Fraction<T>::reduced () const
{
	return Fraction(*this).reduce();
}


template<typename T>
Fraction<T>& Fraction<T>::invert ()
{
	using std::swap;
	swap(_numerator, _denominator);
	return (*this);
}

template<typename T>
Fraction<T> Fraction<T>::inverted () const
{
	return Fraction(*this).invert();
}



template<typename T>
bool Fraction<T>::operator== (const Fraction& other) const
{
	return (*this) <= other && (*this) >= other;
}

template<typename T>
bool Fraction<T>::operator!= (const Fraction& other) const
{
	return !((*this) == other);
}


template<typename T>
bool Fraction<T>::operator< (const Fraction& other) const
{
	return (_numerator*other.denominator() < other.numerator()*_denominator);
}

template<typename T>
bool Fraction<T>::operator> (const Fraction& other) const
{
	return other < (*this);
}

template<typename T>
bool Fraction<T>::operator<= (const Fraction& other) const
{
	return !((*this) > other);
}

template<typename T>
bool Fraction<T>::operator>= (const Fraction& other) const
{
	return !((*this) < other);
}




template<typename T>
Fraction<T>::operator T () const
{
	return _numerator/_denominator;
}


template<typename T>
T Fraction<T>::numerator () const
{
	return _numerator;
}

template<typename T>
void Fraction<T>::numerator (const T n)
{
	_numerator = n;
}


template<typename T>
T Fraction<T>::denominator () const
{
	return _denominator;
}

template<typename T>
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


template<typename T>
Fraction<T>& Fraction<T>::swap (Fraction<T>& other)
{
	_numerator = other.numerator(_numerator);
	_denominator = other.denominator(_denominator);
}


template <typename T>
std::size_t Fraction<T>::hash ()
{
	reduce();
	return (7*std::hash<T>()(_numerator))+(257*std::hash<T>()(_denominator));
}

template<typename T>
std::size_t Fraction<T>::hash () const
{
	return reduced().hash();
}

}