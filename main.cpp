
#include <iostream>
#include <map>
#include "ucontext.h"

#include "coroutine_thread.h"


void func(int arg, Schedule *s) {
    for (int i = 0; i <= 5; i++) {
        std::cout << "func arg:" << arg <<  "  i:" << i <<std::endl;
        if(i == 2){
            s->coroutine_yield();
        }

    }

}

int main() {


    ucontext_t main;
    getcontext(&main);
    std::map<int, Coroutine *> umap;
    auto *s = new Schedule(&main, &umap);


    auto *c1 = new Coroutine(reinterpret_cast<Func>(func), 1, s);
    auto *c2 = new Coroutine(reinterpret_cast<Func>(func), 2, s);
    auto *c3 = new Coroutine(reinterpret_cast<Func>(func), 3, s);


    (*s->s_umap)[s->c_index++] = c1;
    (*s->s_umap)[s->c_index++] = c2;
    (*s->s_umap)[s->c_index++] = c3;

    int i = 0;
    while (true) {
        int c_index = i++ % (s->c_index);
        s->coroutine_resume(c_index);
        std::cout << "coroutine_resume:" << c_index << std::endl;
        if(i > 30){
            break;
        }
    }

}
