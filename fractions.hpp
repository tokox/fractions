#ifndef TOKOX_FRACTIONS
#define TOKOX_FRACTIONS

#include <cstddef>

namespace tokox
{

template<typename T = int>
class Fraction
{
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
		Fraction reduced () const;

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


		std::size_t hash();
		std::size_t hash() const;

	private:
		T _numerator;
		T _denominator;
};

}

#include "fractions.cpp"

#endif