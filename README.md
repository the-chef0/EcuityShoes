# Ecuity Shoes Software
This code is meant to be run on the ESP32 microcontroller as a part of the smart shoes with vibration motors. In it's current state, there is a demo file that we used for the 5G Hub demo and a more general file that isn't fully complete. Make sure your Arudino IDE is configured for the ESP32 before attempting to run.

## 5G Hub demo file
This one controls only the top left and top center motor, so it assumes that the string it receives will only have two numbers from the two segments of the depth image. It also assumes that there are only 3 distance categories, i.e. expects only integers from 0 to 2 in the string.

## Main microcontroller file
This one attempts to be a more general one, accepting strings of length 9 for all 9 motors in a single shoe. It assumes 6 distance categories are being used. But it doesn't really work properly because there was no time to finish it. It might serve as a good starting point to finish it or you might find a better approach to do it. This one is well commented so you should be able to figure out how it works pretty easily.

The string is meant to be sent to the microcontroller via Bluetooth from the [Android app](https://github.com/the-chef0/EcuityGlasses).
