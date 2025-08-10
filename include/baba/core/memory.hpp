#pragma once

/**
 * @file memory.hpp
 * @brief Basic aligned memory allocation utilities
 */

#include <cstddef>
#include <cstring>
#include <memory>

namespace baba::core
{

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

// Arena type enumeration
enum class ArenaType {
    LINEAR,
    STACK,
    POOL,
    FREE_LIST,
    RING_BUFFER,
    SLAB,
    BUDDY,
    HYBRID
};

struct Arena {
    ArenaInitFn    init;
    ArenaAllocFn   alloc;
    ArenaResetFn   reset;
    ArenaDestroyFn destroy;

    // set to nullptr if not supported
    ArenaFreeFn         free;          // Individual deallocation
    ArenaReallocFn      realloc;       // Resize allocation
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

struct LinearArena {
    char*       buffer;
    std::size_t capacity;
    std::size_t offset;
    std::size_t total_allocations; // Statistics
};

struct StackAllocation {
    std::size_t size;
    std::size_t offset; // For validation during free
};

struct StackArena {
    char*            buffer;
    std::size_t      capacity;
    std::size_t      offset;
    StackAllocation* allocation_stack; // Track allocations for LIFO free
    std::size_t      allocation_count;
    std::size_t      max_allocations;
    std::size_t      total_allocations; // Statistics
    std::size_t      total_frees;
};

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
// LINEAR ARENA IMPLEMENTATION
// ============================================================================

void* linear_arena_init(std::size_t capacity)
{
    LinearArena* arena = static_cast<LinearArena*>(std::malloc(sizeof(LinearArena)));
    if (arena == nullptr) {
        return nullptr;
    }
    arena->buffer = static_cast<char*>(std::malloc(capacity));
    if (arena->buffer == nullptr) {
        std::free(arena);
        return nullptr;
    }
    arena->capacity          = capacity;
    arena->offset            = 0;
    arena->total_allocations = 0;
    return arena;
}

void* linear_arena_alloc(void* arena_instance, std::size_t size)
{
    LinearArena* arena = static_cast<LinearArena*>(arena_instance);
    if (arena->offset + size > arena->capacity) {
        return nullptr;
    }
    void* ptr = static_cast<void*>(arena->buffer + arena->offset);
    arena->offset += size;
    arena->total_allocations++;
    return ptr;
}

void* linear_arena_aligned_alloc(void* arena_instance, std::size_t size, std::size_t alignment)
{
    LinearArena* arena = static_cast<LinearArena*>(arena_instance);

    // Calculate aligned offset
    std::size_t aligned_offset = (arena->offset + alignment - 1) & ~(alignment - 1);

    if (aligned_offset + size > arena->capacity) {
        return nullptr;
    }

    void* ptr     = static_cast<void*>(arena->buffer + aligned_offset);
    arena->offset = aligned_offset + size;
    arena->total_allocations++;
    return ptr;
}

void linear_arena_get_stats(void* arena_instance, ArenaStats* stats)
{
    LinearArena* arena         = static_cast<LinearArena*>(arena_instance);
    stats->total_capacity      = arena->capacity;
    stats->used_bytes          = arena->offset;
    stats->free_bytes          = arena->capacity - arena->offset;
    stats->allocation_count    = arena->total_allocations;
    stats->free_count          = 0; // Linear arena doesn't support individual free
    stats->fragmentation_ratio = 0; // No fragmentation in linear arena
    stats->largest_free_block  = arena->capacity - arena->offset;
}

bool linear_arena_can_alloc(void* arena_instance, std::size_t size)
{
    LinearArena* arena = static_cast<LinearArena*>(arena_instance);
    return arena->offset + size <= arena->capacity;
}

void linear_arena_reset(void* arena_instance)
{
    LinearArena* arena = static_cast<LinearArena*>(arena_instance);
    arena->offset      = 0;
}

void linear_arena_destroy(void* arena_instance)
{
    LinearArena* arena = static_cast<LinearArena*>(arena_instance);
    std::free(arena->buffer);
    std::free(arena);
}

// ============================================================================
// STACK ARENA IMPLEMENTATION
// ============================================================================

void* stack_arena_init(std::size_t capacity)
{
    return stack_arena_init_with_max_allocs(capacity, 1000); // Default max allocations
}

void* stack_arena_init_with_max_allocs(std::size_t capacity, std::size_t max_allocations)
{
    StackArena* arena = static_cast<StackArena*>(std::malloc(sizeof(StackArena)));
    if (arena == nullptr) {
        return nullptr;
    }

    arena->buffer = static_cast<char*>(std::malloc(capacity));
    if (arena->buffer == nullptr) {
        std::free(arena);
        return nullptr;
    }

    arena->allocation_stack =
        static_cast<StackAllocation*>(std::malloc(sizeof(StackAllocation) * max_allocations));
    if (arena->allocation_stack == nullptr) {
        std::free(arena->buffer);
        std::free(arena);
        return nullptr;
    }

    arena->capacity          = capacity;
    arena->offset            = 0;
    arena->allocation_count  = 0;
    arena->max_allocations   = max_allocations;
    arena->total_allocations = 0;
    arena->total_frees       = 0;
    return arena;
}

void* stack_arena_alloc(void* arena_instance, std::size_t size)
{
    StackArena* arena = static_cast<StackArena*>(arena_instance);

    if (arena->offset + size > arena->capacity ||
        arena->allocation_count >= arena->max_allocations) {
        return nullptr;
    }

    void* ptr = static_cast<void*>(arena->buffer + arena->offset);

    // Record allocation for LIFO free
    arena->allocation_stack[arena->allocation_count] = {size, arena->offset};
    arena->allocation_count++;
    arena->offset += size;
    arena->total_allocations++;

    return ptr;
}

void stack_arena_free(void* arena_instance, void* ptr)
{
    StackArena* arena = static_cast<StackArena*>(arena_instance);

    if (arena->allocation_count == 0 || ptr == nullptr) {
        return; // Nothing to free
    }

    // Check if this is the most recent allocation (LIFO)
    StackAllocation& last_alloc   = arena->allocation_stack[arena->allocation_count - 1];
    void*            expected_ptr = arena->buffer + last_alloc.offset;

    if (ptr != expected_ptr) {
        return; // Not LIFO order, cannot free
    }

    // Free the allocation
    arena->offset = last_alloc.offset;
    arena->allocation_count--;
    arena->total_frees++;
}

void stack_arena_get_stats(void* arena_instance, ArenaStats* stats)
{
    StackArena* arena          = static_cast<StackArena*>(arena_instance);
    stats->total_capacity      = arena->capacity;
    stats->used_bytes          = arena->offset;
    stats->free_bytes          = arena->capacity - arena->offset;
    stats->allocation_count    = arena->total_allocations;
    stats->free_count          = arena->total_frees;
    stats->fragmentation_ratio = 0; // No fragmentation in stack arena
    stats->largest_free_block  = arena->capacity - arena->offset;
}

bool stack_arena_can_alloc(void* arena_instance, std::size_t size)
{
    StackArena* arena = static_cast<StackArena*>(arena_instance);
    return arena->offset + size <= arena->capacity &&
           arena->allocation_count < arena->max_allocations;
}

void stack_arena_reset(void* arena_instance)
{
    StackArena* arena       = static_cast<StackArena*>(arena_instance);
    arena->offset           = 0;
    arena->allocation_count = 0;
}

void stack_arena_destroy(void* arena_instance)
{
    StackArena* arena = static_cast<StackArena*>(arena_instance);
    std::free(arena->allocation_stack);
    std::free(arena->buffer);
    std::free(arena);
}

// ============================================================================
// POOL ARENA IMPLEMENTATION
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
void* pool_arena_init(std::size_t capacity, std::size_t block_size)
{
    if (block_size < sizeof(PoolBlock)) {
        block_size = sizeof(PoolBlock);
    }

    PoolArena* arena = static_cast<PoolArena*>(std::malloc(sizeof(PoolArena)));
    if (arena == nullptr) {
        return nullptr;
    }

    arena->buffer = static_cast<char*>(std::malloc(capacity));
    if (arena->buffer == nullptr) {
        std::free(arena);
        return nullptr;
    }

    arena->capacity          = capacity;
    arena->block_size        = block_size;
    arena->block_count       = capacity / block_size;
    arena->free_count        = capacity / block_size;
    arena->total_allocations = 0;
    arena->total_frees       = 0;

    arena->free_list_head    = nullptr;
    for (std::size_t i = 0; i < arena->block_count; ++i) {
        PoolBlock* block      = reinterpret_cast<PoolBlock*>(arena->buffer + i * block_size);
        block->next           = arena->free_list_head;
        arena->free_list_head = block;
    }

    return arena;
}

/**
* @brief Allocate a block from the pool arena
* @param arena_instance Pointer to the arena instance
* @param size Size of the block to allocate
*
* If size is greater than the block size, or if no blocks are available, returns nullptr.
*
* @return Pointer to the allocated block, or nullptr if allocation fails
*/
void* pool_arena_alloc(void* arena_instance, std::size_t size)
{
    PoolArena* arena = static_cast<PoolArena*>(arena_instance);

    if (size > arena->block_size || arena->free_list_head == nullptr) {
        return nullptr;
    }

    PoolBlock* block      = arena->free_list_head;
    arena->free_list_head = block->next;
    arena->free_count--;
    arena->total_allocations++;

    return static_cast<void*>(block);
}

/**
* @brief Allocate an aligned block from the pool arena
* @param arena_instance Pointer to the arena instance
* @param ptr Pointer to the block to free
*
* If ptr is nullptr or ptr is not within the arena's buffer, does nothing.
*
*/
void pool_arena_free(void* arena_instance, void* ptr)
{
    if (ptr == nullptr) {
        return;
    }

    PoolArena* arena    = static_cast<PoolArena*>(arena_instance);
    char*      char_ptr = static_cast<char*>(ptr);
    if (char_ptr < arena->buffer || char_ptr >= arena->buffer + arena->capacity) {
        return;
    }

    PoolBlock* block      = static_cast<PoolBlock*>(ptr);
    block->next           = arena->free_list_head;
    arena->free_list_head = block;
    arena->free_count++;
    arena->total_frees++;
}

/**
* @brief Get memory usage statistics for the pool arena_instance
* @param arena_instance Pointer to the arena instance
* @param stats Pointer to ArenaStats structure to fill
*/
void pool_arena_get_stats(void* arena_instance, ArenaStats* stats)
{
    PoolArena* arena           = static_cast<PoolArena*>(arena_instance);
    stats->total_capacity      = arena->capacity;
    stats->used_bytes          = (arena->block_count - arena->free_count) * arena->block_size;
    stats->free_bytes          = arena->free_count * arena->block_size;
    stats->allocation_count    = arena->total_allocations;
    stats->free_count          = arena->total_frees;
    stats->fragmentation_ratio = 0; // No fragmentation in pool arena
    stats->largest_free_block  = arena->free_count > 0 ? arena->block_size : 0;
}

/**
* @brief Check if the pool arena can allocate a block of given size
* @param arena_instance Pointer to the arena instance
* @param size Size of the block to check
* @return true if allocation is possible, false otherwise
*/
bool pool_arena_can_alloc(void* arena_instance, std::size_t size)
{
    PoolArena* arena = static_cast<PoolArena*>(arena_instance);
    return size <= arena->block_size && arena->free_list_head != nullptr;
}

/**
* @brief Reset the pool arena, making all blocks available again
* @param arena_instance Pointer to the arena instance
*
* This function clears the free list and resets the free count to the total block count.
*/
void pool_arena_reset(void* arena_instance)
{
    PoolArena* arena      = static_cast<PoolArena*>(arena_instance);

    arena->free_list_head = nullptr;
    arena->free_count     = arena->block_count;

    for (std::size_t i = 0; i < arena->block_count; ++i) {
        PoolBlock* block      = reinterpret_cast<PoolBlock*>(arena->buffer + i * arena->block_size);
        block->next           = arena->free_list_head;
        arena->free_list_head = block;
    }
}

/**
* @brief Destroy the pool arena and free its resources
* @param arena_instance Pointer to the arena instance
* This function frees the buffer and the arena structure itself.
*/
void pool_arena_destroy(void* arena_instance)
{
    PoolArena* arena = static_cast<PoolArena*>(arena_instance);
    std::free(arena->buffer);
    std::free(arena);
}

// ============================================================================
// ARENA FACTORY FUNCTIONS
// ============================================================================

// Utility functions for ArenaType enum class
const char* arena_type_to_string(ArenaType type)
{
    switch (type) {
    case ArenaType::LINEAR:
        return "LINEAR";
    case ArenaType::STACK:
        return "STACK";
    case ArenaType::POOL:
        return "POOL";
    case ArenaType::FREE_LIST:
        return "FREE_LIST";
    case ArenaType::RING_BUFFER:
        return "RING_BUFFER";
    case ArenaType::SLAB:
        return "SLAB";
    case ArenaType::BUDDY:
        return "BUDDY";
    case ArenaType::HYBRID:
        return "HYBRID";
    default:
        return "UNKNOWN";
    }
}

ArenaType string_to_arena_type(const char* type_name)
{
    if (std::strcmp(type_name, "LINEAR") == 0)
        return ArenaType::LINEAR;
    if (std::strcmp(type_name, "STACK") == 0)
        return ArenaType::STACK;
    if (std::strcmp(type_name, "POOL") == 0)
        return ArenaType::POOL;
    if (std::strcmp(type_name, "FREE_LIST") == 0)
        return ArenaType::FREE_LIST;
    if (std::strcmp(type_name, "RING_BUFFER") == 0)
        return ArenaType::RING_BUFFER;
    if (std::strcmp(type_name, "SLAB") == 0)
        return ArenaType::SLAB;
    if (std::strcmp(type_name, "BUDDY") == 0)
        return ArenaType::BUDDY;
    if (std::strcmp(type_name, "HYBRID") == 0)
        return ArenaType::HYBRID;
    return ArenaType::LINEAR; // Default fallback
}

bool is_valid_arena_type(ArenaType type)
{
    return type >= ArenaType::LINEAR && type <= ArenaType::HYBRID;
}

// Factory functions for creating Arena interfaces
Arena create_linear_arena()
{
    return Arena{.init            = linear_arena_init,
        .alloc                    = linear_arena_alloc,
        .reset                    = linear_arena_reset,
        .destroy                  = linear_arena_destroy,
        .free                     = nullptr, // Linear arena doesn't support individual free
        .realloc                  = nullptr, // Linear arena doesn't support realloc
        .aligned_alloc            = linear_arena_aligned_alloc,
        .get_stats                = linear_arena_get_stats,
        .can_alloc                = linear_arena_can_alloc,
        .type_name                = "LINEAR",
        .min_alignment            = 1,
        .supports_individual_free = false,
        .supports_realloc         = false};
}

Arena create_stack_arena()
{
    return Arena{.init            = stack_arena_init,
        .alloc                    = stack_arena_alloc,
        .reset                    = stack_arena_reset,
        .destroy                  = stack_arena_destroy,
        .free                     = stack_arena_free, // Stack arena supports LIFO free
        .realloc                  = nullptr,          // Stack arena doesn't support realloc
        .aligned_alloc            = nullptr,          // Not implemented yet
        .get_stats                = stack_arena_get_stats,
        .can_alloc                = stack_arena_can_alloc,
        .type_name                = "STACK",
        .min_alignment            = 1,
        .supports_individual_free = true, // LIFO only
        .supports_realloc         = false};
}

Arena create_pool_arena()
{
    return Arena{.init            = nullptr, // Pool arena needs block_size parameter
        .alloc                    = pool_arena_alloc,
        .reset                    = pool_arena_reset,
        .destroy                  = pool_arena_destroy,
        .free                     = pool_arena_free, // Pool arena supports individual free
        .realloc                  = nullptr,         // Pool arena doesn't support realloc
        .aligned_alloc            = nullptr,         // Not implemented yet
        .get_stats                = pool_arena_get_stats,
        .can_alloc                = pool_arena_can_alloc,
        .type_name                = "POOL",
        .min_alignment            = sizeof(PoolBlock),
        .supports_individual_free = true,
        .supports_realloc         = false};
}

// Generic factory function
Arena create_arena(ArenaType type)
{
    switch (type) {
    case ArenaType::LINEAR:
        return create_linear_arena();
    case ArenaType::STACK:
        return create_stack_arena();
    case ArenaType::POOL:
        return create_pool_arena();
    case ArenaType::FREE_LIST:
    case ArenaType::RING_BUFFER:
    case ArenaType::SLAB:
    case ArenaType::BUDDY:
    case ArenaType::HYBRID:
        // TODO: Implement these arena types
        return create_linear_arena(); // Fallback to linear for now
    default:
        return create_linear_arena();
    }
}

// Configuration-based arena creation
Arena create_arena_from_config(const ArenaConfig* config)
{
    if (config == nullptr) {
        return create_linear_arena();
    }

    return create_arena(config->type);
}

// Forward declarations for additional init functions
void* stack_arena_init_with_max_allocs(std::size_t capacity, std::size_t max_allocations);

} // namespace baba::core
