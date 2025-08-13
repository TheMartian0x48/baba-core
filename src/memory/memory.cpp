/**
 * @file memory.cpp
 * @brief Implementation of memory management system including aligned allocation and arena allocators
 */

#include "baba/memory/memory.hpp"

#include <cstdint>
#include <cstdlib>

namespace baba::memory
{

    // Note: All arena implementations are currently in the header file as inline functions
    // This is intentional for performance reasons, but could be moved here if needed

    // ============================================================================
    // LINEAR ARENA IMPLEMENTATION
    // ============================================================================

    // void* linear_arena_init(std::size_t capacity)
    // {
    //     const auto arena = static_cast<LinearArena*>(std::malloc(sizeof(LinearArena)));
    //     if (arena == nullptr) {
    //         return nullptr;
    //     }
    //     arena->buffer = static_cast<char*>(std::malloc(capacity));
    //     if (arena->buffer == nullptr) {
    //         std::free(arena);
    //         return nullptr;
    //     }
    //     arena->capacity          = capacity;
    //     arena->offset            = 0;
    //     arena->total_allocations = 0;
    //     return arena;
    // }
    //
    // void* linear_arena_alloc(void* arena_instance, std::size_t size)
    // {
    //     const auto arena = static_cast<LinearArena*>(arena_instance);
    //     if (arena->offset + size > arena->capacity) {
    //         return nullptr;
    //     }
    //     const auto ptr = static_cast<void*>(arena->buffer + arena->offset);
    //     arena->offset += size;
    //     arena->total_allocations++;
    //     return ptr;
    // }
    //
    // void  linear_arena_free(void* arena_instance, void* ptr) {}
    //
    // void* linear_arena_aligned_alloc(void* arena_instance, std::size_t size, std::size_t alignment)
    // {
    //     const auto arena = static_cast<LinearArena*>(arena_instance);
    //
    //     // Calculate aligned offset
    //     std::size_t aligned_offset = (arena->offset + alignment - 1) & ~(alignment - 1);
    //
    //     if (aligned_offset + size > arena->capacity) {
    //         return nullptr;
    //     }
    //
    //     const auto ptr = static_cast<void*>(arena->buffer + aligned_offset);
    //     arena->offset  = aligned_offset + size;
    //     arena->total_allocations++;
    //     return ptr;
    // }
    //
    // void linear_arena_get_stats(void* arena_instance, ArenaStats* stats)
    // {
    //     const auto arena           = static_cast<LinearArena*>(arena_instance);
    //     stats->total_capacity      = arena->capacity;
    //     stats->used_bytes          = arena->offset;
    //     stats->free_bytes          = arena->capacity - arena->offset;
    //     stats->allocation_count    = arena->total_allocations;
    //     stats->free_count          = 0; // Linear arena doesn't support individual free
    //     stats->fragmentation_ratio = 0; // No fragmentation in linear arena
    //     stats->largest_free_block  = arena->capacity - arena->offset;
    // }
    //
    // bool linear_arena_can_alloc(void* arena_instance, std::size_t size)
    // {
    //     const auto arena = static_cast<LinearArena*>(arena_instance);
    //     return arena->offset + size <= arena->capacity;
    // }
    //
    // void linear_arena_reset(void* arena_instance)
    // {
    //     const auto arena = static_cast<LinearArena*>(arena_instance);
    //     arena->offset    = 0;
    // }
    //
    // void linear_arena_destroy(void* arena_instance)
    // {
    //     const auto arena = static_cast<LinearArena*>(arena_instance);
    //     std::free(arena->buffer);
    //     std::free(arena);
    // }

    // ============================================================================
    // PROXY ARENA IMPLEMENTATION
    // ============================================================================

