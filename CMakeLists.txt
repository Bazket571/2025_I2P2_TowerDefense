cmake_minimum_required(VERSION 3.27)
project(2025_I2P2_TowerDefense)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)
include_directories(Resource)

#Using namespace to copy libraries

add_executable(2025_I2P2_TowerDefense
    Bullet/Bullet.cpp
    Bullet/Bullet.hpp
    Bullet/HomingBullet.cpp
    Bullet/HomingBullet.hpp
    Bullet/FireBullet.cpp
    Bullet/FireBullet.hpp
    Bullet/LaserBullet.cpp
    Bullet/LaserBullet.hpp

    # TODO HACKATHON-3 (3/3): Add the new enemy source files here
    Enemy/Enemy.cpp
    Enemy/Enemy.hpp
    Enemy/PlaneEnemy.cpp
    Enemy/PlaneEnemy.hpp
    Enemy/SoldierEnemy.cpp
    Enemy/SoldierEnemy.hpp
    Enemy/TankEnemy.cpp
    Enemy/TankEnemy.hpp

    Engine/Allegro5Exception.cpp
    Engine/Allegro5Exception.hpp
    Engine/AudioHelper.cpp
    Engine/AudioHelper.hpp
    Engine/Collider.cpp
    Engine/Collider.hpp
    Engine/GameEngine.cpp
    Engine/GameEngine.hpp
    Engine/Group.cpp
    Engine/Group.hpp
    Engine/IControl.cpp
    Engine/IControl.hpp
    Engine/IObject.cpp
    Engine/IObject.hpp
    Engine/IScene.cpp
    Engine/IScene.hpp
    Engine/LOG.cpp
    Engine/LOG.hpp
    Engine/Point.cpp
    Engine/Point.hpp
    Engine/Resources.cpp
    Engine/Resources.hpp
    Engine/Settings.cpp
    Engine/Settings.hpp
    Engine/Sprite.cpp
    Engine/Sprite.hpp
    Engine/spine/spine.cpp
    Engine/spine/spine.hpp

    # TODO HACKATHON-2 (3/3): Add the new scene source files here
    Scene/LoseScene.cpp
    Scene/LoseScene.hpp
    Scene/PlayScene.cpp
    Scene/PlayScene.hpp
    Scene/ScoreScene.cpp
    Scene/ScoreScene.hpp
    Scene/StageSelectScene.cpp
    Scene/StageSelectScene.hpp
    Scene/SettingsScene.cpp
    Scene/SettingsScene.hpp
    Scene/StartScene.cpp
    Scene/StartScene.h
    Scene/WinScene.cpp
    Scene/WinScene.hpp

    Turret/LaserTurret.cpp
    Turret/LaserTurret.hpp
    Turret/MachineGunTurret.cpp
    Turret/MachineGunTurret.hpp
    Turret/Turret.cpp
    Turret/Turret.hpp
    Turret/DefenderTurret.cpp
    Turret/DefenderTurret.hpp
    Turret/TurretButton.cpp
    Turret/TurretButton.hpp
    Turret/HomingTurret.cpp
    Turret/HomingTurret.hpp

    UI/Animation/DirtyEffect.cpp
    UI/Animation/DirtyEffect.hpp
    UI/Animation/ExplosionEffect.cpp
    UI/Animation/ExplosionEffect.hpp
    UI/Animation/Plane.cpp
    UI/Animation/Plane.hpp

    UI/Component/Image.cpp
    UI/Component/Image.hpp
    UI/Component/ImageButton.cpp
    UI/Component/ImageButton.hpp
    UI/Component/Label.cpp
    UI/Component/Label.hpp
    UI/Component/Slider.cpp
    UI/Component/Slider.hpp
    
    main.cpp
    "Turret/FreezeTurret.cpp"  
    "3D/Object3D.cpp" "3D/Group3D.cpp" "3D/Billboard.cpp"
    "Entities/Entity.cpp" "Entities/Operator.cpp" "Entities/Operators/Amiya.cpp" "Entities/Enemy.cpp" "Entities/Enemies/Slime.cpp")

#add_custom_command(TARGET 2025_I2P2_TowerDefense POST_BUILD
#                  COMMAND ${CMAKE_COMMAND} -E copy_directory
#                    ${CMAKE_SOURCE_DIR}/Resource $<TARGET_FILE_DIR:2025_I2P2_TowerDefense>/Resource)
file(COPY ${CMAKE_SOURCE_DIR}/Resource DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
set(TINYGLTF_INSTALL OFF CACHE INTERNAL "" FORCE)
set(TINYGLTF_BUILD_LOADER_EXAMPLE OFF CACHE INTERNAL "" FORCE)
set(TINYGLTF_HEADER_ONLY OFF CACHE INTERNAL "" FORCE)
add_subdirectory(tinygltf)
add_subdirectory(lunasvg)
add_subdirectory(spine-runtimes/spine-cpp)
target_link_libraries(2025_I2P2_TowerDefense lunasvg::lunasvg spine-cpp tinygltf)

IF (WIN32)
    find_package(allegro REQUIRED PATHS allegro)
    if(MSVC)
        target_compile_options(2025_I2P2_TowerDefense PUBLIC /ZI)
    endif()
    add_custom_command(TARGET 2025_I2P2_TowerDefense POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy_if_different
                            $<TARGET_FILE:Allegro::allegro_monolith>
                            $<TARGET_FILE_DIR:2025_I2P2_TowerDefense>)
    target_link_libraries(2025_I2P2_TowerDefense Allegro::allegro_monolith)
ELSE()
    find_package(Allegro)
    set(ALLEGRO5_LIBRARIES 
        Allegro::allegro 
        Allegro::allegro_main 
        Allegro::allegro_font 
        Allegro::allegro_ttf 
        Allegro::allegro_dialog 
        Allegro::allegro_primitives 
        Allegro::allegro_image 
        Allegro::allegro_audio 
        Allegro::allegro_acodec
        Allegro::allegro_color)
    target_link_libraries(2025_I2P2_TowerDefense ${ALLEGRO5_LIBRARIES})
ENDIF()