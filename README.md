# KORG Berlin - SW code challenge 04/2022
Hello :-)

This is the repository for the KORG Berlin software code challenge, referred to as round #3. 

**Congratulations for making it here!**

The aim of this code challenge is to confront you with a real world problem, which is hopefully fun & hands-on to solve.

Please create a kind of revision control system for your work which you can share with me when you are done. Please use your best practices to demonstrate your way of assessing this problem and document your thoughts and results on the way.

I do not expect more than 2-3 hours of you time on this, so see how far you can get.

Max


## functional implementation
```led.hpp``` defines the interface for an led driver.
Please write the corresponding led.cpp to bring it alive. 

Please look through the comments and pseudo code for requirements and state all your assumptions.

## demonstration
Your second task is to demonstrate the led class in a small main loop on a processor of your choice.
You can assume the following pseudo code to use the led objects:

```
IO::Led LED_PWR;
IO::Led LED_CLIP;
IO::Led LED_SW1;

TimerCallback{
   LED_PWR.update();     
   LED_CLIP.update();
   LED_SW1.update();
}

Setup{
   //initialize LEDs here.
   //set LED_PWR to breathe
   //set LED_SW1 on
   //set LED_CLIP to 20% brightness
   
   //setup TimerCallback frequency
}

MainLoop{
   //when button 1 is pressed, toggle LED_SW1
   //when button 2 is pressed, blink and hold on LED_CLIP
}
```


