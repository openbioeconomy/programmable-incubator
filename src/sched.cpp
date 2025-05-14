#include "sched.h"

// Outside of class
Sched *pointerToClass; // Declare a pointer to testLib class

// Define global handler
static void timerCallback(void* arg) 
{ 
  pointerToClass->timerInterruptHandler(); // Calls class member handler
}

Sched::Sched() 
{
  pointerToClass = this; // Assign current instance to pointer (IMPORTANT!!!)
}

void Sched::begin(double &setpoint) 
{
  _setpoint = &setpoint;
  
  // Create timer 
  const esp_timer_create_args_t my_timer_args = {
    .callback = &timerCallback,
    .name = "My Timer"};
  ESP_ERROR_CHECK(esp_timer_create(&my_timer_args, &_timer_handler));

  // Reset the active_step counter
  step_count = 0;
  active_step = 0;
}

void Sched::append(uint8_t temperature, uint32_t period)
{
  // Add step 
  if (step_count < STEP_LIMIT) {
    _activationList[step_count].temperature = temperature;
    _activationList[step_count].period = period;
    step_count++;
  }
}

void Sched::clear()
{
  for (uint8_t i = 0; i < STEP_LIMIT; i++) {
    _activationList[i].temperature = 0;
    _activationList[i].period = 0; 
  }
  step_count = 0; // Reset the step counter
  active_step = 0;
}

void Sched::timerInterruptHandler()
{
  if(countdownTimer> 0) 
  {
    countdownTimer--;
  }
  else if (active_step + 1 < step_count) {
    active_step++;
    *_setpoint = _activationList[active_step].temperature;
    countdownTimer = _activationList[active_step].period;
  }
}

void Sched::play() 
{
  // Reset the step 
  if (step_count > 0) {
    active_step = 0;
    *_setpoint = _activationList[active_step].temperature;
  }
  
  // Start the timer 
  ESP_ERROR_CHECK(esp_timer_start_periodic(_timer_handler, 1000000));

  // Set the state
  enable_state = true;
}

void Sched::stop() {
  // Reset the step 
  active_step = 0;

  // Set th default setpoint
  *_setpoint = DEFAULT_SETPOINT;

  // Stop the timer
  ESP_ERROR_CHECK(esp_timer_stop(_timer_handler));

  // Indicate the state
  enable_state = false;
}


