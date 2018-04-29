#ifndef __H_ELEVATOR_H__
#define __H_ELEVATOR_H__

#define MAX_FLOOR 16
#define MAX_WEIGHT 10
#define STOP_FLOORS_HEAP_ARRAY 2
#include <vector>
#include <set>
#include "heap.h"

class Elevator {
  public:
    Elevator();
    ~Elevator();
    // 模拟电梯的人机交互
    // inside
    void PressButtonFloors(int to_floor);
    void PressButtonOpen();
    void PressButtonClose();
    void PressButtonRing();
    // outside
    void PressButtonCall(int from_floor, int up_down);
    // 电梯的接口
    // run
    void DoCommand();
  private:
    bool _CommandValid();
    void _InsertFloorToHeap(int floor_val, int heap_index);
    void _InsertFloorToTemp(int floor_val);
    void _JudgeDirectionByButton(int floor_val);
    int _GetFloorFromHeap();
    void _Move();
    bool _IsMoveFinish();
    void _DeleteFloorFromHeap();
    void _JudgeDirectionByHeap();
    void _EmergencyStop();
    void _EmergencyRing();
    void _OpenDoor();
    void _CloseDoor();
    void _RingOverWeight();
    void _PrintState();
    bool _FloorNumberValid(int floor_val);
    bool _FloorCallValid(int floor_val, int up_down);
    bool _HeapIndexValid(int heap_index);
    void _ResetFloorButton();
    void _InitFloorButton();
    // variables
    std::vector<int> floor_button_state_;
    bool is_door_open_;
    bool is_moving_;
    bool is_ring_;
    int max_weight_;
    int curr_weight_;
    int curr_floor_;
    int max_floor_; // 0-based
    int direction_;
    Heap<int, std::greater<int> > *stop_floors_;
    int curr_heap_index_;
    std::set<int> temp_floors_;
};

#endif /* ifndef __H_ELEVATOR_H__ */
