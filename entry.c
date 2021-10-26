void main();

// must be the first defined function in the file
void _start() {
    main();
    // control should never reach back here,
    // but if it does, idle loop
    while (1);
}
