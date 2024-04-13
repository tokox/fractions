#ifndef TOKOX_FRACTIONS
#define TOKOX_FRACTIONS

#include <cstddef>
#include <limits>
#include <functional>
#include <concepts>
#include <stdexcept>

#include "numeric_helper_functions.hpp"

namespace tokox
{

template <typename T>
concept Fraction_compatible = requires (T a, T b)
{
	requires std::numeric_limits<T>::is_specialized;
	requires std::numeric_limits<T>::is_signed;
	requires std::numeric_limits<T>::is_exact;
	requires std::numeric_limits<T>::is_integer;
	requires std::copy_constructible<T>;
	requires std::constructible_from<T, int>;
	requires std::assignable_from<T&, T>;
	requires std::totally_ordered<T>;
	requires tokox::can_checkable<T>;
{ -a } -> std::convertible_to<T>;
{ a + b } -> std::convertible_to<T>;
{ a += b } -> std::convertible_to<T>;
{ a - b } -> std::convertible_to<T>;
{ a -= b } -> std::convertible_to<T>;
{ a* b } -> std::convertible_to<T>;
{ a *= b } -> std::convertible_to<T>;
{ a / b } -> std::convertible_to<T>;
{ a /= b } -> std::convertible_to<T>;
{ a% b } -> std::convertible_to<T>;
{ a %= b } -> std::convertible_to<T>;
{ gcd<T>(a, b) } -> std::convertible_to<T>;
{ lcm<T>(a, b) } -> std::convertible_to<T>;
};

template <typename T>
concept Hashable = requires (T & t)
{
	{ std::hash<T>()(t) } -> std::convertible_to<std::size_t>;
};

template<typename T = void>
class FractionDenominatorIsZeroError : public std::domain_error
{
public:
	FractionDenominatorIsZeroError (const std::string& what):
		std::domain_error("denominator is zero in tokox::Fraction"
			+ (typeid(T) == typeid(void) ? "" : "<" + get_typename<T>() + ">")
			+ " in " + what)
	{}
};

template<typename T = void>
class FractionOverflowError : public std::overflow_error
{
public:
	FractionOverflowError (const std::string& what):
		std::overflow_error("overflow in tokox::Fraction"
			+ (typeid(T) == typeid(void) ? "" : "<" + get_typename<T>() + ">")
			+ " in " + what)
	{}
};

template <Fraction_compatible T = int>
class Fraction
{
public:
	Fraction (const T n = T(0), const T d = T(1));

	Fraction (const Fraction& other);


	Fraction& operator= (const Fraction& other);

	Fraction& operator+= (const Fraction& other);
	Fraction operator+ (const Fraction& other) const;
	Fraction operator+ () const;

	Fraction& operator-= (const Fraction& other);
	Fraction operator- (const Fraction& other) const;
	Fraction operator- () const;

	Fraction& operator*= (const Fraction& other);
	Fraction operator* (const Fraction& other) const;

	Fraction& operator/= (const Fraction& other);
	Fraction operator/ (const Fraction& other) const;

	Fraction& operator%= (const Fraction& other);
	Fraction operator% (const Fraction& other) const;

	Fraction& operator++ ();
	Fraction operator++ (int);

	Fraction& operator-- ();
	Fraction operator-- (int);


	Fraction& reduce ();
	const Fraction& reduce () const;
	bool reduced () const;

	Fraction& invert ();
	Fraction inverted () const;


	bool operator== (const Fraction& other) const;
	bool operator!= (const Fraction& other) const;

	bool operator> (const Fraction& other) const;
	bool operator>= (const Fraction& other) const;

	bool operator< (const Fraction& other) const;
	bool operator<= (const Fraction& other) const;


	T value () const;

	T numerator () const;
	void numerator (const T n);

	T denominator () const;
	void denominator (const T d);


	void swap (Fraction& other);

	std::size_t hash() const requires Hashable<T>;

private:
	mutable T _numerator;
	mutable T _denominator;
};

template class Fraction<int>;

}

#include "fractions.cpp"

#endif
