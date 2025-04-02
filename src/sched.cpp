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

void Sched::begin(IncuControl &incuControl) 
{
  // Control class
  _incuControl = &incuControl; 

  // Create timer 
  const esp_timer_create_args_t my_timer_args = {
    .callback = &timerCallback,
    .name = "My Timer"};
  ESP_ERROR_CHECK(esp_timer_create(&my_timer_args, &_timer_handler));

  // Reset the step counter
  step_counter = 0;
  step = 0;
}

void Sched::append(uint8_t temperature, uint32_t period)
{
  // Add step 
  if (step_counter < STEP_LIMIT) {
    _activationList[step_counter].temperature = temperature;
    _activationList[step_counter].period = period;
    step_counter++;
  }
}

void Sched::clear()
{
  for (uint8_t i = 0; i < STEP_LIMIT; i++) {
    _activationList[i].temperature = 0;
    _activationList[i].period = 0; 
  }
  step_counter = 0; // Reset the step counter
  step = 0;
}

void Sched::timerInterruptHandler()
{
  if(countdownTimer> 0) 
  {
    countdownTimer--;
  }
  else if (step + 1 < step_counter) {
    step++;
    double setpoint = _activationList[step].temperature;
    _incuControl->setSetpoint(setpoint);
    countdownTimer = _activationList[step].period;
  }
}

void Sched::play() 
{
  // Reset the step 
  if (step_counter > 0) {
    step = 0;
    double setpoint = _activationList[step].temperature;
    _incuControl->setSetpoint(setpoint);
  }
  
  // Start the timer 
  ESP_ERROR_CHECK(esp_timer_start_periodic(_timer_handler, 1000000));  
}

void Sched::stop() {
  // Reset the step
  step = 0;
  double setpoint = _activationList[step].temperature;
  _incuControl->setSetpoint(setpoint);
  // Stop the timer
  ESP_ERROR_CHECK(esp_timer_stop(_timer_handler));
}


