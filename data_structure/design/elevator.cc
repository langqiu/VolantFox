#include <iostream>
#include <unistd.h>
#include "elevator.h"
using namespace std;

Elevator::Elevator() :
  is_door_open_(false),
  is_moving_(false),
  is_ring_(false),
  max_weight_(MAX_WEIGHT),
  curr_weight_(0),
  curr_floor_(0),
  max_floor_(MAX_FLOOR),
  direction_(0),
  stop_floors_(NULL),
  curr_heap_index_(-1) {
  _InitFloorButton();
  stop_floors_ = new Heap<int, greater<int> >[STOP_FLOORS_HEAP_ARRAY];
}

Elevator::~Elevator() {
  if (stop_floors_ != NULL) {
    delete[] stop_floors_;
    stop_floors_ = NULL;
  }
}

void Elevator::PressButtonFloors(int to_floor) {
  // input detection
  if (!_FloorNumberValid(to_floor)) return;
  // ignore the floor button that has been pressed.
  if (floor_button_state_[to_floor] == 1) return;
  // update floor button
  floor_button_state_[to_floor] = 1;
  // the direction is not zero.
  if (direction_ != 0) {
    bool floor_direction_same = (to_floor-curr_floor_)*direction_ > 0;
    bool floor_direction_same_plus = (to_floor-curr_floor_-1*direction_)*direction_ > 0;
    // the elevator is parked.
    if (!is_moving_) {
      // the floor that is pressed is opposite with the moving direction
      if (!floor_direction_same) return;
      // insert into heap
      _InsertFloorToHeap(to_floor, curr_heap_index_);
    }
    // the elevator is moving.
    else {
      // the floor is not two floors ahead the current floor taking the moving direction into consideration.
      if (!floor_direction_same_plus) return;
      // insert into heap
      _InsertFloorToHeap(to_floor, curr_heap_index_);
    }
  }
  // the direction is zero.
  else {
    // same floor is invalid floor
    if (to_floor == curr_floor_) return;
    // judge the direction since the direction is zero.
    _JudgeDirectionByButton(to_floor);
  }
}

void Elevator::PressButtonOpen() {
  // open when parked and door is open.
  if ((!is_moving_ && is_door_open_) ||
      (!is_moving_ && direction_ == 0)) {
    _OpenDoor();
    _CloseDoor();
  }
}

void Elevator::PressButtonClose() {
  // close when parked and door is open.
  if (!is_moving_ && is_door_open_) {
    _CloseDoor();
  }
}

void Elevator::PressButtonRing() {
  // stop first
  if (is_moving_) {
    _EmergencyStop();
  }
  // ring and sleep
  _EmergencyRing();
}

void Elevator::PressButtonCall(int from_floor, int up_down) {
  // input detection
  if (!_FloorNumberValid(from_floor) ||
      !_FloorCallValid(from_floor, up_down)) return;
  // direction_ is not zero.
  if (direction_ != 0) {
    bool button_direction = up_down*direction_ > 0;
    bool floor_direction_same = (from_floor-curr_floor_)*direction_ > 0;
    bool floor_direction_oppo = (from_floor-curr_floor_)*direction_ < 0;
    bool floor_direction_same_plus = (from_floor-curr_floor_-1*direction_)*direction_ > 0;
    // the elevator is parked.
    if (!is_moving_) {
      // corner case
      if (from_floor == max_floor_ || from_floor == 0) {
        if (curr_floor_ == from_floor && is_door_open_) {
          _OpenDoor();
          _CloseDoor();
        }
        if (button_direction) {
          _InsertFloorToHeap(from_floor, 1-curr_heap_index_);
        }
        else {
          _InsertFloorToHeap(from_floor, curr_heap_index_);
        }
      }
      else {
        // open the door
        if (curr_floor_ == from_floor && is_door_open_) {
          _OpenDoor();
          _CloseDoor();
        }
        // add into the heap which is the same direction.
        if (button_direction && floor_direction_same) {
          _InsertFloorToHeap(from_floor, curr_heap_index_);
        }
        // add into the heap which is the opposite direction.
        if (!button_direction) {
          _InsertFloorToHeap(from_floor, 1-curr_heap_index_);
        }
        // add into the vector.
        if ((button_direction && floor_direction_oppo) ||
            (button_direction && !is_door_open_ && curr_floor_ == from_floor)) {
          _InsertFloorToTemp(from_floor);
        }
      }
    }
    // the elevator is moving.
    else {
      if (from_floor == max_floor_ || from_floor == 0) {
        // button direction
        if (button_direction ) {
          _InsertFloorToHeap(from_floor, 1-curr_heap_index_);
        }
        else {
          _InsertFloorToHeap(from_floor, curr_heap_index_);
        }
      }
      else {
        // add into the heap which is the same direction.
        if (button_direction && floor_direction_same_plus) {
          _InsertFloorToHeap(from_floor, curr_heap_index_);
        }
        // add into the heap which is the opposite direction.
        if (!button_direction) {
          _InsertFloorToHeap(from_floor, 1-curr_heap_index_);
        }
        // add into the vector.
        if (button_direction && !floor_direction_same_plus) {
          _InsertFloorToTemp(from_floor);
        }
      }
    }
  }
  // direction_ is zero.
  else {
    // open the door.
    if (curr_floor_ == from_floor) {
      _OpenDoor();
      _CloseDoor();
    }
    // judge direction
    else {
      _JudgeDirectionByButton(from_floor);
    }
  }
}

