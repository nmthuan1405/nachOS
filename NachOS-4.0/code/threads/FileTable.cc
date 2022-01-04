#include "FileTable.h"

FileTable::FileTable()
{
    this->bmp = new Bitmap(MAX_FILES);
    this->bmp->Mark(0);
    this->bmp->Mark(1);

    this->mode = new int[MAX_FILES];
    for(int i = 0; i < MAX_FILES; i++)
    {
        this->mode[i] = -1;
    }
}

FileTable::~FileTable()
{
    if(this->bmp != NULL)
        delete[] this->bmp;
    for(int i = 2; i < MAX_FILES; i++)
    {
        if(this->file[i])
            fclose(this->file[i]);
    }
    if(this->mode != NULL)
        delete[] this->mode;
}