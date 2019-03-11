# ARehab
##  Whas is ARehab?

ARehab is a free (as in free speech) application for physical rehab, aided by Augmented Reality technologies and user tracking systems, such as Microsoft Kinect v2. It was created during 2015 as a degree project at the University of Granada and the aim of this project is to serve as a physical rehab system that will be usefull for doctors and physical trainers. It's based on different technologies such as augmented reality and user tracking systems.

There are some screenshots of the early release in the project website: [http://jacobodeharo.github.io/ARehab/](http://jacobodeharo.github.io/ARehab/) 

## Hardware/software requirements

ARehab is currently using Kinect as user tracking device. This obviously means that ARehab system requires a Kinect v2 device, and Kinect SDK v2 installed over Windows 8 (x64), Windows 8.1 (x64) or Windows 10 (x64). However, in a near future, I would like to add more hardware devices for specific body parts, like Leap Motion, commonly used for tracking the hands and fingers. The aim is gradually replacing of all the Microsoft components by their open source alternatives (e.g. [libfreenect2 driver](https://github.com/OpenKinect/libfreenect2)) to get a fully multiplatform system for Windows, OSX and Linux.

### Hardware requirements:
* Kinect for Windows v2 (out of Windows Store) / Kinect for Xbox One + Kinect for Windows adapter (out of store).
* See the Kinect v2 requirements [here](https://msdn.microsoft.com/es-es/library/dn782036.aspx).

### Software requirements:
* [Microsoft Visual Studio Community 2017](https://go.microsoft.com/fwlink/?LinkId=532495&clcid=0x409), I suggest to use it because there isn't CMake makefile.
* [Qt SDK 5 OpenSource + Add-in for Qt 5](http://www.qt.io/download-open-source/)
* [GLM](http://glm.g-truc.net/0.9.7/index.html). It's included in source files.
* OpenGL 4.3 / GLSL 430 -> Used as paint subsystem engine.

### Installation instructions (Development environment)
1. Install Visual Studio Community 2017. (msvc 2017).
2. Install Kinect SDK v2.
3. Install Qt SDK 5. (Compatible with version 5.12) and Qt Visual Studio Tools.
6. Ensure that Qt Version is correctly configured on each project.

That's all!

*NOTE:* ARehab is in an early stage of development, therefore, only the source code and resources needed are distributed. The user interface is localized in Spanish, but i18n will be a feature that is in my TODOs list. 

If you are considering to colaborate or have questions, you can contact me by:
- email: jacobodeharo@gmail.com
- twitter: [`@jacobodeharo`](http://twitter.com/jacobodeharo)
- LinkedIn: https://www.linkedin.com/in/jacobodeharo

