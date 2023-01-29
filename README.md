# UP Kirby

## Description

UP Kirby is a 2D platformer where Kirby has to avoid falling into the cliffs in order to be able to arrive at the end of the level. Be careful, throughout your path, you will find enemies, some of them will kill you, and some of them will restart your jumps, be aware of that!

## New Key Features
 - Game items to recover health and coins to accumulate points
 - Checkpoint autosave with feedback on passing + checkpoints teleportation
 - Optimizacion of the game: Cappable to stable 60 fps without vsync.
 - Optimizacion of the game: All movements normalized with dt
 - Optimizacion of the game: Profiled the game with Optick profiler. Screenshot included
 - GUI: Title Screen Main Menu: Play, Continue, Settigs (music, fx, vsync and fullscreen), credits and exit. 
 - GUI: Gameplay Screen HUD: Player Lives, Collectilbes, Skill Colldown, Timer and HighScore.
 - GUI: Gameplay Screen Pause Menu: Resume, Settings, Back to Title, and Exit

## Key Features

 - Tiled TMX map loading and drawing (orthographics)
 - Map collisions detection (platforms, collectibles and enemies) with Box2D
 - Map navigation: player movement and *flying*
 - Player Walk / Jump / Die / Fly / Victory / Descending animations
 - XML config file loading (player parameters, texture paths and audio paths)
 - Load/Save game state using XML file: Player position and enemies position and state
 - Logo screen with fade-in fade-out
 - Title screen with an animation and the option to choose between *2 levels*
 - Ending screen (on dying)
 - Music in all scenes
 - Background with parallax
 - DEBUG keys
 - Audio Feedback
 - Skill with cooldown
 - Enemies and player with Knockback and FX
 - Enemies with Pathfiding
 - 3 different types fo enemies: Peaceful Walking Enemy Type, Angry Walking Enemy Type and Flying Enemt Type.
 - Destroy Enemies with Kirby Skill 
 
 
## New Controls
 - ESC to pop a menu during the game
 - B to teleport to the next unlocked checkpoint during that game
 - F9 Debug Feature: View Debug from Buttons

## Controls

 - AD: horizontal movement
 - S to descend at higher speed
 - G to use sword skill
 - Shift for run
 - Space for jump
 - Flying when pressing space consecutively (up to 3).
 - ESC to close the game
 - Enter to move from Logo screen to Title screen. 
 - Enter to move from Title screen to the game itself.
 - Enter to move from Ending screen (on dying) to the game itself again.
 - Enter to move from winning state to the beggining of the game again.
 - F3 Start from the beginning of the current level
 - F5 Save the current game state
 - F6 Load the previous state (even across levels)
 - F9 View colliders / logic / paths
 - F10 God Mode (fly around, cannot be killed)
 - F11 Enable/Disable FPS cap to 30

## Developers

 - Oriol Martin Corella - Main Programmer
 - Pau Argiz Gutiérrez - Game Designer


## License

This project is licensed under an unmodified MIT license, which is an OSI-certified license that allows static linking with closed source software. Check [LICENSE](LICENSE) for further details.

Additional Licenses
- Tileset: https://unity.com/legal/as-terms
