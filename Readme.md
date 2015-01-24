Benchmarking Intrusively Linked Lists vs. std::vec
--------------------------------------------------

Pretty much what is says on the wrapper. The test is simple, fire 50000 projectiles from 2m off the ground at a rate of 500 projectiles per second. When they are created, put them into three lists: renderable objects, physics objects, and all objects. When they "hit the ground", remove them from the three lists.

The linked list implementation uses the open source Unreal linked list code, found here:

https://github.com/Edgarins29/Doom3/blob/master/neo/idlib/containers/LinkList.h

You are free to fork or submit pull requests; improvements to this code are always welcome.

Reults of current code, from the second consecutive run, on a late 2014 MacBook Air:

   $ time ./benchmark_ill
   Minimum: 0.000000
   Maximum: 0.000129
   Average: 0.000005
   Median: 0.000005
   Total Frames Rendered: 18802872
   Total Run Time: 100.663879

   real	1m40.915s
   user	1m29.857s
   sys	0m11.023s

   $time ./benchmark_v
   Minimum: 0.000000
   Maximum: 0.004233
   Average: 0.000002
   Median: 0.000002
   Total Frames Rendered: 36285393
   Total Run Time: 100.664009

   real	1m41.026s
   user	1m20.739s
   sys	0m20.256s

