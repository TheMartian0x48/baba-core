#ifndef BABA_ALIAS_ALIAS
#define BABA_ALIAS_ALIAS

#include <concepts>

namespace baba::alias
{

// unsigned integer number
using u8 = uint8_t;
using u16 = uint64_t;
using u32 = uint32_t;
using u64 = uint64_t;
// signed integer number
using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
// floating point number
using f32 = float;
using f64 = double;

template <typename T>
concept UnsignedInteger8 = std::same_as<T, u8>;

template <typename T>
concept UnsignedInteger16 = std::same_as<T, u16>;

template <typename T>
concept UnsignedInteger32 = std::same_as<T, u32>;

template <typename T>
concept UnsignedInteger64 = std::same_as<T, u64>;

template <typename T>
concept SignedInteger8 = std::same_as<T, i8>;

template <typename T>
concept SignedInteger16 = std::same_as<T, i16>;

template <typename T>
concept SignedInteger32 = std::same_as<T, i32>;

template <typename T>
concept SignedInteger64 = std::same_as<T, i64>;

template <typename T>
concept SignedInteger =
    SignedInteger8<T> || SignedInteger16<T> || SignedInteger32<T> || SignedInteger64<T>;

template <typename T>
concept UnsignedInteger =
    UnsignedInteger8<T> || UnsignedInteger16<T> || UnsignedInteger32<T> || UnsignedInteger64<T>;

template <typename T>
concept Integer = SignedInteger<T> || UnsignedInteger<T>;

// fractional number

template <typename T>
concept Float32 = std::same_as<T, f32>;

template <typename T>
concept Float64 = std::same_as<T, f64>;

template <typename T>
concept Fraction = Float32<T> || Float64<T>;

// real number
template <typename T>
concept RealNumber = Fraction<T> || Integer<T>;

template <typename T>
concept PrimitiveComparable = std::is_arithmetic_v<T> || std::is_pointer_v<T>;

template <typename Fn, typename T>
concept CompareFn = requires(Fn fn, const T* a, const T* b) {
    { fn(a, b) } -> std::convertible_to<int>;
};

template <typename Fn, typename T>
concept EqualFn = requires(Fn fn, const T* a, const T* b) {
    { fn(a, b) } -> std::convertible_to<bool>;
};

template <typename Fn, typename T>
concept LessThanFn = requires(Fn fn, const T* a, const T* b) {
    { fn(a, b) } -> std::convertible_to<bool>;
};

template <typename Fn, typename T>
concept GreaterThanFn = requires(Fn fn, const T* a, const T* b) {
    { fn(a, b) } -> std::convertible_to<bool>;
};

} // namespace baba::alias
#endif
