# WatchDogTimer
The WatchDogTimer Library written to be fully compatible with C++11 and as header-only to use easily.

## How to use
### 0.  Including `wdt.hpp` header file on top of your cpp file.
```C++
#include <wdt.hpp>
// Or
#include "wdt.hpp"
```

### 1.  Subtyping your dog with `WatchdogTimer` class and implementing `timeout()` to bark to you.
```C++
class MyWatchdogTimer : public WatchdogTimer
{
public:
  virtual void timeout()
  {
    // To do when your dog is bark.
    ...
  }
};
```

### 2.  Creating your dog with subtyped `WatchdogTimer` class.
```C++
MyWatchdogTimer wdt;
```

### 3.  Kicking your dog by calling `kick()`.
```C++
// wdt is scheduled to call timeout() after 100ms once.
wdt.kick(100);
```
As well as, you can also kick your dog to watch forever by calling `kick()` with `loop` flag `true`.
```C++
// wdt is scheduled to call timeout() after 100ms forever.
wdt.kick(100, true);
```

### 4.  Feeding your dog repeatedly to be silence by calling `clear()` repeatedly.
```C++
while (true)
{
  // To do somethings
  ...
  
  if (there_is_no_problem)
    wdt.clear();
}
```

### 5.  Finally, Stopping your dog to watch by calling `stop()`.
```C++
wdt.stop();
```
