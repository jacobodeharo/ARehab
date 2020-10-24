# ARehab
##  Whas is ARehab?

ARehab is a free (as in free speech) application for physical rehab, aided by Augmented Reality technologies and user tracking systems, such as Microsoft Kinect v2. It was created during 2015 as a degree project at the University of Granada and the aim of this project is to serve as a physical rehab system that will be usefull for doctors and physical trainers or therapist. 

There are some screenshots of the early release at the project website: [http://jacobodeharo.github.io/ARehab/](http://jacobodeharo.github.io/ARehab/) 

## Hardware/software requirements

ARehab requires Kinect v2 as user tracking device and Kinect SDK v2 installed over Windows 8 (x64), Windows 8.1 (x64) or Windows 10 (x64).

### Hardware requirements:
* Kinect for Windows v2 (out of Windows Store) / Kinect for Xbox One + Kinect for Windows adapter (out of store). See the Kinect v2 requirements [here](https://msdn.microsoft.com/es-es/library/dn782036.aspx).

### Software requirements:
* [Microsoft Visual Studio Community 2017](https://visualstudio.microsoft.com/es/thank-you-downloading-visual-studio/?sku=Community&rel=15), I suggest to use it because there isn't CMake makefile. Sorry for that!
* [Qt SDK 5 OpenSource + Add-in for Qt 5](http://www.qt.io/download-open-source/).
* [GLM](http://glm.g-truc.net/0.9.7/index.html). It's included in source files.
* OpenGL 4.3 / GLSL 430 -> Used as paint subsystem engine.

### Installation instructions (Development environment)
1. Install Visual Studio Community 2017. (msvc 2017).
2. Install Kinect SDK v2.
3. Install Qt SDK 5. (Compatible with version 5.12.x) and Qt Visual Studio Tools for Microsoft Visual Studio 2007.
6. Ensure that Qt Version is correctly configured on each project.

*NOTE:* ARehab is in an early stage of development, therefore, only the source code and resources needed are distributed. 

If you are considering to colaborate or you have any question, you can contact me by:
- email: jacobodeharo@gmail.com
- LinkedIn: https://www.linkedin.com/in/jacobodeharo

