# 2D-Grid-Game Maze like game
It's a simple 2D grid game where you move through a maze to get the treasure at the end.

## The mission

- Build a maze from a 2D Array.
- Using the keyboard you can navigate in the maze (you can only stay on paths, you cannot cross walls)
- Get to the end before the end time and progress to the nex level.
- levels 1-3 are predifined but randomly generated after.
- levels get progressively more difficult with added ennemies and shooting mechanics? Coins and collectibles to collect as well as upgrades
- beautifull graphics
- second player?

Base game plan
  -> create a 2D array with the base level
  -> DrawMaze() function based on the array
  -> create a struct for the playable character 
  -> add the endPoint (in the 2D array maybe)
  -> add an enum class for the Direction
  -> implement the movement in the KeyDownEvent()
  -> cout "You won" message


The first level looks like this (starting as a red tile, treasure as a yellow tile)

![maze](assets/img/map.png)
