#ifndef BABA_DS_PAIR
#define BABA_DS_PAIR

/**
 * @file pair.hpp
 * @brief Simple pair data structure with multiple accessor patterns
 */

namespace baba::ds
{
    /**
      @brief Simple pair holding two values with context-specific accessors
      
      ```cpp
      Pair<int, int> p{10, 20};
      int sum = p.first + p.second;        // Direct access (fastest)
      float x = p.x(), y = p.y();          // Coordinate context
      auto k = p.key(), v = p.value();     // Key-value context
      ```
      
      @tparam A Type of first element
      @tparam B Type of second element
      @note Accessor functions return copies. Use direct access for large types.
     */
    template <typename A, typename B> struct Pair {
        A first;  ///< First element
        B second; ///< Second element

        /// @brief Get first element (spatial context)
        /// @return Copy of first element
        constexpr inline A left() const noexcept { return first; }

        /// @brief Get second element (spatial context)
        /// @return Copy of second element
        constexpr inline B right() const noexcept { return second; }

        /// @brief Get first element (associative context)
        /// @return Copy of first element
        constexpr inline A key() const noexcept { return first; }

        /// @brief Get second element (associative context)
        /// @return Copy of second element
        constexpr inline B value() const noexcept { return second; }

        /// @brief Get first element (coordinate context)
        /// @return Copy of first element
        constexpr inline A x() const noexcept { return first; }

        /// @brief Get second element (coordinate context)
        /// @return Copy of second element
        constexpr inline B y() const noexcept { return second; }
    };
} // namespace baba::ds
#endif
