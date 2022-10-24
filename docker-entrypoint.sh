#!/usr/bin/env bash
set -e

./ntclient -a caster.centipede.fr -p 2101 -m CT -u centipedeTest -pw centipede -la 46.164 -lo -0.94 | pv -ptebar > /dev/null
