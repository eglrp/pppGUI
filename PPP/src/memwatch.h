/*
** MEMWATCH.H
** Nonintrusive ANSI C memory leak / overwrite detection
** Copyright (C) 1992-2002 Johan Lindh
** All rights reserved.
** Version 2.71
**
************************************************************************
**
** PURPOSE:
**
**  MEMWATCH has been written to allow guys and gals that like to
**  program in C a public-domain memory error control product.
**  I hope you'll find it's as advanced as most commercial packages.
**  The idea is that you use it during the development phase and
**  then remove the MEMWATCH define to produce your final product.
**  MEMWATCH is distributed in source code form in order to allow
**  you to compile it for your platform with your own compiler.
**  It's aim is to be 100% ANSI C, but some compilers are more stingy
**  than others. If it doesn't compile without warnings, please mail
**  me the configuration of operating system and compiler you are using
**  along with a description of how to modify the source, and the version
**  number of MEMWATCH that you are using.
**
************************************************************************

	This file is part of MEMWATCH.

    MEMWATCH is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    MEMWATCH is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with MEMWATCH; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

************************************************************************
**
** REVISION HISTORY:
**
** 920810 JLI   [1.00]
** 920830 JLI   [1.10 double-free detection]
** 920912 JLI   [1.15 mwPuts, mwGrab/Drop, mwLimit]
** 921022 JLI   [1.20 ASSERT and VERIFY]
** 921105 JLI   [1.30 C++ support and TRACE]
** 921116 JLI   [1.40 mwSetOutFunc]
** 930215 JLI   [1.50 modified ASSERT/VERIFY]
** 930327 JLI   [1.51 better auto-init & PC-lint support]
** 930506 JLI   [1.55 MemWatch class, improved C++ support]
** 930507 JLI   [1.60 mwTest & CHECK()]
** 930809 JLI   [1.65 Abort/Retry/Ignore]
** 930820 JLI   [1.70 data dump when unfreed]
** 931016 JLI   [1.72 modified C++ new/delete handling]
** 931108 JLI   [1.77 mwSetAssertAction() & some small changes]
** 940110 JLI   [1.80 no-mans-land alloc/checking]
** 940328 JLI   [2.00 version 2.0 rewrite]
**              Improved NML (no-mans-land) support.
**              Improved performance (especially for free()ing!).
**              Support for 'read-only' buffers (checksums)
**              ^^ NOTE: I never did this... maybe I should?
**              FBI (free'd block info) tagged before freed blocks
**              Exporting of the mwCounter variable
**              mwBreakOut() localizes debugger support
**              Allocation statistics (global, per-module, per-line)
**              Self-repair ability with relinking
** 950913 JLI   [2.10 improved garbage handling]
** 951201 JLI   [2.11 improved auto-free in emergencies]
** 960125 JLI   [X.01 implemented auto-checking using mwAutoCheck()]
** 960514 JLI   [2.12 undefining of existing macros]
** 960515 JLI   [2.13 possibility to use default new() & delete()]
** 960516 JLI   [2.20 suppression of file flushing on unfreed msgs]
** 960516 JLI   [2.21 better support for using MEMWATCH with DLL's]
** 960710 JLI   [X.02 multiple logs and mwFlushNow()]
** 960801 JLI   [2.22 merged X.01 version with current]
** 960805 JLI   [2.30 mwIsXXXXAddr() to avoid unneeded GP's]
** 960805 JLI   [2.31 merged X.02 version with current]
** 961002 JLI   [2.32 support for realloc() + fixed STDERR bug]
** 961222 JLI   [2.40 added mwMark() & mwUnmark()]
** 970101 JLI   [2.41 added over/underflow checking after failed ASSERT/VERIFY]
** 970113 JLI   [2.42 added support for PC-Lint 7.00g]
** 970207 JLI   [2.43 added support for strdup()]
** 970209 JLI   [2.44 changed default filename to lowercase]
** 970405 JLI   [2.45 fixed bug related with atexit() and some C++ compilers]
** 970723 JLI   [2.46 added MW_ARI_NULLREAD flag]
** 970813 JLI   [2.47 stabilized marker handling]
** 980317 JLI   [2.48 ripped out C++ support; wasn't working good anyway]
** 980318 JLI   [2.50 improved self-repair facilities & SIGSEGV support]
** 980417 JLI	[2.51 more checks for invalid addresses]
** 980512 JLI	[2.52 moved MW_ARI_NULLREAD to occur before aborting]
** 990112 JLI	[2.53 added check for empty heap to mwIsOwned]
** 990217 JLI	[2.55 improved the emergency repairs diagnostics and NML]
** 990224 JLI	[2.56 changed ordering of members in structures]
** 990303 JLI	[2.57 first maybe-fixit-for-hpux test]
** 990516 JLI	[2.58 added 'static' to the definition of mwAutoInit]
** 990517 JLI	[2.59 fixed some high-sensitivity warnings]
** 990610 JLI	[2.60 fixed some more high-sensitivity warnings]
** 990715 JLI	[2.61 changed TRACE/ASSERT/VERIFY macro names]
** 991001 JLI	[2.62 added CHECK_BUFFER() and mwTestBuffer()]
** 991007 JLI	[2.63 first shot at a 64-bit compatible version]
** 991009 JLI	[2.64 undef's strdup() if defined, mwStrdup made const]
** 000704 JLI	[2.65 added some more detection for 64-bits]
** 010502 JLI   [2.66 incorporated some user fixes]
**              [mwRelink() could print out garbage pointer (thanks mac@phobos.ca)]
**				[added array destructor for C++ (thanks rdasilva@connecttel.com)]
**				[added mutex support (thanks rdasilva@connecttel.com)]
** 010531 JLI	[2.67 fix: mwMutexXXX() was declared even if MW_HAVE_MUTEX was not defined]
** 010619 JLI	[2.68 fix: mwRealloc() could leave the mutex locked]
** 020918 JLI	[2.69 changed to GPL, added C++ array allocation by Howard Cohen]
** 030212 JLI	[2.70 mwMalloc() bug for very large allocations (4GB on 32bits)]
** 030520 JLI	[2.71 added ULONG_LONG_MAX as a 64-bit detector (thanks Sami Salonen)]
**
** To use, simply include 'MEMWATCH.H' as a header file,
** and add MEMWATCH.C to your list of files, and define the macro
** 'MEMWATCH'. If this is not defined, MEMWATCH will disable itself.
**
** To call the standard C malloc / realloc / calloc / free; use mwMalloc_(),
** mwCalloc_() and mwFree_(). Note that mwFree_() will correctly
** free both malloc()'d memory as well as mwMalloc()'d.
**
** 980317: C++ support has been disabled.
**         The code remains, but is not compiled.
**
**         For use with C++, which allows use of inlining in header files
**         and class specific new/delete, you must also define 'new' as
**         'mwNew' and 'delete' as 'mwDelete'. Do this *after* you include
**         C++ header files from libraries, otherwise you can mess up their
**         class definitions. If you don't define these, the C++ allocations
**         will not have source file and line number information. Also note,
**         most C++ class libraries implement their own C++ memory management,
**         and don't allow anyone to override them. MFC belongs to this crew.
**         In these cases, the only thing to do is to use MEMWATCH_NOCPP.
**
** You can capture output from MEMWATCH using mwSetOutFunc().
** Just give it the adress of a "void myOutFunc(int c)" function,
** and all characters to be output will be redirected there.
**
** A failing ASSERT() or VERIFY() will normally always abort your
** program. This can be changed using mwSetAriFunc(). Give it a
** pointer to a "int myAriFunc(const char *)" function. Your function
** must ask the user whether to Abort, Retry or Ignore the trap.
** Return 2 to Abort, 1 to Retry or 0 to Ignore. Beware retry; it
** causes the expression to be evaluated again! MEMWATCH has a
** default ARI handler. It's disabled by default, but you can enable
** it by calling 'mwDefaultAri()'. Note that this will STILL abort
** your program unless you define MEMWATCH_STDIO to allow MEMWATCH
** to use the standard C I/O streams. Also, setting the ARI function
** will cause MEMWATCH *NOT* to write the ARI error to stderr. The
** error string is passed to the ARI function instead, as the
** 'const char *' parameter.
**
** You can disable MEMWATCH's ASSERT/VERIFY and/or TRACE implementations.
** This can be useful if you're using a debug terminal or smart debugger.
** Disable them by defining MW_NOASSERT, MW_NOVERIFY or MW_NOTRACE.
**
** MEMWATCH fills all allocated memory with the byte 0xFE, so if
** you're looking at erroneous data which are all 0xFE:s, the
** data probably was not initialized by you. The exception is
** calloc(), which will fill with zero's. All freed buffers are
** zapped with 0xFD. If this is what you look at, you're using
** data that has been freed. If this is the case, be aware that
** MEMWATCH places a 'free'd block info' structure immediately
** before the freed data. This block contains info about where
** the block was freed. The information is in readable text,
** in the format "FBI<counter>filename(line)", for example:
** "FBI<267>test.c(12)". Using FBI's slows down free(), so it's
** disabled by default. Use mwFreeBufferInfo(1) to enable it.
**
** To aid in tracking down wild pointer writes, MEMWATCH can perform
** no-mans-land allocations. No-mans-land will contain the byte 0xFC.
** MEMWATCH will, when this is enabled, convert recently free'd memory
** into NML allocations.
**
** MEMWATCH protects it's own data buffers with checksums. If you
** get an internal error, it means you're overwriting wildly,
** or using an uninitialized pointer.
**
************************************************************************
**
** Note when compiling with Microsoft C:
**  -   MSC ignores fflush() by default. This is overridden, so that
**      the disk log will always be current.
**
** This utility has been tested with:
**  PC-lint 7.0k, passed as 100% ANSI C compatible
**  Microsoft Visual C++ on Win16 and Win32
**  Microsoft C on DOS
**  SAS C on an Amiga 500
**  Gnu C on a PC running Red Hat Linux
**  ...and using an (to me) unknown compiler on an Atari machine.
**
************************************************************************
**
** Format of error messages in MEMWATCH.LOG:
**  message: <sequence-number> filename(linenumber), information
**
** Errors caught by MemWatch, when they are detected, and any
** actions taken besides writing to the log file MEMWATCH.LOG:
**
**  Double-freeing:
**      A pointer that was recently freed and has not since been
**      reused was freed again. The place where the previous free()
**      was executed is displayed.
**      Detect: delete or free() using the offending pointer.
**      Action: The delete or free() is cancelled, execution continues.
**  Underflow:
**      You have written just ahead of the allocated memory.
**      The size and place of the allocation is displayed.
**      Detect: delete or free() of the damaged buffer.
**      Action: The buffer is freed, but there may be secondary damage.
**  Overflow:
**      Like underflow, but you've written after the end of the buffer.
**      Detect: see Underflow.
**      Action: see Underflow.
**  WILD free:
**      An unrecognized pointer was passed to delete or free().
**      The pointer may have been returned from a library function;
**      in that case, use mwFree_() to force free() of it.
**      Also, this may be a double-free, but the previous free was
**      too long ago, causing MEMWATCH to 'forget' it.
**      Detect: delete or free() of the offending pointer.
**      Action: The delete or free() is cancelled, execution continues.
**  NULL free:
**      It's unclear to me whether or not freeing of NULL pointers
**      is legal in ANSI C, therefore a warning is written to the log file,
**      but the error counter remains the same. This is legal using C++,
**      so the warning does not appear with delete.
**      Detect: When you free(NULL).
**      Action: The free() is cancelled.
**  Failed:
**      A request to allocate memory failed. If the allocation is
**      small, this may be due to memory depletion, but is more likely
**      to be memory fragmentation problems. The amount of memory
**      allocated so far is displayed also.
**      Detect: When you new, malloc(), realloc() or calloc() memory.
**      Action: NULL is returned.
**  Realloc:
**      A request to re-allocate a memory buffer failed for reasons
**      other than out-of-memory. The specific reason is shown.
**      Detect: When you realloc()
**      Action: realloc() is cancelled, NULL is returned
**  Limit fail:
**      A request to allocate memory failed since it would violate
**      the limit set using mwLimit(). mwLimit() is used to stress-test
**      your code under simulated low memory conditions.
**      Detect: At new, malloc(), realloc() or calloc().
**      Action: NULL is returned.
**  Assert trap:
**      An ASSERT() failed. The ASSERT() macro works like C's assert()
**      macro/function, except that it's interactive. See your C manual.
**      Detect: On the ASSERT().
**      Action: Program ends with an advisory message to stderr, OR
**              Program writes the ASSERT to the log and continues, OR
**              Program asks Abort/Retry/Ignore? and takes that action.
**  Verify trap:
**      A VERIFY() failed. The VERIFY() macro works like ASSERT(),
**      but if MEMWATCH is not defined, it still evaluates the
**      expression, but it does not act upon the result.
**      Detect: On the VERIFY().
**      Action: Program ends with an advisory message to stderr, OR
**              Program writes the VERIFY to the log and continues, OR
**              Program asks Abort/Retry/Ignore? and takes that action.
**  Wild pointer:
**      A no-mans-land buffer has been written into. MEMWATCH can
**      allocate and distribute chunks of memory solely for the
**      purpose of trying to catch random writes into memory.
**      Detect: Always on CHECK(), but can be detected in several places.
**      Action: The error is logged, and if an ARI handler is installed,
**              it is executed, otherwise, execution continues.
**  Unfreed:
**      A memory buffer you allocated has not been freed.
**      You are informed where it was allocated, and whether any
**      over or underflow has occured. MemWatch also displays up to
**      16 bytes of the data, as much as it can, in hex and text.
**      Detect: When MemWatch terminates.
**      Action: The buffer is freed.
**  Check:
**      An error was detected during a CHECK() operation.
**      The associated pointer is displayed along with
**      the file and line where the CHECK() was executed.
**      Followed immediately by a normal error message.
**      Detect: When you CHECK()
**      Action: Depends on the error
**  Relink:
**      After a MEMWATCH internal control block has been trashed,
**      MEMWATCH tries to repair the damage. If successful, program
**      execution will continue instead of aborting. Some information
**      about the block may be gone permanently, though.
**      Detect: N/A
**      Action: Relink successful: program continues.
**              Relink fails: program aborts.
**  Internal:
**      An internal error is flagged by MEMWATCH when it's control
**      structures have been damaged. You are likely using an uninitialized
**      pointer somewhere in your program, or are zapping memory all over.
**      The message may give you additional diagnostic information.
**      If possible, MEMWATCH will recover and continue execution.
**      Detect: Various actions.
**      Action: Whatever is needed
**  Mark:
**      The program terminated without umarking all marked pointers. Marking
**      can be used to track resources other than memory. mwMark(pointer,text,...)
**      when the resource is allocated, and mwUnmark(pointer) when it's freed.
**      The 'text' is displayed for still marked pointers when the program
**      ends.
**      Detect: When MemWatch terminates.
**      Action: The error is logged.
**
**
************************************************************************
**
**  The author may be reached by e-mail at the address below. If you
**  mail me about source code changes in MEMWATCH, remember to include
**  MW's version number.
**
**      Johan Lindh
**      johan@linkdata.se
**
** The latest version of MEMWATCH may be downloaded from
** http://www.linkdata.se/
*/