void Elevator::DoCommand() {
  // detection before move
  if (_CommandValid()) {
    // TODO
    cout << endl;
    _PrintState();
    // move
    _Move();
    // park
    _OpenDoor();
    _JudgeDirectionByHeap(); // judge direction
    _CloseDoor();
    // TODO
    _PrintState();
  }
}

bool Elevator::_CommandValid() {
  // door is closed and direction_ != 0
  return !is_door_open_ && direction_ != 0;
}

void Elevator::_InsertFloorToHeap(int floor_val, int heap_index) {
  // input detection
  if (!_FloorNumberValid(floor_val) ||
      !_HeapIndexValid(heap_index)) return;
  // no direction no heap_index
  if (direction_ == 0) return;
  // up-floor heap
  if ((direction_ == 1 && heap_index == curr_heap_index_) ||
      (direction_ == -1 && heap_index == 1-curr_heap_index_)) {
    stop_floors_[heap_index].Push(floor_val);
  }
  // down-floor heap
  else {
    stop_floors_[heap_index].Push(max_floor_-floor_val);
  }
}

void Elevator::_InsertFloorToTemp(int floor_val) {
  // input detection
  if (!_FloorNumberValid(floor_val)) return;
  // up-floor heap
  if (direction_ == 1) {
    temp_floors_.insert(floor_val);
  }
  // down-floor heap
  else {
    temp_floors_.insert(max_floor_-floor_val);
  }
}

void Elevator::_JudgeDirectionByButton(int floor_val) {
  // input detection
  if (!_FloorNumberValid(floor_val)) return;
  // direction != 0 no need to judge
  if (direction_ != 0) return;
  // same floor
  if (curr_floor_ == floor_val) return;
  // judge direction
  direction_ = floor_val-curr_floor_>0? 1:-1;
  curr_heap_index_ = (direction_+1)/2;
  _InsertFloorToHeap(floor_val, curr_heap_index_);
}

int Elevator::_GetFloorFromHeap() {
  // valid detection
  if (!_HeapIndexValid(curr_heap_index_) ||
      stop_floors_[curr_heap_index_].Empty()) return -1;
  // get next floor from heap
  int next_floor = stop_floors_[curr_heap_index_].Top();
  // process when it's down-floor heap
  if (direction_ == -1) {
    next_floor = max_floor_-next_floor;
  }
  return next_floor;
}

void Elevator::_Move() {
  // update state
  is_moving_ = true;
  cout << "moving..." << endl;
  // move
  while (!_IsMoveFinish()) {
    sleep(3);
    curr_floor_ += direction_;
    // update button
    floor_button_state_[curr_floor_] = 0;
    cout << "curr_floor: " << curr_floor_ << endl;
  }
}

bool Elevator::_IsMoveFinish() {
  // get the next floor we need to park
  int next_floor = _GetFloorFromHeap();
  // if next floor is not valid, stop the elevator
  if (!_FloorNumberValid(next_floor)) return true;
  // else if not reach yet
  if (curr_floor_ != next_floor) return false;
  // finish and update heap
  _DeleteFloorFromHeap();
  is_moving_ = false;
  cout << "park on the " << curr_floor_ << "th floor." << endl;
  return true;
}

