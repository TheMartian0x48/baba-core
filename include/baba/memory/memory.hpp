#ifndef BABA_MEMORY
#define BABA_MEMORY
/**
 * @file memory.hpp
 * @brief Complete memory management system including aligned allocation and arena allocators
 */

#include <cstddef>
#include <cstring>
#include <new>

namespace baba::memory
{

    // ============================================================================
    // LOW-LEVEL ALIGNED MEMORY UTILITIES
    // ============================================================================

    /**
 * @brief Allocate aligned memory
 * @param size Size in bytes to allocate
 * @param alignment Alignment requirement (must be power of 2)
 * @return Pointer to aligned memory or nullptr on failure
 */
    void* aligned_alloc(size_t size, size_t alignment = 16);

    /**
 * @brief Free aligned memory allocated with aligned_alloc
 * @param ptr Pointer to free (can be nullptr)
 */
    void aligned_free(void* ptr);

    /**
 * @brief Check if a pointer is aligned to the specified boundary
 * @param ptr Pointer to check
 * @param alignment Alignment boundary
 * @return true if aligned, false otherwise
 */
    bool is_aligned(const void* ptr, size_t alignment);

    /**
 * @brief Aligned allocator for STL containers
 * @tparam T Type to allocate
 * @tparam Alignment Alignment requirement (default 16 bytes)
 */
    template <typename T, size_t Alignment = 16> class aligned_allocator
    {
      public:
        using value_type      = T;
        using pointer         = T*;
        using const_pointer   = const T*;
        using reference       = T&;
        using const_reference = const T&;
        using size_type       = size_t;
        using difference_type = ptrdiff_t;

        template <typename U> struct rebind {
            using other = aligned_allocator<U, Alignment>;
        };

        aligned_allocator() noexcept = default;

        template <typename U> aligned_allocator(const aligned_allocator<U, Alignment>&) noexcept {}

        pointer allocate(size_type n)
        {
            if (n == 0)
                return nullptr;

            void* ptr = aligned_alloc(n * sizeof(T), Alignment);
            if (!ptr) {
                throw std::bad_alloc();
            }
            return static_cast<pointer>(ptr);
        }

        void                       deallocate(pointer p, size_type) noexcept { aligned_free(p); }

        template <typename U> bool operator==(const aligned_allocator<U, Alignment>&) const noexcept
        {
            return true;
        }

        template <typename U> bool operator!=(const aligned_allocator<U, Alignment>&) const noexcept
        {
            return false;
        }
    };

    // ============================================================================
    // ARENA ALLOCATION SYSTEM
    // ============================================================================

    // Arena function pointer types
    using ArenaInitFn         = void* (*)(std::size_t capacity);
    using ArenaAllocFn        = void* (*)(void* arena_instance, std::size_t size);
    using ArenaResetFn        = void (*)(void* arena_instance);
    using ArenaDestroyFn      = void (*)(void* arena_instance);
    using ArenaFreeFn         = void (*)(void* arena_instance, void* ptr);
    using ArenaReallocFn      = void* (*)(void* arena_instance, void* ptr, std::size_t new_size);
    using ArenaAlignedAllocFn = void* (*)(void* arena_instance,
        std::size_t                             size,
        std::size_t                             alignment);
    using ArenaGetStatsFn     = void (*)(void* arena_instance, struct ArenaStats* stats);
    using ArenaCanAllocFn     = bool (*)(void* arena_instance, std::size_t size);

    struct ArenaStats {
        std::size_t total_capacity;
        std::size_t used_bytes;
        std::size_t free_bytes;
        std::size_t allocation_count;
        std::size_t free_count;
        std::size_t fragmentation_ratio; // Percentage 0-100
        std::size_t largest_free_block;
    };

    namespace ArenaConstant
    {
        inline const char* BUDDY_ARENA_TYPE        = "BUDDY";
        inline const char* FREE_LIST_ARENA_TYPE    = "FREE";
        inline const char* HYBRID_ARENA_TYPE       = "HYBRID";
        inline const char* LINEAR_ARENA_TYPE       = "LINEAR";
        inline const char* POOL_ARENA_TYPE         = "POOL";
        inline const char* PROXY_ARENA_TYPE        = "PROXY";
        inline const char* RING_BUFFER_ARENA_TYPE  = "RING";
        inline const char* SLAB_ARENA_TYPE         = "SLAB";
        inline const char* STACK_ARENA_TYPE        = "STACK";
        inline const char* UNKNOWN_ARENA_TYPE      = "UNKNOWN";
        constexpr uint32_t STACK_ALLOC_START_MAGIC = 0xDEADBEEF;
        constexpr uint32_t STACK_ALLOC_END_MAGIC   = 0xCAFEBABE;
        constexpr uint32_t FREED_MAGIC             = 0xDEADDEAD;
    } // namespace ArenaConstant

    enum class ArenaType {
        BUDDY,
        FREE_LIST,
        HYBRID,
        LINEAR,
        POOL,
        PROXY,
        RING_BUFFER,
        SLAB,
        STACK,
    };

