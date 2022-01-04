#include "PCB.h"

PCB::PCB(int id){

}

PCB::~PCB(){

}

void PCB::JoinWait(){
    joinsem->P();
}

void PCB::JoinRelease(){
    joinsem->V();
}

void PCB::ExitWait(){
    exitsem->P();
}

void PCB::ExitRelease(){
    exitsem->V();
}