    // void* proxy_arena_init([[maybe_unused]] std::size_t capacity)
    // {
    //     // Capacity is ignored - proxy delegates to malloc which has no fixed capacity
    //     // Return a non-null sentinel value to indicate successful "initialization"
    //     return reinterpret_cast<void*>(0x1);
    // }
    //
    // void* proxy_arena_alloc([[maybe_unused]] void* arena_instance, std::size_t size)
    // {
    //     // Direct delegation to malloc - arena_instance is ignored
    //     return std::malloc(size);
    // }
    //
    // void proxy_arena_free([[maybe_unused]] void* arena_instance, void* ptr)
    // {
    //     // Direct delegation to free - arena_instance is ignored
    //     std::free(ptr);
    // }
    //
    // void proxy_arena_get_stats([[maybe_unused]] void* arena_instance, ArenaStats* stats)
    // {
    //     if (stats == nullptr) {
    //         return;
    //     }
    //
    //     // Proxy arena has no meaningful stats since it delegates to malloc
    //     stats->total_capacity      = SIZE_MAX; // Unlimited capacity
    //     stats->used_bytes          = 0;        // Unknown - malloc doesn't track this
    //     stats->free_bytes          = SIZE_MAX; // Unlimited
    //     stats->allocation_count    = 0;        // Unknown - no tracking
    //     stats->free_count          = 0;        // Unknown - no tracking
    //     stats->fragmentation_ratio = 0;        // Unknown - malloc handles this
    //     stats->largest_free_block  = SIZE_MAX; // Unknown - depends on system
    // }
    //
    // bool proxy_arena_can_alloc([[maybe_unused]] void* arena_instance, [[maybe_unused]] std::size_t size)
    // {
    //     // Proxy arena can always attempt allocation (depends on system memory)
    //     // arena_instance and size are ignored - malloc will determine if allocation succeeds
    //     return true;
    // }
    //
    // void proxy_arena_reset([[maybe_unused]] void* arena_instance)
    // {
    //     // No-op - cannot reset malloc allocations
    //     // This is a limitation of the proxy pattern
    // }
    //
    // void proxy_arena_destroy([[maybe_unused]] void* arena_instance)
    // {
    //     // No-op - nothing to destroy since we don't allocate any arena state
    //     // Individual allocations made through proxy_arena_alloc must be freed separately
    // }

    // ============================================================================
    // STACK ARENA IMPLEMENTATION
    // ============================================================================