    struct Arena {
        ArenaInitFn         init;          // Initialize arena instance
        ArenaAllocFn        alloc;         // Allocate memory from the arena
        ArenaReallocFn      realloc;       // Resize allocation
        ArenaResetFn        reset;         // Reset the arena to initial state
        ArenaFreeFn         free;          // Individual deallocation
        ArenaDestroyFn      destroy;       // Destroy the arena and free resources
        ArenaAlignedAllocFn aligned_alloc; // Aligned allocation
        ArenaGetStatsFn     get_stats;     // Memory usage statistics
        ArenaCanAllocFn     can_alloc;     // Check if allocation possible

        const char*         type_name;                // "linear", "stack", "pool", etc.
        std::size_t         min_alignment;            // Minimum alignment guarantee
        bool                supports_individual_free; // Can free individual allocations
        bool                supports_realloc;         // Can resize allocations
    };

    struct ArenaConfig {
        ArenaType   type;
        std::size_t capacity;
        std::size_t alignment;
        bool        enable_statistics;
        bool        enable_debugging;

        union {
            struct {
                std::size_t block_size;
            } pool_config;

            struct {
                std::size_t* size_classes;
                std::size_t  size_class_count;
            } slab_config;

            struct {
                std::size_t max_allocations;
            } stack_config;

            struct {
                std::size_t min_block_size;
            } buddy_config;
        };
    };

    // ============================================================================
    // ALLOCATOR STRUCTURES
    // ============================================================================

    struct FreeBlock {
        std::size_t size;
        FreeBlock*  next;
        FreeBlock*  prev; // For easier coalescing
    };

    struct FreeListArena {
        char*       buffer;
        std::size_t capacity;
        FreeBlock*  free_list_head;
        std::size_t total_allocations;
        std::size_t total_frees;
        std::size_t fragmentation_count;
        bool        enable_coalescing; // Merge adjacent free blocks
    };

    struct RingArena {
        char*       buffer;
        std::size_t capacity;
        std::size_t head;       // Current allocation position
        std::size_t tail;       // Oldest allocation position
        std::size_t wrap_count; // Number of times buffer wrapped
        bool        has_wrapped;
    };

    // Forward declaration
    struct PoolBlock;

    struct Slab {
        char*       buffer;
        std::size_t block_size;
        std::size_t block_count;
        PoolBlock*  free_list_head;
        std::size_t free_count;
        std::size_t total_allocations;
    };

    struct SlabArena {
        Slab*        slabs;
        std::size_t  slab_count;
        std::size_t* size_classes; // e.g., {16, 32, 64, 128, 256, 512, 1024}
        std::size_t  size_class_count;
        std::size_t  total_capacity;
        std::size_t  fallback_threshold; // Size above which to use fallback allocator
    };

    struct BuddyBlock {
        std::size_t size;
        bool        is_free;
        BuddyBlock* next;
        BuddyBlock* prev;
    };

    struct BuddyArena {
        char*        buffer;
        std::size_t  capacity;
        std::size_t  min_block_size; // Smallest allocatable block (e.g., 16 bytes)
        std::size_t  max_order;      // log2(capacity / min_block_size)
        BuddyBlock** free_lists;     // Array of free lists for each order
        std::size_t  total_allocations;
        std::size_t  total_frees;
    };

    // ============================================================================
    // PROXY ARENA
    // ============================================================================

    struct ProxyArena {
    };

    void* proxy_arena_init(std::size_t capacity);
    void* proxy_arena_alloc(void* arena_instance, std::size_t size);
    void  proxy_arena_free(void* arena_instance, void* ptr);
    void  proxy_arena_get_stats(void* arena_instance, ArenaStats* stats);
    bool  proxy_arena_can_alloc(void* arena_instance, std::size_t size);
    void  proxy_arena_reset(void* arena_instance);
    void  proxy_arena_destroy(void* arena_instance);

    // ============================================================================
    // LINEAR ARENA
    // ============================================================================

    struct LinearArena {
        char*       buffer;
        std::size_t capacity;
        std::size_t offset;
        std::size_t total_allocations;
    };

    void* linear_arena_init(std::size_t capacity);
    void* linear_arena_alloc(void* arena_instance, std::size_t size);
    void* linear_arena_aligned_alloc(void* arena_instance, std::size_t size, std::size_t alignment);
    void  linear_arena_get_stats(void* arena_instance, ArenaStats* stats);
    bool  linear_arena_can_alloc(void* arena_instance, std::size_t size);
    void  linear_arena_reset(void* arena_instance);
    void  linear_arena_destroy(void* arena_instance);

    // ============================================================================
    // STACK ARENA INLINE IMPLEMENTATION
    // ============================================================================

    struct StackAllocationMetaData {
        std::size_t magic_start;
        std::size_t size;
        std::size_t offset; // For validation during free
        std::size_t magic_end;
    };

    struct StackArena {
        char*       buffer;
        std::size_t capacity;
        std::size_t offset;
        std::size_t allocation_count;
        std::size_t total_allocations; // Statistics
        std::size_t total_frees;
    };

