VAV
===

About
-----
Very Awesome Vectors (VAV) is an attempt at implementing vectors in C, but
doing so in a manner that doesn't look like something a dog chewed on.

Currently vectors up to 2147483648 in length are possible, but the standard
vec() command only allows for a maximum of 62 because of a limitation with how
the CPP works. However, it is unlikely that anyone will ever need longer
vectors.

If you're familiar with SDS you might think that VAV sounds a bit like SDS, in
fact, I first had this idea before I knew about SDS, but seeing SDS in practice
persuaded me to put this idea to (virtual) paper.

Quick Run-down
--------------
Copy vec.c and vec.h into your project, use vec(your, vector, here) to create a
new vector, this returns a float pointer, this is to use array indices with the
vectors.

`vec_cross` and `vec_dot` should be self explanatory for anyone who knows
anything about vectors (`vec_cross` only works on vec3). They both take two
parameters and dot / cross the respective vectors together.

`vec_len` will return the length of the vector. (`vec_dot(vector, vector)` is
an easy way to get the length squared.)

`vec_addm` takes three parameters, `vector_a`, `vector_b` and `multiplier`.
`vec_addm` returns `vector_a` + `vector_b` * `multiplier`.

`vec_blank` takes one parameter of length and creates a completely zeroed
vector of that length.

`vec_new` is the raw function for creating vectors, it takes a parameter of
length, and a float pointer containing the initial values. `vec_del` can then
be used to deallocate the resulting vector.

`vec_deltemp` will only delete temporary vectors and currently has little use.

`vav_temp` and `vav_perm` set the vector to be temporary or permament
respectively.

Quick Example
-------------
Here's a quick example of how to take the cross product of (3, 5, 2) and (-1,
3, 4) and perform a dot product with the output of that cross product and the
vector (10, -4, -1).

```C
vec_dot(vec_cross(vec(3, 5, 2), vec(-1, 3, 4)), vec(10, -4, -1));
```

Yes, all of that, on one line, and no memory leaks. "How is this possible?" You
might ask. Well, every vector stores, in its header, a singly bit value
defining whether it is temporary. A temporary vector, when passed to a VAV
function, will be freed or reused for the result of that function. This means
that the above only uses 3 mallocs, one for each initial vector.

Technicalities
--------------
Right now VAV uses a 32 bit header containing the length (31 bits) and the
temporary state (1 bit).

The implementation of the vec() macro means that it can figure out how many
arguments you pass it, however, it means that it is limited to just 62 units
length and might make it limited to GCC.

Future Plans
------------
There are future plans to expand the testing (to catch all the bugs) and make
VAV use a 64 bit header.
There are also plans to rename all the functions into something shorter and
more vav oriented, for example: `vav_c, vav_d, vav_l, vav_a and vav` for the
most used stackable functions. Or possible even `v_c, v_d, v_l and v` but
really, this is a drop in solution, want something different? Just changed it.
