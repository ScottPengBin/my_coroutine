//
// Created by 彭彬 on 2023/4/24.
//
#include <cstdlib>
#include <iostream>
#include "coroutine_thread.h"

Schedule::Schedule(ucontext_t *main, std::map<int, Coroutine *> *umap) {
    s_main = main;
    s_umap = umap;
    running_index = -1;
    c_index = 0;
}

void Schedule::coroutine_yield() {
    if (running_index >= 0) {
        Coroutine *c = s_umap->at(running_index);
        c->c_state = SUSPEND;
        running_index = -1;
        swapcontext(&c->c_ctx, s_main);
    }
}

void Schedule::coroutine_resume(int c_index) {
    if (running_index < 0) {
        std::cout << "c_index " << c_index << std::endl;
        auto *c = (*s_umap)[c_index];

        running_index = c_index;
        switch (c->c_state) {
            case READY:
                c->c_ctx.uc_stack.ss_sp = c->c_stack;
                c->c_ctx.uc_stack.ss_size = sizeof(c->c_stack);
                c->c_ctx.uc_stack.ss_flags = RUNNING;
                c->c_ctx.uc_link = s_main;

                makecontext(&c->c_ctx, (void (*)(void)) c->c_func, 2, c->c_arg, c->c_s);
                swapcontext(s_main, &c->c_ctx);

                break;
            case SUSPEND:
                c->c_state = RUNNING;
                swapcontext(s_main, &c->c_ctx);
                break;
            default:
                break;
        }
    }
}

bool Schedule::coroutine_end() {

}

Coroutine::Coroutine(Func func, int arg, Schedule *s) {
    c_state = READY;
    c_func = func;
    c_arg = arg;
    getcontext(&c_ctx);
    c_s = s;
}

