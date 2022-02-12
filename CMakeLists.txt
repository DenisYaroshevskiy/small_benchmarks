cmake_minimum_required(VERSION 3.7)
project(small_benchmarks)

find_package(benchmark)

function(add_benchmark name source)
    set(compiler_options -mavx2 --std=c++20 --stdlib=libc++ -DNDEBUG -g -Werror -Wall -Wextra -Wpedantic -Wno-deprecated-copy -O3)
    add_executable(${name})
    target_sources(${name} PRIVATE ${source})
    target_compile_options(${name} PRIVATE ${compiler_options})
    target_link_libraries(${name} PUBLIC benchmark pthread)
    target_link_options(${name} PRIVATE -stdlib=libc++)
endfunction()

add_benchmark(transform_alignment_issue transform_alignment_issue.cc)
