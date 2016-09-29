# Some weird code explanation:

In the core, map_reports_ is a QList< QPair< CC_Folder *, CC_Report * > > instead of a
QMap< CC_Folder *, CC_Report * >. I have tried to use a QMap or even a std::map but the
insertion during the method make_report block the quit function of QCoreApplication.
That means, you can't leave the program without creating memory leaks. I was not able to
understand how solving this problem. Thus I have chosen to avoid it.

# Memory leaks:

If you valgrind the execution of ComCheck, it is going to show some memory issues. Those
issues come from different places and are caused by Qt :

The usage of QCoreApplication makes valgrind display:

    HEAP SUMMARY:
        in use at exit: 25,923 bytes in 74 blocks
      total heap usage: 564 allocs, 490 frees, 156,796 bytes allocated

    LEAK SUMMARY:
       definitely lost: 192 bytes in 2 blocks
       indirectly lost: 284 bytes in 6 blocks
         possibly lost: 0 bytes in 0 blocks
       still reachable: 25,447 bytes in 66 blocks
            suppressed: 0 bytes in 0 blocks

The usage of QPluginLoader makes valgrind add:

    HEAP SUMMARY:
        in use at exit: 2,011 bytes in 9 blocks
      total heap usage: 1,185 allocs, 1,176 frees, 197,433 bytes allocated

    LEAK SUMMARY:
       definitely lost: 96 bytes in 1 blocks
       indirectly lost: 432 bytes in 4 blocks
         possibly lost: 0 bytes in 0 blocks
       still reachable: 1,483 bytes in 4 blocks
            suppressed: 0 bytes in 0 blocks

The execution of the .exec() of QCoreApplication makes valgrind add then:

    HEAP SUMMARY:
        in use at exit: 156 bytes in 4 blocks
      total heap usage: 7 allocs, 3 frees, 356 bytes allocated

    LEAK SUMMARY:
       definitely lost: 0 bytes in 0 blocks
       indirectly lost: 0 bytes in 0 blocks
         possibly lost: 0 bytes in 0 blocks
       still reachable: 156 bytes in 4 blocks
            suppressed: 0 bytes in 0 blocks

At the end, valgrind display this report :

    HEAP SUMMARY:
        in use at exit: 28,090 bytes in 87 blocks
      total heap usage: 1,772 allocs, 1,685 frees, 356,296 bytes allocated

    LEAK SUMMARY:
       definitely lost: 288 bytes in 3 blocks
       indirectly lost: 716 bytes in 10 blocks
         possibly lost: 0 bytes in 0 blocks
       still reachable: 27,086 bytes in 74 blocks
            suppressed: 0 bytes in 0 blocks
