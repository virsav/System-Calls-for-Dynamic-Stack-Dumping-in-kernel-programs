*****************************************************************************************************************************************************

CSE530: EOSI Assignment 4

Name: Viraj Savaliya

*****************************************************************************************************************************************************

How the patch was created:


From outside the kernel source code, the patch file was created using the following command
diff -uNr kernel.old kernel > viraj.patch

kernel.old is the original source code.
kernel the modified source code for assignment 4.

*****************************************************************************************************************************************************

How to apply patch and build kernel:


from within the root folder of the kernel source code type:
patch -p1 < viraj.patch

NOTE: the root directories name should be kernel and before you patch please transfer the patch file in the root directory of kernel source code.

build the kernel using the following commands in same order.
1. export PATH="/opt/iot-devkit/1.7.2/sysroots/x86_64-pokysdk-linux/usr/bin/i586-poky-linux/:$PATH"
2. ARCH=x86 LOCALVERSION= CROSS_COMPILE=i586-poky-linux- make -j4
3. Now on the terminal you will be asked to enable the CONFIG_DYNAMIC_DUMP_STACK, press 'y' or 'n' according to you and then press 'enter'
4. ARCH=x86 LOCALVERSION= INSTALL_MOD_PATH=../galileo-test CROSS_COMPILE=i586-pokylinux-linux make modules_install
5. cp arch/x86/boot/bzImage ../galileo-test/

NOTE: you must have a directory just outside the kernel root directory name 'galileo-test'

Now replace remove the SD card from the galileo board and then replace the bzimage file in the SD card with the bzimage present in the galileo-test folder.
Reboot your galileo board to boot it up with the new changes you made to the source code.

*****************************************************************************************************************************************************

How to make & run the user program:


In Linux Ubuntu=
On the command line type 'make' 
An executbale binary will be created for the user progam named test_program
(To delete the binary type 'make clean')

Transfer the executable binary onto the galileo board using=
sudo scp <filename> root@<inet_address>:/home/root

On the Galileo board terminal=
To run the user test program on the command line type:
chmod 777 test_program
./test_program

*****************************************************************************************************************************************************

User program testing explanation:

The user program total tests for 3 symbol names and 1 random symbol.

First the user program tests for some random symbol name which should always throw an error
Second, the user program tests for SYMBOL_NAME1 and checks if the rmdump should be allowed always as it is done by the owner.
Third, the user program tests for SYMBOL_NAME2 which is insdump by the owner and hence, its 2 childs should not be allowed to do rmdump.
	when its two child or threads created by child try to call the SYMBOL_NAME2 it should not do dump_stack in mode 1.
	For threads created by owner, dump_stack will be allowed in mode 1.
Fourth, child A does insdump for SYMBOL_NAME3 and hence child B or threads created by child A or B should be allowed to dump_stack when it calls 		SYMBOL_NAME3 in mode 1.

For the heirarchy of the childs and threads created please refer the image file provided.

Mode 0: Only the owner allowed to dump_stack.
Mode 1: Any process with same parent allowed to do dump_stack.
Mode >1: any process allowed to do dump_stack

*****************************************************************************************************************************************************

Sample Output:-


For dumpmode 0:

root@quark:~# ./test_program
Please enter dump stack mode -0
[  426.287075] Dynamic_dump_stack is enabled, SYSCALL INSDUMP started
[  426.388325] SYMBOL NAME not found!
ERROR -1[  426.393272] Dynamic_dump_stack is enabled, SYSCALL INSDUMP started

