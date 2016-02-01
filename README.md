# ce
Simple nested exceptions for C based on setjmp.h.
## usage
`CATCH()` and `FINALLY` are obligatory after `TRY`. `TRY` blocks can be nested. 
```
TRY {
	...
	if (some - thing + goes == wrong) THROW(-1);
	...
	this_can_throw_too();
	...
}
CATCH (ex) {
	// ex: exception code passed to THROW(), int
}
FINALLY {
	// this gets always executed after try/catch blocks
}
```
Throw value is an int error code other than 0. Passing 0 to `THROW()` or
throwing exception outside of any `TRY` block will `abort()`.

Directive `USE_EXCEPTIONS;` has to be placed somewhere in the global scope, like this:
```
#include "ce.h"

USE_EXCEPTIONS;

int main() {
  ...
}
```
Compile with `-DCE_M` to add multithread support.
## requirements
gcc because of local functions. See https://gcc.gnu.org/onlinedocs/gcc/Nested-Functions.html
## license
The MIT
