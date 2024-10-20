# 9LinesGenerator

This programm can generate game layouts for the 9Lines game. It exports a json-file that can be red by the app and then the games from the file can be played. 
Games can be generated in three kevels of difficulty:
- **Easy:** Fewer black boxes, generally spread evenly, with some symmetry.
- **Medium:** More black boxes, some clustering with a mix of open and restricted regions.
- **Hard:** More densely packed black boxes, creating narrow regions and difficult segmentations.
## Strcuture of the algotithm

In the following the structure of the algorithm is described to create a 9 by 9 str8ts game. 

1. Start with an empty field of 81 white and empty boxes boxes
2. Use one of the 90 pregenerated game layouts for the game. There are 30 layouts for each difficulty (Easy, medium &hard). The layout is chosen randomly.
3. Next, the numbers are added to the field: All white fields and black number fields are populated with numbers. Here the constraint of streets is kept in mind. All uninterrupted lines or columns of white fields must form a uniterrupted sequence of numbers (it does not have to be in order).
4. Now the numbers are removed with checks that the game still has a unique solution and the number of removed numbers depending on the chosen degree of difficulty.  

## Compile

Use the following command to compile the programm:

```g++ gameGenerator.cpp -o gameGenerator```

## Run

Use the following command to run the compiled bytecode:

```./gameGenerator```

## Future improvements

In this section a number of future imprpovements is listed that can be implemented to improve the algorithm or expand its functionality.