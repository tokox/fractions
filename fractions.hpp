#ifndef TOKOX_FRACTIONS
#define TOKOX_FRACTIONS

#include <cstddef>
#include <limits>
#include <functional>
#include <numeric>
#include <concepts>

#include "numeric_helper_functions.hpp"

namespace tokox
{

template<typename T>
concept FractionCompatible =
	std::numeric_limits<T>::is_specialized &&
	std::numeric_limits<T>::is_signed &&
	std::numeric_limits<T>::is_integer &&
	std::numeric_limits<T>::is_exact &&
	std::copy_constructible<T> &&
	std::constructible_from<T, int> &&
	std::assignable_from<T&, T> &&
	std::totally_ordered<T> &&
	tokox::can_checkable<T> &&
	requires(T a, T b) {
		{ -a } -> std::convertible_to<T>;
		{ a + b } -> std::convertible_to<T>;
		{ a += b } -> std::convertible_to<T>;
		{ a - b } -> std::convertible_to<T>;
		{ a -= b } -> std::convertible_to<T>;
		{ a * b } -> std::convertible_to<T>;
		{ a *= b } -> std::convertible_to<T>;
		{ a / b } -> std::convertible_to<T>;
		{ a /= b } -> std::convertible_to<T>;
		{ a % b } -> std::convertible_to<T>;
		{ a %= b } -> std::convertible_to<T>;
		{ std::gcd<T, T>(a, b) } -> std::convertible_to<T>;
		{ std::lcm<T, T>(a, b) } -> std::convertible_to<T>;
	};

template<typename T>
concept Hashable = requires(T& t) {
	{ std::hash<T>()(t) } -> std::convertible_to<std::size_t>;
};

template<FractionCompatible T = int>
class Fraction
{
	public:
		Fraction (const T n = T(0), const T d = T(1));
		
		Fraction (const Fraction& other);


		Fraction& operator= (const Fraction& other);

		Fraction& operator+= (const Fraction& other);
		Fraction operator+ (const Fraction& other) const;

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


		Fraction& swap (Fraction& other);


		std::size_t hash() const requires Hashable<T>;

	private:
		mutable T _numerator;
		mutable T _denominator;
};

template class Fraction<int>;

}

#include "fractions.cpp"

#endif
