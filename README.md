# OMOEngine
About:
      This is a game engine utilisng DirectX11 and Direct2D to create a 2D/3D game engine, it is to be used as a learning tool for me to expand my knowledge gained from university and in my own time after graduating. It will utilise an archived unpinned respository (DirectX11Engine - old) to reuse elements that I feel are appropiate, whilst redesigning and/or reimplementing them to reflect a more 2020 modern C++; as well as more closely adhere to https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines. I had started updating the old repository but decided a fresh start would allow me to more accurately reflect what the engine is capable of and documemt updates more throughly as I go forward.

Engine Framework:
    Is the system framework it is the hub of the engine and it stores and initialises each module component for use within the engine. 
Current Modules:-
Input
  Raw input handling:
    For keyboard and mouse utilising windows messaging, when I get to enabling controller support I will be using Xinput to handle this due to the deprecation of Direct Sound.

Renderer:-
    Is the rendering module it prepares the D3D and D2D devices for use to create graphics of various form for the engine.
    It has:
      D3DMgr manager - wrapper for initialising the Direct 3D device
      D2DMgr manager - wrapper for initialsing Direct 2D device

Graphical features:-
      Sprite - it utilises DirectWrite and D2D device utilising the windows imaging component library to draw 2D sprites.
      Animated Sprite - Inherits from the Sprite class, with additional functionality to create flipbook art style sprite sheet animations.
      Text - it utilises DirectWrite and the D2D device to draw text formats and layouts
      
Miscellaneous features:-
      Timer - a high performance window's timer for use within the engine
      State Mgr - is used to encapsulate the creation of things like graphical assets
            Game State - is an abstract base class that is used to instantiate states 
                  Debug state - is the state that is used to test new features
      