    // void* stack_arena_init(std::size_t capacity)
    // {
    //     const auto arena = static_cast<StackArena*>(std::malloc(sizeof(StackArena)));
    //     if (arena == nullptr) {
    //         return nullptr;
    //     }
    //     arena->buffer = static_cast<char*>(std::malloc(capacity));
    //     if (arena->buffer == nullptr) {
    //         std::free(arena);
    //         return nullptr;
    //     }
    //     arena->capacity          = capacity;
    //     arena->offset            = 0;
    //     arena->allocation_count  = 0;
    //     arena->total_frees       = 0;
    //     arena->total_allocations = 0;
    //     return arena;
    // }
    //
    // void* stack_arena_alloc(void* arena_instance, std::size_t size)
    // {
    //     const auto arena      = static_cast<StackArena*>(arena_instance);
    //     const auto total_size = size + sizeof(StackAllocationMetaData);
    //     if (arena->offset + total_size > arena->capacity) {
    //         return nullptr;
    //     }
    //     const auto meta   = reinterpret_cast<StackAllocationMetaData*>(arena->buffer + arena->offset);
    //     meta->magic_start = ArenaConstant::STACK_ALLOC_START_MAGIC;
    //     meta->offset      = arena->offset;
    //     meta->size        = size;
    //     meta->magic_end   = ArenaConstant::STACK_ALLOC_END_MAGIC;
    //
    //     void* user_ptr    = arena->buffer + arena->offset + sizeof(StackAllocationMetaData);
    //     arena->offset += total_size;
    //     arena->allocation_count++;
    //     arena->total_allocations++;
    //     return user_ptr;
    // }
    //
    // void stack_arena_free(void* arena_instance, void* ptr)
    // {
    //     if (ptr == nullptr) {
    //         return;
    //     }
    //     const auto arena    = static_cast<StackArena*>(arena_instance);
    //     const auto user_ptr = static_cast<const char*>(ptr);
    //     if (user_ptr < arena->buffer || user_ptr >= arena->buffer + arena->capacity) {
    //         return;
    //     }
    //
    //     auto metadata =
    //         reinterpret_cast<StackAllocationMetaData*>(const_cast<char*>(user_ptr - sizeof(StackAllocationMetaData)));
    //
    //     if (metadata->magic_start != ArenaConstant::STACK_ALLOC_START_MAGIC ||
    //         metadata->magic_end != ArenaConstant::STACK_ALLOC_END_MAGIC) {
    //         // corrupted data
    //         return;
    //     }
    //
    //     if (arena->offset != metadata->offset + metadata->size + sizeof(StackAllocationMetaData)) {
    //         // not recent stack element
    //         return;
    //     }
    //     metadata->magic_start = ArenaConstant::FREED_MAGIC;
    //     metadata->magic_end   = ArenaConstant::FREED_MAGIC;
    //     arena->offset         = metadata->offset;
    //     arena->allocation_count--;
    //     arena->total_frees++;
    // }
    //
    // void stack_arena_reset(void* arena_instance)
    // {
    //     const auto arena         = static_cast<StackArena*>(arena_instance);
    //     arena->offset            = 0;
    //     arena->allocation_count  = 0;
    //     arena->total_allocations = 0;
    //     arena->total_frees       = 0;
    // }
    //
    // void stack_arena_destroy(void* arena_instance)
    // {
    //     if (arena_instance == nullptr) {
    //         return;
    //     }
    //     const auto arena = static_cast<StackArena*>(arena_instance);
    //     if (arena->buffer != nullptr) {
    //         std::free(arena->buffer);
    //     }
    //     std::free(arena);
    // }
    //
    // void stack_arena_get_stats(void* arena_instance, ArenaStats* stats)
    // {
    //     if (arena_instance == nullptr || stats == nullptr) {
    //         return;
    //     }
    //
    //     StackArena* arena          = static_cast<StackArena*>(arena_instance);
    //     stats->total_capacity      = arena->capacity;
    //     stats->used_bytes          = arena->offset;
    //     stats->free_bytes          = arena->capacity - arena->offset;
    //     stats->allocation_count    = arena->total_allocations;
    //     stats->free_count          = arena->total_frees;
    //     stats->fragmentation_ratio = 0;
    //     stats->largest_free_block  = arena->capacity - arena->offset;
    // }
    //
    // bool stack_arena_can_alloc(void* arena_instance, std::size_t size)
    // {
    //     const auto arena = static_cast<StackArena*>(arena_instance);
    //     return arena->offset + size + sizeof(StackAllocationMetaData) <= arena->capacity;
    // }
    // ============================================================================
    // POOL ARENA IMPLEMENTATION
    // ============================================================================

