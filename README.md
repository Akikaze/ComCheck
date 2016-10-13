# ComCheck

ComCheck is a little tool used for analyzing source code files and counting how many comments are written in these files, regardless of programming language.

Obviously, C++ comments are not written like HTML comments which don't either look like PHP comments. But what is common between every langage is the fact that they require files which contain lines. This is why ComCheck looks every folder of a project and lists every files before checking, for each file, the type of each line (code, comment or mix).

But, for determining if a line is a comment, ComCheck needs to know how are written comments in every langage. And this is really hard for a static system. To be able to understand even future langage still not invented, ComCheck uses a system of modules that you can plug directly in it. By default, ComCheck brings a module for C++. But if you know how to use C++ and QtCreator, you can create your own module for a specific language.

A module defines the file extension, to avoid looking for JAVA comments in Ruby files, and how to get the type of a specific line. Those modules are stored in dynamic librairies (.so or .a and .dll according to your operating system).

# Future evolution

- Understand documentation comments like Doxygen or Javadoc.
- Subdivide comments into several types : header, temporary, todo, ...
