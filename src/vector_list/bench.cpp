#include <random>
#include <array>
#include <vector>
#include <list>
#include <iostream>
#include <algorithm>
#include <chrono>

//Chrono typedefs
typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::milliseconds milliseconds;

static const std::size_t REPEAT = 1;

/* Fill back */

template<typename Container>
void fill_back(std::size_t size){
    Container container;

    for(std::size_t i = 0; i < size; ++i){
        container.push_back(i);
    }
}

void fill_back_vector(std::size_t size){
    std::vector<std::size_t> container(size);

    for(std::size_t i = 0; i < size; ++i){
        container[i] = i;
    }
}

/* Fill front  */

void fill_front_list(std::size_t size){
    std::list<std::size_t> container;

    for(std::size_t i = 0; i < size; ++i){
        container.push_front(i);
    }
}

void fill_front_vector(std::size_t size){
    std::list<std::size_t> container;

    for(std::size_t i = 0; i < size; ++i){
        container.insert(container.begin(), i);
    }
}

/* Find */

template<typename Container>
void find(std::size_t size, Container& container){
    std::mt19937 generator;
    std::uniform_int_distribution<std::size_t> distribution(0, size);

    for(std::size_t i = 0; i < 1000; ++i){
        std::find(container.begin(), container.end(), distribution(generator));
    }
}

/* Insert */

template<typename Container>
void insert(std::size_t size, Container& container){
    std::mt19937 generator;
    std::uniform_int_distribution<std::size_t> distribution(0, size - 1);

    for(std::size_t i = 0; i < 1000; ++i){
        auto it = std::find(container.begin(), container.end(), distribution(generator));
        container.insert(it, size + i);
    }
}

template<typename Function>
void bench(Function function, const std::string& type){
    std::vector<std::size_t> sizes = {1000, 10000, 100000, 1000000/*, 10000000*/};
    for(auto size : sizes){
        Clock::time_point t0 = Clock::now();

        for(std::size_t i = 0; i < REPEAT; ++i){
            function(size);
        }

        Clock::time_point t1 = Clock::now();
        milliseconds ms = std::chrono::duration_cast<milliseconds>(t1 - t0);

        std::cout << type << ":" << size << ":" << ms.count() << "ms" << std::endl;
    }
}

template<typename Container, typename Function>
void bench_pre(Function function, const std::string& type){
    std::vector<std::size_t> sizes = {1000, 10000, 100000, 1000000/*, 10000000*/};
    for(auto size : sizes){
        Container container;

        for(std::size_t i = 0; i < size; ++i){
            container.push_back(i);
        }

        Clock::time_point t0 = Clock::now();

        for(std::size_t i = 0; i < REPEAT; ++i){
            function(size, container);
        }

        Clock::time_point t1 = Clock::now();
        milliseconds ms = std::chrono::duration_cast<milliseconds>(t1 - t0);

        std::cout << type << ":" << size << ":" << ms.count() << "ms" << std::endl;
    }
}

int main(){
    std::cout << "Fill back" << std::endl;
    bench(fill_back_vector, "vector_pre");
    bench(fill_back<std::vector<std::size_t>>, "vector");
    bench(fill_back<std::list<std::size_t>>, "list");
    
    std::cout << "Fill front" << std::endl;
    bench(fill_front_vector, "vector");
    bench(fill_front_list, "list");
    
    std::cout << "Find all" << std::endl;
    bench_pre<std::vector<std::size_t>>(find<std::vector<std::size_t>>, "vector");
    bench_pre<std::list<std::size_t>>(find<std::list<std::size_t>>, "list");
    
    std::cout << "Insert" << std::endl;
    bench_pre<std::vector<std::size_t>>(insert<std::vector<std::size_t>>, "vector");
    bench_pre<std::list<std::size_t>>(insert<std::list<std::size_t>>, "list");

    return 0;
}
