#include <iostream>
#include "elevator.h"
#include <pthread.h>
using namespace std;

Elevator elevator;

void *input_command(void *args) {
  string command;
  while (true) {
    if (cin>>command) {
      int floor_val = atoi(command.c_str());
      string command_type = command.substr(command.size()-1);
      if (command_type.size() != 1) continue;
      char command_ch = command_type[0];
      switch (command_ch) {
        case 'u' : {
                     elevator.PressButtonCall(floor_val, 1);
                     break;
                   }
        case 'd' : {
                     elevator.PressButtonCall(floor_val, -1);
                     break;
                   }
        case 'f' : {
                     elevator.PressButtonFloors(floor_val);
                     break;
                   }
        case 'o' : {
                     elevator.PressButtonOpen();
                     break;
                   }
        case 'c' : {
                     elevator.PressButtonClose();
                     break;
                   }
        case 'r' : {
                     elevator.PressButtonRing();
                     break;
                   }
        default : {
                   ;
                  }
      }
    }
  }
}

int main(int argc, char *argv[])
{
  pthread_t pid;
  int ret = pthread_create(&pid, NULL, input_command, NULL);
  if (ret != 0) { cerr << "error: " << ret << endl; }
  while (true) {
    elevator.DoCommand();
  }
  pthread_exit(NULL);
  return 0;
}
