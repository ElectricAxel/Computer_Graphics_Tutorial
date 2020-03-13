# Computer_Graphics_Tutorial
Following the steps on https://paroj.github.io/gltut/index.html

# Disclaimer
Most of the code here (nearly all of it) was downloaded from https://bitbucket.org/alfonse/gltut/downloads/ ; Offered by the book. I do not own this work, I'm merely following the instructions in the Review section of every chapter and keeping track of my changes.

The License.txt file contains the licensing information for the materials distributed in these tutorials.

# Process
I've been using VS Code as my IDE of choice. 
I've had to use the msys bash installed along MingW to be able to use the 'make' command. 
To set up the make command, I followed the instructions in the book. Downloaded Premake 4, ran Premake4.exe gmake on the root, and also had to run Premake4.exe gmake on every tutorial folder. There's a slight bug where the Make files are missing double quotes nearly at the end, and because the Tutorial Folders all have spaces in them, the make doesn't work. After adding the double quotation marks around the command, make works fine.

Here's an example of the line that causes such error:

```
$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"
```

And here's the fixed version:

```
$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
```

# About my progress
My first upload was after I finished Tutorial 07, so all those changes won't be visible through commits. A shame really, kinda wish I would have thought of keeping this in git before a friend suggested it.