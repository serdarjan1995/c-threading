# c-threading

An application will be written with one server thread and N client threads. N value will be given as a parameter when starting the application.
Client threads will generate a certain number of requests per minute. The number of requests that clients will create will be randomly determined by the application, ranging from 1 to 10.
Requests created by clients will be expressed as a file on the filesystem. Files should be named in TXXX-YYY.txt format.
XXX represents the thread number and YYY represents the request number created by that thread. For example, the file to be created for the 10th request of client number 2 should be named T002-010.txt.
The server thread will process a total of M requests per minute.
If there are more than M requests already, the server thread will wait for the minute to expire and will process the remaining requests in the new minute.
M value will be given as a parameter when starting the application. The server thread will delete the files of the processed requests from the file system.


### RUN

Compile src file with `-pthread` flag  
```
gcc -Wall -pthread thread.c
```

### Demo
```
➜ ./a.out                                                                                             
Client thread count N=5
Server thread per minute capacity M=4
-----
[CLNT-1] T001-001.txt created
[CLNT-1] T001-002.txt created

-----
[CLNT-1] T001-003.txt created
[CLNT-5] T005-001.txt created
[SRV] Current local datetime: Mon Jun 19 00:52:07 2023

-----
[CLNT-3] T003-001.txt created
[CLNT-4] T004-001.txt created
[CLNT-2] T002-001.txt created
[CLNT-1] T001-004.txt created
[SRV] T002-007.txt deleted
[CLNT-1] T001-005.txt created
[CLNT-5] T005-002.txt created
[CLNT-3] T003-002.txt created
[SRV] T002-005.txt deleted
[CLNT-1] T001-006.txt created
[CLNT-3] T003-003.txt created
[CLNT-1] T001-007.txt created
[SRV] T002-004.txt deleted
[CLNT-5] T005-003.txt created
[CLNT-3] T003-004.txt created
[CLNT-4] T004-002.txt created
[CLNT-3] T003-005.txt created
[CLNT-5] T005-004.txt created
[SRV] T002-001.txt deleted
[CLNT-3] T003-006.txt created
[CLNT-4] T004-003.txt created
[CLNT-5] T005-005.txt created
[CLNT-3] T003-007.txt created
[CLNT-4] T004-004.txt created
[CLNT-5] T005-006.txt created
[CLNT-3] T003-008.txt created
[CLNT-3] T003-009.txt created
[CLNT-5] T005-007.txt created
[CLNT-3] T003-010.txt created
[CLNT-5] T005-008.txt created
[CLNT-4] T004-005.txt created
[CLNT-5] T005-009.txt created
[CLNT-4] T004-006.txt created
[CLNT-4] T004-007.txt created
[CLNT-4] T004-008.txt created
```
