# CTimeline
CGI application for timelines. Distributed under the GNU General Public License, version 3.

## Dependencies:
No

## Installation:
```
$ make
$ sudo make install
```

`ctimeline.cgi` and `ctimeline.css` will install into `/var/www/html/ctimeline` by default.

If you would like to configure this location, you may use:
```
$ sudo make install CTIMELINE_PATH='/your/path/to/ctimeline'
```

### Apache configuration:

1) Make sure that CGI module for apache are enabled

2) Add \<Directory\> directive to Apache configuration file like this:

```
<Directory /var/www/html/ctimeline>
	AllowOverride None
	Options +ExecCGI
	Order allow,deny
	Allow from all
</Directory>
```

## Example ctimelinerc file:

```
# Comment that will be ignored

title=Browse random timelines
css=ctimeline.css

header.title=Timelines of projects
header.desc=Here are random but interesting projects. You can explore them at your leisure

branch.name=Linux Kernel
branch.age_from=2000
branch.age_to=2021
branch.desc=Open-source, Unix-like operating system kernel

branch.name=PulseAudio
branch.age_from=2004
branch.age_to=2021
branch.desc=Sound system for POSIX OSes

branch.name=cgit
branch.age_from=2007
branch.age_to=2021
branch.desc=A hyperfast web frontend for git repositories written in C

branch.name=ALSA
branch.age_from=1997
branch.age_to=2020
branch.desc=Advanced Linux Sound Architecture

branch.name=NeXTSTEP
branch.age_from=1989
branch.age_to=1995
branch.desc=Object-oriented, multitasking operating system

branch.name=dpkg
branch.age_from=1994
branch.age_to=2018
branch.desc=Package managment system in the open-source operating system Debian

branch.name=wireguard-linux
branch.age_from=2016
branch.age_to=2021
branch.desc=WireGuard for the Linux kernel

branch.name=ctimeline
branch.age_from=2021
branch.age_to=2021
branch.desc=Web frontend for timelines written in C
```

## Git commands that can help you:

* Get year of first commit:
```
git log --date=format:"%Y" --pretty=format:"%ad" --reverse | head -1
```

* Get year of last commit:
```
git log --date=format:"%Y" --pretty=format:"%ad" | head -1
```
