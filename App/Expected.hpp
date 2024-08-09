# pragma once

# include <variant>

template <typename E, typename T>
using Expected = std::variant<E, T>;

struct Error {
	;
	template <typename E, typename T>
	static constexpr auto& getError(const Expected<E, T>& exp) {
		return std::get<0>(exp);
	}

	template <typename E, typename T>
	static constexpr auto& getValue(const Expected<E, T>& exp) {
		return std::get<1>(exp);
	}

	template <typename E, typename T>
	static constexpr bool isError(const Expected<E, T>& e) {
		return e.index() == 0;
	}

	template <typename E, typename T>
	static constexpr bool isValue(const Expected<E, T>& e) {
		return e.index() == 1;
	}

};