    //     void* pool_arena_init(std::size_t capacity, std::size_t block_size)
    //     {
    //         if (block_size < sizeof(PoolBlock)) {
    //             block_size = sizeof(PoolBlock);
    //         }
    //
    //         const auto arena = static_cast<PoolArena*>(std::malloc(sizeof(PoolArena)));
    //         if (arena == nullptr) {
    //             return nullptr;
    //         }
    //
    //         arena->buffer = static_cast<char*>(std::malloc(capacity));
    //         if (arena->buffer == nullptr) {
    //             std::free(arena);
    //             return nullptr;
    //         }
    //
    //         arena->capacity          = capacity;
    //         arena->block_size        = block_size;
    //         arena->block_count       = capacity / block_size;
    //         arena->free_count        = capacity / block_size;
    //         arena->total_allocations = 0;
    //         arena->total_frees       = 0;
    //
    //         arena->free_list_head    = nullptr;
    //         for (std::size_t i = 0; i < arena->block_count; ++i) {
    //             const auto block      = reinterpret_cast<PoolBlock*>(arena->buffer + i * block_size);
    //             block->next           = arena->free_list_head;
    //             arena->free_list_head = block;
    //         }
    //
    //         return arena;
    //     }
    //
    //     /**
    // * @brief Allocate a block from the pool arena
    // * @param arena_instance Pointer to the arena instance
    // * @param size Size of the block to allocate
    // *
    // * If size is greater than the block size, or if no blocks are available, returns nullptr.
    // *
    // * @return Pointer to the allocated block, or nullptr if allocation fails
    // */
    //     void* pool_arena_alloc(void* arena_instance, std::size_t size)
    //     {
    //         const auto arena = static_cast<PoolArena*>(arena_instance);
    //
    //         if (size > arena->block_size || arena->free_list_head == nullptr) {
    //             return nullptr;
    //         }
    //
    //         PoolBlock* block      = arena->free_list_head;
    //         arena->free_list_head = block->next;
    //         arena->free_count--;
    //         arena->total_allocations++;
    //
    //         return static_cast<void*>(block);
    //     }
    //
    //     /**
    // * @brief Allocate an aligned block from the pool arena
    // * @param arena_instance Pointer to the arena instance
    // * @param ptr Pointer to the block to free
    // *
    // * If ptr is nullptr or ptr is not within the arena's buffer, does nothing.
    // *
    // */
    //     void pool_arena_free(void* arena_instance, void* ptr)
    //     {
    //         if (ptr == nullptr) {
    //             return;
    //         }
    //
    //         const auto arena = static_cast<PoolArena*>(arena_instance);
    //         if (const auto char_ptr = static_cast<char*>(ptr);
    //             char_ptr < arena->buffer || char_ptr >= arena->buffer + arena->capacity) {
    //             return;
    //         }
    //
    //         const auto block      = static_cast<PoolBlock*>(ptr);
    //         block->next           = arena->free_list_head;
    //         arena->free_list_head = block;
    //         arena->free_count++;
    //         arena->total_frees++;
    //     }
    //
    //     /**
    // * @brief Get memory usage statistics for the pool arena_instance
    // * @param arena_instance Pointer to the arena instance
    // * @param stats Pointer to ArenaStats structure to fill
    // */
    //     void pool_arena_get_stats(void* arena_instance, ArenaStats* stats)
    //     {
    //         const auto arena           = static_cast<PoolArena*>(arena_instance);
    //         stats->total_capacity      = arena->capacity;
    //         stats->used_bytes          = (arena->block_count - arena->free_count) * arena->block_size;
    //         stats->free_bytes          = arena->free_count * arena->block_size;
    //         stats->allocation_count    = arena->total_allocations;
    //         stats->free_count          = arena->total_frees;
    //         stats->fragmentation_ratio = 0; // No fragmentation in pool arena
    //         stats->largest_free_block  = arena->free_count > 0 ? arena->block_size : 0;
    //     }
    //
    //     /**
    // * @brief Check if the pool arena can allocate a block of given size
    // * @param arena_instance Pointer to the arena instance
    // * @param size Size of the block to check
    // * @return true if allocation is possible, false otherwise
    // */
    //     bool pool_arena_can_alloc(void* arena_instance, std::size_t size)
    //     {
    //         const auto arena = static_cast<PoolArena*>(arena_instance);
    //         return size <= arena->block_size && arena->free_list_head != nullptr;
    //     }
    //
    //     /**
    // * @brief Reset the pool arena, making all blocks available again
    // * @param arena_instance Pointer to the arena instance
    // *
    // * This function clears the free list and resets the free count to the total block count.
    // */
    //     void pool_arena_reset(void* arena_instance)
    //     {
    //         const auto arena      = static_cast<PoolArena*>(arena_instance);
    //
    //         arena->free_list_head = nullptr;
    //         arena->free_count     = arena->block_count;
    //
    //         for (std::size_t i = 0; i < arena->block_count; ++i) {
    //             const auto block      = reinterpret_cast<PoolBlock*>(arena->buffer + i * arena->block_size);
    //             block->next           = arena->free_list_head;
    //             arena->free_list_head = block;
    //         }
    //     }
    //
    //     /**
    // * @brief Destroy the pool arena and free its resources
    // * @param arena_instance Pointer to the arena instance
    // * This function frees the buffer and the arena structure itself.
    // */
    //     void pool_arena_destroy(void* arena_instance)
    //     {
    //         auto* arena = static_cast<PoolArena*>(arena_instance);
    //         std::free(arena->buffer);
    //         std::free(arena);
    //     }

