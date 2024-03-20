# blackhole

> Simple, Git-powered Backup, in Your Control


## introduction

**blackhole** is a very simple backup utility, originally meant
for minimal use cases or low-power/iot applications.

**Table of Contents**

* [installation](#installation)
* [usage][usage]
* [project principles](#principles)
* [limitations](#limitations)
* [license](#license)


<a name="installation"></a>
### installation

See build and installation instructions [here][build].


<a name="principles"></a>
### project principles

* **minimal**: blackhole just handles creating the snapshots and pushing to
  remote. It does **not** run as a daemon, giving the user the freedom to
  decide on what, when and how to run it. It has hooks for extending itself.
* **lightweight**: we had originally implemented it using good, old Bash but
  we needed more precision and control. LibGit2 promised that. It has **not**
  disappointed.
* **still a Git repo**: we believe Git, as a versioning system, does an
  awesome job. You can issue the same Git commands you would, in any other
  Git repo. This allows you to perform rollbacks and use any hosted-Git-repo
  service as your own backup service.


<a name="limitations"></a>
### limitations

* **no beautiful GUI**: it is built for terminal only (for now).
* **no support for .gitattributes**: we have not gotten to implement this.
  You are welcome to do so.


<a name="license"></a>
## license

***The MIT License (MIT)***

*Copyright (c) 2016 GochoMugo <mugo@forfuture.co.ke>*


[build]:https://github.com/gochomugo/blackhole/blob/master/docs/introduction.md#build
[usage]:https://github.com/gochomugo/blackhole/blob/master/docs/usage.md
