#include <windows.h> 
#include <iostream>
#include <process.h>
#include "thunk.h"

using namespace  std;


class Test {
public:
    Test() {

    }

    bool CreateNewThread() {
        // 重点
        void* func = thunk_.Stdcall(this, Thunk::GetMemberFxnAddr(&Test::ThreadProc));

        HANDLE h = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)func, NULL, 0, NULL);

        return h != NULL;
    }

private:
    unsigned int __stdcall ThreadProc(void* arg) {
        printf("ThreadProc\n");

        return 0;
    }

private:
    Thunk thunk_; // 重点
};

int main() {
    Test t;
    t.CreateNewThread();

    getchar();
    return 0;
}
