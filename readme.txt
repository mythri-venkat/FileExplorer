The terminal file explorer has 2 mode.Normal and command mode.
The application starts in normal mode and files and folder of the current directory are displayed.
-One can navigate to a directory/file by moving the cursor to desired position using up and down arrow keys and pressing enter key.The list scrolls up/down and displays the rest of the files if the list overflows vertically.
-Left arrow key goes to previous directory and right arrow goes forward.
-Backspace goes to parent directory.
-H key goes to Home directory(the directory where the application was started).
-On selecting a file,that file is opened.
-status/mode is written on the bottom of the screen.

One can enter into command mode by pressing ':'.After executing the command,the command mode persists indicated by ':' at the bottom.The commands available are:
1) Copy ‘:copy <source_file(s)> <destination_directory>’
   Move ‘:move <source_file(s)> <destination_directory>’
   Rename ‘:rename <old_filename> <new_filename>’
*All paths are relative to current directory(Both source and destination path).~ indicates home directory(application root).

2) Create file ‘:create_file <file_name> <destination_path>’
   Create directory ‘:create_dir <dir_name> <destination_path>’ 
*All paths are relative to current directory(Both source and destination path).~ indicates home directory(application root). '.' as destination path created file/dir in current directory.

3) Delete file​ : ‘:delete_file <file_path>’
   Delete directory: ‘:delete_dir <directory_path>’
*All paths are relative to home directory(application root).

4) Goto ‘:goto <location>’
*Path can be absolute(relative) to root(starts with /) or relative to current directory.

5) Search ‘:search <filename>’
*searches recursively in current folder.Output is absolute path(relative to app root) displayed in normal mode.one can navigate into the directory by pressing enter.Pressing back will take you to folder where search started(previous folder).

6) Snapshot ‘​ :snapshot <folder> <dumpfile>​ ’
*All paths are relative to current directory(Both source and destination path).~ indicates home directory(application root).

-ESC can be pressed to go back into normal mode.
-Backspace can be used to delete characters of command one by one.error status can be erased by pressing backspace or any other key.
-'q'can be pressed to exit from the application in normal mode as well as command mode.
-Pressing any other keys may lead to unexpected behavior.
-This application was developed on Linux.

