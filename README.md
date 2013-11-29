###MangaDL (CPP-GUI)

A manga downloader with a GTK frontend.
It currently support mangareader and mangapark.

The program is still in alpha stage and needs to be tested and reviewed.
The more I am reading about design patterns the more I feel like a lot of my projects needs to be redisgned and this is one of them.

Depends on: libcURL, sqlite3, and GTK.

How to add an external DL manager:
`[OUTPUT]` represent the output
`[INPUT]` represent the input

Compile with : <pre>g++ gui.cpp -O3 `pkg-config gtk+-2.0 --cflags` `pkg-config gtk+-2.0 --libs` -lsqlite3 -lcurl -o MangaDL</pre>

![Main](https://raw.github.com/venam/MangaDL--CPP-GUI-Version-/master/main.png)
![Configs](https://raw.github.com/venam/MangaDL--CPP-GUI-Version-/master/configs.png)
![Queue](https://raw.github.com/venam/MangaDL--CPP-GUI-Version-/master/queue.png)

Here's the wannabe new design

![New Design](https://github.com/venam/MangaDL--CPP-GUI-Version-/raw/testing/new_design_pattern.png)

PS: You should read your mangas with mcomix or comix.
