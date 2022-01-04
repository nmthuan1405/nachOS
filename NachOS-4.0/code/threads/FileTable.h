#include "bitmap.h"
#include "stdio.h"
#define MAX_FILES 10

typedef int OpenFileID;

class FileTable
{
    private:
        Bitmap* bmp;    //array stores state of each file (in use or not)
        FILE* file[MAX_FILES];
        int* mode;  //array stores open mode of each file
    public:
        FileTable();
        ~FileTable();
        int CreateFile(char* name);
        OpenFileID Open(char* name, int type);
        int Close(OpenFileID id);
        int Read(char* buffer, int charcount, OpenFileID id);
        int Write(char* buffer, int charcount, OpenFileID id);
};