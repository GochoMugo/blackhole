# blackhole

> Simple, Git-powered Backup, in Your Control
>
> :construction: **This is a very young, alpha-quality project** :construction:


## introduction:

**blackhole** is a very simple backup utility, originally meant to
be used by hackers and the like, who find it disturbing to allow
third-party backup services to handle our personal data "covertly".

**Table of Contents**

* [installation](#installation)
* [project principles](#principles)
* [limitations](#limitations)
* [hacking](#hacking)


<a name="installation"></a>
### installation:

See build and installation instructions [here][build]

[build]:https://github.com/forfuturellc/blackhole/blob/master/docs/introduction.md#build


<a name="principles"></a>
### project principles:

* **minimal**: blackhole just handles creating the snapshots and pushing to
  remote. It does **not** run as a daemon, giving the user the freedom to
  decide on what, when and how to run it. It has hooks for extending itself.
* **lightweight**: we had originally implemented it using good, old Bash but
  we needed more precision and control. LibGit2 promised that. It has **not**
  disappointed.
* **still a Git repo**: we believe Git, as a versioning system, does an
  awesome job. You can issue the same Git commands you would, in any other
  Git repo. This allows you to perform rollbacks and use any hosted-Git-repo
  service as your own backup service. (Thank you, Linus and contributors)


<a name="limitations"></a>
### limitations:

Some limitations, that some may perceive, are features (:smile:) as described
in the section on the project principles. But where blackhole misses out
include:

* **no beautiful GUI**: it is being built by terminal freaks who care less
  about this.
* **no support for .gitattributes**: we have not gotten to implement this.
  You are welcome to do so.
* **young and hacker-friendly**: this project is quite young and inclines
  to be used by hackers i.e. people who like messing up with the source code.


<a name="hacking"></a>
## hacking:

To start hacking on the blackhole, read [HACKING.md][hacking] for
guidelines and other relevant information.

[hacking]:https://github.com/forfuturellc/blackhole/blob/master/HACKING.md


## license:

***The MIT License (MIT)***

*Copyright (c) 2016 GochoMugo <mugo@forfuture.co.ke><br>
Copyright (c) 2016 Forfuture LLC <we@forfuture.co.ke>*

**#P002**
