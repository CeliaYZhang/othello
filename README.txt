What each group member did:
Celia implemented the heuristic function and set up the github. 
She set up the constructor and player.hpp. She set up the 
toggling between heuristic and minimax algorithms. 
Loko implemented the minimax tree and integrated the heuristic
function into the minimax tree. He finished the function doMove 
in player.cpp, and performed testing for both algorithms. 
Celia and Loko worked together to improve the algorithm to make
it tournament-ready by introducing things we will discuss in the
following paragraph. 

Improvements:
To make our AI tournament-worthy, we tried to make some changes
to fix the minimax tree. At first, we weren't able to get it working,
resulting in the minimax implementing losing to the ConstantTimePlayer
most of the time when combining it with our heuristic. Thus, we could
not add any of the modifications to do with the minimax. However, we
tweaked our heuristic so that it could defeat ConstantTimePlayer a majority
of the time. Finally, we debugged our minimax tree, and got it to defeat
ConstantTimePlayer consistantly for depth = 4. We combined this with
our heuristic to finish our algorithm. 