[  426.411397] SYMOBOL NAME is located! Inserting KPROBE...
[  426.416782] Dumpstack mode of the owner is - 0
[  426.421703] KPROBE successfully inserted!
[  426.425801] Task Process Id: 340
[  426.429059] Task Threadg Id: 340
[  426.431299] Task RealParent: 282
[  426.431299] DDS Process ID: 340
[  426.431299] DDS Threadg ID: 340
[  426.431299] DDS RealParent ID: 282
[  426.431299] CPU: 0 PID: 340 Comm: test_program Not tainted 3.19.8-yocto-standard #1
[  426.431299] Hardware name: Intel Corp. QUARK/GalileoGen2, BIOS 0x01000200 01/01/2014
[  426.431299]  cd76cda8 cd76cda8 cd515f14 c14539b1 cd515f2c c124cf21 00000154 cd76cda8
[  426.431299]  ce648840 cd515f74 cd515f44 c10a27d2 cd76cdb0 cd515f74 ce648840 c111af21
[  426.431299]  cd515f5c c1027fa4 c111af20 cd515f74 00000000 080485f0 cd515f6c c1002964
[  426.431299] Call Trace:
[  426.431299]  [<c14539b1>] dump_stack+0x16/0x18
[  426.431299]  [<c124cf21>] Pre_Handler+0xb1/0xc0
[  426.431299]  [<c10a27d2>] aggr_pre_handler+0x32/0x70
[  426.431299]  [<c111af21>] ? SyS_open+0x1/0x20
[  426.431299]  [<c1027fa4>] kprobe_int3_handler+0xb4/0x130
[  426.431299]  [<c111af20>] ? do_sys_open+0x210/0x210
[  426.431299]  [<c1002964>] do_int3+0x44/0xa0
[  426.431299]  [<c1457f13>] int3+0x33/0x40
[  426.431299]  [<c111af21>] ? SyS_open+0x1/0x20
[  426.431299]  [<c14573c4>] ? syscall_call+0x7/0x7
[  426.562223] Dynamic_dump_stack is enabled, SYSCALL RMDUMP started
[  426.568370] Dumpstack exists! Removing KPROBE...
DUMPSTACKID 6 is[  426.575561] Dynamic_dump_stack is enabled, SYSCALL INSDUMP started
 removed
[  426.586367] SYMOBOL NAME is located! Inserting KPROBE...
[  426.593999] Dumpstack mode of the owner is - 0
[  426.599232] KPROBE successfully inserted!
IN CHILD B:[  426.609772] Task Process Id: 342
[  426.613352] Task Threadg Id: 342
[  426.613352] Task RealParent: 340
[  426.613352] DDS Process ID: 340
[  426.613352] DDS Threadg ID: 340
[  426.613352] DDS RealParent ID: 282
IN CHILD A:[  426.637104] Task Process Id: 341
[  426.640201] Task Threadg Id: 341
[  426.640201] Task RealParent: 340
[  426.640201] DDS Process ID: 340
[  426.640201] DDS Threadg ID: 340
[  426.640201] DDS RealParent ID: 282

[  426.657819] Task Process Id: 343
[  426.661088] Task Threadg Id: 340
[  426.664339] Task RealParent: 282
[  426.667537] DDS Process ID: 340
[  426.667537] DDS Threadg ID: 340
[  426.667537] DDS RealParent ID: 282
TRYING TO REMOVE[  426.677788] Dynamic_dump_stack is enabled, SYSCALL RMDUMP started
 DUMPSTACKID 7
[  426.685307] Dumpstack does not exists!
[  426.691985] Task Process Id: 344
[  426.695253] Task Threadg Id: 342
[  426.698504] Task RealParent: 340
[  426.700463] DDS Process ID: 340
[  426.700463] DDS Threadg ID: 340
[  426.700463] DDS RealParent ID: 282
TRYING TO REMOVE[  426.720466] Dynamic_dump_stack is enabled, SYSCALL RMDUMP started
 DUMPSTACKID 7
