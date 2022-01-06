#ifndef SEM_H
#define SEM_H
#include "synch.h"
#include "string.h"

class Sem
{
private:
    char name[50];
    Semaphore *sem;

public:
    // Tạo Semaphore để quản lý
    // khởi tạo đối tượng Sem. Gán giá trị ban đầu là null
    // nhớ khởi tạo bm sử dụng
    Sem(char *na, int i)
    {
        strcpy(this->name, na);
        sem = new Semaphore(name, i);
    }
    ~Sem()
    {
        // hủy các đối tượng đã tạo
        delete sem;
    }
    void wait()
    {
        sem->P();
    }
    void signal()
    {
        sem->V();
    }
    char *GetName()
    {
        return name;
    }
};
#endif