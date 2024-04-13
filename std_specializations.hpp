#ifndef TOKOX_FRACTIONS_SPECIALIZATIONS
#define TOKOX_FRACTIONS_SPECIALIZATIONS

#include <functional>
#include <cstddef>
#include <limits>
#include <cmath>
#include <ostream>
#include <istream>
#include <stdexcept>

#include "fractions.hpp"

template <typename T>
void swap (tokox::Fraction<T>& one, tokox::Fraction<T>& two)
{
	one.swap(two);
}

template <typename T>
struct std::hash<tokox::Fraction<T>>
{
	std::size_t operator() (const tokox::Fraction<T>& f) const
	{
		return f.hash();
	}
};

template <typename T>
std::ostream& operator<< (std::ostream& o, const tokox::Fraction<T>& f)
{
	return o << f.numerator() << '/' << f.denominator();
}

namespace tokox
{

template<typename T = void>
class FractionInputError : public std::exception
{
public:
	FractionInputError(const std::string& where):
		message("wrong input for tokox::Fraction"
			+ (typeid(T) == typeid(void) ? "" : "<" + get_typename<T>() + ">")
			+ " in " + where)
	{}

	const char* what() const noexcept override
	{
		return message.c_str();
	}
private:
	std::string message;
};

}

template <typename T>
std::istream& operator>> (std::istream& i, tokox::Fraction<T>& f)
{
	T v;
	i >> v;
	if (i.fail())
	{
		throw tokox::FractionInputError<T>("std::istream::operator>>");
	}
	f.numerator(v);
	char c;
	i >> c;
	if (c != '/' || i.fail())
	{
		throw tokox::FractionInputError<T>("std::istream::operator>>");
	}
	i >> v;
	if (i.fail())
	{
		throw tokox::FractionInputError<T>("std::istream::operator>>");
	}
	f.denominator(v);
	return i;
}

template <typename T>
class std::numeric_limits<tokox::Fraction<T>>
{
public:
	static constexpr bool is_specialized = true;
	static constexpr bool is_signed = true;
	static constexpr bool is_integer = false;
	static constexpr bool is_exact = true;
	static constexpr bool has_infinity = false;
	static constexpr bool has_quiet_NaN = false;
	static constexpr bool has_signaling_NaN = false;
	static constexpr std::float_denorm_style has_denorm = std::denorm_absent;
	static constexpr bool has_denorm_loss = false;
	static constexpr std::float_round_style round_style = std::round_indeterminate;
	static constexpr bool is_iec559 = false;
	static constexpr bool is_bounded = std::numeric_limits<T>::is_bounded;
	static constexpr bool is_modulo = false;
	static constexpr int digits = std::numeric_limits<T>::digits * 2;
	static constexpr int digits10 = std::numeric_limits<T>::digits10 * 2;
	static constexpr int max_digits10 = std::numeric_limits<T>::max_digits10 * 2;
	static constexpr int radix = std::numeric_limits<T>::radix;
	static constexpr int min_exponent = is_bounded ? std::log2(std::numeric_limits<T>::max()) / std::log2(std::numeric_limits<T>::radix) + 1 : 0;
	static constexpr int min_exponent10 = is_bounded ? std::log10(std::numeric_limits<T>::max()) + 1 : 0;
	static constexpr int max_exponent = is_bounded ? std::log2(std::numeric_limits<T>::max()) / std::log2(std::numeric_limits<T>::radix) + 1 : 0;
	static constexpr int max_exponent10 = is_bounded ? std::log10(std::numeric_limits<T>::max()) + 1 : 0;
	static constexpr bool traps = true;
	static constexpr bool tineness_before = false;

	static constexpr tokox::Fraction<T> min () noexcept
	{
		if constexpr (is_bounded)
		{
			return tokox::Fraction<T>(1, std::numeric_limits<T>::max());
		}
		else
		{
			return tokox::Fraction<T>();
		}
	}
	static constexpr tokox::Fraction<T> lowest () noexcept
	{
		if constexpr (is_bounded)
		{
			return tokox::Fraction<T>(std::numeric_limits<T>::lowest(), std::numeric_limits<T>::max());
		}
		else
		{
			return tokox::Fraction<T>();
		}
	}
	static constexpr tokox::Fraction<T> max () noexcept
	{
		if constexpr (is_bounded)
		{
			return tokox::Fraction<T>(std::numeric_limits<T>::max(), 1);
		}
		else
		{
			return tokox::Fraction<T>();
		}
	}
	static constexpr tokox::Fraction<T> epsilon () noexcept
	{
		if constexpr (is_bounded)
		{
			return tokox::Fraction<T>(1, std::numeric_limits<T>::max() - 1);
		}
		else
		{
			return tokox::Fraction<T>();
		}
	}
	static constexpr tokox::Fraction<T> round_error () noexcept
	{
		return tokox::Fraction<T>(0);
	}
	static constexpr tokox::Fraction<T> infinity () noexcept
	{
		return tokox::Fraction<T>();
	}
	static constexpr tokox::Fraction<T> quiet_NaN () noexcept
	{
		return tokox::Fraction<T>();
	}
	static constexpr tokox::Fraction<T> signaling_NaN () noexcept
	{
		return tokox::Fraction<T>();
	}
	static constexpr tokox::Fraction<T> denorm_min () noexcept
	{
		return tokox::Fraction<T>();
	}
};

#endif
