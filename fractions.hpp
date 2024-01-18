#ifndef TOKOX_FRACTIONS
#define TOKOX_FRACTIONS

#include <cstddef>
#include <limits>

namespace tokox
{

template<typename T = int>
class Fraction
{
	static_assert(std::numeric_limits<T>::is_specialized,
		"std::numeric_limits have to be specialized for type T in tokox::Fraction<T>");

	static_assert(std::numeric_limits<T>::is_signed,
		"type T has to be signed in tokox::Fraction<T>");

	static_assert(std::numeric_limits<T>::is_integer,
		"type T has to be an integer in tokox::Fraction<T>");

	static_assert(std::numeric_limits<T>::is_exact,
		"type T has to be exact in tokox::Fraction<T>");

	public:
		Fraction (const T n = 0, const T d = 1);
		
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


		operator T () const;

		T numerator () const;
		void numerator (const T n);

		T denominator () const;
		void denominator (const T d);


		Fraction& swap (Fraction& other);


		std::size_t hash() const;

	private:
		mutable T _numerator;
		mutable T _denominator;
};

}

#include "fractions.cpp"

#endif