    // ============================================================================
    // ARENA FACTORY FUNCTIONS
    // ============================================================================

    // const char* arena_type_to_string(ArenaType type)
    // {
    //     switch (type) {
    //     case ArenaType::BUDDY:
    //         return ArenaConstant::BUDDY_ARENA_TYPE;
    //     case ArenaType::FREE_LIST:
    //         return ArenaConstant::FREE_LIST_ARENA_TYPE;
    //     case ArenaType::HYBRID:
    //         return ArenaConstant::HYBRID_ARENA_TYPE;
    //     case ArenaType::LINEAR:
    //         return ArenaConstant::LINEAR_ARENA_TYPE;
    //     case ArenaType::POOL:
    //         return ArenaConstant::POOL_ARENA_TYPE;
    //     case ArenaType::PROXY:
    //         return ArenaConstant::PROXY_ARENA_TYPE;
    //     case ArenaType::RING_BUFFER:
    //         return ArenaConstant::RING_BUFFER_ARENA_TYPE;
    //     case ArenaType::SLAB:
    //         return ArenaConstant::SLAB_ARENA_TYPE;
    //     case ArenaType::STACK:
    //         return ArenaConstant::STACK_ARENA_TYPE;
    //     default:
    //         return ArenaConstant::UNKNOWN_ARENA_TYPE;
    //     }
    // }

    // ArenaType string_to_arena_type(const char* type_name)
    // {
    //     if (std::strcmp(type_name, ArenaConstant::BUDDY_ARENA_TYPE) == 0)
    //         return ArenaType::BUDDY;
    //     if (std::strcmp(type_name, ArenaConstant::FREE_LIST_ARENA_TYPE) == 0)
    //         return ArenaType::FREE_LIST;
    //     if (std::strcmp(type_name, ArenaConstant::HYBRID_ARENA_TYPE) == 0)
    //         return ArenaType::HYBRID;
    //     if (std::strcmp(type_name, ArenaConstant::LINEAR_ARENA_TYPE) == 0)
    //         return ArenaType::LINEAR;
    //     if (std::strcmp(type_name, ArenaConstant::POOL_ARENA_TYPE) == 0)
    //         return ArenaType::POOL;
    //     if (std::strcmp(type_name, ArenaConstant::PROXY_ARENA_TYPE) == 0)
    //         return ArenaType::PROXY;
    //     if (std::strcmp(type_name, ArenaConstant::RING_BUFFER_ARENA_TYPE) == 0)
    //         return ArenaType::RING_BUFFER;
    //     if (std::strcmp(type_name, ArenaConstant::SLAB_ARENA_TYPE) == 0)
    //         return ArenaType::SLAB;
    //     if (std::strcmp(type_name, ArenaConstant::STACK_ARENA_TYPE) == 0)
    //         return ArenaType::STACK;
    //     return ArenaType::LINEAR; // Default fallback
    // }

