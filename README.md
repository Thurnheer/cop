# cop
Communication protocol. A comunication protocol for embedded devices written in C++. This is a header only library developed in TDD style.

The intention is to have the same stack running on small microconrollers and on "big" hosts like PC's.
This means there are no dependencies to heap allocation or threading.

The basic idea is to have a serial communication protocol where one can define Commands (they are for sure delivered like e.g. TCP) and one can send events (no garanties to be delivered e.g. like UDP).

One needs to declare all Events and Commands which will be sent or received to the channel as a tuple.
This is only needed to know the types, the tuple does not need to be initialised and will not be by the library.

```cpp
using allMessages = std::tuple<myEvent, theOtherEvent, ...>
cop::Channel<allMessages> channel;
```

To send an Event just inherit from Event.
To be able to parse your data one needs to declare a templated parse function.


Like:

```cpp

enum Ids {
  myId = 101
};

struct myEvent : cop::Event<myId> {
int iData;
double dData;

template<class Coder>
auto parse(Coder coder) {
  return coder | iData | dData
}
};

cop::Channel<allMessages> channel;
channel.send(myEvent());
```
The channel has also an optional parameter to specify if the messages are created within a local buffer (size of the bigest message) or dynamicaly on the heap.
e.g.
```cpp
constexpr bool useStaticMemory = true;
cop::Channel<allMessages, useStaticMemory> channel;
```

A frame consist of a start and end frame byte (ASCII 'A') and escape byte (ASCII backslasch '\') 
Then it has an one byte header 
bit 1/2 are for indicating if it is an event or a command

Then a 2 byte identifier field to indicate the event or command id.
Then the actual binary data commes.
To verify if the transmition was correct a 16Bit crc is used. ( implementation from https://github.com/lammertb/libcrc/blob/master/src/crc16.c )

currently only tested on gcc 9.3
