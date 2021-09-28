# morse-coder
A text to morse code transceiver

It's pretty dumb at the moment.  No numbers.  Only capitalized letters-international standard does not differentiate case.

See the [project board](https://github.com/users/atjacobs/projects/1) for status, goals, etc.

## To build:

```
cd src
cc -o morse morse.c main.c
```

## To use:

`./morse HELLO THERE`

