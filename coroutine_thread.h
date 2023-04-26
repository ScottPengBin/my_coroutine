//
// Created by 彭彬 on 2023/4/24.
//
#include <unordered_map>
#include <map>
#include "ucontext.h"

#define DEFAULT_STACK_SZIE 1024*4

typedef void (*Func)(void *arg);


enum ThreadState {
    DEAD,
    READY,
    RUNNING,
    SUSPEND
};

class Coroutine;

class Schedule;

//调度器
class Schedule {
public:
    ucontext_t *s_main;
    std::map<int, Coroutine *> *s_umap;
    int running_index;
    int c_index;
public:

    Schedule(ucontext_t *main, std::map<int, Coroutine *> *map);

    void coroutine_yield();

    void coroutine_resume(int c_index);

    bool coroutine_end();



};

//协程上下文
class Coroutine {
public:
    ucontext_t c_ctx;
    Func c_func;
    int c_arg;
    enum ThreadState c_state;
    char c_stack[DEFAULT_STACK_SZIE];
    Schedule *c_s;

public:

    //Coroutine(Func func, void *arg);

    Coroutine(Func func, int arg, Schedule *s);

};




