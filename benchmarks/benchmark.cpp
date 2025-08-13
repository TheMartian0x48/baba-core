// #include <baba/ds/array.hpp>
// #include <baba/memory.hpp>
// #include <chrono>
// #include <iomanip>
// #include <iostream>
// #include <vector>
//
// using namespace baba::ds::array;
// using namespace baba::memory;
// using namespace std::chrono;
//
// i32 N;
// int counter;
//
// // f64           benchmark_linear_arena()
// // {
// //     Arena arena = create_linear_arena((counter + 2) * N * sizeof(i32));
// //     auto  start = high_resolution_clock::now();
// //     auto  arr   = make_with_capacity<i32>(&arena, 0, N);
// //
// //     // Append
// //     for (i32 i = 0; i < N; ++i) {
// //         arr.append(i);
// //     }
// //
// //     // Access
// //     volatile i32 sum = 0;
// //     for (u32 i = 0; i < N; ++i)
// //         sum += arr.at(i);
// //
// //     // Remove
// //     for (u32 i = 0; i < 1000; ++i)
// //         arr.remove(0);
// //     kill(&arr);
// //
// //     auto end = high_resolution_clock::now();
// //     return duration_cast<milliseconds>(end - start).count();
// // }
// //
// // f64 benchmark_proxy_arena()
// // {
// //     Arena arena = create_proxy_arena();
// //
// //     auto  start = high_resolution_clock::now();
// //     auto  arr   = make_with_capacity<i32>(&arena, 0, N);
// //
// //     // Append
// //     for (i32 i = 0; i < N; ++i) {
// //         arr.append(i);
// //     }
// //
// //     // Access
// //     volatile i32 sum = 0;
// //     for (u32 i = 0; i < N; ++i)
// //         sum += arr.at(i);
// //
// //     // Remove
// //     for (u32 i = 0; i < 1000; ++i)
// //         arr.remove(0);
// //     kill(&arr);
// //
// //     auto end = high_resolution_clock::now();
// //     return duration_cast<milliseconds>(end - start).count();
// // }
// //
// struct Test {
//     i32 a, b, c;
// };
//
// std::pair<f32, i32> benchmark_struct(Arena arena, i32 sum)
// {
//
//     auto start = high_resolution_clock::now();
//     auto arr   = make_with_capacity<Test>(&arena, 0, N);
//
//     // Append
//     for (i32 i = 0; i < N; ++i) {
//         auto t = Test{
//             .a = i + 1,
//             .b = i + 2,
//             .c = i + 3,
//         };
//         arr.append(&t);
//     }
//
//     for (i32 i = 0; i < N; ++i) {
//         auto t = arr.at(i);
//         sum += t.a;
//         sum += t.b;
//         sum += t.c;
//     }
//
//     // Remove
//     for (u32 i = 0; i < 1000; ++i)
//         arr.remove(0);
//     kill(&arr);
//
//     auto end = high_resolution_clock::now();
//     return {duration_cast<milliseconds>(end - start).count(), sum};
// }
//
int main()
{
    //     // std::vector<int> counters = {10000, 100000, 1000000};
    //     std::vector<int> ns       = {100000};
    //     // std::vector<int> counters = {10, 100, 1000, 10000, 100000, 1000000};
    //     std::vector<int> counters = {10, 100, 1000, 10000, 10000};
    //     for (auto cc : counters) {
    //         counter = cc;
    //         for (auto nn : ns) {
    //             N = nn;
    //             std::cout << "N : " << N << " Counter : " << counter << "\n";
    //             Arena proxy_arena        = create_proxy_arena();
    //             Arena linear_arena       = create_linear_arena((counter + 2) * N * sizeof(i32));
    //             auto  linear_arena_times = std::vector<i32>(counter);
    //             auto  proxy_arena_times  = std::vector<i32>(counter);
    //
    //             int   sum                = 0;
    //             for (int i = 0; i < counter; ++i) {
    //                 auto [linear_arena_time, s] = benchmark_struct(linear_arena, sum);
    //                 auto [proxy_arena_time, ss] = benchmark_struct(proxy_arena, sum);
    //                 sum /= 1000000;
    //                 linear_arena_times[i] = static_cast<i32>(linear_arena_time);
    //                 proxy_arena_times[i]  = static_cast<i32>(proxy_arena_time);
    //                 linear_arena_reset(linear_arena.instance);
    //             }
    //             f64 linear_arena_avg = 0.0;
    //             f64 proxy_arena_avg  = 0.0;
    //             for (int i = 0; i < counter; ++i) {
    //                 // std::cout << std::fixed << std::setprecision(5) << linear_arena_times[i] << "\t"
    //                 //           << proxy_arena_times[i] << "\t"
    //                 //           << (proxy_arena_times[i] - linear_arena_times[i]) << "\n";
    //                 linear_arena_avg += linear_arena_times[i];
    //                 proxy_arena_avg += proxy_arena_times[i];
    //             }
    //             std::cout << "Total Time for LinearArena: " << linear_arena_avg << " ms" << std::endl;
    //             std::cout << "Total Time for ProxyArena : " << proxy_arena_avg << " ms" << std::endl;
    //             linear_arena_avg /= counter;
    //             proxy_arena_avg /= counter;
    //             std::cout << "Average Time for LinearArena: " << linear_arena_avg << " ms" << std::endl;
    //             std::cout << "Average Time for ProxyArena : " << proxy_arena_avg << " ms" << std::endl;
    //             arena_destroy(&linear_arena);
    //         }
    //     }
}