void Elevator::_DeleteFloorFromHeap() {
  // valid detection
  if (!_HeapIndexValid(curr_heap_index_) ||
      stop_floors_[curr_heap_index_].Empty()) return;
  // pop heap
  stop_floors_[curr_heap_index_].Pop();
}

void Elevator::_JudgeDirectionByHeap() {
  // valid detection
  if (!_HeapIndexValid(curr_heap_index_)) return;
  // curr heap is not empty.
  if (!stop_floors_[curr_heap_index_].Empty()) return;
  // curr heap is empty
  // oppo heap is empty
  if (stop_floors_[1-curr_heap_index_].Empty()) {
    direction_ = 0;
    curr_heap_index_ = -1;
  }
  // oppo heap is not empty
  else {
    // change direction
    _ResetFloorButton();
    direction_ = -1*(direction_);
    curr_heap_index_ = (direction_+1)/2;
    // temp vector copy
    if (temp_floors_.size() > 0) {
      for (set<int>::iterator iter=temp_floors_.begin(); iter!=temp_floors_.end(); iter++) {
        if (!_FloorNumberValid(*iter)) continue;
        _InsertFloorToHeap(*iter, 1-curr_heap_index_);
      }
      temp_floors_.clear();
    }
  }
}

void Elevator::_EmergencyStop() {
  cout << "emergency stop!" << endl;
}

void Elevator::_EmergencyRing() {
  // ring and sleep
  is_ring_ = true;
  cout << "ring..." << endl;
  sleep(3600*24*10);
}

void Elevator::_OpenDoor() {
  // open
  if (!is_moving_) {
    is_door_open_ = true;
    cout << "door is open!" << endl;
    sleep(3);
  }
}

void Elevator::_CloseDoor() {
  // door has been closed
  if (!is_door_open_) return;
  // over weight
  while (curr_weight_ > max_weight_) {
    _RingOverWeight();
    sleep(1);
  }
  // close
  sleep(1);
  is_door_open_ = false;
  cout << "door is closed!" << endl;
}

void Elevator::_RingOverWeight() {
  cout << "over weight!" << endl;
}

void Elevator::_PrintState() {
  /*
   *cout << "direction:" << direction_ << "\t"
   *  << "is moving:" << is_moving_ << "\t"
   *  << "door open:" << is_door_open_ << "\t"
   *  << "curr heap:" << curr_heap_index_ << endl;
   */
  // button state
  /*
   *cout << "button: ";
   *for (int i=0; i<=max_floor_; i++) {
   *  if (i != 0) cout << ",";
   *  cout << floor_button_state_[i];
   *}
   *cout << endl;
   */
  // heap state
  cout << "heap[1]: ";
  for (int i=0; i<stop_floors_[1].get_heap_size(); i++) {
    if (i != 0) cout << ",";
    cout << stop_floors_[1].get_elements()[i];
  }
  cout << endl;
  cout << "heap[0]: ";
  for (int i=0; i<stop_floors_[0].get_heap_size(); i++) {
    if (i != 0) cout << ",";
    cout << max_floor_-stop_floors_[0].get_elements()[i];
  }
  cout << endl;
  cout << "temp: ";
  for (set<int>::iterator iter=temp_floors_.begin(); iter!=temp_floors_.end(); iter++) {
    if (iter != temp_floors_.begin()) cout << ",";
    cout << *iter;
  }
  cout << endl;
}

bool Elevator::_FloorNumberValid(int floor_val) {
  return floor_val >= 0 && floor_val <= max_floor_;
}

bool Elevator::_FloorCallValid(int floor_val, int up_down) {
  if (up_down != 1 && up_down != -1) return false;
  if ((floor_val == 0 && up_down == -1) ||
      (floor_val == max_floor_ && up_down == 1)) return false;
  return true;
}

bool Elevator::_HeapIndexValid(int heap_index) {
  return heap_index == 0 || heap_index == 1;
}

void Elevator::_ResetFloorButton() {
  for (int i=0; i<floor_button_state_.size(); i++) {
    floor_button_state_[i] = 0;
  }
}

void Elevator::_InitFloorButton() {
  floor_button_state_.reserve(max_floor_+1);
  for (int i=0; i<=max_floor_; i++) {
    floor_button_state_.push_back(0);
  }
}
