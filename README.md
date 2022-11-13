# UP Kirby

## Description

UP Kirby is a 2D platformer where Kirby has to avoid falling into the cliffs in order to be able to arrive at the end of the level. Be careful, throughout your path, you will find enemies, some of them will kill you, and some of them will restart your jumps, be aware of that!

## Key Features

 - Tiled TMX map loading and drawing (orthographics)
 - Map collisions detection (platforms, collectibles and enemies) with Box2D
 - Map navigation: player movement and *flying*
 - Player Walk / Jump / Die / Fly / Victory / Descending animations
 - XML config file loading (player parameters, texture paths and audio paths)
 - Load/Save game state using XML file: Player position
 - Logo screen with fade-in fade-out
 - Title screen with an animation and the option to choose between *2 levels*
 - Ending screen (on dying)
 - Music in all scenes
 - SFX when hiting a coin and when dying
 - Background with parallax
 - DEBUG keys
 
 
## Controls

 - AD for horizontal movement
 - S for descending at higher speed
 - Shift for run
 - Space for jump
 - Flying when pressing space consecutively (up to 3).
 - ESC to close the game
 - Enter to move from Logo screen to Title screen. 
 - Enter to move from Title screen to the game itself.
 - 0 for the character to die inmediatly. 
 - Enter to move from Ending screen (on dying) to the game itself again.
 - Enter to move from winning state to the beggining of the game again.

## Developers

 - Oriol Martín Corella - Main Programmer
 - Pau Argiz - Game Designer


## License

This project is licensed under an unmodified MIT license, which is an OSI-certified license that allows static linking with closed source software. Check [LICENSE](LICENSE) for further details.

Additional Licenses
- Tileset: https://unity.com/legal/as-terms
