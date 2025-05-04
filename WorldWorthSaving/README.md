<img width="1222" alt="Screenshot 2025-03-30 at 23 31 33" src="https://github.com/user-attachments/assets/5160f6ee-2d94-4b08-9384-ea4c141c2eb6" />

# This is Jam's Escape! 🐷

A little jumping game build for the Hive's March Gamejam. Total of 22 hours were used to build the game. <br>
Features: <br> Animated sprites <br> Moving enemies <br> Double jump logic based on how long 'space' is pressed <br> music <br> Enemy spawn from death point logic <br> Point counter <br> All features, graphs and music made by me <br>
Made in C++ and SFML (version 2.6.s)

## How to build?

With make (on mac):

1. clone the repo
2. Download SFML 2.6.2
3. On the root use
   ```bash
   make

   ./JamsEscape
   ```

5. To clean 
   ``` bash
   fclean
   ```

With Cmake(on linux):

1. Clone the repo
2. Download SFML 2.6.2 cource code from 
	https://www.sfml-dev.org/download/sfml/2.6.2/
3. Add SFML on the root (name the folder SFML)
4. From the root:

   ```bash
   mkdir build
   cd build
   cmake ..
   make

   ./JamsEscape
   ```

3. After building, if you get an error like Library not loaded: libsfml-graphics.2.6.dylib, try this:
```
cp SFML/lib/libsfml-*.dylib .
./JamsEscape
```

4. To clean

   ``` rm -rf build/ ```

Have fun playing!