[  426.727908] Dumpstack does not exists!
[  426.740141] Dynamic_dump_stack is enabled, SYSCALL INSDUMP started
[  426.751923] SYMOBOL NAME is located! Inserting KPROBE...
[  426.767795] Dumpstack mode of the owner is - 0
[  426.773135] KPROBE successfully inserted!
[  426.777233] Task Process Id: 341
[  426.780491] Task Threadg Id: 341
[  426.782323] Task RealParent: 340
[  426.782323] DDS Process ID: 341
[  426.782323] DDS Threadg ID: 341
[  426.782323] DDS RealParent ID: 340
[  426.782323] CPU: 0 PID: 341 Comm: test_program Not tainted 3.19.8-yocto-standard #1
[  426.782323] Hardware name: Intel Corp. QUARK/GalileoGen2, BIOS 0x01000200 01/01/2014
[  426.782323]  cd76c928 cd76c928 cd0c1f14 c14539b1 cd0c1f2c c124cf21 00000155 cd76c928
[  426.782323]  cd6385a0 cd0c1f74 cd0c1f44 c10a27d2 cd76c930 cd0c1f74 cd6385a0 c10777a1
[  426.782323]  cd0c1f5c c1027fa4 c10777a0 cd0c1f74 00000000 080485f0 cd0c1f6c c1002964
[  426.782323] Call Trace:
[  426.782323]  [<c14539b1>] dump_stack+0x16/0x18
[  426.782323]  [<c124cf21>] Pre_Handler+0xb1/0xc0
[  426.782323]  [<c10a27d2>] aggr_pre_handler+0x32/0x70
[  426.782323]  [<c10777a1>] ? SyS_nanosleep+0x1/0x70
[  426.782323]  [<c1027fa4>] kprobe_int3_handler+0xb4/0x130
[  426.782323]  [<c10777a0>] ? hrtimer_nanosleep+0x190/0x190
[  426.782323]  [<c1002964>] do_int3+0x44/0xa0
[  426.782323]  [<c1457f13>] int3+0x33/0x40
[  426.782323]  [<c1457f13>] int3+0x33/0x40
[  426.782323]  [<c10777a1>] ? SyS_nanosleep+0x1/0x70
[  426.782323]  [<c14573c4>] ? syscall_call+0x7/0x7
[  426.891519] Task Process Id: 345
[  426.894788] Task Threadg Id: 341
[  426.898040] Task RealParent: 340
[  426.900035] DDS Process ID: 340
[  426.900035] DDS Threadg ID: 340
[  426.900035] DDS RealParent ID: 282
[  426.911531] Task Process Id: 341
[  426.914801] Task Threadg Id: 341
[  426.918053] Task RealParent: 340
[  426.921085] DDS Process ID: 341
[  426.921085] DDS Threadg ID: 341
[  426.921085] DDS RealParent ID: 340
[  426.921085] CPU: 0 PID: 341 Comm: test_program Not tainted 3.19.8-yocto-standard #1
[  426.921085] Hardware name: Intel Corp. QUARK/GalileoGen2, BIOS 0x01000200 01/01/2014
[  426.921085]  cd76c928 cd76c928 cd0c1f30 c14539b1 cd0c1f48 c124cf21 00000155 cd76c928
[  426.921085]  cd6385a0 cd0c1f74 cd0c1f60 c10a2772 cd76c930 00000246 00000000 bfc1c25c
[  426.921085]  cd0c1f6c c102853a bfc1c25c cd0c0000 d2777223 bfc1c25c bfc1c25c 000000e0
[  426.921085] Call Trace:
[  426.921085]  [<c14539b1>] dump_stack+0x16/0x18
[  426.921085]  [<c124cf21>] Pre_Handler+0xb1/0xc0
[  426.921085]  [<c10a2772>] opt_pre_handler+0x32/0x60
[  426.921085]  [<c102853a>] optimized_callback+0x5a/0x70
[  426.921085]  [<c10777a1>] ? SyS_nanosleep+0x1/0x70
[  426.921085]  [<c14573c4>] ? syscall_call+0x7/0x7
[  428.291969] Task Process Id: 229
[  428.295238] Task Threadg Id: 229
[  428.298490] Task RealParent: 1
[  428.299978] DDS Process ID: 340
[  428.299978] DDS Threadg ID: 340
[  428.299978] DDS RealParent ID: 282
[  428.321273] Task Process Id: 234
[  428.324542] Task Threadg Id: 229
[  428.327793] Task RealParent: 1
[  428.330049] DDS Process ID: 340
[  428.330049] DDS Threadg ID: 340
[  428.330049] DDS RealParent ID: 282
[  430.733833] Removed KPROBE, Exiting Process 340.

-------------------------------------------------------------------------------------------------------------------------

For dumpmode 1:

root@quark:~# ./test_program
Please enter dump stack mode -1
[  218.858761] Dynamic_dump_stack is enabled, SYSCALL INSDUMP started
[  218.959746] SYMBOL NAME not found!
ERROR -1[  218.964735] Dynamic_dump_stack is enabled, SYSCALL INSDUMP started

