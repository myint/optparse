#!/bin/bash -eux

trap "echo -e '\x1b[01;31mFailed\x1b[0m'" ERR

make

./test
./test --more-milk
# shellcheck disable=SC2065
./test --invalid-option 2>&1 | grep 'no such option'
./test --complex='(1.2, 3.4)' | grep 'complex: (1.2,3.4)'
./test -kkk | grep '^k: 3$'

echo -e '\x1b[01;32mOK\x1b[0m'
