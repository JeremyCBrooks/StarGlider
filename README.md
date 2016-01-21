# Star Glider
A cross platform mobile game with one-tap play mechanics (think "Flappy Bird").
<img src="https://raw.github.com/JeremyCBrooks/StarGlider/blob/master/assets_src/screenshot1.PNG" alt="screenshot" width="200">

##What is this?
This was created as a for-fun project over a weekend in February of 2014, at the height of the Flappy Bird craze. The goal was to gain mobile development experience and go through the process of publishing an app to the iOS app store (ended up going for Android as well thanks to help from my good friend Costa!).

The interesting things about this project are:
 - You get to see the *full source code and all assets* from an actual published mobile game
   - https://itunes.apple.com/us/app/star-glider/id824529923?mt=8
   - https://play.google.com/store/apps/details?id=com.brooks.jeremy.starglider
 - See how cross platform development can be done (targets Windows PC, iOS, and Android with a large amount of shared code)
 - No frameworks, just pure C++ with a small amount of Java and Objective-C (just enough to bootstrap the app on Android or iOS respectively)
 - OpenGL for fast 2D graphics
 - AdMob and iAD integration
 - See how a simple "Flappy Bird" type game works under the hood
 - **Feel free to use this code as the basis for your own game** (just give credit please)

##Get started
The "build" folder contains the projects and source code specific to each platform. The core game code is located in "src/core".
 - On Windows, you will need Visual Studio 2013 or 2015 (one of the free editions should work fine). Browse to "build/win" and open star_catcher.sln. Everything should build and run with no special instructions.
 - On Mac, you will need the latest version of XCode. Browse to "build/ios/star catcher" and open Star Glider.xcodeproj.
 - The Android build will take a bit more work. Once you have an Android dev environment set up, you will first need to build the jni code. Go into "build/android/jni" and read the BUILD_INSTRUCTIONS document (assumes you are building on Windows, also pay attention to paths because they will likely be different for you). After the jni code is built, you should be able to load the Eclipse project in "build/android" and build the Java code.