[  218.982863] SYMOBOL NAME is located! Inserting KPROBE...
[  218.988248] Dumpstack mode of the owner is - 1
[  218.993168] KPROBE successfully inserted!
[  218.997266] Task Process Id: 333
[  219.000523] Task Threadg Id: 333
[  219.002766] Task RealParent: 282
[  219.002766] DDS Process ID: 333
[  219.002766] DDS Threadg ID: 333
[  219.002766] DDS RealParent ID: 282
[  219.002766] CPU: 0 PID: 333 Comm: test_program Not tainted 3.19.8-yocto-standard #1
[  219.002766] Hardware name: Intel Corp. QUARK/GalileoGen2, BIOS 0x01000200 01/01/2014
[  219.002766]  cd76c928 cd76c928 cd2cbf14 c14539b1 cd2cbf2c c124cf21 0000014d cd76c928
[  219.002766]  cd4bd600 cd2cbf74 cd2cbf44 c10a27d2 cd76c930 cd2cbf74 cd4bd600 c111af21
[  219.002766]  cd2cbf5c c1027fa4 c111af20 cd2cbf74 00000000 080485f0 cd2cbf6c c1002964
[  219.002766] Call Trace:
[  219.002766]  [<c14539b1>] dump_stack+0x16/0x18
[  219.002766]  [<c124cf21>] Pre_Handler+0xb1/0xc0
[  219.002766]  [<c10a27d2>] aggr_pre_handler+0x32/0x70
[  219.002766]  [<c111af21>] ? SyS_open+0x1/0x20
[  219.002766]  [<c1027fa4>] kprobe_int3_handler+0xb4/0x130
[  219.002766]  [<c111af20>] ? do_sys_open+0x210/0x210
[  219.002766]  [<c1002964>] do_int3+0x44/0xa0
[  219.002766]  [<c1457f13>] int3+0x33/0x40
[  219.002766]  [<c111af21>] ? SyS_open+0x1/0x20
[  219.002766]  [<c14573c4>] ? syscall_call+0x7/0x7
[  219.133221] Dynamic_dump_stack is enabled, SYSCALL RMDUMP started
[  219.139366] Dumpstack exists! Removing KPROBE...
DUMPSTACKID 3 is[  219.146477] Dynamic_dump_stack is enabled, SYSCALL INSDUMP started
 removed
[  219.157272] SYMOBOL NAME is located! Inserting KPROBE...
[  219.164876] Dumpstack mode of the owner is - 1
[  219.170265] KPROBE successfully inserted!
IN CHILD A:[  219.180905] Task Process Id: 334
[  219.185011] Task Threadg Id: 334
[  219.188263] Task RealParent: 333
[  219.190081] DDS Process ID: 333
[  219.190081] DDS Threadg ID: 333
[  219.190081] DDS RealParent ID: 282

IN CHILD B:[  219.208445] Task Process Id: 335
[  219.211312] Task Threadg Id: 335
[  219.211312] Task RealParent: 333
[  219.211312] DDS Process ID: 333
[  219.211312] DDS Threadg ID: 333
[  219.211312] DDS RealParent ID: 282
[  219.229160] Task Process Id: 336
[  219.232430] Task Threadg Id: 333
[  219.235681] Task RealParent: 282
[  219.238879] DDS Process ID: 333
[  219.238879] DDS Threadg ID: 333
[  219.238879] DDS RealParent ID: 282
[  219.238879] CPU: 0 PID: 336 Comm: test_program Not tainted 3.19.8-yocto-standard #1
[  219.238879] Hardware name: Intel Corp. QUARK/GalileoGen2, BIOS 0x01000200 01/01/2014
[  219.238879]  cd76c928 cd76c928 ce757f30 c14539b1 ce757f48 c124cf21 00000150 cd76c928
[  219.238879]  cd4bdd80 ce757f74 ce757f60 c10a2772 cd76c930 00000246 00000000 b77beb40
[  219.238879]  ce757f6c c102853a b77be1da ce756000 d277711f b77be1da 5722eabf 000000e0
[  219.238879] Call Trace:
[  219.238879]  [<c14539b1>] dump_stack+0x16/0x18
[  219.238879]  [<c124cf21>] Pre_Handler+0xb1/0xc0
[  219.238879]  [<c10a2772>] opt_pre_handler+0x32/0x60
[  219.238879]  [<c102853a>] optimized_callback+0x5a/0x70
[  219.238879]  [<c103007b>] ? __change_page_attr_set_clr+0x63b/0x990
[  219.238879]  [<c1046021>] ? SyS_newuname+0x1/0x80
[  219.238879]  [<c14573c4>] ? syscall_call+0x7/0x7
TRYING TO REMOVE[  219.350447] Dynamic_dump_stack is enabled, SYSCALL RMDUMP started
 DUMPSTACKID 4
[  219.357889] Dumpstack does not exists!
TRYING TO REMOVE[  219.363491] Dynamic_dump_stack is enabled, SYSCALL RMDUMP started
 DUMPSTACKID 4
