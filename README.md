# Distributed Transparent Memory

Many nodes shared their local chunk of memory to form a big monolithic chunk of memory. Suppose there are 4 HDD each of which has a 2GB capacity working collectively to present a monolithic contiguous 8GB of HDD
to the user. Memory is distributed across nodes, but user is not aware of this distributedness. This is what Tannenbaum called Transparency. Such a memory model is used when volumnious amounts of data needs to be stored on several machines. For example, Hadoop Distributed File Systems (HDFS) is built on this model. In this project, I will build the distributed memory while the user would not have to keep track of the information about where data is located or being fetched from.

Regarding project deployment, I simulate 6 node topology by running the same process multiple times in different terminals of the same machine. So, we have a 6 node ring topology

User can perform two operations on each node:
* **PUT K X**, where K and X are two integers K=key and X=Value.
* **GET K**, where K is an integer, returns X for K.

Originator is a node on which the user has issued put/get request.

In order to run the code:
./node.out 127.0.0.1 <Own UDP port> <Sucessor UDP prot> <Own TCP Port>