    // Arena create_proxy_arena()
    // {
    //     auto arena = Arena{
    //         proxy_arena_init,                // init
    //         proxy_arena_alloc,               // alloc
    //         nullptr,                         // realloc
    //         proxy_arena_reset,               // reset
    //         proxy_arena_free,                // free
    //         proxy_arena_destroy,             // destroy
    //         nullptr,                         // aligned_alloc
    //         proxy_arena_get_stats,           // get_stats
    //         proxy_arena_can_alloc,           // can_alloc
    //         nullptr,                         // instance
    //         ArenaConstant::PROXY_ARENA_TYPE, // type_name
    //         1,                               // min_alignment
    //         true,                            // supports_individual_free
    //         false                            // supports_realloc
    //     };
    //     return arena;
    // }
    //
    // Arena create_linear_arena(std::size_t capacity)
    // {
    //     Arena arena = Arena{
    //         linear_arena_init,          // init
    //         linear_arena_alloc,         // alloc
    //         nullptr,                    // realloc
    //         linear_arena_reset,         // reset
    //         linear_arena_free,          // free
    //         linear_arena_destroy,       // destroy
    //         linear_arena_aligned_alloc, // aligned_alloc
    //         linear_arena_get_stats,     // get_stats
    //         linear_arena_can_alloc,     // can_alloc
    //         nullptr,                    // instance
    //         "LINEAR",                   // type_name
    //         1,                          // min_alignment
    //         false,                      // supports_individual_free
    //         false                       // supports_realloc
    //     };
    //     arena.instance = linear_arena_init(capacity);
    //     return arena;
    // }
    //
    // Arena create_stack_arena()
    // {
    //     return Arena{
    //         stack_arena_init,      // init
    //         stack_arena_alloc,     // alloc
    //         nullptr,               // realloc
    //         stack_arena_reset,     // reset
    //         stack_arena_free,      // free
    //         stack_arena_destroy,   // destroy
    //         nullptr,               // aligned_alloc
    //         stack_arena_get_stats, // get_stats
    //         stack_arena_can_alloc, // can_alloc
    //         nullptr,               // instance
    //         "STACK",               // type_name
    //         1,                     // min_alignment
    //         true,                  // supports_individual_free
    //         false                  // supports_realloc
    //     };
    // }
    //
    // Arena create_pool_arena()
    // {
    //     return Arena{
    //         nullptr,              // init - Pool arena needs block_size parameter
    //         pool_arena_alloc,     // alloc
    //         nullptr,              // realloc
    //         pool_arena_reset,     // reset
    //         pool_arena_free,      // free
    //         pool_arena_destroy,   // destroy
    //         nullptr,              // aligned_alloc
    //         pool_arena_get_stats, // get_stats
    //         pool_arena_can_alloc, // can_alloc
    //         nullptr,              // instance
    //         "POOL",               // type_name
    //         sizeof(PoolBlock),    // min_alignment
    //         true,                 // supports_individual_free
    //         false                 // supports_realloc
    //     };
    // }
    //
    // // Generic factory function
    // Arena create_arena(ArenaType type)
    // {
    //     switch (type) {
    //     // case ArenaType::LINEAR:
    //     //     return create_linear_arena();
    //     case ArenaType::STACK:
    //         return create_stack_arena();
    //     case ArenaType::POOL:
    //         return create_pool_arena();
    //     case ArenaType::FREE_LIST:
    //     case ArenaType::RING_BUFFER:
    //     case ArenaType::SLAB:
    //     case ArenaType::BUDDY:
    //     case ArenaType::HYBRID:
    //         // TODO: Implement these arena types
    //     default:
    //         return create_stack_arena();
    //         // return create_linear_arena();
    //     }
    // }
    //
    // // Configuration-based arena creation
    // Arena create_arena_from_config(const ArenaConfig* config)
    // {
    //     // if (config == nullptr) {
    //     //     return create_linear_arena();
    //     // }
    //
    //     return create_arena(config->type);
    // }

} // namespace baba::memory
