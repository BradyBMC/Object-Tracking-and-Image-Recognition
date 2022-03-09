# OpenCV Starter Code for Working with Qt
This starter code will be used for CRSN 151C and other OpenCV/Qt related projects that involve video processing in OpenCV and Qt.
	
This repository contains:
* This README
* `videos/`: A sample video to use for tracking objects
* opencv_qt_starter: The project code containing all that is necessary for opencv and qt to talk nicely with each other. The classes that are within this include:
    * `MainWindow`: The class in charge of displaying the camera output, it will get the camera values from `VideoProcessor`'s thread and display on the input and output labels.
    * `VideoProcessor`: The class that will initialize the opencv code on a seperate thread getting the camera input, before calling functions within your code that will perform the adjustments on the camera's Mat.

Merge please!
Merge again!
