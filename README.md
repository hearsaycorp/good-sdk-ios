## Setup

To update this repo:

1. Download the latest Blackberry Dynamics SDK.
2. Run scripts/GD/prepare.sh against the tar file.
3. Copy the contents back

# Understanding the code

Use the `lipo` command to extract out the x86 version of the Good Mach-O binrary:

```bash
lipo -thin x86_64 -output GD_x86 Frameworks/GD.framework/Versions/A/GD
```

The output of this file is an archive file (ar) which contains a bunch of object files.  You can unpack the file using the `ar` tool:


```bash
ar -xv GD_x86
```

You can then use [Hopper](https://www.hopperapp.com) to view the code.    The main file to review is the `libgdios4.a-x86_64-master.o`. 
