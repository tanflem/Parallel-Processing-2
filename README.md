# Programming-Assignment-2
Problem 1: For problem one I used a condition variable and a mutex to lock the labyrinth. I am not sure that this was the most efficient way to do this, however I followed a long with a tutortial similar to this problem and this was the solution they used. 


Problem 2:

Solution 1: Any guest can come in any time to try and enter. Try and set lock.

Solution 2: Sign on the door guests are responsible for flipping. Spin lock. 

Solution 3: Queue outside the door. Queue lock.  


Pros of Solution 1:
Least amount of work for the minotaur/developer since their is nothing to set up other than letting the guests know only one person in that room at a time.

Cons of Solution 1:
It is really inefficient.
If a guest has to open the door and check just to see if they can go in they waste time they could have used to explore the rest of the castle. 
Constatly having to open the door and hope.

Pros of Solution 2:
More efficient than the first solution since guests can just check the sign instead of having to open the door. 
Allows guests the freedom to do what they want when waiting for the sign to be flipped. 

Cons of Solution 2: 
When the sign gets flipped it could cause a panic at the door if every guest wants to see the vase. 

Pros of Solution 3:
Every guest that wants to see the vase can see it if they wait in the queue. 

Cons of Solution 3: 
Guests waiting in line cannot explore the rest of the castle. 
