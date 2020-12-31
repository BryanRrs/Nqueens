# N-queens Genetic Algorithm

This repo has the solution to the N-queens problem with a genetic algorithm implemented in c.

At the beginning of the file there are 4 values:
- __SIZE__: The Size of the board. The problem has solution for all sizes except 2 and 3.
- __POP__: The population or amount of individuals created to solve the problem.
- __TOURNAMENT__: The amount of winners to the population tournament. It should be smaller than the 
population and ideally a divisor.
- __MUTATION__: Mutation rate, or the probability that a member of the population will mutate.

Most of the implementation is inspired in this [paper](https://arxiv.org/pdf/1802.02006.pdf#:~:text=Genetic%20Algorithm%20is%20used%20with,Crossover%20are%20described%20in%20detail.)
with some smaller differences like the mutation rate and that I don't use the repo of bad genes, but 
added the leader (the best member of the population) randomly to the next gen.

The default values are small: SIZE = 8, POP = 100 and TOURNAMENT = 10. You can change them however you want.

The program will also show you every 50 iterations the actual fitness, if you see that it doesn't change in a long time (and it __will__ happen some times)
it's better to just stop it and start again.

# How to run

Use your favorite c compiler like gcc or clang. It only uses stdio.h and time.h.
Ex:

```
gcc nqueens.c -o nqueens
```

Then run it like this
```
./nqueens
```

## Disclaimer
This project was made to practice a little of c so I know it must have a lot of ways to make it
better or more 'c-like' but it works so I'm happy with it.
