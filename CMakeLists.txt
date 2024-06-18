cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

set(PROJECT_NAME HMM)
project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} 
    src/main.cpp

    src/Renderer/ShaderProgram.cpp
    src/Renderer/ShaderProgram.h
    src/Renderer/Texture2D.cpp
    src/Renderer/Texture2D.h
    src/Renderer/Sprite.cpp
    src/Renderer/Sprite.h
    src/Renderer/SpriteAnimator.cpp
    src/Renderer/SpriteAnimator.h

    src/Renderer/VertexBuffer.cpp
    src/Renderer/VertexBuffer.h
    src/Renderer/IndexBuffer.cpp
    src/Renderer/Indexbuffer.h
    src/Renderer/VertexArray.cpp
    src/Renderer/VertexArray.h
    src/Renderer/VertexBufferLayout.cpp
    src/Renderer/VertexBufferLayout.h
    src/Renderer/Renderer.cpp
    src/Renderer/Renderer.h

    src/Resources/ResourceManager.cpp
    src/Resources/ResourceManager.h
    src/Resources/stb_image.h

    src/System/Timer.cpp
    src/System/Timer.h

    src/Physics/PhysicsEngine.cpp
    src/Physics/PhysicsEngine.h

    src/Game/Game.cpp
    src/Game/Game.h

    src/Game/AIcomponent.cpp
    src/Game/AIcomponent.h

    src/Game/GameObject/IGameObject.cpp
    src/Game/GameObject/IGameObject.h
    src/Game/GameObject/Hero.cpp
    src/Game/GameObject/Hero.h
    src/Game/GameObject/BrickRoad.cpp
    src/Game/GameObject/BrickRoad.h
    src/Game/GameObject/House.cpp
    src/Game/GameObject/House.h
    src/Game/GameObject/HouseLeftOne.cpp
    src/Game/GameObject/HouseLeftOne.h
    src/Game/GameObject/HouseLeftTwo.cpp
    src/Game/GameObject/HouseLeftTwo.h
    src/Game/GameObject/HouseRightOne.cpp
    src/Game/GameObject/HouseRightOne.h
    src/Game/GameObject/HouseRightTwo.cpp
    src/Game/GameObject/HouseRightTwo.h
    src/Game/GameObject/Water.cpp
    src/Game/GameObject/Water.h
    src/Game/GameObject/Border.cpp
    src/Game/GameObject/Border.h
    src/Game/GameObject/Arrow.cpp
    src/Game/GameObject/Arrow.h
    src/Game/GameObject/CloseCombat.cpp
    src/Game/GameObject/CloseCombat.h
    src/Game/GameObject/DistantCombat.cpp
    src/Game/GameObject/DistantCombat.h

    src/Game/GameState/IGameState.cpp
    src/Game/GameState/IGameState.h
    src/Game/GameState/Level.cpp
    src/Game/GameState/Level.h
    src/Game/GameState/ShopScreen.cpp
    src/Game/GameState/ShopScreen.h
    src/Game/GameState/FightScreen.cpp
    src/Game/GameState/FightScreen.h
)

add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} glad)

add_subdirectory(external/glm)
target_link_libraries(${PROJECT_NAME} glm)

include_directories(external/rapidjson/include)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
                    COMMAND ${CMAKE_COMMAND} -E copy_directory 
                    ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)