    void* stack_arena_init(std::size_t capacity);
    void* stack_arena_alloc(void* arena_instance, std::size_t size);
    void  stack_arena_free(void* arena_instance, void* ptr);
    void  stack_arena_reset(void* arena_instance);
    void  stack_arena_destroy(void* arena_instance);
    void  stack_arena_get_stats(void* arena_instance, ArenaStats* stats);
    bool  stack_arena_can_alloc(void* arena_instance, std::size_t size);

    // ============================================================================
    // STACK ARENA IMPLEMENTATION
    // ============================================================================

    // struct StackArenaWithMetaData {
    //     char*                    buffer;
    //     std::size_t              capacity;
    //     std::size_t              offset;
    //     StackAllocationMetaData* allocation_stack; // Track allocations for LIFO free
    //     std::size_t              allocation_count;
    //     std::size_t              max_allocations;
    //     std::size_t              total_allocations; // Statistics
    //     std::size_t              total_frees;
    // };
    //
    // void* stack_arena_init(std::size_t capacity);
    // void* stack_arena_init_with_max_allocs(std::size_t capacity, std::size_t max_allocations);
    // inline void* stack_arena_init(std::size_t capacity);
    // void* stack_arena_init_with_max_allocs(std::size_t capacity, std::size_t max_allocations);
    // void* stack_arena_alloc(void* arena_instance, std::size_t size);
    // void  stack_arena_free(void* arena_instance, const void* ptr);
    // void  stack_arena_get_stats(void* arena_instance, ArenaStats* stats);
    // bool  stack_arena_can_alloc(void* arena_instance, std::size_t size);
    // void  stack_arena_reset(void* arena_instance);
    // void  stack_arena_destroy(void* arena_instance);

    // ============================================================================
    // POOL ARENA
    // ============================================================================

    struct PoolBlock {
        PoolBlock* next; // Pointer to next free block in the list
    };

    struct PoolArena {
        char*       buffer;
        std::size_t capacity;
        std::size_t block_size;
        std::size_t block_count;
        PoolBlock*  free_list_head;
        std::size_t free_count;
        std::size_t total_allocations;
        std::size_t total_frees;
    };

    /**
    * @brief Initialize a pool arena with given capacity and block size
    * @param capacity Total size of the arena in bytes
    * @param block_size Size of each block in the pool
    *
    * block_size must be at least sizeof(PoolBlock) to store the next pointer.
    * block_size == sizeof(PoolBlock) means no user data, just the block management.
    *
    * @return Pointer to the initialized arena instance, or nullptr on failure
    */
    void* pool_arena_init(std::size_t capacity, std::size_t block_size);
    /**
    * @brief Allocate a block from the pool arena
    * @param arena_instance Pointer to the arena instance
    * @param size Size of the block to allocate
    *
    * If size is greater than the block size, or if no blocks are available, returns nullptr.
    *
    * @return Pointer to the allocated block, or nullptr if allocation fails
    */
    void* pool_arena_alloc(void* arena_instance, std::size_t size);
    /**
    * @brief Allocate an aligned block from the pool arena
    * @param arena_instance Pointer to the arena instance
    * @param ptr Pointer to the block to free
    *
    * If ptr is nullptr or ptr is not within the arena's buffer, does nothing.
    *
    */
    void pool_arena_free(void* arena_instance, void* ptr);
    /**
    * @brief Get memory usage statistics for the pool arena_instance
    * @param arena_instance Pointer to the arena instance
    * @param stats Pointer to ArenaStats structure to fill
    */
    void pool_arena_get_stats(void* arena_instance, ArenaStats* stats);
    /**
    * @brief Check if the pool arena can allocate a block of given size
    * @param arena_instance Pointer to the arena instance
    * @param size Size of the block to check
    * @return true if allocation is possible, false otherwise
    */
    bool pool_arena_can_alloc(void* arena_instance, std::size_t size);
    /**
    * @brief Reset the pool arena, making all blocks available again
    * @param arena_instance Pointer to the arena instance
    *
    * This function clears the free list and resets the free count to the total block count.
    */
    void pool_arena_reset(void* arena_instance);
    /**
    * @brief Destroy the pool arena and free its resources
    * @param arena_instance Pointer to the arena instance
    * This function frees the buffer and the arena structure itself.
    */
    void pool_arena_destroy(void* arena_instance);

    // ============================================================================
    // ARENA FACTORY FUNCTIONS
    // ============================================================================

    const char* arena_type_to_string(ArenaType type);
    ArenaType   string_to_arena_type(const char* type_name);

    inline bool is_valid_arena_type(ArenaType type)
    {
        return type >= ArenaType::LINEAR && type <= ArenaType::HYBRID;
    }

    Arena create_linear_arena();
    Arena create_stack_arena();
    Arena create_pool_arena();
    Arena create_arena(ArenaType type);
    Arena create_arena_from_config(const ArenaConfig* config);
} // namespace baba::memory

#endif
