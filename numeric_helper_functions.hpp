#ifndef TOKOX_FRACTIONS_NUMERIC_HELPER_FUNCTIONS
#define TOKOX_FRACTIONS_NUMERIC_HELPER_FUNCTIONS

#include <limits>
#include <type_traits>
#include <concepts>

namespace tokox
{

template<typename T>
concept can_checkable = 
	std::numeric_limits<T>::is_specialized &&
	std::numeric_limits<T>::is_integer &&
	std::numeric_limits<T>::is_exact &&
	std::numeric_limits<T>::is_signed &&
	std::totally_ordered<T> &&
	std::constructible_from<T, int> &&
	requires(const T& a, const T& b) {
		{ a + b } -> std::convertible_to<T>;
		{ a - b } -> std::convertible_to<T>;
		{ a * b } -> std::convertible_to<T>;
		{ a / b } -> std::convertible_to<T>;
		{ -a } -> std::convertible_to<T>;
	};

template<typename T>
requires can_checkable<T>
bool can_add (const T& a, const T& b)
{
	if constexpr (!std::numeric_limits<T>::is_bounded) {
		return true;
	} else {
		if((a < T(0)) ^ (b < T(0))) {
			return true;
		} else if(a < T(0)) {
			return std::numeric_limits<T>::lowest()-a <= b;
		} else {
			return std::numeric_limits<T>::max()-a >= b;
		}
	}
}

template<std::integral T>
requires can_checkable<T>
bool can_add (const T& a, const T& b)
{
	return !__builtin_add_overflow_p(a, b, T(0));
}


template<typename T>
bool can_sub (const T& a, const T& b)
{
	if constexpr (!std::numeric_limits<T>::is_bounded) {
		return true;
	} else {
		if(b < 0) {
			return std::numeric_limits<T>::max()+b >= a;
		} else {
			return std::numeric_limits<T>::lowest()+b <= a;
		}
	}
}

template<std::integral T>
requires can_checkable<T>
bool can_sub (const T& a, const T& b)
{
	return !__builtin_sub_overflow_p(a, b, T(0));
}


template<typename T>
requires can_checkable<T>
bool can_neg (const T& a)
{
	return can_sub<T>(T(0), a);
}


template<typename T>
bool can_mul (const T& a, const T& b)
{
	if constexpr (!std::numeric_limits<T>::is_bounded) {
		return true;
	} else {
		if(a < T(0)) {
			if(b < T(0)) {
				if(!can_neg<T>(a) || !can_neg<T>(b)) {
					return false;
				}
				if constexpr (std::numeric_limits<T>::round_style == std::round_indeterminate
					|| std::numeric_limits<T>::round_style == std::round_to_nearest
					|| std::numeric_limits<T>::round_style == std::round_toward_infinity) {
					return std::numeric_limits<T>::max()/-a > -b;
				} else {
					return std::numeric_limits<T>::max()/-a >= -b;
				}
			} else {
				if constexpr (std::numeric_limits<T>::round_style == std::round_indeterminate
					|| std::numeric_limits<T>::round_style == std::round_to_nearest
					|| std::numeric_limits<T>::round_style == std::round_toward_neg_infinity) {
					return std::numeric_limits<T>::lowest()/b < a;
				} else {
					return std::numeric_limits<T>::lowest()/b <= a;
				}
			}
		} else {
			if(b < T(0)) {
				if constexpr (std::numeric_limits<T>::round_style == std::round_indeterminate
					|| std::numeric_limits<T>::round_style == std::round_to_nearest
					|| std::numeric_limits<T>::round_style == std::round_toward_neg_infinity) {
					return std::numeric_limits<T>::lowest()/a < b;
				} else {
					return std::numeric_limits<T>::lowest()/a <= b;
				}
			} else {
				if constexpr (std::numeric_limits<T>::round_style == std::round_indeterminate
					|| std::numeric_limits<T>::round_style == std::round_to_nearest
					|| std::numeric_limits<T>::round_style == std::round_toward_infinity) {
					return std::numeric_limits<T>::max()/a > b;
				} else {
					return std::numeric_limits<T>::max()/a >= b;
				}
			}
		}
	}
}

template<std::integral T>
requires can_checkable<T>
bool can_mul (const T& a, const T& b)
{
	return !__builtin_mul_overflow_p(a, b, T(0));
}

}

#endif
