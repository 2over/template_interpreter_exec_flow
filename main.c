#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <memory.h>


// 模拟生成一给对象
int new() {
    return 11;
}

// run function new code
int template_new() {
    typedef int (*p_fun)();

    char code[] = {
//            0x55,                       // push %rbp  创建栈帧
//            0x48, 0x89, 0xe5,           // mov %rsp, %rbp
            0xb8, 0x0b, 0x00, 0x00, 0x00, // mov $0xb, %eax
//            0x5d,                       // pop %rbp  弹出栈帧
            0xc3                        // retq

    };

    // 创建一块可读可写可执行的内存区域
    void *temp = mmap(
            NULL, // 映射区的开始地址， 设置为NULL或0表示由系统决定
            getpagesize(), // 申请的内存大小按照内存页对其，这里直接调用函数获取内存页大小
            PROT_READ | PROT_WRITE | PROT_EXEC, // 映射的内存区的权限: 可读可写可执行
            MAP_ANONYMOUS | MAP_PRIVATE, // 映射对象的类型
            -1,
            0
    );

    // 将val函数的机器吗写入内存
    memcpy(temp, code, sizeof(code));

    p_fun fun = temp;
    return fun();
}

int main() {
    int obj = new();
    int obj2 = template_new();


    printf("obj=%d, obj2=%d\n", obj, obj2);
    return 0;
}
