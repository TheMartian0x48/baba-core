#ifndef BABA_MEMORY
#define BABA_MEMORY
/**
 * @file memory.hpp
 * @brief Complete memory management system including aligned allocation and arena allocators
 */

#include <cstddef>
#include <cstdlib>
#include <cstring>

namespace baba::memory
{
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
        inline const char*    BUDDY_ARENA_TYPE        = "BUDDY";
        inline const char*    FREE_LIST_ARENA_TYPE    = "FREE";
        inline const char*    HYBRID_ARENA_TYPE       = "HYBRID";
        inline const char*    LINEAR_ARENA_TYPE       = "LINEAR";
        inline const char*    POOL_ARENA_TYPE         = "POOL";
        inline const char*    PROXY_ARENA_TYPE        = "PROXY";
        inline const char*    RING_BUFFER_ARENA_TYPE  = "RING";
        inline const char*    SLAB_ARENA_TYPE         = "SLAB";
        inline const char*    STACK_ARENA_TYPE        = "STACK";
        inline const char*    UNKNOWN_ARENA_TYPE      = "UNKNOWN";
        constexpr std::size_t STACK_ALLOC_START_MAGIC = 0xDEADBEEFCAFEBABE;
        constexpr std::size_t STACK_ALLOC_END_MAGIC   = 0xBABECAFEDEADBEEF;
        constexpr std::size_t FREED_MAGIC             = 0xDEADDEADDEADDEAD;
        constexpr std::size_t DEFAULT_ALIGNMENT       = 2 * sizeof(void*);
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
        UNKNOWN,
    };

    inline const char* arena_type_to_string(ArenaType type)
    {
        switch (type) {
        case ArenaType::BUDDY:
            return ArenaConstant::BUDDY_ARENA_TYPE;
        case ArenaType::FREE_LIST:
            return ArenaConstant::FREE_LIST_ARENA_TYPE;
        case ArenaType::HYBRID:
            return ArenaConstant::HYBRID_ARENA_TYPE;
        case ArenaType::LINEAR:
            return ArenaConstant::LINEAR_ARENA_TYPE;
        case ArenaType::POOL:
            return ArenaConstant::POOL_ARENA_TYPE;
        case ArenaType::PROXY:
            return ArenaConstant::PROXY_ARENA_TYPE;
        case ArenaType::RING_BUFFER:
            return ArenaConstant::RING_BUFFER_ARENA_TYPE;
        case ArenaType::SLAB:
            return ArenaConstant::SLAB_ARENA_TYPE;
        case ArenaType::STACK:
            return ArenaConstant::STACK_ARENA_TYPE;
        default:
            return ArenaConstant::UNKNOWN_ARENA_TYPE;
        }
    }

    inline ArenaType string_to_arena_type(const char* type_name)
    {
        if (std::strcmp(type_name, ArenaConstant::BUDDY_ARENA_TYPE) == 0)
            return ArenaType::BUDDY;
        if (std::strcmp(type_name, ArenaConstant::FREE_LIST_ARENA_TYPE) == 0)
            return ArenaType::FREE_LIST;
        if (std::strcmp(type_name, ArenaConstant::HYBRID_ARENA_TYPE) == 0)
            return ArenaType::HYBRID;
        if (std::strcmp(type_name, ArenaConstant::LINEAR_ARENA_TYPE) == 0)
            return ArenaType::LINEAR;
        if (std::strcmp(type_name, ArenaConstant::POOL_ARENA_TYPE) == 0)
            return ArenaType::POOL;
        if (std::strcmp(type_name, ArenaConstant::PROXY_ARENA_TYPE) == 0)
            return ArenaType::PROXY;
        if (std::strcmp(type_name, ArenaConstant::RING_BUFFER_ARENA_TYPE) == 0)
            return ArenaType::RING_BUFFER;
        if (std::strcmp(type_name, ArenaConstant::SLAB_ARENA_TYPE) == 0)
            return ArenaType::SLAB;
        if (std::strcmp(type_name, ArenaConstant::STACK_ARENA_TYPE) == 0)
            return ArenaType::STACK;
        return ArenaType::UNKNOWN;
    }

    inline bool is_valid_arena_type(ArenaType type) { return type >= ArenaType::BUDDY && type <= ArenaType::STACK; }

    // ============================================================================
    //  PROXY ARENA
    // ============================================================================
    struct ProxyArena {
        // Empty - stateless proxy that delegates to malloc/free
        static void* make([[maybe_unused]] std::size_t capacity)
        {
            ProxyArena* p = static_cast<ProxyArena*>(std::malloc(sizeof(ProxyArena)));
            if (!p)
                return nullptr;
            return p;
        }
        static void kill([[maybe_unused]] void* arena_instance)
        {
            // No resources to free, just free the arena instance itself
            std::free(arena_instance);
        }
        static void  reset([[maybe_unused]] void* arena_instance) {}

        static void* alloc([[maybe_unused]] void* arena_instance, std::size_t size, [[maybe_unused]] std::size_t alignment)
        {
            return std::malloc(size);
        }
        static void  dealloc([[maybe_unused]] void* arena_instance, void* ptr) { std::free(ptr); }
        static void* realloc([[maybe_unused]] void* arena_instance, void* ptr, std::size_t size)
        {
            return std::realloc(ptr, size);
        }

        static ArenaStats stats([[maybe_unused]] void* arena_instance) noexcept
        {
            return {
                .total_capacity      = SIZE_MAX,
                .used_bytes          = 0,
                .free_bytes          = SIZE_MAX,
                .allocation_count    = 0,
                .free_count          = 0,
                .fragmentation_ratio = 0,
                .largest_free_block  = SIZE_MAX,
            };
        }

        static bool can_alloc([[maybe_unused]] void* arena_instance, std::size_t size) { return true; }
    };

    // ============================================================================
    //  LINEAR ARENA
    // ============================================================================
    struct LinearArena {
        char*       buffer;
        std::size_t capacity;
        std::size_t offset;
        std::size_t total_allocations;

        //
        static void* make(std::size_t capacity)
        {
            const auto arena = static_cast<LinearArena*>(std::malloc(sizeof(LinearArena)));
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
        static void reset(void* arena_instance)
        {
            auto arena    = static_cast<LinearArena*>(arena_instance);
            arena->offset = 0;
        }
        static void kill(void* arena_instance)
        {
            auto arena = static_cast<LinearArena*>(arena_instance);
            std::free(arena->buffer);
            std::free(arena);
        }

        static void* alloc(void* arena_instance, std::size_t size, [[maybe_unused]] std::size_t alignment)
        {
            auto arena = static_cast<LinearArena*>(arena_instance);
            if (arena->offset + size > arena->capacity) {
                return nullptr;
            }
            const auto ptr = static_cast<void*>(arena->buffer + arena->offset);
            arena->offset += size;
            arena->total_allocations++;
            return ptr;
        }

        static void dealloc([[maybe_unused]] void* arena_instance, [[maybe_unused]] void* ptr) {}
        static void*
        realloc([[maybe_unused]] void* arena_instance, [[maybe_unused]] void* ptr, [[maybe_unused]] std::size_t new_size)
        {
            return nullptr;
        }

        static ArenaStats stats(void* arena_instance) noexcept
        {
            const auto arena = static_cast<LinearArena*>(arena_instance);
            return {
                .total_capacity      = arena->capacity,
                .used_bytes          = arena->offset,
                .free_bytes          = arena->capacity - arena->offset,
                .allocation_count    = arena->total_allocations,
                .free_count          = 0, // Linear arena doesn't support individual free
                .fragmentation_ratio = 0, // No fragmentation in linear arena
                .largest_free_block  = arena->capacity - arena->offset,
            };
        }
        static bool can_alloc(void* arena_instance, std::size_t size)
        {
            const auto arena = static_cast<LinearArena*>(arena_instance);
            return arena->offset + size <= arena->capacity;
        }
    };

    // ============================================================================
    //  POOL BLOCK ARENA
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
        static void* make(std::size_t capacity, std::size_t block_size);
        /**
        * @brief Destroy the pool arena and free its resources
        * @param arena_instance Pointer to the arena instance
        * This function frees the buffer and the arena structure itself.
        */
        void kill() { std::free(buffer); }
        /**
        * @brief Reset the pool arena, making all blocks available again
        * @param arena_instance Pointer to the arena instance
        *
        * This function clears the free list and resets the free count to the total block count.
        */
        void reset()
        {
            free_list_head = nullptr;
            free_count     = block_count;

            for (std::size_t i = 0; i < block_count; ++i) {
                const auto block = reinterpret_cast<PoolBlock*>(buffer + i * block_size);
                block->next      = free_list_head;
                free_list_head   = block;
            }
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
        void* alloc(std::size_t size)
        {
            if (size > block_size || free_list_head == nullptr) {
                return nullptr;
            }

            PoolBlock* block = free_list_head;
            free_list_head   = block->next;
            free_count--;
            total_allocations++;

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
        void dealloc(void* ptr)
        {
            if (ptr == nullptr) {
                return;
            }

            if (const auto char_ptr = static_cast<char*>(ptr); char_ptr < buffer || char_ptr >= buffer + capacity) {
                return;
            }

            const auto block = static_cast<PoolBlock*>(ptr);
            block->next      = free_list_head;
            free_list_head   = block;
            free_count++;
            total_frees++;
        }
        void realloc(void* ptr) {}
        /**
        * @brief Get memory usage statistics for the pool arena_instance
        * @param arena_instance Pointer to the arena instance
        * @param stats Pointer to ArenaStats structure to fill
        */
        void stats(ArenaStats* stats) {}
        /**
        * @brief Check if the pool arena can allocate a block of given size
        * @param arena_instance Pointer to the arena instance
        * @param size Size of the block to check
        * @return true if allocation is possible, false otherwise
        */
        inline bool can_alloc(std::size_t size) { return size <= block_size && free_list_head != nullptr; }
    };

    using ArenaMakeFn     = void* (*)(std::size_t capacity);
    using ArenaResetFn    = void (*)(void* arena_instance);
    using ArenaKillFn     = void (*)(void* arena_instance);

    using ArenaAllocFn    = void* (*)(void* arena_instance, std::size_t size, std::size_t alignment);
    using ArenaReallocFn  = void* (*)(void* arena_instance, void* ptr, std::size_t new_size);
    using ArenaDeallocFn  = void (*)(void* arena_instance, void* ptr);

    using ArenaGetStatsFn = ArenaStats (*)(void* arena_instance);
    using ArenaCanAllocFn = bool (*)(void* arena_instance, std::size_t size);

    struct Arena {
        void*           instance; // instance of actual arena
        ArenaMakeFn     make_fn;  // Function to create arena instance
        ArenaResetFn    reset_fn; // Function to reset arena instance
        ArenaKillFn     kill_fn;  // Function to destroy arena instance

        ArenaAllocFn    alloc_fn;   // Function to allocate memory from arena
        ArenaReallocFn  realloc_fn; // Function to reallocate memory in arena
        ArenaDeallocFn  dealloc_fn; // Function to deallocate memory in arena

        ArenaGetStatsFn stats_fn;     // Function to get arena statistics
        ArenaCanAllocFn can_alloc_fn; // Function to check if arena can allocate memory

        // Funtions
        static Arena* make(ArenaType arenaType, std::size_t capacity)
        {
            Arena* arena = static_cast<Arena*>(std::malloc(sizeof(Arena)));
            if (!arena)
                return nullptr;
            if (ArenaType::LINEAR == arenaType) {
                arena->instance = LinearArena::make(capacity);
                if (!arena->instance) {
                    std::free(arena);
                    return nullptr;
                }
                arena->make_fn      = LinearArena::make;
                arena->reset_fn     = LinearArena::reset;
                arena->kill_fn      = LinearArena::kill;
                arena->alloc_fn     = LinearArena::alloc;
                arena->realloc_fn   = LinearArena::realloc;
                arena->dealloc_fn   = LinearArena::dealloc;
                arena->stats_fn     = LinearArena::stats;
                arena->can_alloc_fn = LinearArena::can_alloc;
            } else {
                // Default to ProxyArena
                arena->instance = ProxyArena::make(capacity);
                if (!arena->instance) {
                    std::free(arena);
                    return nullptr;
                }
                arena->make_fn      = ProxyArena::make;
                arena->reset_fn     = ProxyArena::reset;
                arena->kill_fn      = ProxyArena::kill;
                arena->alloc_fn     = ProxyArena::alloc;
                arena->realloc_fn   = ProxyArena::realloc;
                arena->dealloc_fn   = ProxyArena::dealloc;
                arena->stats_fn     = ProxyArena::stats;
                arena->can_alloc_fn = ProxyArena::can_alloc;
            }
            return arena;
        }
        static inline void kill(Arena* arena)
        {
            if (arena == nullptr)
                return;
            arena->kill_fn(arena->instance);
            std::free(arena);
        }
        inline void  reset() { reset_fn(instance); }

        inline void* alloc(std::size_t size) { return aligned_alloc(instance, size, ArenaConstant::DEFAULT_ALIGNMENT); }
        inline void* aligned_alloc(std::size_t size, std::size_t alignment) { return alloc_fn(instance, size, alignment); }
        inline void  dealloc(void* ptr) { dealloc_fn(instance, ptr); }
        inline void* realloc(void* ptr, std::size_t new_size) { return realloc_fn(instance, ptr, new_size); }

        inline bool  can_alloc(std::size_t size) { return can_alloc_fn(instance, size); }
        inline ArenaStats get_stats() { return stats_fn(instance); }
    };
} // namespace baba::memory

#endif
