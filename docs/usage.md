# blackhole: Setup

To setup a blackhole directory:

1. `git init` a repository
1. `mkdir .blackhole`
1. `touch .blackhole/config.ini`
1. `git remote add origin <Git-URL using SSH e.g. git@github.com:YourUsername/blackhole>`
1. [optional] `git remote add redudant-backup <Git-URL using SSH>`
1. [optional] configure cron to run blackhole every 5 minutes.
   ```bash
   # Run bh every 5 mins
   */5 * * * * /bin/bh-cron-wrapper /bin/bh --path /config --sync >> /var/log/bh.log 2>&1
   ```


**Note**: blackhole expects to find **origin** for pulling changes.


# blackhole: Configuration

**blackhole** is configured using a `config.ini` file in the configuration
directory.

```ini
[daemon]
; Name assigned to this instance, preferably unique.
; Defaults to 'blackhole-daemon'
name=HomeDesktop-Docs

; Email address assigned to this instance
; Defaults to 'blackhole-daemon@local'
email=john.doe@example.com

[runstate]
; Path to an existing directory in which to store state
; Defaults to the directory this file lies in.
path=/home/johndoe/var/run/blackhole/docs/

[intervals]
; This section allows configuring the number of errors experienced before
; executing hooks. These key-value pairs are in the format:
; <event-id>=<integer>
; where <event-id> can be one of the event IDs (documented below)
; and <integer> is the number of times before executing hook (again)
; For example,
pull.errors=5
```

Available hook events:

* `pull.errors`
* `merge-conflicts.errors`
* `push-origin.errors`
* `push-remote.errors`
* `fatal.errors`
