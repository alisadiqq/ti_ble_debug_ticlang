# Introduction 
This branch is show how to build TI project with CMake + TICLANG + TI sysconfig

# Getting Started
After some research, found that TI is using sysconfig tool to generate some code, and syntax/parameter of TICLANG has some difference comparing with GCC.
In each demo, there is makefile which is can be run and you can just run "make" to build, so, this makefile shows all required parameters for TICLANG and how to call sysconfig
Thus, to unify in BAT, I convert this makefile to CMakeLists.txt.
Code generating is in build.sh

Now, the build pass and the built image can run properly. (Only test basic_ble_off_chip image)


# Build and Test
Now, user can only build it from Git-Bash
1. Open Git-bash
2. Switch to demo code directory
   $ cd simplelink_lowpower_f3_sdk_8_40_02_01/examples/rtos/LP_EM_CC2340R53/ble5stack/basic_ble

3. Because it depends on TICLANG and TI Sysconfig.
   Please open build.sh and CMakeLists.txt to change the path before build.
   Build
   $ ./build.sh -d

Note:


# Contribute
Kent Lee (kent_lee@bat.com)

If you want to learn more about creating good readme files then refer the following [guidelines](https://docs.microsoft.com/en-us/azure/devops/repos/git/create-a-readme?view=azure-devops). You can also seek inspiration from the below readme files:
- [ASP.NET Core](https://github.com/aspnet/Home)
- [Visual Studio Code](https://github.com/Microsoft/vscode)
- [Chakra Core](https://github.com/Microsoft/ChakraCore)
