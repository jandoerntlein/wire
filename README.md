# wire v0.0.1

A minimalistic signal-slot pattern implementation in C++14. This implemenation has been used with success in a small emulation software but it has not been tested extensively yet so use with care.

### Prerequisites
The example has been tested on Linux with clang 3.5.0.

### How to build the example
First make sure you have a recent version of clang installed. Then just type `make`.

### How to use
Copy the file `wire.h` to your current working directory and include it like this:

```c++
#define WIRE_USE_EXCEPTIONS // optional
#include "wire.h"
using namespace wire;
```

### Credits
See http://stackoverflow.com/questions/21192659/variadic-templates-and-stdbind and http://stackoverflow.com/questions/26129933/bind-to-function-with-an-unknown-number-of-arguments-in-c.

### License
The MIT License (MIT), see [LICENSE](LICENSE).
