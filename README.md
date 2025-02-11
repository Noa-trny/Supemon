# Supemon

<img src="https://github.com/simonpotel/Supemon/blob/dc7ce2d2567c5f18d53b435cc58f975b509309ed/assets/logo_supemon.png" width="300px" height="300px">


![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-%23008FBA.svg?style=for-the-badge&logo=cmake&logoColor=white)


## Overview
Supemon is a simple C-based game in terminal where players can choose and battle with their Supemons. This project is developed as part of a school assignment at SUPINFO.


## Features
- CLI Supemon in C
- Player name and starter Supemon selection
- Three initial Supemon types: Supmander, Supasaur, Supirtle
- Turn-based battle system with random encounters
- Battle mechanics including move damage, dodging, and statistic modifications
- Experience and leveling system
- In-game shop with item buying/selling (Potion, Super Potion, Rare Candy)
- Supemon Center for healing
- Save/load game functionality
- Capture mechanism for new Supemon
- Random battle rewards (Supcoins and experience)
  
## Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/simonpotel/Supemon
   cd Supemon
   ```
2. Build the project using CMake:
   ```bash
   mkdir build
   cd build
   cmake ..
   make

## Documentation
Please see differents files in `docs/`.
- `battle.md`: Explains battle mechanics, moves, and combat system
- `center.md`: Details Supemon Center healing functionality and services
- `items.md`: Lists available items, their effects, and usage
- `save.md`: Explains save/load game system and file structure
- `project.md`: Explains how the game works with all features
