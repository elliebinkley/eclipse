//
// IpcXsi.cpp
//
//  Created on: Apr 6, 2017
//  Author: Burley
// Description : IPC exercises.Ansi-style
//             : 1. One process spawns N processes via fork(). Each of the N processes, spawns M threads where
//                  M > 10 and half the threads are readers and half are writers.
//                  The main process creates X shared regions of random length shared memory, each one protected by a sempahore,, where
//                  X > 3*N*M. It creates a single data structure that describes where each region is and how big
//                  it is in shared memory and the algorithm(s) to be used on the shared memory, e.g. sort/add/multiply
//                  The writer threads access the regions of shared memory that have not been taken by other writers.
//                  They randomly seize a chunk of memory and then randomly write the memory if it has not already
//                  been written. and then release the memory.  Then the seize another chunk of memroy and then perform
//                  one ofthe operations on the memory.  Then the y release it.
//                  Once the last operation is done, then the write a file with the results of the shared memory.
//                  The readers are randomly trying to get access to the shared memory to check the work of the writes
//                  and randomly trying to get access to the set of files to check the work against what is in
//                  the shared memory.  Once all readers have checked all work in all shared memory segments against
//                  the files, then they report success and join the main thread in that process. Then the proicesses
//                  join.
//
//                  Then the final thread checks all the work by itself in a single threaded way.
//                  This is coded in C.
//                  Note that in cygwin the /usr/sbin/cygserver.exe windows service must be started to allow IPC operations.
//                  The cygserver supports persistence of IPC data structures.
//                  See https://cygwin.com/cygwin-ug-net/using-cygserver.html
//                  cygserver supports XSI IPC Message Queues,  XSI IPC Semaphores, and XSI IPC Shared
//                  Memory ( sys/ipc.h, sys/mman.h, sys/sem.h ). These are originally Unix System V and are not posix compliant.
//                  They are kernel space objects, so if the process is killed or terminated unexpected, they can be backed out.
//                  But they are slow since they require a rou nd trip to call into the kernel.
//
//                  <semaphore.h> defines POSIX semaphores, which are designed in such a way that they can be implemented entirely in userspace,
//                  POSIX semaphores also offer you the choice of whether you want a process-local semaphore (for use in a multi-threaded
//                  environment, or even, in some conditions, a signal handler in a single-threaded program) or a process-shared one, and
//                  in the latter case, you also have the choice whether to let the system handle allocating it in a shared namespace by name,
//                  or to obtain shared memory yourself and initialize it in shared memory.
//                  ( See http://stackoverflow.com/questions/11058045/differences-between-semaphore-h-and-sys-sem-h )
//
//                  Also test BoostAtomivc.
//                  Also test various compiler builins that do atomic actions, see
//





