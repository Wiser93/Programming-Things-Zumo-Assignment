# Programming-Things-Zumo-Assignment

The scenario motivating this project is that the robot is trying to find/rescue people trapped on a single floor in a building which is filled with smoke. The robot moves through a corridor and people are to be discovered in rooms or in the corridor. When the robot discovers a 'person' it signals back to ‘base’ so that a search party can come in to rescue that person. The robot, however, continues to search, signalling as and when it finds people in other rooms. When the searchDroid reaches the end of the corridor, it turns around and returns to base (by the quickest route possible, but visiting all the locations where it has found people to confirm they have been rescued). 

The controls for the robot are as follows:

w - move forwards

a - rotate left

s - reverse

d - rotate right

space - stop

r - outside a room

c - finished corner/room
