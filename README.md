# vmpc

Using cmake-gui to create a Visual Studio or Xcode project, the whole thing should compile without problems and produce a standalone EXE, a VST2 and a VST3 plugin on Windows, and an .app on OSX. The Debug build gives crap performance, so make sure to set it to Release for actual use.

You need Boost to be installed. I'm using 1.64 on Windows and 1.65 on OSX, but I think many versions will work fine.

Logfile when running vMPC is located in /Users/JohnDoe/vMPC/.

You will also need this repo to be put in the correct dir:
https://github.com/izzyreal/vmpc-home-dir

For keyboard mapping see https://github.com/izzyreal/vmpc/raw/master/keyboard.txt

There's also an Unreal Engine 4 plugin in development here:
https://github.com/izzyreal/vmpc-unreal-plugin

Some experiments for the 3D version:
https://www.youtube.com/watch?v=led-x_qqzL8&index=6&list=PL8f3Xp_9Hpr6kAiRlKq_ToWYBkHQw1HL4

Screenshot of 2D version:
![alt text](https://i.imgur.com/42mrtk3.jpg)
