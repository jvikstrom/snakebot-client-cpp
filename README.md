# Snakebot Client for C++
Do you find a bug or something that doesn't work as expected? Make an issue, or even better: fix it and make a pull request!

## Editing
Edit the snake at `src/snake.cpp`, primarily the `get_next_move` function.

## Building
Ensure you have the necessary websocket package:
```
git submodule init            
git submodule update --recursive 
```

Then make the project:
```
make
```

## Running 
Run the project via docker:
```
docker-compose run build
docker-compose up snakebot
``` 
