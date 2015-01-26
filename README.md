SA-Ve
=====

About
-----
SA-Ve (Simple Automatic Vectors) is a library which started as an attempt to
make C vectors look less unweildy, went through a phase of bad ideas, and
eventually turned into something which might be a bit overkill for high
performance projects but still, in the end, makes vectors look a lot more
natural.

SA-Ve also provides vectors of any length, well, anywhere between 0 and
2147483648, just in case you need such vectors.

If you're familiar with SDS you might think that SA-Ve sounds a bit like SDS,
in fact, I first had this idea before I knew about SDS, but seeing SDS in
practice persuaded me to put this idea to (virtual) paper.

How it all works
----------------
Here's a quick example of how to take the cross product of (3, 5, 2) and (-1,
3, 4) and perform a dot product with the output of that cross product and the
vector (10, -4, -1).

```C
vec_dot(vec_cross(vec(3, 3, 5, 2), vec(3, -1, 3, 4)), vec(3, 10, -4, -1));
```

Yes, all of that, on one line, and no memory leaks. How is this possible? You
might ask. Well, every vector stores, in its header, a singly bit value
defining whether it is temporary. A temporary vector, when passed to a
function, will in essence be freed or reused for the result of that function.
This means that the above only uses 3 mallocs, one for each initial vector.

Technicalities
--------------
Right now SA-Ve uses a 32 bit header containing the length (31 bits) and the
temporary state (1 bit).