[  219.371043] Dumpstack does not exists!
[  219.380147] Dynamic_dump_stack is enabled, SYSCALL INSDUMP started
[  219.391919] SYMOBOL NAME is located! Inserting KPROBE...
[  219.402188] Task Process Id: 337
[  219.405457] Task Threadg Id: 335
[  219.408709] Task RealParent: 333
[  219.410054] DDS Process ID: 333
[  219.410054] DDS Threadg ID: 333
[  219.410054] DDS RealParent ID: 282
[  219.428647] Dumpstack mode of the owner is - 1
[  219.434001] KPROBE successfully inserted!
[  219.438099] Task Process Id: 334
[  219.441356] Task Threadg Id: 334
[  219.443177] Task RealParent: 333
[  219.443177] DDS Process ID: 334
[  219.443177] DDS Threadg ID: 334
[  219.443177] DDS RealParent ID: 333
[  219.443177] CPU: 0 PID: 334 Comm: test_program Not tainted 3.19.8-yocto-standard #1
[  219.443177] Hardware name: Intel Corp. QUARK/GalileoGen2, BIOS 0x01000200 01/01/2014
[  219.443177]  cd76c828 cd76c828 ce761f14 c14539b1 ce761f2c c124cf21 0000014e cd76c828
[  219.443177]  cd55d420 ce761f74 ce761f44 c10a27d2 cd76c830 ce761f74 cd55d420 c10777a1
[  219.443177]  ce761f5c c1027fa4 c10777a0 ce761f74 00000000 080485f0 ce761f6c c1002964
[  219.443177] Call Trace:
[  219.443177]  [<c14539b1>] dump_stack+0x16/0x18
[  219.443177]  [<c124cf21>] Pre_Handler+0xb1/
[  219.443177]  [<c10a27d2>] aggr_pre_handler+0x32/0x70
[  219.443177]  [<c10777a1>] ? SyS_nanosleep+0x1/0x70
[  219.443177]  [<c1027fa4>] kprobe_int3_handler+0xb4/0x130
[  219.443177]  [<c10777a0>] ? hrtimer_nanosleep+0x190/0x190
[  219.443177]  [<c1002964>] do_int3+0x44/0xa0
[  219.443177]  [<c1457f13>] int3+0x33/0x40
[  219.443177]  [<c10777a1>] ? SyS_nanosleep+0x1/0x70
[  219.443177]  [<c14573c4>] ? syscall_call+0x7/0x7
[  219.551527] Task Process Id: 338
[  219.554796] Task Threadg Id: 334
[  219.558048] Task RealParent: 333
[  219.560037] DDS Process ID: 333
[  219.560037] DDS Threadg ID: 333
[  219.560037] DDS RealParent ID: 282
[  219.571547] Task Process Id: 334
[  219.574816] Task Threadg Id: 334
[  219.578069] Task RealParent: 333
[  219.581092] DDS Process ID: 334
[  219.581092] DDS Threadg ID: 334
[  219.581092] DDS RealParent ID: 333
[  219.581092] CPU: 0 PID: 334 Comm: test_program Not tainted 3.19.8-yocto-standard #1
[  219.581092] Hardware name: Intel Corp. QUARK/GalileoGen2, BIOS 0x01000200 01/01/2014
[  219.581092]  cd76c828 cd76c828 ce761f30 c14539b1 ce761f48 c124cf21 0000014e cd76c828
[  219.581092]  cd55d420 ce761f74 ce761f60 c10a2772 cd76c830 00000246 00000000 bfe1f40c
[  219.581092]  ce761f6c c102853a bfe1f40c ce760000 d2777160 bfe1f40c bfe1f40c 000000e0
[  219.581092] Call Trace:
[  219.581092]  [<c14539b1>] dump_stack+0x16/0x18
[  219.581092]  [<c124cf21>] Pre_Handler+0xb1/0xc0
[  219.581092]  [<c10a2772>] opt_pre_handler+0x32/0x60
[  219.581092]  [<c102853a>] optimized_callback+0x5a/0x70
[  219.581092]  [<c10777a1>] ? SyS_nanosleep+0x1/0x70
[  219.581092]  [<c14573c4>] ? syscall_call+0x7/0x7
[  219.662252] Task Process Id: 333
[  219.665521] Task Threadg Id: 333
[  219.668773] Task RealParent: 282
[  219.671673] DDS Process ID: 334
[  219.671673] DDS Threadg ID: 334
[  219.671673] DDS RealParent ID: 333
[  223.662087] Removed KPROBE, Exiting Process 334.
[  223.682154] Removed KPROBE, Exiting Process 333.

